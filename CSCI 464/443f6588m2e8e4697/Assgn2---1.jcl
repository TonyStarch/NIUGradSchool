//Z030199A JOB ,'Brandon Tweed',LINES=10
/*JOBPARM ROOM=199
//       JCLLIB ORDER=(T90CSCI.CSCI464.PROCLIB)
//S1     EXEC ASSIST
//SYSIN  DD *
*******************************************************
* BRANDON TWEED
* CSCI 464
*******************************************************
R0       EQU   0      REGISTER EQUATE FOR READABILITY
R1       EQU   1      REGISTER EQUATE FOR READABILITY
R2       EQU   2      REGISTER EQUATE FOR READABILITY
R3       EQU   3      REGISTER EQUATE FOR READABILITY
R4       EQU   4      REGISTER EQUATE FOR READABILITY
R5       EQU   5      REGISTER EQUATE FOR READABILITY
R6       EQU   6      REGISTER EQUATE FOR READABILITY
R7       EQU   7      REGISTER EQUATE FOR READABILITY
R8       EQU   8      REGISTER EQUATE FOR READABILITY
R9       EQU   9      REGISTER EQUATE FOR READABILITY
R10      EQU  10      REGISTER EQUATE FOR READABILITY
R11      EQU  11      REGISTER EQUATE FOR READABILITY
R12      EQU  12      REGISTER EQUATE FOR READABILITY
R13      EQU  13      REGISTER EQUATE FOR READABILITY
R14      EQU  14      REGISTER EQUATE FOR READABILITY
R15      EQU  15      REGISTER EQUATE FOR READABILITY
         TITLE '*** CSCI 464 BASE REGISTER REPLACEMENT ROUTINE ***'
         SPACE
         PRINT NOGEN
********************************************************************
BRENTRY  DSECT   A DSECT FOR A BRT ENTRY
BRSID    DS X    BRT SECTION ID
BRLCV    DS XL3  BRT LOCATION COUNTER VALUE
********************************************************************
BROPS2   CSECT               MAIN PROGRAM CSECT
BRTABLE  DS XL64             DEFINITION OF BASE REGISTER TABLE
* THESE ENTRY STATEMENTS PUT THE ADDRESS OF THE BASE REGISTER
* TABLE AS WELL AS THE ADDRESS OF THE ROUTINES IN THE 
* EXTERNAL SYMBOL DICTIONARY FOR USE DURING ASSEMBLY
         ENTRY BRTABLE
         ENTRY BRINIT,BRUSIN,BRDROP,BRDISP
         EJECT
********************************************************************
*                           BRINIT
*
* FUNCTION:     The purpose of this routine is to initialize
*               the base register table so that it is empty.
*
* INPUT:        none
* 
* OUTPUT:       none
*
* ENTRY CONDITIONS:
*               none
*
* EXIT CONDITIONS:
*               none
*
* LOGIC:
* 
* 1) Perform Entry Linkage
* 2) Move zeros to the base register table
* 3) Perform Exit Linkage
*
********************************************************************
BRINIT   DS 0H                   BRINIT ROUTINE
         USING BRINIT,R15        SET UP ROUTINE ADDRESSABILITY
         L R11,=A(BRTABLE)       GET ADDRESS OF BRT
         USING BRTABLE,R11       SET UP ADDRESSABILITY FOR BRT
         MVC BRTABLE(32),=XL64'00'  CLEAR THE BRT BY PUTTING IN ZEROS
         DROP R11                DONE WITH BRT
         DROP R15                DONE WITH ADDRESSABILITY
         BR R14                  LEAVE THIS ROUTINE
         LTORG                   LITERAL STORAGE AREA
         EJECT
********************************************************************
* 
*              BRUSIN
*
* FUNCTION:    This routine is called whenever a using instruction
*              is encountered by ASSIST.
*
* ENTRY CONDITIONS:
*              R7 - number of the register to be used (0<= R7 <=15)
*              R8 - address declared for base register
*              R9 - SID of the address
*
* EXIT CONDITIONS:
*              none
* 
* LOGIC:
* P IS A POINTER TO AN ENTRY IN THE BASE REGISTER TABLE
* R IS THE NUMBER OF THE REGISTER TO BE USED
*
* SET P TO POINT TO ENTRY 0
* WHILE P < R IN BRT
*     P = P + SIZEOF(BASE REGISTER TABLE ENTRY)
* END WHILE
* SET ENTRY P WITH THE SID AND THE ADDRESS      
*
********************************************************************
BRUSIN   DS 0H
         USING BRUSIN,R15    SET UP ADDRESSABILITY
         L R11,=A(BRTABLE)   GET START OF THE BRT
         USING BRENTRY,R11   SET UP BRT ADDRESSABILITY     
         LTR R7,R7           SEE IF REGISTER USED IS R0
         BZ SAVEIT           IF IT IS, CAN SAVE AT START OF TABLE
START    LA R11,2(R11)       MOVE POINTER TO NEXT BRT ENTRY
         BCT R7,START        REDUCE COUNTER AND LOOP
* R11 SHOULD NOW POINT TO THE PROPER POSITION IN THE BRT
SAVEIT   SLL R9,12           MOVE THE SID OVER
         OR  R8,R9           COMBINE THE TWO REGISTERS
         STH R8,BRSID        SAVE THE RESULT
         DROP R11            DONE WITH THE BRT
         DROP R15            DONE WITH ADDRESSABILITY
         BR R14              END OF BRUSIN
         LTORG               LITERAL STORAGE AREA
         EJECT
********************************************************************
*
* BRDROP
*
* FUNCTION:    This routine is called whenever a DROP statement
*              is encountered by ASSIST. The entry in the base
*              register table corresponding with the base
*              register specified is removed from the table.
*
* ENTRY CONDITIONS:
*              R7 - Contains the number of the register to be
*                   taken out of the base register table.
*
* EXIT CONDITIONS:
*              R8 - Set to 0 if register specified was an active 
*                   base register and successfully DROPed.
*              
*                   Set to 4 if the register specified was not
*                   in use at the time the DROP was performed.
*
* LOGIC:
*
* LET P POINT TO THE BEGINNING OF THE TABLE
* LET Q DENOTE THE # OF THE REGISTER TO BE DROPPED
* DO WHILE(Q > 0)
*    P = P + SIZEOF(BRT_ENTRY)
*    Q = Q -1
* END DO
* IF( P->ADDRESS != 0 )
*     CLEAR THE ENTRY
*     SET R8 TO 0 TO INDICATE NO ERROR
* ELSE
*     SET R8 TO 4 TO INDICATE ERROR DROPPING THE REGISTER
* ENDIF
*
********************************************************************
BRDROP   DS 0H               START OF BRDROP ROUTINE
         USING BRDROP,R15    SET UP ADDRESSABILITY
         L R11,=A(BRTABLE)   GET START OF TABLE
         USING BRENTRY,R11   SET UP BRT ADDRESSABILITY
         LTR R7,R7           IF R7 IS 0
         BZ CHECK            IF IT IS, PROCEED TO CHECKING THE BRT
NEXTENT  LA R11,2(R11)        MOVE TO NEXT BRT ENTRY
         BCT R7,NEXTENT      DECREMENT AND ITERATE
CHECK    LH R10,BRSID        GET THE BRT ENTRY
         CH R10,=H'0'        IF BRT ENTRY = 0 (ENTRY NOT ACTIVE)
         BE DRPERR           HANDLE THE DROP ERROR
         XR R8,R8            INDICATE NO ERROR
         STH R8,BRSID        CLEAR THE BRT ENTRY      
         B ENDDROP           ALL DONE, END THE ROUTINE
DRPERR   LA R8,4             SET R8 TO INDICATE AN ERROR   
ENDDROP  DS 0H
         DROP R15            END ROUTINE ADDRESSABILITY
         BR R14              END OF BRDROP ROUTINE
         LTORG               LITERAL STORAGE AREA
********************************************************************
         EJECT
********************************************************************
* BRDISP
*
* FUNCTION:    Convert an address to base-displacement form and
*              return it in that form using a register.
*
* ENTRY CONDITIONS:
*              R7 - address to be converted to base-displacement
*                   form (value between 0 and 2^24-1)
*              R8 - SID of address to be converted
*
* EXIT CONDITIONS:
*              R7 - base-displacement form of address in bits
*                   16-31 of the register. Bits 0-15 are zeros.
*              R8 - set to 0 if the address was properly converted
*                   set to 4 if there was an addressability error
* 
* REGISTER USAGE:
*
* R3 - STARTING POINT OF BRT FOR THE LOOP
* R4 - INCREMENT AMOUNT FOR THE LOOP
* R5 - END OF BRT TABLE, TERMINATING POINT FOR LOOP
* R6 - USED AS A COUNTER TO DETERMINE THE # OF THE BASE
*      REGISTER TABLE INDICATED BY THE BRT ENTRY
* R9  - THE CALCULATED DISPLACEMENT
* R10 - REPRESENTS THE BEST DISPLACEMENT CALCULATED SO FAR
* R11 - SID FROM BRT ENTRY
* R12 - ADDRESS FROM BRT ENTRY
*
* LOGIC:
*
* SET BEST = 4096
* SET REGISTER-COUNTER TO ZERO
* DO
*    GET A BRT ENTRY
*    IF SID = INCOMING SID
*        CALCULATE DISPLACEMENT
*        IF DISPLACEMENT <= BEST
*            BEST = DISPLACEMENT
*            SAVE REGISTER-COUNTER
*        END-IF
* WHILE NOT END OF TABLE
* IF BEST = 4096
*    SET THE ERROR CODE
* ELSE
*    PUT BEST IN B(D) FORM
*    SET ERROR CODE TO ZERO
* ENDIF
*
********************************************************************
BRDISP   DS 0H               START OF BRDISP
         USING BRDISP,R15    SET UP ADDRESSABILITY
         STM R12,R6,SAVEAREA SAVE REGISTERS EXCEPT 7-11
         XR R6,R6            CLEAR BASE REG COUNTER
         L R10,=F'4096'      SET BEST = HIGH VALUE
* GET VALUES FOR START, INCREMENT, TERMINATE
         LM R3,R5,=A(BRTABLE,2,BRTABLE+16*2-2) 
STRT01   LH R11,0(,R3)       GET THE BRT ENTRY
         LR R12,R11          COPY THE BRT ENTRY        
         SRL R11,12          GET RID OF ADDRESS
         CR R11,R8           SEE IF BRSID = INCOMING SID
         BNE SKIPIT          IF NOT EQUAL, SKIP THE ENTRY
* CLEAR THE SID FROM THE BRT ENTRY
         SLL R12,20          GET RID OF SID
         SRL R12,20          PUT ADDRESS BACK IN PLACE
* CALCULATE THE DISPLACEMENT
         LR R9,R7            MAKE SURE R7 STAYS CLEAN
         SR R9,R12           DISPLACEMENT -> R9
         BM SKIPIT           DISPLACEMENT < 0, SKIPIT
         CR R9,R10           COMPARE DISPLACEMENT TO BEST
         BH SKIPIT           IF DISPLACEMENT > BEST, SKIP IT
         LR R10,R9           SET BEST = DISPLACEMENT
         STH R6,BESTBASE     SAVE THE BASE REGISTER
SKIPIT   LA R6,1(R6)         INCREMENT BASE REG COUNTER
         BXLE R3,R4,STRT01   GO TO NEXT BRT ENTRY AND REPEAT
         C R10,=F'4096'      SEE IF BR WAS FOUND
         BNE FINE            IF IT WAS, GOTO "FINE"
         LA R8,4             OTHERWISE, THERE WAS AN ERROR
         XR R7,R7            DO NOT RETURN ANY B(D) ADDRESS
         B ENDIT             END THE ROUTINE
FINE     XR R8,R8            CLEAR R8 TO INDICATE NO ERROR
         LH R6,BESTBASE      GET THE BASE REGISTER
         SLL R6,12           ALIGN THE VALUE IN THE REG
         OR R6,R10           COMBINE THE BASE & DISPL.
         LR R7,R6            RETURN THE RESULT IN R7
ENDIT    LM R12,R6,SAVEAREA  RESTORE REGISTERS EXCEPT FOR 7-11
         DROP R15            END ADDRESSABILITY FOR BRDISP
         BR R14              END OF BRDISP
********************************************************************
* WORKING STORAGE FOR BRDISP ROUTINE
********************************************************************
SAVEAREA DS 12F
BESTBASE DS H
         END                 
         EJECT
*SYSLIB  BROPSRUN
         BROPSRUN
