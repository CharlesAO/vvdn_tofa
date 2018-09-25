# ***************************************************************************
# Copyright (C) 2016 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : coreconfig.tcl
# Description: Configure Myriad Core Settings
# Created on : Mar 12, 2016
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {

    namespace eval cli::CoreConfig {

        namespace path ::mdbg

        cli::MultiCmdArgParser create ArgumentParser -auto -brief "Configure core settings" -description {

            Configure core settings

            *** ARGUMENTS

            !!! Type "help coreconfig safety" to get help on arguments.

            *** EXAMPLES

            Do not allow any memory operations while running:
            % coreconfig safety strict
            Make an exception to allow disassembly from uncached memory:
            % coreconfig safety -dasm bypass
            We still want to stop the core when setting the breakpoints:
            % coreconfig safety -breakpoint safe

            Specify all three at once for LeonOS:
            % coreconfig safety -target LOS -default strict -breakpoint safe -dasm bypass

            Enable live RTEMS thread list update (from L2 cache data):
            % coreconfig safety LOS -l1dread bypass -l2read unsafe
            % coreconfig safety LRT -l1dread bypass -l2read unsafe

            Restore default TCF behaviour:
            % coreconfig safety LOS default

        } -default help command {
            safety {
                -brief "Get/set core safety"
                -synopsis {
                    coreconfig safety ?[-target] target? ?general | options...?
                }
                -description {
                    Core safety policy configuration.

                    The core safety policy determines what should happen when different memory operations are\
                    requested from a running core.

                    The following policies are implemented:
                    - Safe (default): let TCF handle safety by always interrupting the core's execution.\
                    Notify user and resume automatically.
                    - Strict: do not stop the core, return memory access failure
                    - Bypass: do not stop the core, bypass the cache memories and access uncached data.
                    - Unsafe: do not stop the core, act as if it were stopped.

                    The following scenarios are supported:
                    - MemGet: Data Read
                    - MemSet: Data Write (implies reading)
                    - Dasm: Instruction Read
                    - Breakpoint: Instruction Write (implies reading)
                    - Level 1 Data Cache Read
                    - Level 1 Data Cache Write
                    - Level 1 Instruction Cache Read
                    - Level 1 Instruction Cache Write
                    - Level 2 Cache Read
                    - Level 2 Cache Write

                    A fallback mechanism is implemented to imply "default" policies per operation:

                        Default > MemGet > MemSet

                        MemGet  > L1 Data Read   > L1 Data Write
                        MemSet  > L1 Data Write

                        Default > Dasm > Breakpoint

                        Dasm       > L1 Instruction Read > L1 Instruction Write
                        Breakpoint > L1 Instruction Read > L1 Instruction Write

                        Default > L2 Cache Read > L2 Cache Write

                    *** NOTES

                    - If Level 1 and Level 2 Cache Data Read policy becomes Unsafe or Bypass\
                    then the RTEMS thread list will be updated during runtime.

                    - Cores in `No clock`, `No power`, `No Debug Support` and `Reset` state are considered\
                    as running by the TCF framework.

                    *** EXAMPLES

                    !!! Type "help coreconfig" to see examples.
                }
                -args {
                    target {
                       -optional
                       -hidden
                       -var target
                       -not-regexp "^(safe|strict|bypass|unsafe|default)$"
                    }
                    -target {
                        -synopsis {
                            [-target] target
                        }
                        -optional
                        -brief "Specify target."
                        -description {
                            If missing, the current target is used.
                        }
                    }
                    -general      { -brief "General safety. Overwrites all previous settings"}

                    -default      { -brief "Default Safety"                        }
                    -mget         { -brief "Data Memory Read Safety"               }
                    -mset         { -brief "Data Memory Write Safety"              }
                    -dasm         { -brief "Instruction Memory Read Safety"        }
                    -breakpoint   { -brief "Instruction Memory Write Safety"       }
                    -l1dread      { -brief "Level 1 Data Cache Read safety"        }
                    -l1dwrite     { -brief "Level 1 Data Cache Write safety"       }
                    -l1iread      { -brief "Level 1 Instruction Cache Read safety" }
                    -l1iwrite     { -brief "Level 1 Instruction Cache Read safety" }
                    -l2read       { -brief "Level 2 Cache Read Safety"             }
                    -l2write      { -brief "Level 2 Cache Write Safety"            }

                    -quiet        { -flag -brief "Return instead of printing." }

                    general       { -optional -brief {Same as "-general"} }

                    {-general -default -mget -mset -dasm -breakpoint -l1dread -l1dwrite -l1iread -l1iwrite -l2read -l2write general} {
                        -optional
                        -choice {safe strict bypass unsafe default}
                    }
                }
                -mutex {
                    {
                        -general general
                        {
                            -default
                            -mget -mset -dasm -breakpoint -default -mget -mset -dasm -breakpoint
                            -l1dread -l1dwrite -l1iread -l1iwrite -l2read -l2write
                        }
                    }
                }
            }
            stateupdate {
                -brief "Enable / Disable Core State Update"
                -description {
                    Control or query the automatic core state update.

                    By default all cores are monitored.

                    *** EXAMPLES

                    Disable state polling for current core (group):
                    % coreconfig stateupdate off

                    Disable state polling for LRT:
                    % coreconfig stateupdate LRT off

                    Disable state polling for All cores:
                    % coreconfig stateupdate A off

                    Enable state polling for Shave Cores:
                    % coreconfig stateupdate -target SALL true

                    Enable state polling for Leon Cores:
                    % coreconfig stateupdate LALL on

                    Enable state polling for Shave Cores, set base interval to 1 second:
                    % coreconfig stateupdate SALL on 1000

                    Set fast state polling interval for LOS
                    % coreconfig stateupdate LOS -interval 50
                }
                -args {
                    target {
                       -optional
                       -hidden
                       -var target
                       -not-regexp "(?i)^(on|off|0|1|true|false|yes|no)$"
                    }
                    -target {
                        -synopsis {
                            [-target] target
                        }
                        -optional
                        -brief "Specify target."
                        -description {
                            If missing, the current target is used.
                        }
                    }
                    -interval {
                        -optional
                        -regexp {^([1-9][0-9]*|default)$}
                        -brief "Specify base update interval in ms."
                        -description {
                            The base update interval is the time between core state updates when\
                            the core state is Suspended. Unpowered/Unclocked state is updated 10x slower\
                            and Running state is updated 10x faster.

                            `default` is 100ms
                        }
                    }
                    -quiet        { -flag -brief "Return instead of printing." }
                    -nocomplain   { -flag -brief "Don't throw error if target does not exist." }
                    enabled {
                        -optional
                        -type boolean
                        -brief "set state (optional)"
                        -description {
                            A Tcl boolean value is expected, e.g.
                            `0`, `1`, `false`, `true`, `on`, `off` etc.

                            If missing, current state will be returned.
                        }
                    }
                    interval {
                        -optional
                        -regexp {^([1-9][0-9]*|default)$}
                        -requires {enabled}
                        -brief "Same as -interval argument"
                        -description {
                            This parameter was added for convenience.
                        }
                    }
                }
            }
            cacheaccess {
                -brief "Get/set cache access policy"
                -synopsis {
                    coreconfig cacheacess ?[-target] target? ?general | options...?
                }
                -description {
                    Cache access policy configuration.

                    The cache access policy determines the cache coherency management during different memory operations of the TCF model.

                    The following policies are implemented:
                    - Precise (default): Cache memory is accessed via debug/diagnostic access.\
                      Write operations update all the data present in all the cache levels. Read operations retrieve\
                      data from nearest cache.
                    - Simple: caches are flushed before read and write, invalidated after write.
                    - Bypass: caches are bypassed. Uncached memory is accessed.

                    The following scenarios are supported:
                    - Default: applies to all operations
                    - Level 1: applies to Level 1 Instruction and Data
                    - Level 1 Data: applies to Read and Write
                    - Level 1 Data Read
                    - Level 1 Data Write
                    - Level 1 Instruction: applies to Read and Write
                    - Level 1 Instruction Read
                    - Level 1 Instruction Write
                    - Level 2: applies to Read and Write
                    - Level 2 Read
                    - Level 2 Write

                *** NOTES

                    - The `general` setting clears all previous settings.
                    - The default general setting is `precise`.

                    - The rule hierarchy is that the more generic settings provide the defaults to more specific settings. For example, \
                    the setting for `Level 1` is the default for `Level1 Instruction` which is the default for `Level 1 Instruction Read`.

                    - Cache access policy only affects the TCF memory model operations, that is, the following are not affected: PIPE I/O, \
                    UART polling, `mget -jtag`, `mset -jtag`, `mdump -jtag`, `loadfile`, `savefile`.

                    - Cache access policy does NOT affect how memory is accessed while the target is seen as RUNNING. See `coreconfig safety` for that.

                *** EXAMPLES

                    Disable cache support entirely. Access only uncached data:
                    % coreconfig cacheaccess A bypass

                    Use simple level 1 instruction cache management for all LEON cores
                    % coreconfig cacheaccess LALL -l1i simple

                    Use simple level 2 cache management for all SHAVE cores
                    % coreconfig cacheaccess SALL -l2 simple

                    Restore default behaviour to all cores
                    % coreconfig cacheaccess A default
                }
                -args {
                    target {
                       -optional
                       -hidden
                       -var target
                       -not-regexp "^(precise|simple|bypass|default)$"
                    }
                    -target {
                        -synopsis {
                            [-target] target
                        }
                        -optional
                        -brief "Specify target."
                        -description {
                            If missing, the current target is used.
                        }
                    }
                    -general      { -brief "General policy. Overwrites all previous settings."}

                    -default      { -brief "Default policy" }

                    -l1           { -brief "Level 1 cache access policy" }

                    -l1i          { -brief "Level 1 Instruction cache access policy" }

                    -l1iread      { -brief "Level 1 Instruction cache read access policy" }
                    -l1iwrite     { -brief "Level 1 Instruction cache write access policy" }

                    -l1d          { -brief "Level 1 Data cache access policy"        }

                    -l1dread      { -brief "Level 1 Data cache read access policy" }
                    -l1dwrite     { -brief "Level 1 Data cache write access policy" }

                    -l2           { -brief "Level 2 cache access policy" }

                    -l2read      { -brief "Level 2 cache read access policy" }
                    -l2write     { -brief "Level 2 cache write access policy" }

                    -quiet        { -flag -brief "Return instead of printing." }

                    general       { -optional -brief {Same as "-general"} }

                    {-general -default -l1 -l1i -l1iread -l1iwrite -l1d -l1dread -l1dwrite -l2 -l2read -l2write general} {
                        -optional
                        -choice {precise simple bypass default}
                    }
                }
                -mutex {
                    {
                        -general general
                        {
                            -default
                            -l1 -l1i -l1iread -l1iwrite -l1d -l1dread -l1dwrite
                            -l2 -l2read -l2write
                        }
                    }
                }
            }
            safety - stateupdate - cacheaccess {
                -arg-complete-scripts {
                    {-target target} {
                        mdbg::cli::Target::getValidTargets $PREFIX*
                    }
                    enabled {
                        lsearch -all -inline -glob {on off true false yes no 0 1} $PREFIX*
                    }
                }
                -mutex {
                    {-target target}
                    {-interval interval}
                }
            }
            help {
                -brief "Get help"
                -args {
                    args {
                        -var helpArgs
                    }
                }
            }
        }

        proc getConfig {core} {
            set config [::tcf::MyriadCore::getConfig $core]
            tcf::fields::getNamedFieldDict MyriadCore::CONFIG $config
        }

        proc setConfig {core config} {
            set config [::json::ObjectFromFieldDict $config ::tcf::MyriadCore::CONFIG]
            ::tcf::MyriadCore::setConfig $core $config
        }

        proc getSafetyValue {var} {
            dict get {
               default DEFAULT
               safe    SAFE
               strict  STRICT
               bypass  BYPASS_CACHE
               unsafe  UNSAFE
            } $var
        }

        proc getCacheAccessValue {var} {
            dict get {
               default DEFAULT
               precise PRECISE
               simple  SIMPLE
               bypass  BYPASS
            } $var
        }

        proc ReadableString {UGLY_STRING} {
            lmap WORD [split $UGLY_STRING "_ "] {
                string totitle $WORD
            }
        }

        cli::GridDisplay create SafetyTable {
            0 {
                -header Scenario
                -min-width 16
                -transform { ReadableString $0 }
            }
            1 {
                -header Policy
                -min-width 16
                -transform { ReadableString $1 }
            }
        }

        oo::objdefine SafetyTable {

            mixin cli::ResultTable

            method display {core safety} {
                my SetTitle "$core Safety"
                set table [list]
                switch -exact [llength $safety] {
                    0 {
                        lappend table [list GENERAL DEFAULT]
                    }
                    1 {
                        lappend table [list GENERAL $safety]
                    }
                    default {
                        dict for {key value} $safety {
                            lappend table [list $key $value]
                        }
                    }
                }
                next $table
                puts ""
            }
         }

        cli::GridDisplay create StateUpdateTable {
            0 {
                -header "Core"
                -min-width 16
            }
            1 {
                -header ""
                -expr {
                    $1 ? "ON" : "OFF"
                }
                -align right
                -min-width 4
            }
            2 {
                -header "Interval"
                -min-width 8
                -align right
                -expr {
                    $2 == "default" ? "" : "$2 ms"
                }
            }
        }

        oo::objdefine StateUpdateTable {
            mixin cli::ResultTable

            method display {dict} {
                my SetTitle "State Monitoring"
                set table [list]
                dict for {core sup} $dict {
                    lappend table [concat [list $core] $sup]
                }
                next $table
                puts ""
            }
        }

        cli::GridDisplay create CacheAccessTable {
            0 {
                -header "Cache Access"
                -min-width 16
                -transform { ReadableString $0 }
            }
            1 {
                -min-width 16
                -header Policy
                -transform { ReadableString $1 }
            }
        }

        oo::objdefine CacheAccessTable {

            mixin cli::ResultTable

            method display {core cacheAccess} {
                my SetTitle "$core Cache Access Policy"
                set table [list]
                switch -exact [llength $cacheAccess] {
                    0 {
                        lappend table [list GENERAL DEFAULT]
                    }
                    1 {
                        lappend table [list GENERAL $cacheAccess]
                    }
                    default {
                        dict for {key value} $cacheAccess {
                            lappend table [list $key $value]
                        }
                    }
                }
                next $table
                puts ""
            }
        }

    } ; # namespace eval cli::CoreConfig

    proc coreconfig {args} {
        cli::CoreConfig::ArgumentParser parseCallerArgs 1 $args
        cli::NoErrorStack {

            switch $command {
                safety {
                    set context [cli::Target::getContextIdFromTargetVar target]
                    set cores [tcf::MyriadCore::getCoreList $context]
                    set result [dict create]
                    foreach core $cores {
                        set config [cli::CoreConfig::getConfig $core]
                        dict update result $core safety {
                            set safety [tcf::getField $config SAFETY DEFAULT]
                            set change 0

                            if {[info exists general]} {
                                set safety [cli::CoreConfig::getSafetyValue $general]
                                incr change
                            } else {
                                foreach {
                                    varName          fieldName
                                } {
                                    default          DEFAULT
                                    mget             MEMORY_READ
                                    mset             MEMORY_WRITE
                                    dasm             DISASSEMBLY
                                    breakpoint       SOFTWARE_BREAKPOINT
                                    l1dread          L1_DATA_CACHE_READ
                                    l1dwrite         L1_DATA_CACHE_WRITE
                                    l1iread          L1_INSTR_CACHE_READ
                                    l1iwrite         L1_INSTR_CACHE_WRITE
                                    l2read           L2_CACHE_READ
                                    l2write          L2_CACHE_WRITE
                                } {
                                    if {[info exists $varName]} {
                                        if {[incr change] == 1} {
                                            if {[llength $safety] == 1} {
                                                set safety [dict create DEFAULT $safety]
                                            }
                                        }
                                        set value [set $varName]
                                        set fieldValue [cli::CoreConfig::getSafetyValue $value]
                                        if {$value == "DEFAULT"} {
                                            dict unset safety $fieldName
                                        } else {
                                            dict set safety $fieldName $fieldValue
                                        }
                                    }
                                }
                            }
                            if {$change} {
                                set config [dict create SAFETY $safety]
                                cli::CoreConfig::setConfig $core $config
                            }
                            if {!$quiet} {
                                cli::CoreConfig::SafetyTable display $core $safety
                            }
                        } ; # dict update $core safety
                    } ; # foreach core

                    if {$quiet} {
                        return $result
                    } else {
                        return
                    }
                }
                cacheaccess {
                    set context [cli::Target::getContextIdFromTargetVar target]
                    set cores [tcf::MyriadCore::getCoreList $context]
                    set result [dict create]
                    foreach core $cores {
                        set config [cli::CoreConfig::getConfig $core]
                        dict update result $core cacheAccess {

                            set cacheAccess [tcf::getField $config CACHE_ACCESS DEFAULT]
                            set change 0

                            if {[info exists general]} {
                                set cacheAccess [cli::CoreConfig::getCacheAccessValue $general]
                                incr change
                            } else {
                                foreach {
                                    varName          fieldName
                                } {
                                    default          DEFAULT
                                    l1               LEVEL_1
                                    l1i              LEVEL_1_INSTR
                                    l1iread          LEVEL_1_INSTR_READ
                                    l1iwrite         LEVEL_1_INSTR_WRITE
                                    l1d              LEVEL_1_DATA
                                    l1dread          LEVEL_1_DATA_READ
                                    l1dwrite         LEVEL_1_DATA_WRITE
                                    l2               LEVEL_2
                                    l2read           LEVEL_2_READ
                                    l2write          LEVEL_2_WRITE
                                } {
                                    if {[info exists $varName]} {
                                        if {[incr change] == 1} {
                                            if {[llength $cacheAccess] == 1} {
                                                set cacheAccess [dict create DEFAULT $cacheAccess]
                                            }
                                        }
                                        set value [set $varName]
                                        set fieldValue [cli::CoreConfig::getCacheAccessValue $value]
                                        if {$value == "DEFAULT"} {
                                            dict unset cacheAccess $fieldName
                                        } else {
                                            dict set cacheAccess $fieldName $fieldValue
                                        }
                                    }
                                }
                            }
                            if {$change} {
                                set config [dict create CACHE_ACCESS $cacheAccess]
                                cli::CoreConfig::setConfig $core $config
                            }
                            if {!$quiet} {
                                cli::CoreConfig::CacheAccessTable display $core $cacheAccess
                            }
                        } ; # dict update $core cacheAccess
                    } ; # foreach $core

                    if {$quiet} {
                        return $result
                    } else {
                        return
                    }
                }
                stateupdate {
                    if {[catch {
                        set context [cli::Target::getContextIdFromTargetVar target]
                    } error options]} {
                        if {$nocomplain} {
                            if {$quiet} {
                                return
                            }
                            return $error
                        }
                        return -code error -options $options $error
                    }
                    set cores [tcf::MyriadCore::getCoreList $context]
                    set result [dict create]
                    foreach core $cores {
                        dict update result $core stateUpdate {
                            set config [dict create]
                            if {[info exists enabled]} {
                                dict set config STATE_UPDATE $enabled
                            }
                            if {[info exists interval]} {
                                dict set config STATE_UPDATE_INTERVAL $interval
                            }
                            if {[dict size $config]} {
                                cli::CoreConfig::setConfig $core $config
                            }
                            set config [cli::CoreConfig::getConfig $core]
                            set stateUpdate [
                                tcf::getFields $config {STATE_UPDATE STATE_UPDATE_INTERVAL} {true default}
                            ]
                        }
                    }
                    if {$quiet} {
                        return $result
                    }
                    cli::CoreConfig::StateUpdateTable display $result
                }
                help {
                    help coreconfig {*}$helpArgs
                }
                default {
                    cli::NotImplemented 2
                }
            }
        }
    }

    shell::autocomplete::addCommand coreconfig cli::CoreConfig::ArgumentParser autocomplete
    cli::Help::Manager add coreconfig -parser cli::CoreConfig::ArgumentParser

    proc getSafetyDict {safety} {
        tcf::fields::getNamedFieldDict MyriadCore::CONFIG::SAFETY $coreSafety
    }

} ; # namespace eval mdbg
