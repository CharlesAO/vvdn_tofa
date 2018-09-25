namespace eval mdbg::shell {

    # Setup Eclipse TM terminal colours
    proc SetupEclipseColors {} {

        global env
        variable ECLIPSE_TM_TERMINAL

        if {[info exists env(ECLIPSE_HOME)] && [info exists env(ECLIPSE_WORKSPACE)]} {

            set ECLIPSE_TM_TERMINAL white

            set path [file join \
                $env(ECLIPSE_WORKSPACE) \
                .metadata \
                .plugins \
                org.eclipse.core.runtime \
                .settings
            ]
            set file1 [file join \
                $path \
                org.eclipse.tm.terminal.control.prefs \
            ]
            set file2 [file join \
                $path \
                org.eclipse.tm.terminal.control.prefs \
            ]

            expr { [catch {set settingsFile [open $file1 r]}] == 0
                || [catch {set settingsFile [open $file2 r]}] == 0
            }

            if {[info exists settingsFile]} {
                catch {
                    while {![eof $settingsFile]} {
                        set line [gets $settingsFile]
                        if {[regexp {^\s*TerminalPrefInvertColors\s*=\s*([^\s]*)\s*$} $line -> invert ]} {
                            if {[string is true -strict $invert]} {
                                set ECLIPSE_TM_TERMINAL black
                            }
                            break
                        }
                    }
                }
                catch {
                    close $settingsFile
                }
                unset settingsFile
            }
            if {$ECLIPSE_TM_TERMINAL == "white" || $::tcl_platform(platform) == "windows"} {
                catch {chan configure mdbgeventout -textattr cyan}
            } else {
                catch {chan configure mdbgeventout -textattr white}
            }

            if {$ECLIPSE_TM_TERMINAL == "white"} {
                foreach {chan color} {
                    warning     magenta
                    info        blue
                    vcshooks    blue
                    uart        {blue    bold}
                    pipeout     {blue    bold}
                    pipeerr     {magenta bold}
                } {
                    catch {
                        chan configure mdbg${chan}out -textattr $color
                    }
                }
            }
        }
    }

    proc RemapColorInEclipseTerminal {color} {
        variable ECLIPSE_TM_TERMINAL
        if {![info exists ECLIPSE_TM_TERMINAL]} {
            return $color
        }

        set bold ""
        set invert ""
        set fore ""
        set back ""

        foreach attr [split $color] {
            switch -exact $attr {
                bold - normal {
                    set bold $attr
                }
                invert - direct {
                    set invert $attr
                }
                default {
                    if {$fore == ""} {
                        set fore $attr
                    } elseif {$back == ""} {
                        set back $attr
                    }
                }
            }
        }

        set TOGGLE_BOLD {
            if {$bold == "bold"} {
                set bold "normal"
            } else {
                set bold "bold"
            }
        }

        set INVERT_BW_FORE {
            switch -exact $fore {
                black {
                    set fore white
                }
                white {
                    set fore black
                }
            }
        }

        set INVERT_BW_BACK {
            switch -exact $back {
                black {
                    set back white
                }
                white {
                    set back black
                }
            }
        }

        switch -exact $::tcl_platform(platform) {
            windows {
                switch -exact $invert {
                    invert {
                        set invert direct
                        lassign [list $fore $back] back fore
                    }
                }
                switch -exact win_$ECLIPSE_TM_TERMINAL {
                    win_white {
                        switch -exact fore:$fore {
                            fore:default {
                                if {$bold == "bold"} {
                                    set fore black
                                }
                            }
                            fore:black {
                                set fore black
                                uplevel 0 $TOGGLE_BOLD
                            }
                        }
                        switch -exact back:$back {
                            back:black {
                                set back white
                                uplevel 0 $INVERT_BW_FORE
                            }
                            back:default -
                            back:white {
                                set back black
                                uplevel 0 $INVERT_BW_FORE
                            }
                        }
                    }
                    win_black {
                        switch -exact fore:$fore {
                            fore:default -
                            fore:white {
                                set fore black
                            }
                            fore:black {
                                set fore white
                                uplevel 0 $TOGGLE_BOLD
                            }
                        }
                    }
                }
            }
            default {
                switch -exact linux_$ECLIPSE_TM_TERMINAL {
                    linux_black {
                        switch -exact $fore {
                            black {
                                set fore white
                                uplevel 0 $TOGGLE_BOLD
                            }
                            white {
                                set fore black
                            }
                        }
                        uplevel 0 $INVERT_BW_BACK
                    }
                    linux_white {
                        switch -exact $fore {
                            black {
                                uplevel 0 $TOGGLE_BOLD
                            }
                        }
                    }
                }
            }
        }

        list {*}$fore {*}$bold {*}$back {*}$invert
    }

} ; # namespace eval mdbg::shell

mdbg::shell::SetupEclipseColors

