Shell in C

This is an implementation of a basic interactive shell done in C as an assignment for our Operating Systems course.
Running the code

    Download the repository as a zip, or clone it. cd into directory
    Run ./makefile using the command "make" . Ignore any warnings shown.

    run ./a.out

In order to exit the shell, type exit

Requirements as specified
Minimum requirements to be met:

    Display requirement < username@systemname:path/to/whereever>

    Builtins: ls, echo, cd, pwd, jobs, setenv,unsetenv, kjob, fg, bg, overkill.  Don't use execvp.

    Make an ls functionality to enable: -l, -a, -al/la

    Option to execute system command, like the ones in /bin/. Implement foreground, background processes. Should have arguments.

    pinfo. Prints information related to the running shell program process.

    pinfo pid should print the process related information about the process with pid.

    Print correct information about the background process exiting safely.  

    Quit command is the only command that is allowed to get the shell to exit. No other command and no other signal

    CTRL + Z : should change the status of currently running job to stop and push it to background.

    CTRL + C : it should cause a sigint to be sent to the currently running job. If there is no currently running job, then the shell should do nothing.

    BONUS : history implementation Inbuilt
    BONUS : Command recall using UP arrow key ( the number of keys should be less or equal to 10)
    
Stuff to note:

    Usage of system is strictly prohibited.
    Get error handling right
    rollnum_assgn2.tar.gz



Steps to Run the Code:


1. run make

