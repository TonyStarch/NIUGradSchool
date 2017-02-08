#!/bin/csh

@ choice = 0
while ($choice != 4)
    echo "Menu"
    echo "*****"
    echo "[1] Display today's date"
    echo "[2] Find out how many people are logged on"
    echo "[3] Find out how many user accounts are  in the password file"
    echo "[4] Exit"
    echo -n "Enter your choice:"
    @ choice = $<
    echo "Your choice was: $choice"
end
