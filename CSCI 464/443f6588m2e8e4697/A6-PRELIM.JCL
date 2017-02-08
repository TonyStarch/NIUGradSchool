//Z030199A JOB ,'Brandon Tweed'
/*JOBPARM ROOM=199
//         JCLLIB ORDER=(T90MES1.C464.PROCLIB)
//STEP1    EXEC HLA464CG
//GO.XSNAPOUT DD UNIT=AFF=SYSPRINT
//ASM.SYSIN DD *
         XSet XSave=OFF,XReturn=OFF,XSnap=OFF,XStop=OFF Tracing off
         Copy ASMMSP         Get IBM structured macro defs
         PRINT NOGEN         Don't Expand Macros
*
*********************************************************************
* Node DSect
*
* Used to define the structure of a Node within the binary search
* tree.
*
*********************************************************************
*
Node     DSect ,
Link     DS    0A                  To reference LLink or RLink
LLink    DS    A                   Left subtree pointer
RLink    DS    A                   Right subtree pointer
Key      DS    H                   Key of this node
B        DS    X                   Balance factor of this node
Length   DS    X                   Length of word in this node
Word     DS    CL12                Data for this node
*
*********************************************************************
* ParmList DSect (Used by InTrav)
*********************************************************************
ParmList DSect ,             Parameter List
Parm1    DS    A             Address of root, or zero
Parm2    DS    A             Address of print queue vector
*********************************************************************
* PBuf DSect (Used by InTrav)
*********************************************************************
PBuf     DSect ,             Print buffer
PLink    DS    A             Link field (in Queue and in Avail list)
PCC      DS    C             Carriage control
PText    DS    CL80          Print text (80 bytes)
PTextEnd EQU   *             End of text
PBufEnd  DS    0D            Round up to doubleword
PLen     EQU   *-PBuf        Length of print buffer
*********************************************************************
* PrQD DSect (Used by InTrav)
*********************************************************************
PrQD     DSect ,             Print queue values
PrQF     DS    A             Front of print queue, initially empty
PrQR     DS    A             Rear of print queue, initially empty
PrQB     DS    F             Bytes left in current buffer, init. zero
PrQA     DS    A             Top of buffer Avail stack, init. empty
*
*********************************************************************
* WorkArea DSect (Used by InTrav)
*********************************************************************
WorkArea DSECT ,             WorkArea for InTrav
         DS 18F              SaveArea
WorkParm DS 2A
WorkLen  EQU *-WorkArea      Calculates length of WorkArea
*
*********************************************************************
* AvPool Macro
* Note: Works because Node size works conveniently with 
*       boundary alignments. If the Node size were to change,
*       the macro would need modification.
*********************************************************************
*
         MACRO
&Name    AvPool &Num,&Size
         LCLA &NUMLEFT,&LEFT
&LEFT    SETA &Size-4
&NUMLEFT SETA &Num
&Name    DC A(*+16)
         DC CL12'Avail Stack '
.LOOP    DC A(*+&Size)
         DC &LEFT.X'00'
&NUMLEFT SETA &NUMLEFT-1
         AIF  (&NUMLEFT GT 1).LOOP
         DC A(0)
         DC &LEFT.X'00'
         MEND
*
*********************************************************************
* AvPop Macro
*
* Removes the top node from the avail stack defined by AvPool
*
*********************************************************************
*
         MACRO
&Name    AvPop &Avail,&Reg=1
         ICM &Reg,15,&Avail  Get contents of Avail pointer, set CC
         BZ ND&SYSNDX        If Avail is Null, AvPool Empty
Top      Using Node,&Reg     Set up Node Addressability
         MVC &Avail,Top.Link Update Avail 
         Drop Top            Dirtiest line of assembler ever written
ND&SYSNDX DS 0H              Label for Skipping acquiring a node
         MEND
         EJECT
*
*********************************************************************
* ASSIGN6
*
* Logic:
*
*  [1] Perform standard initialization
*  [2] Read first input record, containing a key and a word
*  [3] Convert key to binary and place in Node1
*  [4] Calculate the length of the word
*  [5] Place word and word length in Node1, initializing the tree
*  [6] Read the second input record
*  [7] Do While (NOT End-of-File)
*  [8]    Convert key to binary and place in parameter list
*  [9]    Calculate the length of the word
* [10]    Place word and word length in parameter list
* [11]    Call BalTreSI to insert the node & balance the tree
* [12]    Read the next input record
* [13] End Do
* [14] Call InTrav to display the tree
* [15] Print buffer queue
* [16] Print a message showing the height of the tree
* [17] Perform standard termination
*
* REGISTER USAGE:
* R2  - Used as a scratch register
* R3  - Used in Converting Key to Binary
* R8  - Holds start of word for length calculation
* R9  - Addressability to the Node
* R10 - 
*********************************************************************
*
*  [1] Perform standard initialization
*
ASSIGN6  XSAVE OPT=CSECT,TR=NO   Start of Driver for assignment 6
         PGMDUMP
*
*  [2] Read first input record, containing a key and a word
*
         XREAD INPUT,80
*
*  [3] Convert key to binary and place in Node1
*
         XDECI 3,INPUT       Convert Key to Binary
         LA 9,Node1          Get @ Node1 into R9
         Using Node,9        Establish Node Addressability
         STH 3,Key           Save Key Into Node1
*
*  [4] Calculate the length of the word
*
         AHI 1,1             Move to start of word in input record
         LR 8,1              Copy @ word start into 8
         TRT 0(76,1),TBL1    Perform TRT starting at where R1 points
         SR 1,8              Calculate length of word
*
*  [5] Place word and word length in Node1, initializing the tree
*
         STC 1,Length        Save length into Node1
         MVC Word(12),0(8)   Copy the Word into Node1
         XR 2,2              Clear 2
         LHI 2,8             8 represents a zero BF
         STC 2,B             Save the BF to Node1
         Drop 9              Done working with Node1
*
*  [6] Read the second input record
*
         XREAD INPUT,80
*
*  [7] Do While (NOT End-of-File)
*
         DO WHILE=(Z)       While the CC=0 (Record read success)
*
*  [8]    Convert key to binary and place in parameter list
*
         XDECI 3,INPUT      Convert key to binary
         STH 3,PKEY         Place it in the parameter list
*
*  [9]    Calculate the length of the word
*
         AHI 1,1            Move to start of word in INPUT
         LR 8,1             Copy @ word start into 8
         TRT 0(76,1),TBL1   Find end of word
         SR 1,8             Calculate Word Length
*
* [10]    Place word and word length in parameter list
*
         STC 1,PWRDLEN      Save Length to Parm List
         MVC PWRD(12),0(8)  Move word into parameter list
*
* [11]    Call BalTreSI to insert the node & balance the tree
*
         LA 1,Parms1         Pass the parameter list
         XCALL BalTreSI      Call BalTreSI
*
* [12]    Read the next input record
*
         XREAD INPUT,80
*
* [13] End Do
*
         ENDDO   
*
* [14] Call InTrav to display the tree
*
*         L 0,InitRoot        Get @ of Root of Tree
*         ST 0,InTParms       Save it to the Parameter List
*         LA 1,InTParms       Get Parms for InTrav
*         L 15,=V(InTrav)     Get @ of InTrav
*         BASR 14,15          Call InTrav
*
* [15] Print buffer queue
*
*         LA 9,PrQ            Get @ of the Print Queue
*         Using PrQD,9        Set up Addressability to the Print Queue
*         DO WHILE=(ICM,10,15,PrQF,NZ)
*         USING PBuf,10       Set up buffer addressability
*         XPRNT PCC,81        Print contents of the buffer
*         LR 1,10             Put address of old buf into R1
*         L 10,PLink          Get @ of next buffer
*         ST 10,PrQF          Save @ of next buffer to front
*         STORAGE RELEASE,LENGTH=PLen,ADDR=(1) Release old buffer
*         EndDo
*         Drop 10             Done with buffer
*         Drop 9              Done with PrQ
*
* [16] Print a message showing the height of the tree
*
         L 2,Height          Get Height from Head Node
         XDECO 2,THEIGHT     Convert it for output
         XPRNT OUTPUT1,29    Print the Message
*
* [17] Perform standard termination
*
         XRETURN SA=*,TR=NO  End of ASSIGN6 Routine
         LTORG
*
*********************************************************************
* ASSIGN6 WORKING STORAGE
*********************************************************************
*
* Print Queue
*
PrQ      DC    A(0)          Print queue front pointer (empty)
         DC    A(PrQ)        Print queue rear pointer (empty)
         DC    F'0'          Bytes remaining in current print buffer
         DC    A(0)          Top of avail stack (initially empty)
*         
InTParms DC    A(0)          Address of root of tree to traverse
         DC    A(PrQ)        Address of print queue and other fields
*
* Parameter List for BalTrSI
*
Parms1   DC    A(Head)       Address of Tree Head
         DC    A(PKEY)       Address of key of word to be inserted
         DC    A(PWRDLEN)    Address of length of word
         DC    A(PWRD)       Address of the word to insert
*
* Parmlist Data for BalTrSI
*
PKEY     DS    H             Parameter List - Key
PWRDLEN  DS    X             Parameter List - Word Length
PWRD     DS    CL12          Parameter List - Word
*
INPUT    DS    CL80          Storage area for input record
*
OUTPUT1  DC    C' '          Single space
         DC    CL16'HEIGHT OF TREE: '   
THEIGHT  DS    CL12         
*
TBL1     DC    256X'00'      Table to find blank
         ORG   TBL1+C' '     Locate TBL1 + X'40'
         DC    X'FF'         Stop when blank found
         ORG   ,
*
* Definition for Head Node of Tree
*
Head     DS    0D                  Head node of tree (nice alignment)
Height   DC    F'1'                Initial height (with first node)
InitRoot DC    A(Node1)            Pointer to initial root
*                                  Remaining fields not needed
*
* Definition of Root Node of Tree
*
Node1    DS    24X'00'             Initialize to all zeros
*
         EJECT
         EJECT
*********************************************************************
* BalTreSI
*
* **** A1 [initialize] ****
* [1] Set T <- HEAD
* [2] Set P <- RLINK(HEAD)
* [3] Set S <- P
*
* **** A2 [compare] ****
* [4]  IF K < KEY(P)
* [5]     goto A3
* [6]  ENDIF
* [7]  IF K > KEY(P)
* [8]      goto A4
* [9]  ENDIF
* [10] IF K = KEY(P)
* [11]    Terminate search successfully(?)
* [12] ENDIF
* 
* **** A3 [Move left.] ****
* 
* [13] Set Q <- LLINK(P)
* [14] IF Q = Null 
* [15]    Q <= AVAIL
* [16]    LLINK(P) = Q
* [17]    goto step A5
* [18] ELSE
* [19]    IF B(Q) NOT = Zero
* [20]        set T <- P
* [21]        set S <- Q
* [22]    ENDIF
* [23] ENDIF
* [24] set P <- Q
* [25] Goto step A2
* 
* **** A4 [Move right] ****
* 
* [26] Set Q <- RLINK(P)
* [27] If Q = null
* [28]     Q <= AVAIL
* [29]     RLINK(P) <- Q
* [30]     goto step A5
* [31] ELSE
* [32]    IF B(Q) NOT = Zero
* [33]        set T <- P
* [34]        set S <- Q
* [35]    ENDIF
* [36] ENDIF
* [37] set P <- Q
* [38] Goto step A2
* 
* **** A5 [Insert] ****
* 
* [39] KEY(Q) <- K
* [40] LLINK(Q) <- null
* [41] RLINK(Q) <- null
* [42] B(Q) <- 0
* 
* 
* **** A6 [adjust balance factors] ****
* 
* [43] IF K < KEY(S)
* [44]   a <- -1
* [45] ELSE
* [46]   a <- +1
* [47] ENDIF
* [48] P <- LINK(a,S)
* [49] R <- P
* [50] Do WHILE ( P not = Q)
* [51]    IF K < KEY(P)
* [52]        B(P) = -1
* [53]        P <- LLINK(P)
* [54]    ENDIF
* [55]    IF K > KEY(P)
* [56]        B(P) <- +1
* [57]        P <- RLINK(P)
* [58]    ENDIF
* (deleted) [59]    ENDIF
* [60] ENDDO
* 
* **** A7 [Balancing act] ****
* 
* [61] IF B(S) = 0
* [62]    B(S) <- a
* [63]    LLINK(HEAD) <- LLINK(HEAD) + 1
* [64]    terminate algorithm
* [65] ENDIF
* [66] IF B(S) = -a
* [67]    B(S) <- 0
* [68]    terminate algorithm
* [69] ENDIF
* [70] IF B(S) = a
* [71]    IF B(R) = a
* [72]        goto step A8
* [73]    ENDIF
* [74]    IF B(R) = -a
* [75]        goto step A9
* [76]    ENDIF
* [77] ENDIF
* 
* **** A8 [single rotation] ****
* 
* [78] P <- R
* [79] LINK(a,S) <- LINK(-a,R)
* [80] LINK(-a,R) <- S
* [81] B(S) <- 0
* [82] B(R) <- 0
* [83] goto step A10
* 
* **** A9 [Double rotation] ****
* 
* [84]  P <- LINK(-a,R)
* [85]  LINK(-a,R) <- LINK(a,P)
* [85]  LINK(a,P) <- R
* [86]  LINK(a,S) <- LINK(-a,P)
* [87]  LINK(-a,P) <- S
* [88]  IF B(P) = a
* [89]     B(S) <- -a
* [90]     B(R) <- 0
* [91]  ELSE
* [92]     IF B(P) = 0
* [93]         B(S) <- 0 
* [94]         B(R) <- 0
* [95]     ELSE
* [96]         IF B(P) = -a
* [97]             B(S) <- 0
* [98]             B(R) <- a
* [99]         ENDIF
* [100]    ENDIF
* [101] ENDIF
* [102] B(P) <- 0
* 
* **** A10 [finishing touch] ****
* 
* [103] IF S = RLINK(T)
* [104]     RLINK(T) <- P
* [105] ELSE
* [106]     LLINK(T) <- P
* [107] ENDIF
* 
*********************************************************************
*
*********************************************************************
* REGISTER USAGE:
*
* R5  - a
* R6  - (-a)
* R7  - Q
* R8  - K
* R9  - T/R
* R10 - P
* R11 - S
* R12 - Base Register for Routine Addressability
*
*********************************************************************
BalTreSI XSAVE SA=SAV1,OPT=CSECT
*
* **** A1 [initialize] **********************************************
*
* INCOMING REGISTER STATUS:
* none
*
* REGISTER USAGE:
*
* R2  - Used as a scratch pad
* R8  - Used to hold value of K
* R9  - Used to get A(Head) from Parameter List
* R10 - Allocated to Hold value for P
* R11 - Allocated to Hold value for S
*
*********************************************************************
*
         L 2,4(,1)           Get @ Key
         XR 8,8              Clear R8
         LH 8,0(,2)          Get K into 8
         L 2,8(,1)           Get @ word length into 2
         IC 2,0(,2)          Get Length into 2
         STC 2,INWRDLEN      Save length to local storage area
         L 2,12(,1)          Get @ word into 2
         MVC INWRD(12),0(2)  Move Word to local storage area
*         
* [1] Set T <- HEAD
*
         L 9,0(,1)           Get A(Head) into 9
         ST 9,T@             Save A(Head) into T
*
* [2] Set P <- RLINK(HEAD)
*
         L 10,4(,9)          Get InitRoot into 10
*
* [3] Set S <- P
*
         LR 11,10            Set 11 to same value as P, which is 10
*
* **** A2 [compare] *************************************************
*
* INCOMING REGISTER STATUS:
*
* R7  - Value of Q ( Allocated in steps A3 and A4 ) 
* R8  - Holds value of K
* R10 - Value of P
* R11 - Value of S
* 
* REGISTER USAGE:
*
* R2  - Scratch pad register
*
*********************************************************************
A2       DS    0H            Label used for branching back to A2
         XSnap Label='*** A2 Entered ***'
P        Using Node,10
*
* [4]  IF K < KEY(P)
*
         IF (CH,8,LT,P.KEY) THEN
*
* [5]     goto A3
*
         B A3
*
* [6]  ENDIF
*
         ENDIF
*
* [7]  IF K > KEY(P)
*
         IF (CH,8,GT,P.KEY) THEN
*
* [8]      goto A4
*
         B A4
*
* [9]  ENDIF
*
         ENDIF
*
* [10] IF K = KEY(P)
*
         IF (CH,8,EQ,P.KEY) THEN
*
* [11]    Terminate search successfully(?)
*
         B SUCCESS
*
* [12] ENDIF
*
         ENDIF
         Drop P
*
* **** A3 [Move left.] **********************************************
*
* INCOMING REGISTER STATUS:
* R8  - Used to Hold K
* R10 - Value of P
* R11 - Value of S
*
* REGISTER USAGE:
* R6 - Scratch pad register
* R7 - Allocated for Q
*
*********************************************************************
A3       DS    0H
         XSnap Label='*** A3 Entered ***'
P        USING Node,10
Q        USING Node,7
* 
* [13] Set Q <- LLINK(P)
*
         L 7,P.LLINK
*
* [14] IF Q = Null
*
         XR 6,6              Set R6 to null value
         IF (CR,7,EQ,6) THEN
*
* [15]    Q <= AVAIL
*
         PRINT GEN
         AvPop AVAIL         Get node from AVAIL stack
         PRINT NOGEN
         IF (Z) THEN          If Condition Code was zero
         EX 0,*              Cause a SOC3 ABEND
         ENDIF
         LR 7,1              Get @ of new node into R7 (Q)
*
* [16]    LLINK(P) = Q
*
         ST 7,P.LLINK        Save Q to LLINK(P)
*
* [17]    goto step A5
*
         B A5
*
* [18] ELSE
*
         ELSE
*
* [19]    IF B(Q) NOT = Zero
* correct this
*Q        USING Node,7        Set up addressability to Node Q
         XR 6,6              Clear R6
         LHI 6,8             8 represents BF of zero
         IF (CLM,6,B'0001',NE,Q.B) THEN
*
* [20]        set T <- P
*
         ST 10,T@
*
* [21]        set S <- Q
*
         LR 11,7             Copy Q into S
*
* [22]    ENDIF
*
         ENDIF
*
* [23] ENDIF
*
         ENDIF
*
* [24] set P <- Q
*
         LR 10,7
*
* [25] Goto step A2
*
         B A2
         Drop Q
         Drop P
*
* **** A4 [Move right] **********************************************
* 
* INCOMING REGISTER STATUS:
* R8  - Used to Hold K
* R10 - Value of P
* R11 - Value of S
*
* REGISTER USAGE:
*
* R6 - Used as a scratch pad register
* R7 - Allocated for Value of Q
*
*********************************************************************
*
A4       DS    0H
         XSnap Label='*** A4 Entered ***'
P        USING Node,10
Q        USING Node,7
*
* [26] Set Q <- RLINK(P)
*
         L 7,P.RLINK
*
* [27] If Q = null
*
         XR 6,6              Clear R6
         IF (CR,6,EQ,7) THEN
*
* [28]     Q <= AVAIL
*
         PRINT GEN
         AvPop AVAIL
         PRINT NOGEN
         IF (Z) THEN          If CC=0 after AvPop
         EX 0,*              Crash program with SOC3 ABEND
         ENDIF               EndIf
         LR 7,1              Set Q to address of retrieve node
*
* [29]     RLINK(P) <- Q
*
         ST 7,P.RLINK        save current value of Q into RLINK(P)
*
* [30]     goto step A5
*
         B A5                go to step A5
*
* [31] ELSE
*
         ELSE
*
* [32]    IF B(Q) NOT = Zero
* 
*Q        USING Node,7
         XR 6,6              Clear R6
         LHI 6,8             8 represents BF of zero
         IF (CLM,6,B'0001',NE,Q.B) THEN
*
* [33]        set T <- P
*
         ST 10,T@            Save P to T
*
* [34]        set S <- Q
*
         LR 11,7             Copy Q into S
*
* [35]    ENDIF
*
         ENDIF
*
* [36] ENDIF
*
         ENDIF
*
* [37] set P <- Q
*
         LR 10,7             copy Q into P
*
* [38] Goto step A2
*
         B A2 
         Drop Q              Done with Q for this step
         Drop P              Done with P for this step
*
***** A5 [Insert] ***************************************************
*
* INCOMING REGISTER STATUS:
*
* R7  - Value of Q
* R8  - Used to Hold K
* R10 - Value of P
* R11 - Value of S
*
* REGISTER USAGE:
*
* R6  - Used as a scratch register
* 
*********************************************************************
A5       DS    0H
         XSnap Label='*** A5 Entered ***'
Q        Using Node,7        Establish addressability for Q
*
* [39] KEY(Q) <- K
* note: also need to copy word and word length into Q
*
         XR 5,5
         IC 5,INWRDLEN       Get word length
         STC 5,Q.Length      Save length into Q
         LA  5,INWRD         Get address of word
         MVC Q.WORD(12),0(5) Copy word into Q
         STH 8,Q.Key         Save K into Key(Q)
*
* [40] LLINK(Q) <- null
*
         XR 6,6              Clear 6
         ST 6,Q.LLINK        set LLINK(Q) to null
*
* [41] RLINK(Q) <- null
*
         ST 6,Q.RLINK        set RLINK(Q) to null
*
* [42] B(Q) <- 0
*
         LHI 6,8             8 represents a BF of zero
         STC 6,Q.B           set B(Q) to BF of zero
         Drop Q              Done with Q for this stemp
         EJECT
* 
* **** A6 [adjust balance factors] **********************************
* 
* INCOMING ACTIVE REGISTERS:
* R7  - VALUE FOR Q
* R8  - VALUE FOR K
* R10 - VALUE FOR P
* R11 - VALUE FOR S
*
* REGISTER USAGE
*
* R0  - Used as a scratch register
* R5  - Allocated for use as "a"
* R6  - Allocated for use as "-a"
* R9  - Allocated to hold R
*
*********************************************************************
A6       DS    0H            Label to Mark beginning of A6
         XSnap Label='*** A6 Entered ***'
S        USING Node,11       Establish addressability for S
P        USING Node,10       Establish addressability for P
         XR 5,5              Clear 5 for allocation
         XR 6,6              Clear 6 for allocation
*
* [43] IF K < KEY(S)
*
         IF (CH,8,LT,S.KEY) THEN
*
* [44]   a <- -1
*
         LHI 5,0             use zero instead of -1
         LHI 6,4             use 4 instead of +1
*         
* [45] ELSE
*
         ELSE
*
* [46]   a <- +1
*
         LHI 5,4             use 4 instead of +1
         LHI 6,0             use 0 instead of -1
*
* [47] ENDIF
*
         ENDIF
*
* [48] P <- LINK(a,S)
*
         L 10,S.LINK(5)      Update P with Link(a,S)
*
* [49] R <- P
*
         LR 9,10            Copy P into R (use 9 for R)
*
* [50] Do WHILE ( P not = Q)
*
         DO WHILE=(CR,10,NE,7)
*
* [51]    IF K < KEY(P)
*
         IF (CH,8,LT,P.Key) THEN
*
* [52]        B(P) = -1
*
         XR 0,0              Clear reg 0 (zero represents -1 BF)
         STC 0,P.B           Save BF of -1 into B(P)
*
* [53]        P <- LLINK(P)
*
         L 10,P.LLINK        Update P with LLINK(P)
*
* [54]    ENDIF
*
         ENDIF
*
* [55]    IF K > KEY(P)
*
         IF (CH,8,GT,P.Key) THEN
*
* [56]        B(P) <- +1
*
         LHI 0,4             BF of +1 represented by 4
         STC 0,P.B           Set B(P) to +1
*
* [57]        P <- RLINK(P)
*
         L 10,P.RLink        Update P with RLINK(P)
*
* [58]    ENDIF
*
         ENDIF
*
* [60] ENDDO
*   
         ENDDO
         Drop S              Done with S for the moment
         Drop P              Done with P for this step
*
* **** A7 [Balancing act] *******************************************
*
* INCOMING ACTIVE REGISTERS:
*
* R5  - Value for a
* R6  - Value for -a
* R7  - Value for Q (not used in this step)
* R8  - Holds value for K
* R9  - VALUE FOR R
* R10 - VALUE FOR P 
* R11 - VALUE FOR S
*
* REGISTER USAGE:
*
* R2 - Scratch Pad Register to hold value for BF of zero
* R3 - Used to gain addressability to Head Node
*
*********************************************************************
A7       DS    0H
         XSnap Label='*** A7 Entered ***'
S        USING Node,11       Addressability for Node S
R        USING Node,9
*
* [61] IF B(S) = 0
*
         XR 2,2              Clear R2
         LHI 2,8             8 represents a BF of zero
         IF (CLM,2,B'001',EQ,S.B) THEN
*
* [62]    B(S) <- a
*
         STC 5,S.B           set B(S) to a
*
* [63]    LLINK(HEAD) <- LLINK(HEAD) + 1
*
         L 3,T@              Get Addr HEAD
H        USING Node,3        Set up addressability to Head Node
         L 2,H.LLink         Get LLINK(HEAD)
         AHI 2,1             LLINK(HEAD) <- LLINK(HEAD) + 1
         ST 2,H.LLink        Save the Changes
         Drop H
*
* [64]    terminate algorithm
*
         B SUCCESS           terminates the algorithm
*
* [65] ENDIF
*
         ENDIF
*
* [66] IF B(S) = -a
*
         IF (CLM,6,B'0001',EQ,S.B) THEN
*
* [67]    B(S) <- 0
*
         XR 2,2              Clear R2
         LHI 2,8             8 represents BF of zero
         STC 2,S.B           Set B(S) to Zero
*
* [68]    terminate algorithm
*
         B SUCCESS           terminates the algorithm
*
* [69] ENDIF
*
         ENDIF
*
* [70] IF B(S) = a
*
         IF (CLM,5,B'0001',EQ,S.B) THEN
*
* [71]    IF B(R) = a
*
         IF (CLM,5,B'0001',EQ,R.B) THEN
*
* [72]        goto step A8
         B A8
*
* [73]    ENDIF
*
         ENDIF
*
* [74]    IF B(R) = -a
*
         IF (CLM,6,B'0001',EQ,R.B) THEN
*
* [75]        goto step A9
*
         B A9
*
* [76]    ENDIF
*
         ENDIF
*
* [77] ENDIF
*
         ENDIF
         DROP S
         DROP R
         EJECT
*
* **** A8 [single rotation] *****************************************
*
* INCOMING ACTIVE REGISTERS:
*
* R5  - Value of a
* R6  - Value of -a
* R7  - Value for Q
* R8  - VALUE FOR K
* R9  - Value of R 
* R10 - Value of P
* R11 - Value of S
*
* REGISTER USAGE:
*
* R0  - Scratch Pad Register
* R2  - Scratch Pad Register
*********************************************************************
* 
A8       DS    0H
         XSnap Label='*** A8 Entered ***'
R        USING Node,9        Set up Addressability to R
S        USING Node,11       Set up Addressability to S
*
* [78] P <- R
*
         LR 10,9             Set P to Value of R
*
* [79] LINK(a,S) <- LINK(-a,R)
*
         L 0,R.LINK(6)       Get LINK(-a,R)
         ST 0,S.LINK(5)      Save it into LINK(a,S)
*
* [80] LINK(-a,R) <- S
*
         ST 11,R.LINK(6)
*
* [81] B(S) <- 0
*
         XR 2,2              Clear 2
         LHI 2,8             We use 8 to represent balance factor 0
         STC 2,S.B           Set B(S) to zero balance factor
*
* [82] B(R) <- 0
*
         STC 2,R.B           Set B(R) to zero balance factor
*
* [83] goto step A10
*
         B A10
         DROP R
         DROP S
         EJECT
*
* **** A9 [Double rotation] *****************************************
* 
* INCOMING ACTIVE REGISTERS:
*
* R5  - Value of a
* R6  - Value of -a
* R7  - VALUE FOR Q
* R8  - VALUE FOR K
* R9  - Value of R 
* R10 - Value of P
* R11 - Value of S
*
* REGISTER USAGE:
*
* R0  - Scratch Pad Register
* R2  - Scratch Pad Register
* R3  - Scratch Pad Register
*
*********************************************************************
A9       DS    0H
         XSnap Label='*** A9 Entered ***'
R        USING Node,9        Set up addressability for R
P        USING Node,10       Set up addressability for P
S        Using Node,11       Set up addressability for S
*
* [84]  P <- LINK(-a,R)
*
         L 10,R.LINK(6)      Set P to LINK(-a,R)
*
* [85]  LINK(-a,R) <- LINK(a,P)
*
         L 0,P.LINK(5)
         ST 0,R.LINK(6)
*
* [85]  LINK(a,P) <- R
*
         ST 9,P.LINK(5)
*
* [86]  LINK(a,S) <- LINK(-a,P)
*
         L 0,P.LINK(6)       Get LINK(-a,P)
         ST 0,S.LINK(5)      Save it to LINK(a,S)
*
* [87]  LINK(-a,P) <- S
*
         ST 11,P.LINK(6)     Save S into LINK(-a,P)
*
* [88]  IF B(P) = a
*
         XR 3,3              Clear 3
         IC 3,P.B            Get B(P) into 3
         IF (CR,3,EQ,5) THEN
*
* [89]     B(S) <- -a
*
         STC 6,S.B          Set B(S) to -a
*
* [90]     B(R) <- 0
*
         XR 2,2              Clear 2
         LHI 2,8             8 represents balance factor zero
         STC 2,R.B           Set B(R) to Zero
*
* [91]  ELSE
*
         ELSE
*
* [92]     IF B(P) = 0
*
         XR 3,3              Clear 3
         XR 2,2              Clear 2
         LHI 2,8             8 represents balance factor zero
         IC 3,P.B            Get B(P) into 3
         IF (CR,2,EQ,3) THEN
*
* [93]         B(S) <- 0 
*
         XR 2,2              Clear 2
         LHI 2,8             8 represents balance factor zero
         STC 2,S.B           Save zero to B(S)
*
* [94]         B(R) <- 0
*
         STC 2,R.B           Save zero to B(R)
*
* [95]     ELSE
*
         ELSE
*
* [96]         IF B(P) = -a
*
         XR 3,3              Clear 3
         IC 3,P.B            Put B(P) into 3
         IF (CR,3,EQ,6) THEN  See if B(P) = -a
*
* [97]             B(S) <- 0
*
         XR 2,2              Clear 2
         LHI 2,8             Use value of 8 for BF zero
         STC 2,S.B           Set B(S) to zero
*
* [98]             B(R) <- a
*
         STC 5,R.B           Set B(R) to a
*
* [99]         ENDIF
*
         ENDIF
*
* [100]    ENDIF
*
         ENDIF
*
* [101] ENDIF
*
         ENDIF
*
* [102] B(P) <- 0
*
         XR 2,2              Clear 2
         LHI 2,8             8 represents balance factor zero
         STC 2,P.B           Set B(P) to zero
         Drop P
         Drop R
         Drop S
*
***** A10 [finishing touch] *****************************************
* 
* INCOMING ACTIVE REGISTERS:
*
* R5  - Value of a   (not needed)
* R6  - Value of -a  (not needed)
* R7  - VALUE FOR Q  (not needed)
* R8  - VALUE FOR K  (no longer needed)
* R9  - Value of R   (no longer needed)
* R10 - Value of P
* R11 - Value of S
*
*********************************************************************
*
A10      DS    0H
         XSnap Label='*** A10 Entered ***'
         L 9,T@              Use R9 for T
T        USING Node,9        Set up addressability for T
*
* [103] IF S = RLINK(T)
*
         IF (C,11,EQ,T.RLINK) THEN
*
* [104]     RLINK(T) <- P
*
         ST 10,T.RLINK
*
* [105] ELSE
*
         ELSE
*
* [106]     LLINK(T) <- P
*
         ST 10,T.LLINK
*
* [107] ENDIF
*
         ENDIF
SUCCESS  XRETURN SA=SAV1
         LTORG
*********************************************************************
* BalTrSI Working Storage
*********************************************************************
*         PRINT GEN

INHEAD   DS    A
INWRDLEN DS    X             Length of Word to be inserted
INWRD    DS    CL12          The word being inserted
*
T@       DS    A             Storage space for T
Q@       DS    A             Storage space for Q
R@       DS    A             Storage space for R
P@       DS    A             Storage space for P
S@       DS    A             Storage space for S
*
AVAIL    AvPool 270,24       Call AvPool to create AVAIL Stack
         END ,
/*
//GO.XREAD DD DSN=T90MES1.C464.DATA(ASN6DATA),DISP=SHR
//
