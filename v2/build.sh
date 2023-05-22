gcc main.c -o prog -lSDL2 -lSDL2main -lm -I include
if [ $? -eq 0 ]; then
    echo "Command Executed Successfully"
    ./prog
else
    echo "Command Failed"
fi
