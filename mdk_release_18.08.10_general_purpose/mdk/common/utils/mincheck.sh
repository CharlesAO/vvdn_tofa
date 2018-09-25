#!/bin/bash

# Movidius script file checking if the mdk system is set up correctly

ENDMESSAGE="MV_TOOLS_DIR set"

function CheckCommandExistence {
command_to_check=/bin/$1
if [ ! -x $command_to_check ]; then
    echo Your cygwin installation is missing the $1
    ENDMESSAGE="Your MDK setup has problems as highlighted above."
fi
}

function CheckExecutablePermissions {
    mv_crt_executable=$1
    crt_executable_permissions=$(ls -all $mv_crt_executable | cut -d' ' -f1 | head -c4 | tail -c1)
    if [ ! $crt_executable_permissions = x ]; then
        echo No executable permissions on $mv_crt_executable.
        ENDMESSAGE="Your MDK setup has problems as highlighted above."
    fi
}

function CheckLinux {
#check existence of MV_TOOLS_DIR
tools_path=`set | grep "^MV_TOOLS_DIR" | cut -d'=' -f1`
if [ "$tools_path" != "MV_TOOLS_DIR" ]; then
    #Signal absence of MV_TOOLS_DIR
    echo Your MV_TOOLS_DIR path is not set through an environment variable.
    echo Please redo the steps from docs/MDK-Getting_Started.pdf
    ENDMESSAGE="Your MDK setup has problems as highlighted above."
fi
}

function CheckCygwinWindows {
#check existence of MV_TOOLS_DIR
tools_path=`set | grep "^MV_TOOLS_DIR" | cut -d'=' -f1`
if [ "$tools_path" != "MV_TOOLS_DIR" ]; then
    #Signal absence of MV_TOOLS_DIR
    echo Your MV_TOOLS_DIR path is not set through an environment variable.
    echo Please redo the steps from docs/MDK-Getting_Started.pdf
    ENDMESSAGE="Your MDK setup has problems as highlighted above."
fi
}

#determine operating system first
OSVAR=`uname -a |cut -d ' ' -f1`
if [ "$OSVAR" = "Linux" ]; then
    CheckLinux
else
    CheckCygwinWindows
fi

echo $ENDMESSAGE