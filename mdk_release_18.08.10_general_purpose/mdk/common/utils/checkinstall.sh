#!/bin/bash

# Movidius script file checking if the mdk system is set up correctly

ENDMESSAGE="Everything should be all right with your MDK installation."

function CheckCommandExistence {
command_to_check=/bin/$1
if [ ! -x $command_to_check ]; then
    echo Your cygwin installation is missing the $1 command.
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

function CheckLibraryPermissions {
    #check them for leonDll.dll  
    mv_crt_library=$1
    crt_lib_permissions=$(ls -all $mv_crt_library | cut -d' ' -f1 | head -c4 | tail -c1)
    if [ ! $crt_lib_permissions = x ]; then
        echo No executable permissions on $mv_crt_library.
        ENDMESSAGE="Your MDK setup has problems as highlighted above."
    fi
}

function CheckLinux {
#check existence of MV_TOOLS_DIR
tools_path=`set | grep "^MV_TOOLS_DIR" | cut -d'=' -f1`
if [ "$tools_path" != "MV_TOOLS_DIR" ]; then
    #Signal absence of MV_TOOLS_DIR
    echo Your MV_TOOLS_DIR path is not set through an environment variable.
    ENDMESSAGE="Your MDK setup has problems as highlighted above."
else
    #check if the selected tools from generic.mk exist in the tools folder
    mv_tools_version=`make -C../ print-MV_TOOLS_VERSION | grep ^MV_TOOLS_VERSION | cut -d'=' -f2 | cut -d' ' -f2`
    mv_tools_path=`set | grep ^MV_TOOLS_DIR | cut -d'=' -f2 | cut -d' ' -f1`/$mv_tools_version
    if [ ! -d $mv_tools_path ]; then
        echo Current mdk version tools version, $mv_tools_version, not installed in
        echo $mv_tools_path
        ENDMESSAGE="Your MDK setup has problems as highlighted above."
    fi

    #check permissions on JTAG libraries
    #check them for libFTCJTAG.so  
    mv_crt_library=$mv_tools_path/linux64/drivers/libFTCJTAG.so
    crt_lib_permissions=$(ls -all $mv_crt_library | cut -d' ' -f1 | head -c4 | tail -c1)
    if [ ! $crt_lib_permissions = x ]; then
        echo No executable permissions on $mv_crt_library.
        ENDMESSAGE="Your MDK setup has problems as highlighted above."
    fi
    #check them for libftd2xx.so  
    mv_crt_library=$mv_tools_path/linux64/drivers/libftd2xx.so
    crt_lib_permissions=$(ls -all $mv_crt_library | cut -d' ' -f1 | head -c4 | tail -c1)
    if [ ! $crt_lib_permissions = x ]; then
        echo No executable permissions on $mv_crt_library.
        ENDMESSAGE="Your MDK setup has problems as highlighted above."
    fi
    ##check them for libftd2xx.so.0
    #mv_crt_library=$mv_tools_path/linux64/drivers/libftd2xx.so.0
    #crt_lib_permissions=$(ls -all $mv_crt_library | cut -d' ' -f1 | head -c4 | tail -c1)
    #if [ ! $crt_lib_permissions = x ]; then
        #echo No executable permissions on $mv_crt_library.
        #ENDMESSAGE="Your MDK setup has problems as highlighted above."
    #fi

    #check permissions for Movidius tools
    #check permissions for moviAsm
    CheckExecutablePermissions $mv_tools_path/linux64/bin/moviAsm
    #check permissions for moviDebug
    CheckExecutablePermissions $mv_tools_path/linux64/bin/moviDebug2
    #check permissions for moviDump
    CheckExecutablePermissions $mv_tools_path/linux64/bin/moviDump
    #check permissions for moviConvert
    CheckExecutablePermissions $mv_tools_path/linux64/bin/moviConvert
    #check permissions for moviCompile
    CheckExecutablePermissions $mv_tools_path/linux64/bin/moviCompile
    
fi

#check that MV_COMMON_BASE is NOT defined
common_path=`set | grep "^MV_COMMON_BASE" | cut -d'=' -f1`
if [ "$common_path" = "MV_COMMON_BASE" ]; then
    echo You have a environment variable named MV_COMMON_BASE set. Please unset it.
    ENDMESSAGE="Your MDK setup has problems as highlighted above."
fi

#check that /tmp exists and is writable
#first check that it exists and it is a directory
if [ ! -d /tmp ]; then
    echo The /tmp folder does not exist on your cygwin installation. Please create it.
    ENDMESSAGE="Your MDK setup has problems as highlighted above."
else
    #Check that /tmp is writable
    if [ ! -w /tmp ]; then
        echo The /tmp folder is not writable. Please change permissions on it.
        ENDMESSAGE="Your MDK setup has problems as highlighted above."
    fi
fi
}

function CheckCygwinWindows {
#check existence of MV_TOOLS_DIR
tools_path=`set | grep "^MV_TOOLS_DIR" | cut -d'=' -f1`
if [ "$tools_path" != "MV_TOOLS_DIR" ]; then
    #Signal absence of MV_TOOLS_DIR
    echo Your MV_TOOLS_DIR path is not set through an environment variable.
    ENDMESSAGE="Your MDK setup has problems as highlighted above."

else
	CheckCommandExistence cygpath
    
	#check that MV_TOOLS_DIR starts with /cygdrive/x
    mv_tools_path=`cygpath $MV_TOOLS_DIR | head -c10`
    if [ ! "$mv_tools_path" = /cygdrive/ ]; then
        echo Your MV_TOOLS_DIR path does not begin with /cygdrive/*
        echo Please configure cuygwin to mount your drives to /cygdrive 
        ENDMESSAGE="Your MDK setup has problems as highlighted above."
    fi
	
    #check if the selected tools from generic.mk exist in the tools folder
    mv_tools_version=`make -C../ print-MV_TOOLS_VERSION | grep ^MV_TOOLS_VERSION | cut -d'=' -f2 | cut -d' ' -f2`
    mv_tools_path=`cygpath $MV_TOOLS_DIR | cut -d' ' -f1`/$mv_tools_version
    if [ ! -d $mv_tools_path ]; then
        echo Current mdk version tools version, $mv_tools_version, not installed in
        echo $mv_tools_path
        ENDMESSAGE="Your MDK setup has problems as highlighted above."
    fi

    #check permissions on JTAG libraries
    #check them for FTCJTAG.dll  
    mv_crt_library=$mv_tools_path/win32/drivers/FTCJTAG.dll
    crt_lib_permissions=$(ls -all $mv_crt_library | cut -d' ' -f1 | head -c4 | tail -c1)
    if [ ! $crt_lib_permissions = x ]; then
        echo No executable permissions on $mv_crt_library.
        ENDMESSAGE="Your MDK setup has problems as highlighted above."
    fi
    #check them for FTD2XX.dll  
    mv_crt_library=$mv_tools_path/win32/drivers/FTD2XX.dll
    crt_lib_permissions=$(ls -all $mv_crt_library | cut -d' ' -f1 | head -c4 | tail -c1)
    if [ ! $crt_lib_permissions = x ]; then
        echo No executable permissions on $mv_crt_library.
        ENDMESSAGE="Your MDK setup has problems as highlighted above."
    fi
    
    #and check permissions for all moviSim libraries
    CheckLibraryPermissions $mv_tools_path/win32/models/leonDll.dll
    CheckLibraryPermissions $mv_tools_path/win32/models/shavesDll.dll
    CheckLibraryPermissions $mv_tools_path/win32/models/timersDll.dll
    CheckLibraryPermissions $mv_tools_path/win32/models/sippDll.dll

    #check permissions for Movidius tools
	#check permissions for moviAsm
    CheckExecutablePermissions $mv_tools_path/win32/bin/moviAsm.exe
	#check permissions for moviDebug
    CheckExecutablePermissions $mv_tools_path/win32/bin/moviDebug2.exe
	#check permissions for moviDump
    CheckExecutablePermissions $mv_tools_path/win32/bin/moviDump.exe
	#check permissions for moviConvert
    CheckExecutablePermissions $mv_tools_path/win32/bin/moviConvert.exe
	#check permissions for moviCompile
    CheckExecutablePermissions $mv_tools_path/win32/bin/moviCompile.exe
	
fi

#check that MV_COMMON_BASE is NOT defined
common_path=`set | grep "^MV_COMMON_BASE" | cut -d'=' -f1`
if [ "$common_path" = "MV_COMMON_BASE" ]; then
    echo You have a environment variable named MV_COMMON_BASE set. Please unset it.
    ENDMESSAGE="Your MDK setup has problems as highlighted above."
fi

#check that /tmp exists and is writable
#first check that it exists and it is a directory
if [ ! -d /tmp ]; then
    echo The /tmp folder does not exist on your cygwin installation. Please create it.
    ENDMESSAGE="Your MDK setup has problems as highlighted above."
else
    #Check that /tmp is writable
    if [ ! -w /tmp ]; then
        echo The /tmp folder is not writable. Please change permissions on it.
        ENDMESSAGE="Your MDK setup has problems as highlighted above."
    fi
fi

#check that TEMP exists and points to the /tmp folder
temp_var=`set | grep ^TEMP`
if [ ! "$temp_var" = "TEMP=/tmp" ]; then
    echo You don\'t have the TEMP environment variable set to TEMP=/tmp
    ENDMESSAGE="Your MDK setup has problems as highlighted above."
fi
#check if some basic tools exist (sed/awk/egrep/xargs/cut/sort)
CheckCommandExistence sed
CheckCommandExistence grep
CheckCommandExistence awk
CheckCommandExistence egrep
CheckCommandExistence xargs
CheckCommandExistence cut
CheckCommandExistence sort
CheckCommandExistence make
CheckCommandExistence realpath

}

#determine operating system first
OSVAR=`uname -a |cut -d ' ' -f1`
if [ "$OSVAR" = "Linux" ]; then
    CheckLinux
else
    CheckCygwinWindows
    echo If using Cygwin+Windows please make sure the used terminal is NOT mintty or rxvt.
    echo This is also mentioned in the release notes and Getting started guide.
fi

echo $ENDMESSAGE
 
