#!/bin/csh

################################################################
# Brandon Tweed
# Z030199
# CSCI 330
# Section 2
# Assignment 6 - Part A
#
# Description:
# This shell program brings up a menu for the user, which
# displays one of four options that the user can choose
# from. These options allow the user to check her assignment
# grades, quiz grades, or exam grade.
#
################################################################

# see if the correct number of arguments was given
if ($#argv == 0 || $#argv > 1) then
    echo "This progrm requires one argument, your section number."
    exit 1
endif

#intially set an invalid choice so that loop is executed
set choice = 0
clear
# while we have not made a choice of 4
while ($choice != "4")
    #display the menu
    echo "    Grade Checking Menu"
    echo "---------------------------"
    echo
    echo "[1] Check Assignment Grades"
    echo "[2] Check Quiz Grades"
    echo "[3] Check Exam Grades"
    echo "[4] Exit the Menu"
    echo 
    echo "---------------------------"
    echo
    # prompt the user for his/her choice
    echo -n "Please enter the number of your choice:"
    set choice = $<
    # run matching program according to choice
    switch($choice)
         case "1":
                /home/ux/krush/unix/grades/330assign-graderun $1
                breaksw
         case "2":
                /home/ux/krush/unix/grades/330quiz-graderun $1
                breaksw
         case "3":
                /home/ux/krush/unix/grades/330exam-graderun $1
                breaksw
         case "4":
                echo "You have chosen to exit. Thank you\!"
                exit 0
                breaksw
         default:
                echo "That was an invalid choice. Please try again."
                sleep 3
                breaksw
    endsw
end

