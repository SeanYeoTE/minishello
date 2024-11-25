#!/bin/bash

# Basic Commands
test_cases=(

    # Basic Commands \ Absolute Path
    "/bin/ls"
    "/bin/pwd"
    "/bin/echo hello"
    "/bin/cat supp.txt"
    "ls"
    "pwd"
    "echo hello"
    "echo hello world"
    "hello"
    "''"
    """"
    # run simple command with absolute path, command with arguments without single and double quotes
    "/bin/echo hello world"
    "/bin/echo hello world 123"
    "/bin/echo hello world 123 456"
    "/bin/echo hello world 123 456 789 101112"

    # echo with random tabs and spaces
    "echo 'hello world'"
    "echo 'hello world'  "
    "   echo"
    "   echo   "
    "echo -nnnnn"
    "echo -nnnnnnnn hello"
    "echo -nnnnnnna hello"
    "echo -nnn hello -nnnna"
    "echo -n-n abc"
    "echo -n  -n -n abc"
    "echo -n-n-n-n abc"
    "echo -n "-n-n-n -n abc""
    "echo -nnnnnn -nnnnnnnnnnnnnnnnn-n -nnnnnnnnnnnnnnna -n"
    "echo -nnnnnnnnna hello world -n -nnnnnnnnna hello world -n"

    # echo with variables
    "echo $a"
    "export a"
    "echo $USER $HOME"

    # misc echo
    "echo "                  " | cat -A"
    "'e''c''h''o' 13"

    # return values
    "echo $?"
    "expr $? + $?"
    

    # Pipes
    "ls | wc -l"
    "echo hello | cat"
    "ls | grep test"
    "env | grep SHLVL"
    
    # Redirections
    "echo hello > test_out"
    "cat < test_out"
    
    # Multiple Pipes
    "ls | grep test | wc -l"
    "cat /etc/passwd | grep root | wc -l"
    
    # Quotes
    "echo 'hello world'"
    "echo \"hello world\""
    
    # Environment Variables
    "echo \$PATH"
    "echo \$HOME"
    
    # Built-ins
    "cd .."
    "env"
    "export TEST=123"
    "unset TEST"
)
