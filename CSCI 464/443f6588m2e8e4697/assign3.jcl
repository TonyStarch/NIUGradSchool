//Z030199A JOB ,'Brandon Tweed'
/*JOBPARM ROOM=199
//         JCLLIB ORDER=(T90MES1.C464.PROCLIB) 
//STEP1    EXEC HLA464CG
//GO.XSNAPOUT DD UNIT=AFF=SYSPRINT
//ASM.SYSIN DD *
         Copy QDriver    Driver for Assignment 3
         Copy ASMMSP     Get IBM structured macro defs
* Definition for a Node
* Note: Link portion of the node is first
Node     DSect ,                   Node description
AVLink   DS    0A                  Avail Link field
Info     DS    F                   Information field
Link     DS    A                   For linked allocation (only)
ASSGN3   CSECT
ASSGN3   AMODE 24        Set 24-bit addressing mode
         XSAVE SA=A3SAV,ENTRY
         PGMDUMP
         PRINT NOGEN     Don't expand macros
         EJECT
********************************************************************
*
*  QINSERT
*
*  ENTRY CONDITIONS:
*
*  EXIT CONDITIONS:
*
*  REGISTER USAGE:
*  
*  R1 - Points to the parm list
*       Parm list:
*       =A(InValue)
*       =A(RetCode)
*  R7 - Contains InValue once retrieved from parm list
*  R8 - Contains the @ of RetCode from parm list
*  R9 - Used for comparing against return code from
*       QFull
*  R10 - Used to temporarily hold the address of a
*        new node grabbed from the Avail stack in the case
*        of linked allocation or from the rear stack in the case
*        of sequential allocation.
*        
*  LOGIC:
*
*  [1] Perform standard linkage and initialization
*  [2] Get input parameters (InValue, RetCode)
*  [3] If queue is full [call QFull]
*  [4]    Then set return code 4 (no room in queue)
*  [5]    Else If front stack is empty
*  [6]            Then insert InValue onto front stack
*  [7]            Else insert InValue onto rear stack
*  [8]         EndIf
*  [9] EndIf
*
********************************************************************
QINSERT  XSAVE SA=SAVAREA1,OPT=ENTRY,TR=NO
*  [2] Get input parameters (InValue, RetCode)
         L R7,0(,R1)          Get @Invalue -> R7
         L R7,0(,R7)          Get InValue -> R7
         L R8,4(,R1)          Get @RetCode -> R8
*  [3] If queue is full [call QFull]
         XCALL QFull,(0,ISFULL)  Call QFull Routine
         XDUMP ISFULL,4
         IF (ICM,R9,15,ISFULL,Z)     If RetCode = 0 (Q is full)
*  [4]    Then set return code 4 (no room in queue)
         LHI R9,4           set return code in R9
         ST R9,0(,R8)       save it to the parm list
*  [5]    Else If front stack is empty
         ELSE
         LHI R9,0           Zero for return code
         ST R9,0(,R8)       Set Return Code to Zero
* Check (Front stack top pointer = null)
         IF (ICM,R9,15,F@,Z) THEN
*  [6]            Then insert InValue onto front stack
         L R10,Avail        get the @ of available node
X        USING Node,R10     X is our temp link variable
         MVC Avail,X.AVLink Avail now points to next node
         ST R7,X.Info       Save InValue -> X.Info
         MVC X.Link,F@      Save top pointer -> X.Link
         ST R10,F@          top pointer -> new node
         DROP X             Release temp link variable
*  [7]            Else insert InValue onto rear stack
         ELSE
         L R10,R@           Get Rear Stack Pointer
         AHI R10,4          Move R@ forward to new location
         ST R7,0(,R10)      Save InValue to new location
         ST R10,R@          Save new Rear Stack pointer value
*  [8]         EndIf
         ENDIF
*  [9] EndIf
         ENDIF
QINSEXT  XRETURN SA=SAVAREA1,TR=NO
         LTORG             Accumulate Literals
*******************************
* QINSERT WORKING STORAGE
*******************************
ISFULL   DS F   Return value from QFull
         EJECT
********************************************************************
*
*  QDELETE
*
*  REGISTER USAGE:
*  R1 - @ OF PARM LIST
*       
*
*  LOGIC:
*  [1] Perform standard linkage and initialization
*  [2] Get input parameters (InValue, RetValue)
*  [3] If queue is empty [call QEmpty]
*  [4]    Then return Underflow - set RetValue
*  [5]    Else Delete entry from front stack
*  [6]         If front stack is empty
*  [7]            Then Do While rear stack not empty
*  [8]                    Remove entry from rear stack
*  [9]                    Insert entry on front stack
* [10]                 End Do
* [11]         EndIf
* [12] EndIf
********************************************************************
*  [1] Perform standard linkage and initialization
QDELETE  XSAVE SA=SAVAREA2,OPT=ENTRY,TR=NO
*         XDUMP AVAIL,76
*  [2] Get input parameters (InValue, RetValue)
         L R7,0(,R1)       Get @ of InValue
         L R8,4(,R1)       Get @ of RetValue
*  [3] If queue is empty [call QEmpty]
         XCALL QEMPTY,(0,ISEMPTY) Call QEmpty
         IF (ICM,R9,15,ISEMPTY,Z) THEN
*  [4]    Then return Underflow - set RetValue
         XDUMP
         LHI R9,4          Return code is 4
         ST R9,0(,R8)      Save return code in RetValue
         XDUMP 0(,R8),4
*  [5]    Else Delete entry from front stack
         ELSE
         XR R9,R9            Clear R9
         ST R9,0(,R8)        Save the return code
         L R10,F@            GET T
P        USING Node,R10      P <-T
         MVC F@,P.Link       T <- LINK(P)  
         MVC 0(4,R7),P.Info  Y <- INFO(P)
* Avail <== P
         MVC P.AVLink,Avail  Link(P) <- Avail
         ST R10,Avail        Avail <-P
         DROP P              Done with P
*  [6]         If front stack is empty
         IF (ICM,R9,15,F@,Z) THEN
*  [7]            Then Do While rear stack not empty
         LA R6,RStack-4   Get @ of RStack - > R6
         DO  WHILE=(C,R6,NE,R@)
*  [8]                    Remove entry from rear stack
         L R10,R@         Set up Node Addressability
P        USING Node,R10      Call Node "P"
         MVC TMPINFO,P.Info  Put info into a temporary place
         DROP P
         AHI R10,-4         Move R@ back an entry
         ST R10,R@          Make change permanent
*  [9]                      Insert entry on front stack
         L R10,Avail        Get @ of first available node
P        USING Node,R10     Set up Node Addressability
         MVC Avail,P.AVLink Update Avail
         MVC P.Info,TMPINFO P.Info <- Y
         MVC P.Link,F@      P.Link <- T
         ST R10,F@          T <- P
         DROP P             Done with P
* [10]                 End Do
         ENDDO
* [11]         EndIf
         ENDIF
* [12] EndIf
         ENDIF
*         XDUMP AVAIL,76
QDELEXT  XRETURN SA=SAVAREA2,TR=NO
         LTORG             Accumulate Literals
*******************************
* QDELETE WORKING STORAGE
*******************************
ISEMPTY  DS F   Stores result from QEmpty
TMPINFO  DS F   Stores Value being pulled from stack
         EJECT
********************************************************************
*
*  QFRONT
*
*  LOGIC:
*  [1] Perform standard linkage and initialization
*  [2] Get input parameters (InValue, RetValue)
*  [3] If queue is empty [call QEmpty]
*  [4]    Then return Underflow - set RetValue
*  [5]    Else return top entry from front stack
*  [6] EndIf
********************************************************************
QFRONT   XSAVE SA=SAVAREA3,OPT=ENTRY,TR=NO
*  [2] Get input parameters (InValue, RetValue)
         L R7,0(,R1)    Get @ InValue
         L R8,4(,R1)    Get @ RetValue
*  [3] If queue is empty [call QEmpty]
         XCALL QEmpty,(0,QMTRET)
         IF (ICM,R9,15,QMTRET,Z) THEN
*  [4]    Then return Underflow - set RetValue
         LHI R9,4        Set Return Code to 4
         ST R9,0(,R8)    Save Return Code in RetValue
*  [5]    Else return top entry from front stack
         ELSE
         XR R9,R9        Set return code to 0
         ST R9,0(,R8)    Save the Return Code
         L R10,F@        Set up Node Addressability
F        USING Node,R10  Call the Node "F"
         MVC 0(4,R7),F.Info  Get the Info into InValue
         DROP F              Done with F
*  [6] EndIf
         ENDIF
QFTEXT   XRETURN SA=SAVAREA3,TR=NO
         LTORG             Accumulate Literals
*********************************
* QFRONT WORKING STORAGE
*********************************
QMTRET   DS F
         EJECT
********************************************************************
*
*  QEMPTY
*
*  LOGIC:
*  [1] Perform standard linkage and initialization
*  [2] Get and initialize return parameter (RetCode)
*  [3] If front stack is empty
*  [4]    Then queue is empty
*  [5]    Else queue is not empty
*  [6] EndIf
*
*  ENTRY CONDITIONS:
*  R1 - @ OF PARM LIST
*       PARM LIST:
*       =A(InValue)
*       =A(RetValue)
*
********************************************************************
*  [1] Perform standard linkage and initialization
QEMPTY   XSAVE SA=SAVAREA4,OPT=ENTRY,TR=NO
*  [2] Get and initialize return parameter (RetCode)
         L R8,4(,R1)   GET @ OF Retvalue --> R8
*  [3] If front stack is empty (F is null)
         IF (ICM,R9,15,F@,Z) THEN 
*  [4]    Then queue is empty (Set RetCode = 0)
         ST R9,0(,R8)
         ELSE
*  [5]    Else queue is not empty (Set RetCode = 4)
         LHI R9,4
         ST R9,0(,R8)
*  [6] EndIf
         ENDIF
QMTEXT   XRETURN SA=SAVAREA4,TR=NO
         LTORG         ACCUMULATE LITERALS HERE
*******************************
* QEMPTY WORKING STORAGE
*******************************
QESA     DC 18F'0'
         EJECT
********************************************************************
*
*  QFULL         
*
*  LOGIC:
*
*  [1] Perform standard linkage and initialization
*  [2] Get and initialize return parameter (RetCode)
*  [3] If rear stack is full (i.e., Avail is empty)
*  [4]    Then queue is full
*  [5]    Else queue is not full
*  [6] EndIf
*
********************************************************************
*  [1] Perform standard linkage and initialization
QFULL    XSAVE SA=SAVAREA5,OPT=ENTRY,TR=NO
*  [2] Get and initialize return parameter (RetCode)
         L R8,4(,R1)  Get @ RetCode
*  [3] If rear stack is full (i.e., Avail is empty)
         L R9,R@             Get Rear Stack Top
         IF (C,R9,EQ,RMax) THEN
*  [4]    Then queue is full
         XR R9,R9            Clear R9
         ST R9,0(,R8)        Save 0 in RetCode
*  [5]    Else queue is not full
         ELSE
         LHI R9,4            Set return value = 4
         ST R9,0(,R8)        Save 4 in RetCode
*  [6] EndIf
         ENDIF
QFULLEXT XRETURN SA=SAVAREA5,TR=NO
         LTORG             Accumulate Literals
********************************************************************
* QFULL WORKING STORAGE
********************************************************************
* Definitions for stack elements
*
F@       DC    A(0)                Front stack top pointer
R@       DC    A(RStack-4)         Rear stack top pointer
*
RStack   DS    10F                 Rear stack has 10 entries
RMax     DC    A(*-4)                Rear stack upper bound
*
Avail    DC    A(*+4)              Avail list top pointer
         DC    9A(*+8,0),A(0,0)    Avail list of 10 entries
*
R0       EQU   0         REGISTER EQUATE FOR READABILITY
R1       EQU   1         REGISTER EQUATE FOR READABILITY
R2       EQU   2         REGISTER EQUATE FOR READABILITY
R3       EQU   3         REGISTER EQUATE FOR READABILITY
R4       EQU   4         REGISTER EQUATE FOR READABILITY
R5       EQU   5         REGISTER EQUATE FOR READABILITY
R6       EQU   6         REGISTER EQUATE FOR READABILITY
R7       EQU   7         REGISTER EQUATE FOR READABILITY
R8       EQU   8         REGISTER EQUATE FOR READABILITY
R9       EQU   9         REGISTER EQUATE FOR READABILITY
R10      EQU  10         REGISTER EQUATE FOR READABILITY
R11      EQU  11         REGISTER EQUATE FOR READABILITY
R12      EQU  12         REGISTER EQUATE FOR READABILITY
R13      EQU  13         REGISTER EQUATE FOR READABILITY
R14      EQU  14         REGISTER EQUATE FOR READABILITY
R15      EQU  15         REGISTER EQUATE FOR READABILITY
A3END    XRETURN SA=A3SAV
         END ,
/*
//GO.XREAD DD DSN=T90MES1.C464.DATA(ASN3DATA),DISP=SHR
//