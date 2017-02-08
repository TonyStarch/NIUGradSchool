********************************************************************
* This macro alters the contents of
* R7, and the specified register
********************************************************************
         MACRO
         BASE &INDEX
         LA R7,HeapAray      Get start of the Array
* check this
         SLL &INDEX,2        Multiply index by 4
         AR &INDEX,R7        Add to get the address for the item
         MEND

ParmList DSECT ,             Parm list for Heapify
Parm1    DS A                Address of Array
Parm2    DS A                Address of Index

WorkArea DSECT ,             WorkArea for Heapify
         DS 18F              SaveArea
NewIndex DS F                New Index
RNDX     DS F                Index of Right Child
LNDX     DS F                Index of Left Child
RVAL     DS F                Value of Right Child
LVAL     DS F                Value of Left Child
INDX     DS F                Value of the index
Largest  DS F                Value of Largest
WorkParm DS 2A               One for @array, @index
WorkLen  EQU *-WorkArea      Calculates length of WorkArea

********************************************************************
* Heapify
*
* Implements the Max-Heapify algorithm
*
* 1)  Perform Re-entrant Entry Linkage and Initialize
* 2)  set L <- Left(i)
* 3)  set R <- Right(i)
* 4)  IF( L <= HeapSize[A] and A[L] > A[R] )
* 5)    Largest <- L
* 6)  ELSE
* 7)    Largest <- i
* 8)  ENDIF
* 9)  IF ( R <= HeapSize[A] and A[R] > A[Largest] )
* 10)   Largest <- R
* 11) ENDIF
* 12) IF Largest Not = i
* 13)    Exchange A[i] with A[largest]
* 14)    Call Heapify( A, Largest )
* 15) ENDIF
* 16) Perform Re-entrant Exit Linkage
*
* REGISTER USAGE:
*
* R0 - A[Largest]
* R2 - A[l]
* R3 - A[r]
* R4 - Largest
* R5 - l
* R6 - r
* R7 - i
* R8 - heap-size[A]
* R10 - Addressability to the Heap
* R11 - addressability to the parm list
* R12 - Routine base register
* R13 - Addressability to the WorkArea
* 
*
********************************************************************
Heapify  RSECT               Start of Heapify Routine
* 1)  Perform Re-entrant Entry Linkage and Initialize
         STM 14,12,12(13)    Store caller regs in caller SA
         LR 12,15            Sets up R12 as base register
         USING Heapify,12    Set up Heapify Addressability
         LR 11,1             Put @ParmList in R11
In       USING ParmList,11   Set up ParmList Addressability
         STORAGE OBTAIN,LENGTH=WorkLen Get storage for WorkArea
         ST 13,4(,1)         Link Calling SA with
         ST 1,8(,13)         current routine's SA
         LR 13,1             Put @ WorkArea in R13
         Using WorkArea,13   Set up WorkArea Addressability
         L 10,Parm1          Get @array into 10
         USING Heap,10       Set up Heap addressability
         L 7,Parm2           Get @i into 7
         L 7,0(,7)           Get i into 7
         ST 7,INDX           Save i into storage
         L 8,HeapSize        Get HeapSize[A] into 8
*
* 2)  set L <- Left(i)
*
         Left 7,5            Get index of left subchild into 5
         ST 5,LNDX           Save index of left subchild
         BASE 5              Get @ of A[l]
         L 5,0(,5)           Get A[l]
         ST 5,LVAL           Save it to storage
*
* 3)  set R <- Right(i)
*
         L 7,INDX            Restore i in 7
         Right 7,6           Get index of right subchild into 6
         ST 6,RNDX           Save index of right subchild
         BASE 6              Get @ of A[r]
         L 6,0(,6)           Get A[r]
         ST 6,RVAL           Save it to storage
*
* 4)  IF( l <= HeapSize[A] and A[l] > A[r] )
*
         L 5,LNDX            Get value of index l
         L 2,LVAL            Get A[l] into R2
         L 3,RVAL            Get A[r] into R3
         IF (CR,5,LE,8),AND,(CR,2,GT,3) THEN
*
* 5)    Largest <- l
*
         ST 5,Largest        Get value of index l into Largest
*
* 6)  ELSE
*
         ELSE
*
* 7)    Largest <- i
*
         MVC Largest(4),INDX Get value of i into Largest
*
* 8)  ENDIF
*
         ENDIF
*
* 9)  IF ( r <= HeapSize[A] and A[r] > A[Largest] )
*
         L 6,RINDX           Get r into 6
         L 4,Largest         Get Largest
         BASE 4              Get @A[Largest]
         L 4,0(,4)           Get A[Largest] into 4
         IF (CR,6,LE,8),AND,(CR,3,GT,4) THEN
*
* 10)   Largest <- R
*
         ST 6,Largest
*
* 11) ENDIF
*
         ENDIF
*
* 12) IF Largest Not = i
*
         IF (CR,,NE,)
* 13)    Exchange A[i] with A[largest]
* 14)    Call Heapify( A, Largest )
* 15) ENDIF
         ENDIF
* 16) Perform Re-entrant Exit Linkage
         LR 1,13             Put @WorkArea in R1
         L 13,4(,13)         Load pointer to previous SA
         STORAGE RELEASE,LENGTH=WorkLen,ADDR=(1) Return WorkArea
         LM 14,12,12(13)     Restore Caller Registers
         BR 14               Exit Routine
         LTORG               Accumulate Literals HereX
********************************************************************
* BldHeap
* LOGIC:
*
* 1) Initialize
* 2) heap-size[A] <- length[A]
* 3) for i=floor(length[A]/2) downto 1
* 4)   do Max-Heapify(A,i)
* 5) Exit Linkage
*
********************************************************************
BldHeap  XSAVE OPT=CSECT     Start of BldHeap
         L 10,0(,1)          Get @ of array into 10
         XRETURN SA=*        End of BldHeap
         LTORG               Accumulate Literals Here
