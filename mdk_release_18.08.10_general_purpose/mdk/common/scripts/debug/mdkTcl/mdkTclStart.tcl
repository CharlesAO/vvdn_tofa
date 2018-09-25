########################################################################################################################
#
# (C) Copyright Movidius Ltd. http://www.movidius.com 2016
#
# This module adds mdk specific functionality to moviDebug2
# In future additional mdk specific functionality may be added here
#
# Features:
# 1) Allows moviDebug2 to be automatically extended by running ~/.movitcl/mdkLocal.tcl from user home directory if it exists
#
########################################################################################################################

if { [info exists ::env(HOME) ] } {
    if {[file exists $::env(HOME)/.mvtcl/mdkLocal.tcl]} {
        source $::env(HOME)/.mvtcl/mdkLocal.tcl
    }
}

set autoExtend [file join [file dirname [info script]] "commands.scr"]

if { [file exists $autoExtend ] } {
    source $autoExtend
}

# vim: ft=tcl
