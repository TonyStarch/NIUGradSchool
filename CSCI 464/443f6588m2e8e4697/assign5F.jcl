//Z030199A JOB ,'Brandon Tweed'
/*JOBPARM ROOM=199
//         JCLLIB ORDER=(T90MES1.C464.PROCLIB)
//STEP1    EXEC HLA464CG
//GO.XSNAPOUT DD UNIT=AFF=SYSPRINT
//ASM.SYSIN DD *
********************************************************************      
* BRANDON TWEED
* ASSIGNMENT 5
* INSTRUCTOR: STACK
* TA: BESSEE
* DUE: 3-25-2004
*
* DESCRIPTION:
*
* This program implements an inorder traversal of a binary tree.
* Each node of this tree contains a word from Lincoln's famous 
* address at Gettysburg. During the visitation of each node,
* words are added to a queue of print buffers. Once the traversal
* is completed, the queue of print buffers is printed, resulting
* in a listing of the entire address.
*
********************************************************************
         Copy ASMMSP         Get IBM structured macro defs
         PRINT NOGEN         Don't Expand Macros
ParmList DSect ,             Parameter List
Parm1    DS    A             Address of root, or zero
Parm2    DS    A             Address of print queue vector
*
PBuf     DSect ,             Print buffer
PLink    DS    A             Link field (in Queue and in Avail list)
PCC      DS    C             Carriage control
PText    DS    CL80          Print text (80 bytes)
PTextEnd EQU   *             End of text
PBufEnd  DS    0D            Round up to doubleword
PLen     EQU   *-PBuf        Length of print buffer
*
Node     DSect
LLink    DS    A             Address of left subtree, if any
RLink    DS    A             Address of right subtree, if any
Length   DS    X             Number of characters in Word
Word     DS    0C            Start of the word this node contains
*
PrQD     DSect ,             Print queue values
PrQF     DS    A             Front of print queue, initially empty
PrQR     DS    A             Rear of print queue, initially empty
PrQB     DS    F             Bytes left in current buffer, init. zero
PrQA     DS    A             Top of buffer Avail stack, init. empty
*
WorkArea DSECT ,             WorkArea for InTrav
         DS 18F              SaveArea
WorkParm DS 2A
WorkLen  EQU *-WorkArea      Calculates length of WorkArea
         EJECT
********************************************************************
* A5DRIVER
*
* Description: This is the driver routine that calls InTrav to 
*              perform the traversal of the tree. After the tree
*              has been traversed, the resulting queue of print
*              buffers is printed and each buffer's memory 
*              is sequentially returned to the operating sytem.
*
* INPUT: none
*
* OUTPUT: Contents of queue of print buffers (Gettysburg Address)
*
* ENTRY CONDITIONS: None to speak of
*
* EXIT CONDITIONS: None to speak of
*
* REGISTER USAGE:
*    R1  - Holds address of print queue buffer being released
*    R9  - Used for addressability of PrQD (the print queue)
*    R10 - Holds print queue front pointer (PrQF)
*            
* LOGIC:
*
*    [1] Call InTrav, passing root of tree and address of print
*        queue buffer
*    [2] Do While (Print Queue Not Empty)
*    [3]    Print Contents of Buffer at Front of the Queue
*    [4]    Remove the buffer from the front of the queue
*    [5]    Give the buffer's memory back to the OS
*    [6] EndDo
*
********************************************************************
A5DRIVER XSAVE OPT=CSECT,TR=NO   Start of Driver for assignment 5
         PGMDUMP
*
*    [1] Call InTrav, passing root of tree and address of print
*        queue buffer
*
         LA 1,Parms          Set R1 point to Parm list
         L 15,=V(InTrav)     Put address of InTrav in R15
         BASR 14,15          Call InTrav
*
*    [2] Do While (Print Queue Not Empty)
*
         LA 9,PrQ            Get @ of the Print Queue
         Using PrQD,9        Set up Addressability to the Print Queue
         DO WHILE=(ICM,10,15,PrQF,NZ)
*
*    [3]    Print Contents of Buffer at Front of the Queue
*
         USING PBuf,10       Set up buffer addressability
         XPRNT PCC,81        Print contents of the buffer
*
*    [4]    Remove the buffer from the front of the queue
*
         LR 1,10             Put address of old buf into R1
         L 10,PLink          Get @ of next buffer
         ST 10,PrQF          Save @ of next buffer to front
*
*    [5]    Give the buffer's memory back to the OS
*
         STORAGE RELEASE,LENGTH=PLen,ADDR=(1) Release old buffer
         Drop 10             Done with buffer
*
*    [6] EndDo
*
         ENDDO               EndDo
         Drop 9              Done with Print Queue
         XRETURN SA=*,TR=NO  End of A5DRIVER Routine
         LTORG
********************************************************************
* A5DRIVER WORKING STORAGE
********************************************************************
PrQ      DC    A(0)          Print queue front pointer (empty)
         DC    A(PrQ)        Print queue rear pointer (empty)
         DC    F'0'          Bytes remaining in current print buffer
         DC    A(0)          Top of avail stack (initially empty)
*
         Extrn TREE          External data for the tree
Parms    DC    A(TREE)       Address of root of tree to traverse
         DC    A(PrQ)        Address of print queue and other fields
*
         EJECT
********************************************************************
* INTRAV
*
* FUNCTION:
* Recursively traverses a binary tree in symmetric order.
* (Traverses the tree using an "inorder" traversal)
*
* INPUT: none
*
* OUTPUT: none
*
* ENTRY CONDITIONS:
*        R1 points to a parameter list, which is described below:
*        0(,R1) - Address of the root for a tree
*        4(,R1) - Address of a print queue vector
*
* EXIT CONDITIONS:
*
*        Print queue buffer updated with word at current
*        tree node added.      
*
* REGISTER USAGE:
*
*        R1  - Used in obtaining memory as well as parameter
*              passing.
*        R2  - Increment register for setting up newly acquired
*              print buffer space
*        R3  - Holds address of last print buffer in contiguous
*              space of newly acquired print buffers
*        R4  - Addressability for rear of print queue
*              Used for Setting PLINK in last acquired buffer to 0
*        R5  - Addressability for the print queue (PrQ)
*        R6  - Used setting new value for PrQB
*        R7  - Used to determine address of current place to insert
*              a new word in the current print buffer.
*        R8  - Used to hold the value of Length for a brief period
*              also used for dealing with PrQA
*              also used for addressability of P
*        R9  - Points to the current tree node
*        R10 - Used in updating PrQB
*        R11 - @ of incoming parm list
*        R12 - Addressability for InTrav
*        R13 - points to the WorkArea
*
* Logic:
*   [1] Perform reentrant entry linkage
*   [2] If passed tree pointer is not empty, then
*   [3]   Call InTrav, passing left subtree for traversal
*   [4]   If PrQB < Length, set PrQB to zero (no space in print buf)
*   [5]   If PrQB is zero, then
*   [6]     If PrQA (avail stack ptr) is zero then
*   [7]       STORAGE OBTAIN enough memory for five buffers (PLen*5)
*   [8]       Set PrQA to address of first buffer
*   [9]       Do From=buffer1 By=bufferlength To=lastbuffer
*  [10]         Set PLink to point to next buffer
*  [11]         Set PCC to C'0', PText to blanks
*  [12]       End Do
*  [13]       Set PLink in last buffer to zero
*  [14]     End If
*  [15]     Remove top buffer from avail stack
*  [16]     Add buffer to rear of print queue
*  [17]     Set PrQB to 80 (all bytes available for words)
*  [18]   End If
*  [19]   Get current print buffer location from rear of queue
*  [20]   Copy this node's word to next text spot, using EX and MVC
*  [21]   Update PrQB by subtracting this word's length plus a blank
*  [22]   Call InTrav, passing right subtree for traversal
*  [23] End If
*  [24] Perform reentrant exit linkage
*
********************************************************************
InTrav   RSECT                 InTrav is Re-entrant
*
*   [1] Perform reentrant entry linkage
*
         STM 14,12,12(13)      Store caller regs in caller SA
         LR 12,15              Sets up R12 as the base register
         USING InTrav,12       Set up InTrav Addressability
         LR 11,1               Put address of ParmList in R11
In       USING ParmList,11     Set up ParmList Addressability
         STORAGE OBTAIN,LENGTH=WorkLen  Get storage for WorkArea
         ST 13,4(,1)           Link Calling SA with
         ST 1,8(,13)           current routine's SA
         LR 13,1               Put @WorkArea in R13
         Using WorkArea,13     Set up Addressability for WorkArea
*
*   [2] If passed tree pointer is not empty, then
*
         IF (ICM,9,15,In.Parm1,NZ) THEN
*
*   [3]   Call InTrav, passing left subtree for traversal
*
T        USING Node,9          Set up Tree Addressability
Out      USING ParmList,WorkParm Set Up WorkArea Parm List Address.
         MVC Out.Parm1,T.LLink  Set up Parm1 of new parm list
         MVC Out.Parm2,In.Parm2 Set up Parm2 of new parm list
         LA 1,WorkParm          Get @ new parm list into R1
         L 15,=V(InTrav)        Get @ InTrav Routine
         BASR 14,15             Call InTrav
*
*   [4]   If PrQB < Length, set PrQB to zero (no space in print buf)
*
         L 5,In.Parm2           Get @ of the print queue
         USING PrQD,5           Set up addressability to PrQ
         L 10,PrQB              Get PrQB
         XR 8,8                 Clear 8
         IC 8,T.Length          Get the length
         IF (CR,10,LT,8) THEN
         XR 10,10
         ST 10,PrQB
         ENDIF
*
*   [5]   If PrQB is zero, then
*
         IF (ICM,10,15,PrQB,Z) THEN
*
*   [6]     If PrQA (avail stack ptr) is zero then
*
         IF (ICM,8,15,PrQA,Z) THEN
*
*   [7]       STORAGE OBTAIN enough memory for five buffers (PLen*5)
*
         STORAGE OBTAIN,LENGTH=PLen*5  obtain memory for buffers
*
*   [8]       Set PrQA to address of first buffer
*
         ST 1,PrQA             Make change to permanent PrQA
*
*   [9]       Do From=buffer1 By=bufferlength To=lastbuffer
*
         LR 8,1                Copy start of obtained buffer space
         USING PBuf,8          Set up print buffer addressability
         LR 3,8                Get beginning of table into 3
         AHI 3,PLen*4          Now 3 points to start of last buffer
         LHI 2,PLen            Set Increment amount to PLen
         DO  FROM=(8),TO=(3),BY=(2)
*
*  [10]         Set PLink to point to next buffer
*
         LR 4,8                Get @ current buf into 4
         AHI 4,PLen            Calc addr of next buf
         ST 4,PLink            Save it in PLink
*
*  [11]         Set PCC to C'0', PText to blanks
*
         MVI PCC,C'0'          Set PCC to C'0'
         MVC PText,=CL80' '    Set PText to blanks
*
*  [12]       End Do
*
         ENDDO
*
*  [13]       Set PLink in last buffer to zero
*
         AHI 8,-1*PLen       Step Back to last buffer
         XR 4,4              Clear R4
         ST 4,PLink          Set PLink in last buffer to zero
         Drop 8              Done working with PBuf
*
*  [14]     End If
*
         ENDIF
*
*  [15]     Remove top buffer from avail stack
*
         L 8,PrQA            P <= Avail
P        Using PBuf,8        Set up print buffer addressability
         MVC PrQA,P.PLink    Avail <- Next(P)
*
*  [16]     Add buffer to rear of print queue
*
         L 4,PrQR            Get Rear Pointer of Print Queue
R        USING PBuf,4        Set up Addressability for R
         XR 6,6              Clear R6
         ST 8,R.PLink        Set LINK(R) = P
         ST 6,P.PLink        Set LINK(P) = Null
         ST 8,PrQR           R <- P
         Drop P              Done with P
         Drop R              Done with R
*
*  [17]     Set PrQB to 80 (all bytes available for words)
*
         LHI 6,80            Set R6 = 80
         ST 6,PrQB           Save 80 to PrQB
*
*  [18]   End If
*
         ENDIF
*
*  [19]   Get current print buffer location from rear of queue
*
         L 8,PrQR               Get Rear pointer into R8
         USING PBuf,8           Set up addr to print buffer
*
*  [20]   Copy this node's word to next text spot, using EX and MVC
*
         LA 7,PTextEnd          Get End of Text
         S 7,PrQB               Subtract PrQB from End of Text
         XR 6,6                 Clear R6
         IC 6,T.Length          Get the length of node's word
         AHI 6,-1               Instead of Length, want length code
         EX 6,MOVE              Copy the word to the print buf
*
*  [21]   Update PrQB by subtracting this word's length plus a blank
*
         L 7,PrQB               Get PrQB
         AHI 6,2                Add length of a blank to word
         SR 7,6                 PrQB = PrQB - (Length + blank)
         ST 7,PrQB              Save value for PrQB
         Drop 8
*
*  [22]   Call InTrav, passing right subtree for traversal
*
         MVC Out.Parm1,T.RLink  Set up Parm1 of new parm list
         MVC Out.Parm2,In.Parm2 Set up Parm2 of new parm list
         LA 1,WorkParm          Get @ new parm list into R1
         L 15,=V(InTrav)        Get @ InTrav Routine
         BASR 14,15             Call InTrav
*
*  [23] End If
*
         ENDIF
*
*  [24] Perform reentrant exit linkage
*
         LR 1,13               Put @WorkArea in R1
         L 13,4(,13)           Load pointer to previous SAVAREA
         STORAGE RELEASE,LENGTH=WorkLen,ADDR=(1) Returns memory
         LM 14,12,12(13)       Restore Caller Registers
         BR 14                 Exit Routine
         LTORG                 Accumulate literals here
*********************************************************************
* INTRAV WORKING STORAGE
*********************************************************************
MOVE     MVC 0(*-*,7),T.WORD   For var length words moved into buf
         DROP T
         Drop In
         Drop Out
         Drop 5
         Drop 12
         Drop 13
         END ,
/*
//