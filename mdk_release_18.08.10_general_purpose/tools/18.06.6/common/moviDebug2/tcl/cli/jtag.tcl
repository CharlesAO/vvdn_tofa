namespace eval mdbg {

    cli::ArgumentParser create cli::JtagPins::ArgumentParser {
        -auto
        -args {
            -trst {
                -choice {low high toggle}
                -default {}
            }
            -srst {
                -choice {low high toggle}
                -default {}
            }
            -led {
                -choice {low high toggle}
                -default {}
            }
        }
    }

    cli::ArgumentParser create cli::JtagScan::ArgumentParser {
        -args {
            reg {
                -synopsis "dr|ir"
                -complete-script {
                    mdbg::shell::autocomplete::filterList \
                        [list dr ir] \
                        [string tolower $PREFIX]*
                }
            }
            count {
                -var numBits
                -type integer
                -not-match {-*}
            }
            pattern {
                -synopsis "pattern ?...?"
            }
            args {
                -hidden
            }
        }
    }

    memory::JtagMem create jtag

    oo::objdefine jtag {
        mixin mdbg::memory::BlockFormatter

        method Hex32 {addr} {
            format {0x%.8X} [expr {$addr & 0xFFFFFFFF}] 
        }

        # override
        method Reg2Addr {addressVar} {
            upvar 1 $addressVar address
            set address [::mdbg::getRegAddr $address]
        }

        method get32 {address} {
            my Reg2Addr address
            my CheckAligned $address
            ::mvproto::get32 $address
        }

        method get64 {address} {
            my Reg2Addr address
            my CheckAligned $address 8
            ::mvproto::get64 $address
        }

        method set32 {address value} {
            my Reg2Addr address
            my CheckAligned $address
            ::mvproto::set32 $address $value
        }

        method set64 {address value} {
            my Reg2Addr address
            my CheckAligned $address 8
            ::mvproto::set64 $address $value
        }

        method getBurst32 {address numWords32} {
            my Reg2Addr address
            my CheckAligned $address
            my CheckBurstSize $address $numWords32
            ::mvproto::getBurst32 $address $numWords32
        }

        method setBurst32 {address wordList} {
            my Reg2Addr address
            my CheckAligned $address
            my CheckBurstSize $address [expr {4 * [llength $wordList]}]
            ::mvproto::setBurst32 $address $wordList
        }

        method UpExpr {level upexpr} {
            incr level
            my Hex32 [uplevel $level [list expr $upexpr]]
        }

        method change32 {address operator expression args} {

            my Reg2Addr address
            my CheckAligned $address

            set VALID_OPS $::mdbg::VALID_REG_CHANGE_OPS ; # defined in mdbg2/registers.tcl
            if {$operator ni $VALID_OPS} {
                return -code error "Invalid operator: must be one of [join $VALID_OPS {, }]"
            }
            set op [string range $operator 0 end-1]

            set changeValue [my UpExpr 1 [concat $expression {*}$args]]
            set oldValue [::mvproto::get32 $address]
            set newValue [my UpExpr 0 [list $oldValue $op $changeValue]]

            ::mvproto::set32 $address $newValue

            list $oldValue $newValue
        }

        method map32 {address command args} {

            my Reg2Addr address
            my CheckAligned $address

            set oldValue [::mvproto::get32 $address]
            set newValue [my Hex32 [uplevel 1 [list $command {*}$args $oldValue]]]

            ::mvproto::set32 $address $newValue

            list $oldValue $newValue
         }

        method CheckAligned {address {alignment 4}} {
            if {$address & ( $alignment - 1 )} {
                return -level 2 -code error "Address \"$address\" is not aligned to $alignment bytes."
            }
        }

        method CheckBurstSize {address numWords32} {
            if { (( $address + $numWords32 * 4 ) & ~1023) != ( $address & ~1023 ) } {
                return -level 2 -code error "Burst across 1K boundary!"
            }
        }

        # otherwise it shows up in autocomplete
        unexport destroy

        method SetPinVal {setbitsVar clearbitsVar state bit} {
            upvar 1 $setbitsVar setbits
            upvar 1 $clearbitsVar clearbits
            if { $state in {toggle high} } {
                set setbits [expr {$setbits | $bit}]
            }
            if { $state in {toggle low} } {
                set clearbits [expr {$clearbits | $bit}]
            }
        }

        method pins {args} {
            ::mdbg::cli::JtagPins::ArgumentParser parseCallerArgs 1 $args
            ::mdbg::cli::NoErrorStack {
               set setbits   0
               set clearbits 0
               my SetPinVal setbits clearbits $trst 1
               my SetPinVal setbits clearbits $srst 2
               my SetPinVal setbits clearbits $led  4
               if {[::mvproto::jtagPins $setbits $clearbits]} {
                   return
               }
            }
            return -code error "Not supported by server"
        }

        method ir {value} {
            if {[::mvproto::jtagSendIR $value]} {
                return
            }
            return -code error "Not supported by server"
        }

        method scan {args} {

            mdbg::cli::JtagScan::ArgumentParser parseCallerArgs 2 $args

            mdbg::cli::NoErrorStack {
                set regIdx [lsearch {dr ir} [string tolower $reg]]
                if {$regIdx < 0} {
                    error "Invalid JTAG register: must be \"dr\" or \"ir\"."
                }

                set lsbFormat [list]
                set lsbData ""

                set values [list $pattern {*}$args]

                foreach value [lreverse $values] {
                    if {[regexp -nocase {^0x([0-9a-z]+)$} $value -> hexStr]} {
                        set len [string length $hexStr]
                        set binStr [format %0*b [expr {4 * $len}] $value]
                        lappend lsbFormat 0x $len
                    } elseif {[regexp -nocase {^0b([01]+)$} $value -> binStr]} {
                        lappend lsbFormat 0b [string length $binStr]
                    } else {
                        error "`pattern`: expected 0b<bin> or 0x<hex> number, got \"$value\""
                    }
                    append lsbData [string reverse $binStr]
                }
                set actualBits [string length $lsbData]
                if {$actualBits != $numBits} {
                    error "bit count mismatch: expected $numBits, got $actualBits"
                }

                # mdbg::cli::PutDebug "0b[string reverse $lsbData]"

                set inOutByteData [binary format b* $lsbData]

                ::mvproto::jtagScan $regIdx inOutByteData $numBits

                if {![binary scan $inOutByteData b[expr {$numBits}] lsbResult]} {
                    error "Server returned invalid data (length does not match)."
                }

                # mdbg::cli::PutDebug "0b[string reverse $lsbResult]"

                set lsbValues [list]
                set pos 0
                foreach {fmt len} $lsbFormat {
                    set beg $pos
                    if {$fmt == "0x"} {
                        incr pos [expr {4 * $len}]
                    } else {
                        incr pos $len
                    }
                    set end [expr {$pos-1}]
                    set value 0b[string reverse [string range $lsbResult $beg $end]]
                    if {$fmt == "0x"} {
                        set value [format 0x%0*X $len $value]
                    }
                    lappend lsbValues $value
                }

                lreverse $lsbValues
            }
        }

    } ; # oo objdefine jtag

    proc ADD_JTAG_HELP {} {
        foreach {subcmd params short extra} {
            {}                 {subcommand ?args?...}              "Direct JTAG interface."                 {
                -long {
                    Provides interface to various JTAG operations.

                    See "help jtag *" for list of subcommands.
                }
            }
            {get32}            {address}                           "Read word (32bit)."                     {
                -args {
                    address {
                        -short "address or register name +offset"
                    }
                }
            }
            {set32}            {address value32}                   "Write word (32bit)."                    {
                -args {
                    address {
                        -short "address or register name +offset"
                    }
                    value32 {
                        -short "32-bit integer value"
                    }
                }
            }
            {change32}         {address operator expr...}          "Change word (32-bit)."                  {
                -long {
                    Combine read-modify-write operation in a single command.

                *** EXAMPLES

                    Enable the LEON OS and RT DSU by setting the corresponding bits in the register:
                    % jtag change32 CPR_GEN_CTRL_ADR |= 1 << 15 | 1 << 17

                   Disable the LEON OS and RT DSU by clearing the same bits:
                    % jtag change32 CPR_GEN_CTRL_ADR &= ~(1 << 15 | 1 << 17)
                }
                -args {
                    address {
                        -short "address or register name +offset"
                    }
                    operator {
                        -short "modify operator"
                        -long {
                            One of: `|=`, `&=`, `^=`, `+=`, `-=`, `*=`, `/=`, `<<=`, `>>=`
                        }
                    }
                    expr... {
                        -short "value or expression to apply"
                        -long {
                            Can be a numeric value or any Tcl `expr` expression.
                        }
                    }
                }
            }

            {get64}            {address}                           "Read double word (64-bit)."             {
                -args {
                    address {
                        -short "address or register name +offset"
                    }
                }
            }
            {set64}            {address value64}                   "Write double word (64-bit)."            {
                -args {
                    address {
                        -short "address or register name +offset"
                    }
                    value64 {
                        -short "64-bit integer value"
                    }
                }
            }
            {getBurst32}       {address numWords32}                "Burst read 32-bit words."               {}
            {setBurst32}       {address listOfWords}               "Burst write 32-bit words."              {}
            {ir}               {number}                            "Send JTAG instruction register value."  {}
            {scan}             {dr|ir count pattern ...}           "Scan JTAG register bits."               {
                -long {
                    Shifts bits of JTAG register in and out.

                    The bits can be specified in groups of either binary or hexadecimal format.

                    The concatenated patterns will be shifted in least significant bit first.
                    The result will be in exactly the same format as the input.
                }
                -args {
                    dr|ir {
                        -short "JTAG register."
                    }
                    count {
                        -short "Total bit count."
                        -long {
                            This value is checked against the bit-length of the pattern list provided.
                            An error is generated on mismatch.
                        }
                    }
                    pattern {
                        -synopsis "pattern ?...?"
                        -short "Bit patterns."
                        -long {
                            One or more binary or hexadecimal bit patterns.
                            (Given as separate arguments).

                            Binary patterns must begin with "0b", hex patterns with "0x".

                            The number of digits in the pattern will determine the number of bits.
                            (One hex digit equals four bits.)

                            The big endian bit patterns are concatenated and shifted in least significant bit first.
                            The result will be a list of patterns having the same format.
                        }
                    }
                }
            }
            {pins}             {?-trst high|low|toggle? ?-srst high|low|toggle? ?-led high|low|toggle?}
                                                                   "Set JTAG pins state." {
                -args {
                    -trst {
                        -synopsis "?-trst high|low|toggle?"
                        -short "Change TRST pin state."
                    }
                    -srst {
                        -synopsis "?-srst high|low|toggle?"
                        -short "Change SRST pin state."
                    }
                    -led {
                        -synopsis "?-led high|low|toggle?"
                        -short "Change LED pin state."
                    }
                }
            }

            {readBlock}        {address size}                      "Read block."                            {}
            {writeBlock}       {address data}                      "Write block."                           {}
            {fillBlock}        {address size pattern}              "Fill block."                            {}

            {scanBlock}        {format address size varnames...}   "Read+scan block into variables."        {}
            {formatBlock}      {format address size arguments...}  "Format+write arguments."                {}

            {getBlock}         {?-scan binFmt? address size}        "Read (and scan) block."                {
                -args {
                    binFmt {
                        -synopsis "?-scan binFmt?"
                        -short "Format specifier compatible with Tcl built-in `binary format`"
                    }
                }
            }
            {setBlock}         {?-format binFmt? address data}     "(Format data and) write block."         {
                    -args {
                        binFmt {
                            -synopsis "?-format binFmt?"
                            -short "Format specifier compatible with Tcl built-in `binary format`"
                        }
                    }
            }
            {lock}             {script}                             "Lock JTAG access during script"             {
                    -args {
                        script {
                            -short "Block of commands to execute within exclusive access block."
                        }
                    }
                    -long {
                        Prevent other threads from accessing the JTAG interface during given script.

                        Normally the TCF thread will periodically access the JTAG interface. This can\
                        be prevented by moving select operations inside a `jtag lock` block.

                    *** NOTES

                        - Scripts should be short and mostly using `jtag` subcommands!
                        - UART polling and pipe processing are performed on the same thread, so\
                          either disable UART and remove all pipes or avoid any Tcl event handling inside the script.\
                          That is, don't call `update`, `wwait`, `wait`.
                        - Invoking TCF-related commands within the script will cause the system to lock up\
                          because the TCF thread will probably be hung waiting on the JTAG lock held by this thread.
                        - Do not use the `mget` command in the script (except maybe `mget -jtag -reg XXXX`), \
                          this command will talk to TCF and it also does Tcl event processing while doing so.
                        - Use `jtag get32`, `jtag set32` and `jtag change32` commands to access registers
                        - Use the simple `after` command if you want to introduce delays inside the lock script.

                    *** EXAMPLES

                        A kind a software reset implemented manually:

                                jtag lock {
                                    # Set SRST first physical low then physical high
                                    foreach state {low high} {
                                        jtag pins -srst $state
                                    }
                                    # Wait 50 ms without event processing
                                    after 50
                                    # Send IR sequence
                                    foreach cmd {0x1 0xe 0xe} {
                                        jtag ir $cmd
                                    }
                                    # Enable Leon OS and RT DSU
                                    jtag change32 CPR_GEN_CTRL_ADR |= 1<<15 | 1 <<17
                                }
                    }
            }
        } {
            uplevel 1 [list \
                cli::Help::Manager add "jtag $subcmd" \
                -short $short \
                -synopsis "jtag $subcmd $params" \
                {*}$extra
            ]
        }
        rename [lindex [info level 0] 0] ""
    }

    ADD_JTAG_HELP

    shell::autocomplete::addScript jtag {
        # $COMMAND $PARAMS $PREFIX
        if {$PARAMS == {}} {
            shell::autocomplete::filterList \
                [info object methods $COMMAND -all] $PREFIX*
        } else {
            set PARAMS [lassign $PARAMS COMMAND]
            switch -exact $COMMAND {
                pins {
                    cli::JtagPins::ArgumentParser autocomplete $COMMAND $PARAMS $PREFIX
                }
                scan {
                    cli::JtagScan::ArgumentParser autocomplete $COMMAND $PARAMS $PREFIX
                }
                get32 - set32 -
                get64 - set64 -
                getBurst32 - setBurst32 -
                readBlock - writeBlock -
                fillBlock {
                    if {[llength $PARAMS] == 0} {
                        ::mdbg::findReg $PREFIX*
                    } else {
                        list $PREFIX
                    }
                }
                change32 {
                    switch -exact [llength $PARAMS] {
                        0 {
                            ::mdbg::findReg $PREFIX*
                        }
                        1 {
                            lsearch -all -inline -glob $::mdbg::VALID_REG_CHANGE_OPS $PREFIX*
                        }
                        default {
                            list $PREFIX
                        }
                    }
                }
                default {
                    list $PREFIX
                }
            }
        }
    }

    namespace export jtag

} ; # namespace eval mdbg