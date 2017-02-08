//Z030199A JOB ,'Brandon Tweed'
/*JOBPARM ROOM=199
//         JCLLIB ORDER=(T90MES1.C464.PROCLIB) 
//STEP1    EXEC HLA464CG
//GO.XSNAPOUT DD UNIT=AFF=SYSPRINT
//ASM.SYSIN DD *
         Copy ASMMSP     Get IBM structured macro defs
*
SucNode  DSect ,             Successor in a relationship
Suc      DS    F             Index of this successor
Next     DS    A             Link to next successor in stack
*
CntNode  DSect ,             Table entry
Count    DS    F             Count of predecessors
         Org   Count         Redefine for use when Count is zero
QLink    DS    F             Link to next element in output queue
Top      DS    A             Top of successor stack
*
* Macro BASE to convert an index to an address
* Macro INDEX to convert an address to an index
         MACRO
         BASE &INDEX
         LA R7,CNTTBL        Get start of the table
         SLA &INDEX,3        Multiply index by 8
         AR &INDEX,R7        Add to get the address for the item
         MEND
TOPSORT  CSECT               Beginning of Topological Sort Routine
         XSAVE SA=TSSAV      Entry linkage the easy way
         PRINT NOGEN         Don't print macro code
         PGMDUMP             Shorten Dumps
*********1*****X***2*********3*****X***4*********5*********6*********7*
*
* Function:  This program is a solution to CSCI 464 Assignment 4,
*    Spring 2004.  It implements Knuth's Algorithm 2.2.3T
*    (Topological Sort).
*
***********************************************************************
*
* T1 - Initialize.
*    1 Input the value of n.
*      Set COUNT(k)<-0            (Defined with initial values.)
*      Set TOP(k)<-0 for 1=<k=<n. (Defined with initial values.)
*    2 Set N<-n.
*
***********************************************************************
*
         XREAD INPUT,80      Get the first input record      
         XDECI R5,INPUT      Read the zero value
         XDECI R5,0(,1)      Get n
         LA R6,CNTTBL        Get start of table
C        USING CntNode,R6    Set up table addressability
         LHI R4,1            Set k = 1
         LHI R7,0            User R7 for initializing table entries
         DO WHILE=(CR,R4,LE,R5)  Do While(k <= n)
         ST R7,C.Count       Save zero to COUNT(K)
         ST R7,C.Top         Save zero to TOP(K)
         AHI R6,8            Move to next entry in the count table
         AHI R4,1            k = k + 1
         ENDDO               EndDo
         DROP C
         ST R5,N             Set N <- n
*         
***********************************************************************
*
* T2 - Next Relation.
*    1 Get the next relation "j<k" from the input;
*    2 if the input has been exhausted, however, go to T4.
*
***********************************************************************
*    1 Get the next relation "j<k" from the input;
T2       XREAD INPUT,80
*    2 if the input has been exhausted, however, go to T4.
         BC B'0100',T4
         XDECI R5,INPUT      Get j
         XDECI R6,0(,1)      Get k
         ST R5,J
         ST R6,K
*
***********************************************************************
*
* T3 - Record the relation.
*    1 Increase COUNT(k) by one.
*    2 Set P<=AVAIL
*    3 Set SUC(P)<-k
*    4 Set NEXT(P)<-TOP(j)
*    5 Set TOP(j)<-P.
*    6 Go back to step T2
*
***********************************************************************
*
*    1 Increase COUNT(k) by one.
         L R8,K              Get k into R8
         BASE R8             Find Count(K) in the table
C        USING CntNode,R8    Set up table addressability
         LHI R9,1            For adding 1 to count(k)
         A R9,C.Count        Increase Count(k) by one
         ST R9,C.Count       Save the Result
         Drop C              Done with Count(k)
*    2 Set P<=AVAIL
         L R10,Avail         Get Avail
P        Using SucNode,R10   Set up Node Addressability
         MVC Avail,P.Suc     Avail <- P.Next (updates Avail)
*    3 Set SUC(P)<-k
         MVC P.Suc,K         SUC(P) <- K
*    4 Set NEXT(P)<-TOP(j)
         L R8,J              Get j into R8
         BASE R8             Find position j in table
         LR R7,R8
C        USING CntNode,R7    Set up addressability for TOP(j)         
         MVC P.Next,C.Top    Next(P) <- Top(j)                 
*    5 Set TOP(j)<-P.
         ST R10,C.Top        Since R10 = P
         Drop C
         Drop P
*    6 Go back to step T2
         B T2
***********************************************************************
*
* T4 - Scan for zeroes.
*    1 Set R<-0 and QLINK(0)<-0.
*    2 For 1=<k=<n, examine COUNT(k), and if it is zero,
*          set QLINK(R)<-k and R<-k.
*    3 After this has been done for all k, set F<-QLINK(0).
*
***********************************************************************
*
*    1 Set R<-0 and QLINK(0)<-0.
T4       XR R9,R9            Clear R9
         ST R9,R@            Set R <- 0
         LA R7,CNTTBL        Get the start of the table
C        Using CntNode,R7    Set up Count Table Addressability
         ST R9,C.QLink       Set QLINK(0) <- 0
         AHI R7,8            Move to Count(1)
*    2 For 1=<k=<n, examine COUNT(k), and if it is zero,
*          set QLINK(R)<-k and R<-k.
         LHI R3,1            Set k to 1
         DO WHILE=(C,R3,LE,N)
         L R6,C.Count        Get Count(k)->R6
         IF (ICM,R6,15,C.Count,Z) THEN
         L R4,R@             Get R
         LA R5,CNTTBL        Get Start of table
         SLA R4,3            Multiply R by 8
         AR R5,R4            Let R5 point to Count(R)
B        USING CntNode,R5    Set addressability for QLINK(R)
         ST R3,B.QLink       Set QLINK(R) <- k
         ST R3,R@            Set R <- k
         DROP B              Done with B
         ENDIF               Endif
         AHI R7,8            Move to Next Entry in Count table
         AHI R3,1            Increment k
         ENDDO               enddo
         DROP C              Done with C
*    3 After this has been done for all k, set F<-QLINK(0).
         LA R7,CNTTBL        Get the start of the table
C        USING CntNode,R7    Set up addressability
         MVC F@,C.QLink      Set F <- QLINK(0)
         DROP C              Done with C
***********************************************************************
*
* T5 - Output front of queue.
*    1 Output the value of F.
*    2 If F=0, go to T8;
*    3      otherwise, set N <- N-1
*    4                 set P<-TOP(F).
*
***********************************************************************
*
*    1 Output the value of F.
T5       L R3,F@             Get the value of F
         XDECO  R3,OUTPUT1   Put the value of F on the print line
         XPRNT  CC1,81       Print the line of output
*    2 If F=0, go to T8;
         IF (ICM,R3,15,F@,Z) THEN
         B T8                Go to T8
         ENDIF
*    3      otherwise, set N <- N-1
         L R5,N              Get N
         AHI R5,-1           Set N <- N-1
         ST R5,N             Save N
*    4                 set P<-TOP(F).
         BASE R3             Point R3 to entry F in table
F1       Using CntNode,R3    Set up table addressability
         MVC P@,F1.Top         R8 is P. Set it to Top(F)
         Drop F1             Done with F1
***********************************************************************
*
* T6 - Erase relations.
*    1 If P=0, go to T7.
*    2      Otherwise, decrease COUNT(SUC(P)) by one
*    3      if it has thereby gone down to zero,
*    4            set QLINK(R)<-SUC(P)
*    5            set R<-SUC(P).
*    6      Set P<-NEXT(P) and repeat.
*
***********************************************************************
*
*    1 If P=0, go to T7.
T6       ICM R8,15,P@
         BZ T7
*    2      Otherwise, decrease COUNT(SUC(P)) by one        
P        USING SucNode,R8
         L R5,P.Suc          Get SUC(P) into R5
         LR R4,R5            SUC(P) into R4
         BASE R5             Find address of SUC(P)
C        USING CntNode,R5    Set Addressability for COUNT(SUC(P))
         L R6,C.Count        Get Count(Suc(P))       
         AHI R6,-1           Decrement Count(Suc(P))
         ST R6,C.Count       Save new value for Count(Suc(P))
*    3      if it has thereby gone down to zero,
         IF (ICM,R6,15,C.COUNT,Z) THEN
*    4            set QLINK(R)<-SUC(P)   
         L R3,R@             Get R -> R3
         BASE R3             Find Item R in the Table
R        USING CntNode,R3    Set up addressability for QLINK(R)
         ST R4,R.QLINK       Set QLINK(R)<- SUC(P)
         DROP R
*    5            set R<-SUC(P). 
         ST R4,R@            Set R <-SUC(P)
         ENDIF
*    6      Set P<-NEXT(P) and repeat.
         L R8,P.Next
         ST R8,P@
         DROP P
         B T6                REPEAT STEP 6
***********************************************************************
*
* T7 - Remove from queue.
*      Set F<-QLINK(F) and go back to T5.
*
***********************************************************************
*
T7       L R3,F@             Get F
         BASE R3             Find Item F in the table
F        USING CntNode,R3    Set addressability up for F
         MVC F@,F.QLink      Set F <- QLINK(F)
         DROP F              Done working with F
         B T5                Go back to step T5
***********************************************************************
*
* T8 - End of process.  The algorithm terminates.  If N=0, we
*      have output all of the object numbers in the desired
*      "topological order," followed by a zero.  Otherwise,
*      the N object numbers not yet output contain a loop, in
*      violation of the hypothesis of partial order.
*
***********************************************************************
*
T8       XRETURN SA=TSSAV
         LTORG
****************************************************************
*  WORKING STORAGE
****************************************************************
*
J        DS    F
K        DS    F
P@       DS    A
F@       DS    F             The value for F
N        DS    F             The value for N
R@       DS    F             The value for R
INPUT    DS    CL80          Storage area for input record
*
CC1      DC    C'0'          Double space the output
OUTPUT1  DS    CL80          Print line storage area
*
CNTTBL   DS    52F           Table for storing counts
*
Avail    DC    A(*+4)        Avail pointer (top of avail stack)
         DC    24A(*+8,0)    Two words each, link in first
         DC    A(0,0)        Last node, with null link
*
R0       EQU   0         REGISTER EQUATE FOR READABILITY
*R1       EQU   1         REGISTER EQUATE FOR READABILITY
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
         END ,
/*
//GO.XREAD DD DSN=T90MES1.C464.DATA(ASN4DATA),DISP=SHR
//