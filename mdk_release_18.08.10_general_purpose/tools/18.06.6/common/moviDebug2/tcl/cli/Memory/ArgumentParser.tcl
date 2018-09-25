# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/memory/ArgumentParser.tcl
# Description: Argument parser for mget, mset, mdump, mfill etc
# Created on : Sep 14, 2017
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::cli::Memory {

    cli::MultiCmdArgParser create ArgumentParser -auto -brief {
        Memory operations (get/set/fill)
    } memCmd {
        get {
            -brief "Get value from memory location."
            -description {
                Display or return value of specified address, register, symbol, variable or expression.

                *** ALIASES

                ALIAS       COMMAND
                mgetv       mget -value
                mgetb       mget -binary
                mgets       mget -asciiz

                *** EXAMPLES

                Display 32-bit unsigned value at some address:
                % mget 0x14400010

                Display the value of the LRT PC register:
                % mget PC -target LRT

                Display the value of the local variable called PC on LRT:
                % mget -target LRT -var PC

                Display the value of the global symbol called PC as the LRT sees it:
                % mget -target LRT -sym PC

                Display the values of 32 input registers of LRT as signed integers, starting from I0:
                % mget I0 -reg -type int -count 32 -target LRT

                Display the V0 register value of Shave 0 (will show four rows of values):
                % mget V0 -target S0 -type float

                Get PC register value and the value of the next 2 registers of LRT:
                % mget pc -target lrt -count 3

                Get the value of the I0 register as two half-precision floating point values:
                % mget i0 -type float -size 2 -count 2

                Return 8 double precision and 16 single precision LEON floating point register values\
                (little endian) as a list of two lists, first element has 8 values, second has 16 values:
                % mget f0 -size [expr 8*8+16*4] -binary -scan q8r16
            }
        }
        set {
            -brief "Set value to memory location."
            -description {
                Set value of specified address, register, symbol, variable or expression.

                *** EXAMPLES

                Set a 32-bit word representing the value 10 at address:
                % mset 0x14400010 10

                Set PC register value of LRT:
                % mset -target LRT -reg PC 0x76000000

                Set the variable named "PC" variable tLRT:
                % mset -target LRT -var PC  10

                Set the value of the global symbol named "PC",\
                as if performed by the LeonRT (updating the L1+L2 cache):
                % mset -target LRT -sym PC 15

                Set athe IEEE 32-bit float representation of 4.3 in the I0 register:
                % mset -reg i0 -type float 4.3

            }
        }
        fill {
            -brief "Fill memory location with pattern."
            -description {
                Fill value of specified address, register, symbol, variable or expression with pattern.

                *** EXAMPLES

                Fill an array with the byte value 255:
                % mfill my255Array -format c 255

                Fill an array with the Ascii pattern "Hello!":
                % mfill myHelloArray "Hello!"

                Fill an array with the  pattern `0xCAFEBABE`:
                % mfill myCafeArray -format i 0xCAFEBABE

                Fill an array with the 4 byte pattern `0xDEAD`:
                % mfill myDeadArray -format s 0xDEAD

                Fill the first 256KiB of DDR with zeros, using the JTAG interface (faster):
                % mfill -jtag -reg DDR_BASE_ADR -size [expr 256*1024] -zero

                Fill the same area with the pattern "Hello!\r\n":
                % mfill -jtag -reg DDR_BASE_ADR -size [expr 256*1204] -format a*cc Hello! 13 10
            }
        }
        dump {
            -brief "Dump memory in a canonical hexdump-like format."
            -description {
                Display specified number of values from specified address in a human readable format.

                *** EXAMPLES

                Display 10 32-bit words from DDR in hexdump-like format\
                (hexadecimal little endian 32-bit words + ASCII):
                % mdump 0x70000000 10
            }
        }
        get - set - fill {
            -args {
                location(1) {
                    -optional
                    -var where
                    -brief "Memory location - address, register, symbol, variable or expression (before switches)."
                    -description {
                        One and only one of the `location(*)` arguments must be provided.

                        The type will be tried to be detected automatically, unless one of\
                        "-addr", "-reg", "-sym", "-var", "-expr" options is also given.
                    }
                }
                {-addr -reg -sym -var -expr -id} {
                    -flag
                    -opt-param    location(2)
                    -opt-var      where
                    -hidden
                }

                -addr     {-brief "Treat `location(*)` as numeric address."}
                -reg      {-brief "Treat `location(*)` as register name."}
                -sym      {-brief "Treat `location(*)` as (global) symbol."}
                -var      {-brief "Treat `location(*)` as (local) variable."}
                -expr     {-brief "Treat `location(*)` as expression."}
                -id       {-brief "Treat `location(*)` as TCF Symbol ID"}
                {-addr -reg -sym -var -expr -id} {
                    -description {
                        One and only one of the `location(*)` arguments must be provided.
                    }
                }
            }
            -mutex {
                {-addr -reg -sym -var -expr -id}
                {-frame {-addr -reg}}
            }
        }
        get - set {
            -args {
                -type {
                    -choice {int hex unsigned float pointer binary asciiz}
                    -optional
                    -synopsis {
                        ?-type typeclass?
                    }
                    -brief "Force type class for value."
                    -description {
                        Affects display/returned value (get) or binary format of scanned string value (set).

                        The following type classes are recognized:

                        int      - signed integer               1, 2, 4, 8 bytes
                        hex      - unsigned hexadecimal integer 1, 2, 4, 8 bytes
                        unsigned - unsigned decimal integer     1, 2, 4, 8 bytes
                        float    - floating point                  2, 4, 8 bytes
                        pointer  - pointer                            4, 8 bytes
                        binary   - raw binary string, any size
                        asciiz   - null terminated ASCII string or string pointer
                    }
                }
            }
        }
        fill {
            -args {
                -size {
                    -type integer
                    -var size
                    -optional
                    -brief "Specifiy fill size"
                    -description {
                        Default is to fill the entire `location`.
                    }
                }
                -zero {
                    -flag
                    -brief "Fill with zero"
                    -description {
                        Data is zeros.

                        Cannot be used with "-offset", "-length", "-format" and "-data"
                    }
                }
                -offset {
                    -type integer
                    -var srcSliceOffset
                    -default 0
                    -brief "Take data from offset `srcSliceOffset` of input."
                    -description {
                        Default is to take from beginning of the binary formatted input.
                    }
                }
                -length {
                    -type integer
                    -var srcSliceLength
                    -optional
                    -brief "Take only `srcSliceLength` bytes from input."
                    -description {
                        Default is to take all of the binary formatted input from ?srcSliceOffset? to end (see "-offset").
                    }
                }
                -format {
                    -optional
                    -var tclBinaryFormat
                    -brief "Format non-binary input argument(s)."
                    -description {
                        Normally the input data should already be in raw binary format.
                        This switch allows formatting non-binary data using Tcl's built-in "binary format" command.
                        The `tclBinaryFormat` argument is the same as for the aforementioned comand's `format` argument.

                        The formatted binary input will be optionally sliced according to the "-offset" and "-length" arguments\
                        and used as fill pattern.

                        Tcl `binary format` specfiers quick reference:

                        `a` : Stores a byte string in the output string. Every character is taken as modulo 256 (i.e. the low byte of every character is used, and the high byte discarded)
                        `A` : Same as `a`, except that spaces are used for padding instead of nulls.
                        `b` : Stores a string of binary digits in low-to-high order within each byte in the output string.
                        `B` : Same as `b` except that the bits are stored in high-to-low order within each byte.
                        `H` : Stores a string of count hexadecimal digits in high-to-low within each byte in the output string.
                        `h` : Same as `H` except that the digits are stored in low-to-high order within each byte.
                        `c` : Stores one or more 8-bit integer values in the output string.
                        `s` : Same as `c` except that it stores one or more 16-bit integers in little-endian byte order in the output string.
                        `S`:  Same as `s` except that it stores one or more 16-bit integers in big-endian byte order in the output string.
                        `t`:  (mnemonically "tiny") same as `s` and `S` except that it stores the 16-bit integers in the output string in the native byte order of the machine.
                        `i`:  Same as `c` except that it stores one or more 32-bit integers in little-endian byte order in the output string.
                        `I`:  Same as `i` except that it stores one or more one or more 32-bit integers in big-endian byte order in the output string.
                        `w`:  Same as `c` except that it stores one or more 64-bit integers in little-endian byte order in the output string.
                        `W`:  Same as `w` except that it stores one or more one or more 64-bit integers in big-endian byte order in the output string.
                        `m`:  (mnemonically the mirror of `w`) is the same as `w` and `W` except that it stores the 64-bit integers in the output string in the native byte order of the machine where the Tcl script is running.
                        `f`:  Same as `c` except that it stores one or more one or more single-precision floating point numbers in the machine's native representation in the output string.
                        `r`:  (mnemonically "real") same as `f` except that it stores the single-precision floating point numbers in little-endian order.
                        `R`:  Same as `r` except that it stores the single-precision floating point numbers in big-endian order.
                        `d`:  Same as `f` except that it stores one or more one or more double-precision floating point numbers in the machine's native representation in the output string.
                        `q`:  (mnemonically the mirror of `d`) is the same as `d` except that it stores the double-precision floating point numbers in little-endian order.
                        `Q`:  Same as `q` except that it stores the double-precision floating point numbers in big-endian order.
                        `x`:  Stores count `null` bytes in the output string. This type does not consume an argument.
                        `X`:  Moves the cursor back count bytes in the output string. If count is * or is larger than the current cursor position, then the cursor is positioned at location 0 so that the next byte stored will be the first byte in the result string. If count is omitted then the cursor is moved back one byte. This type does not consume an argument.
                        `@`:  Moves the cursor to the absolute location in the output string specified by count. Position 0 refers to the first byte in the output string. If count refers to a position beyond the last byte stored so far, then null bytes will be placed in the uninitialized locations and the cursor will be placed at the specified location. If count is *, then the cursor is moved to the current end of the output string. If count is omitted, then an error will be generated. This type does not consume an argument.

                        To determine what the native byte order of the machine is, refer to the `byteOrder` element of the `tcl_platform` array.
                    }

                    -complete-script {
                        mdbg::shell::autocomplete::filterList {
                            c s i w a* b* r q
                            H* H2 H4 H8 B*
                            S I W R Q
                        } $PREFIX*
                    }
                }
            }
        }
        get - set {
            -args {
                -size {
                    -type integer
                    -optional
                    -brief "Specifiy size of location."
                    -description {
                        This is usually used in conjuction with the "-type" argument,
                        to override the detected size of memory location.

                        NOTE: not all -type(class) / -size combinations are legal, see "-type".
                    }
                }
                -endian {
                    -choice {little big}
                    -default little
                    -brief "Force endiannes of value."
                    -description {
                        Only little-endian is supported.
                    }
                    -hidden
                }
            }
        }
        get {
            -args {
                -value {
                    -flag
                    -brief "Return value."
                    -description {
                        Only return the (formatted/structured) value of the memory location.

                        NOTE:
                        Supresses display by default, but this can be reenabled by the "-full" flag.
                        Structures and arrays may be returned as Tcl dictionary/list hierarchies.

                        Incompatible with "-quiet" flag.
                    }
                }
                -binary {
                    -brief "-type binary -value"
                    -description {
                        Return binary value directly.

                        Cannot be used in conjuction with "-type" and\
                        whatever the "-value" switch is incompatible with\
                        (e.g. "-quiet").
                    }
                    -flag
                }
                -scan {
                    -var tclBinaryFormat
                    -optional
                    -requires {-binary}
                    -brief "Scan binary value."
                    -description {
                        Allows scanning non-binary data using Tcl's built-in "binary scan" command.
                        The `tclBinaryFormat` argument is passed along as `binary scan`'s `format` argument.

                        NOTE
                        A list will be returned if the `binary scan` command would have required more than one\
                        variable as its remaining arguments.

                        This switch requires the use of the "-binary" switch.

                        Tcl `binary scan` specifiers quick reference:

                        `a`: The data is a byte string.
                        'A': This form is the same as `a`, except trailing blanks and nulls are stripped from the scanned value before it is stored.
                        `b`: The data is turned into a string of count binary digits in low-to-high order represented as a sequence of “1” and “0” characters.
                        `B`: Same as `b`, except the bits are taken in high-to-low order within each byte
                        `H`: The data is turned into a string of hexadecimal digits in high-to-low order.
                        `h`: Same as `H`, except the digits are taken in reverse (low-to-high) order within each byte
                        `c`: The data is turned into 8-bit signed integers and stored as a list.\
                        `s`: The data is interpreted as (a list of) 16-bit signed integers represented in little-endian byte order.
                        `S`: Same as `s` except that the data is interpreted as count 16-bit signed integers represented in big-endian byte order.
                        `t`: The data is interpreted as 16-bit signed integers represented in the native byte order.
                        `i`: The data is interpreted as 32-bit signed integers represented in little-endian byte order.
                        `I`: Same as `i` except that the data is interpreted as count 32-bit signed integers represented in big-endian byte order. For example,
                        `n`: The data is interpreted as 32-bit signed integers represented in the native byte order of the machine running the Tcl script.
                        `w`: The data is interpreted as 64-bit signed integers represented in little-endian byte order.
                        `W`: Same as `w` except that the data is interpreted as count 64-bit signed integers represented in big-endian byte order.
                        `m`: The data is interpreted as 64-bit signed integers represented in the native byte order of the machine running the Tcl script.
                        `f`: The data is interpreted as single-precision floating point numbers in the machine's native representation.
                        `r`: Same as `f` except that the data is interpreted as count single-precision floating point number in little-endian order.
                        `R`: Same as `f` except that the data is interpreted as count single-precision floating point number in big-endian order.
                        `d`: Same as `f` except that the data is interpreted as count double-precision floating point numbers in the machine's native representation.
                        `q`: Same as `d` except that the data is interpreted as count double-precision floating point number in little-endian order.
                        `Q`: Same as `d` except that the data is interpreted as count double-precision floating point number in big-endian order.
                        `x`: Moves the cursor forward `count` bytes in string. If `count` is "*" or is larger than the number of bytes after the current cursor position,\
                        then the cursor is positioned after the last byte in string. If `count` is omitted, then the cursor is moved forward one byte.\
                        Note that this type does not consume an argument.
                        `X`: Moves the cursor back `count` bytes in string. If count is "*" or is larger than the current cursor position, then the cursor is positioned at location 0 so that the next byte scanned will be the first byte in string. If count is omitted then the cursor is moved back one byte. Note that this type does not consume an argument.
                        `@`: Moves the cursor to the absolute location in the data string specified by `count`. Note that position 0 refers to the first byte in string.\
                        If count refers to a position beyond the end of string, then the cursor is positioned after the last byte.

                        To determine what the native byte order of the machine is, refer to the `byteOrder` element of the `tcl_platform` array.
                    }
                }
                -asciiz {
                    -brief "-type asciiz -value"
                    -description {
                        Return value as decoded ASCII string.

                        Location will be treated as if start address of a null terminated string,\
                        EXCEPT when it's detected to be a pointer, in this case it will be dereferenced.
                        Location is read until the null terminator or the limit is reached.

                        The "-size" option limits the size of the returned string.

                        Cannot be used in conjuction with "-binary" switch.
                    }
                    -flag
                }
                -count {
                    -type integer
                    -optional
                    -brief "Number of items beginning at location."
                    -description {
                        Default is 1.

                        For scalars and structures, if specified, `count-1` items following the specified
                        location will also be retrieved.

                        For arrays, it will retrieve the first `count` elements of the array.

                        NOTES:
                        Causes the return value to be a list!
                        Some local vector register names are configured to set this option automatically!
                        "-structdepth" is adjusted to at least `1` when this is specified for an array variable.

                        Cannot be used together with final `count` argument.
                    }
                }
                -structdepth {
                    -var maxDepth
                    -type integer
                    -optional
                    -synopsis {
                        ?-[struct]depth?
                    }
                    -brief "Structure fields/Array items recursion limit."
                    -description {
                        Default is 1.
                        Determines how deeply recursively the display of a structure/array should be performed.

                        NOTE:
                        ALL OF THE DATA is retrieved first and this only affects the display!
                    }
                }
                -depth {
                    -var maxDepth
                    -type integer
                    -optional
                    -hidden
                }
                -quiet {
                    -flag
                    -brief "Don't print anything. Return data as Tcl structure (list/dictionary)."
                    -description {
                        Normally the command shows a nice tabular display of the information and does not return a value.
                        This can be suppressed and the machine-readable format will be returned instead.

                        NOTES:
                        This option is for advanced cases and internal reuse.
                        The "-value" flag is better suited for typical use-cases.

                        Specifying the "-count" option makes the return value a list of dictionaries, even when "count" is 1.
                        This also applies for some vector registers where's an implicit count.
                    }
                    -hidden
                }
                -full {
                    -flag
                    -brief "Display full information."
                    -description {
                        This is the default behaviour, so this switch only needs to be explicitly given when "-value" is also present.

                        This option is for scripting, when value is needed but the table should be also displayed.
                    }
                    -hidden
                }
                {-A -R -S -V -X} {
                    -flag -hidden
                    -opt-param location(2)
                    -opt-var   where
                    -opt-hidden
                }
                -A {-brief "-value -addr"}
                -R {-brief "-value -reg"}
                -S {-brief "-value -sym"}
                -V {-brief "-value -var"}
                -X {-brief "-value -expr"}
            }
            -mutex {
                {{-value -binary -asciiz} -quiet}
                {-type -binary -asciiz}
                {-depth -structdepth}
                {-A -R -S -V -X {
                    -addr -reg -sym -var -expr -id
                    -value -quiet -full
                    }
                }
                {-frame {-A -R}}
            }
        }
        set {
            -args {
                {-list -dict} {
                    -flag
                }
                -binary {
                    -brief "-type binary -size <length of `what`>"
                    -description {
                        Treat `what` as binary data and write all of it
                        at the specified location.

                        Cannot be used in conjuction with "-type" and "-size"
                    }
                    -flag
                }
                -list {
                    -brief "Set array from list."
                    -description {
                        !! NOT IMPLEMENTED !!
                        Takes each list element from supplied input and sets the array's elements individually.
                    }
                }
                -dict {
                    -brief "Set array/structure from list/dictionary."
                    -description {
                        !! NOT IMPLEMENTED !!
                        Takes each dictionary key/value item from supplied input and sets the structure fields individually.
                    }
                }
            }
            -mutex {
                {-list -dict}
                {-binary {-type -size}}
            }

        }
        get - set - fill - dump {
            -args {
                -target {
                    -default {}
                    -brief "Operate on specific target."
                    -description {
                        If missing, current target is used.
                    }
                    -complete-script {
                        lsort -dictionary [cli::Target::getValidTargets $PREFIX*]
                    }
                }
                -frame {
                    -type integer
                    -optional
                    -brief "Specify reference stack frame."
                    -description {
                        Search for variable(s) on stack frame context #`frame`.
                        Default is 0 (top frame) for "-var", otherwise uses non-frame context.
                    }
                }
                -jtag {
                    -flag
                    -brief "Use direct JTAG interface for memory I/O."
                    -description {
                        This flag forces the command to bypass TCF's memory context infrastructure to do memory I/O.
                        JTAG I/O is done directly using the "jtag" command which will use direct protocol commands
                        from the "::mvproto" namespace.
                        It is useful in certain scenarios where cache coherence is not a problem.
                        Incompatible with "-mem" argument.
                    }
                }
                -mem {
                    -brief "Target for memory I/O."
                    -description {
                        Normally the memory operations go through the target context's associated memory context.
                        This argument allows specifying a different context to do I/O through.
                        Incompatible with "-jtag" flag.
                    }
                    -optional
                    -var memTarget
                    -complete-script {
                        cli::Target::getValidTargets $PREFIX*
                    }
                }
            }
            -mutex {
                {-jtag -mem}
            }
        }
        get - set - fill {
           -args {
                location(3) {
                    -optional
                    -var where
                    -brief "Memory location - address, register, symbol. variable or expression (after switches)."
                    -description {
                        One and only one of the `location(*)` arguments must be provided.

                        The type will be tried to be detected automatically, unless one of\
                        "-addr", "-reg", "-sym", "-var", "-expr" flags is also given.
                    }
                }
            }
            -mutex {
                {location(1) location(2) location(3)}
            }
            -requires {
                {location(1) location(2) location(3)}
            }
            -arg-complete-script {
                {location(1) location(3)} {
                    # no need to provide for location(2) since it's handled by location(3)
                    mdbg::cli::Memory::ArgumentParser autocomplete-location $PARAMS $PREFIX
                }
            }
        }
        set {
            -args {
                what {
                    -brief "Input value."
                    -description {
                        Its form should be compatible with the memory location's type.
                    }
                }
            }
        }
        get {
            -args {
                count {
                    -type integer
                    -optional
                    -brief "Number of values beginning with address."
                    -description {
                        Same as "-count" option. Added for convenience.

                        Cannot be used together with "-count".
                    }
                }
            }
            -mutex {
                {-count count}
            }
        }
        fill {
            -args {
               data {
                   -brief "Input value(s)."
                   -description {
                       Normally it will be a binary/ascii string value, unless the "-format" option is also present.

                       In this case there can be multiple arguments, but at least one must be present.

                       See: "-format" option.
                   }
                   -multi
                   -optional
               }
            }
            -mutex {
                {-zero {-offset -length -format data}}
            }
            -requires {
                {data -zero}
            }
        }
        dump {
            -args {
                location {
                    -var where
                    -brief "Memory location - address, register, symbol, variable or expression."
                    -description {
                        Start Address from where values are being displayed.

                        The type will be tried to be detected automatically.
                    }
                    -complete-script {
                        mdbg::cli::Memory::ArgumentParser autocomplete-location $PARAMS $PREFIX
                    }
                }
                count {
                    -type integer
                    -brief "The number of words being displayed."
                    -description {
                        Must be an integer.
                    }
                }
            }
        }
    }

    oo::objdefine ArgumentParser {

        method setupArgs { {level 0} } {
            incr level
            upvar $level memCmd memCmd
            if {$memCmd == "get"} {
                upvar $level value value
                foreach {shortVar longVar} {
                         A        addr
                         S        sym
                         R        reg
                         V        var
                         X        expr
                } {
                    upvar $level $shortVar short
                    upvar $level $longVar long
                    if {$short} {
                        lassign {1 1} long value
                    }
                    unset short
                }
                upvar $level maxDepth maxDepth
                if {![info exists maxDepth]} {
                    set maxDepth 1
                }
            }

            my CheckArgs $level
            my SetupCommonArgs $level
            my SetupContext $level
        }

        method createSymInfo {} {
            uplevel 1 {
                set symInfo [expr {
                    $addr ? [cli::Memory::createAddrSymInfo $where $contextId] :
                    $reg ? [cli::Memory::createRegSymInfo $where $contextId] :
                    $sym ? [cli::Memory::createVarSymInfo $where $contextId] :
                    $id ? [cli::Memory::createIdSymInfo $where $symCache] :
                    $var ? [cli::Memory::createVarSymInfo $where $frameId $PC] :
                    $expr ? [cli::Memory::createExprSymInfo $where $frameId] :
                    [cli::Memory::getSymInfo $where $contextId $frameId]
                }]
                if {![dict exists $symInfo NAME]} {
                    dict set symInfo NAME $where
                }
            }
        }

        method fillSymInfo {} {
            uplevel 1 {
                if {[info exists binary] && $binary} {
                    dict set symInfo TYPE binary
                    dict set symInfo TYPE_NAME <binary>
                    # mset -binary ... ... what
                    if {[info exists what]} {
                        set what [binary format a* $what]
                        dict set symInfo SIZE [string length $what]
                    }
                    # mget -binary ?-scan tclBinaryFormat? ...
                    if {[info exists value]} {
                        if {[info exists tclBinaryFormat]} {
                            dict set symInfo TCL_BINARY_FORMAT $tclBinaryFormat
                        }
                        set value 1
                    }
                } elseif {[info exists asciiz] && $asciiz || [info exists type] && $type == "asciiz"} {
                    # mget -asciiz ?-size size?
                    dict set symInfo ASCIIZ_INDIRECT 0
                    if {[dict exists $symInfo TYPE]} {
                        switch -exact [dict get $symInfo TYPE] {
                            POINTER {
                                dict set symInfo ASCIIZ_INDIRECT 1
                                dict set symInfo ASCIIZ_MAX_LEN 4096
                            }
                            default {
                                switch -exact [dict get $symInfo KIND] {
                                    -addr {
                                        dict set symInfo ASCIIZ_MAX_LEN 4096
                                    }
                                }
                            }
                        }
                    }
                    if {![dict get $symInfo ASCIIZ_INDIRECT]} {
                        if {![dict exists $symInfo ASCIIZ_MAX_LEN]} {
                            if {[dict exists $symInfo SIZE]} {
                                dict set symInfo ASCIIZ_MAX_LEN [dict get $symInfo SIZE]
                            }
                        }
                    }
                    if {[info exists size]} {
                        dict set symInfo ASCIIZ_MAX_LEN $size
                    }
                    dict set symInfo TYPE ASCIIZ
                    dict set symInfo TYPE_NAME <asciiz>

                    if {[info exists asciiz] && $asciiz} {
                        if {[info exists value]} {
                            set value 1
                        }
                    }
                } else {
                    if {[info exists type]} {
                        dict set symInfo TYPE $type
                        dict set symInfo TYPE_NAME "<$type>"
                    }
                }
                if {[info exists size]} {
                    dict set symInfo SIZE $size
                }
                dict set symInfo TARGET_ID $contextId

                if {$jtag} {
                    dict set symInfo JTAG JTAG
                } else {
                    if {![dict exists $symInfo JTAG]
                        || [info exists memTarget]
                    } {
                        dict set symInfo MEMORY_ID $memContextId
                    }
                }
                if {$memCmd in {fill}} {
                    dict set symInfo FILL FILL
                }
            }
        }

        method autocomplete-location {params prefix} {
            if {[llength $params] > 0 && ![string match -* [lindex $params 0]]} {
                return
            }

            set kind [my AutoGetSymKind $params]
            if {$kind == "-addr" || [string match -* $prefix]} {
                return
            }
            set context [my AutoGetContext $params]

            set SORT mdbg::shell::autocomplete::sortNames

            set localRegs [list]
            set globalRegs [list]

            if {$kind in {ALL -reg}} {
                set localRegisterMap $::mdbg::cli::Memory::LOCAL_REGISTER_MAP
                if {$context != ""} {
                    set target [mdbg::cli::Target::getTargetNameFromContextId $context]
                    if {[dict exists $localRegisterMap $target]} {
                        set targetRegDict [dict get $localRegisterMap $target]
                        set localRegPrefix [string toupper $prefix]
                        set localRegs [dict keys $targetRegDict $localRegPrefix*]
                    }
                }
                set globalRegs [mdbg::findReg $prefix*]
            }

            set varNames [list]
            set symNames [list]

            if {$context != "" && $kind in {ALL -sym -var -expr}} {

                if {[::tcf::hasContextState $context] && [::tcf::isContextSuspended $context]} {
                    set frame [my AutoGetFrame $params $context]
                    if {$kind in {-var ALL} || $frame != $context} {
                        set varNames [mdbg::cli::Symbols::getLocalVars name $frame $prefix*]
                    }
                }
                set afterid [after 1000 {puts $mdbg::Channels::event "please wait..."}]
                try {
                    set symNames [mdbg::cli::Symbols::getElfSymbols name $context $prefix*]
                } finally {
                    after cancel $afterid
                }
            }

            concat [$SORT $varNames] [$SORT $localRegs] \
                   [$SORT [concat $symNames $globalRegs] ]
        }

        #
        #  Private
        #

        method CheckArgs {{level 0}} {
            incr level
            uplevel $level {
                if {[info exists endian] && $endian != "little"} {
                    return -code error {-endian other than "little" not implemented}
                }
            }
        }

        method SetupCommonArgs { {level 0} } {
            incr level
            uplevel $level {
                if {[cli::Target::isSet] || !$jtag} {
                    set contextId [cli::Target::getContextIdFromTargetVar target]
                    set memContextId [cli::Target::getContextIdFromTargetVar memTarget target]
                } else {
                    set contextId ""
                    set memoContextId ""
                }
            }
        }

        method SetupContext { {level 0} } {
            incr level
            uplevel $level {
                if {$contextId != ""} {
                    if {[info exists frame]} {
                        set frameId [cli::CallStack::getFrameId $contextId $frame]
                    } else {
                        set frameId $contextId
                        if {$sym || $reg || $addr} {
                            # symbols, registers, addresses are frame-independent
                        } else {
                            if {[catch {
                                set frameId [tcf::getTopStackFrameId $contextId]
                            } error options]} {
                                if {$error != "Execution context is running"} {
                                    if {$var} {
                                        return -code error -options $options $error
                                    }
                                    if {!$expr} {
                                        cli::Warning "$error"
                                    }
                                }
                            }
                        }
                    }
                } else {
                    set frameId ""
                }
                if {$sym} {
                    set PC 0
                } else {
                    if {[catch {
                        set PC [tcf::getContextPC $frameId]
                    } error options]} {
                        if {$var} {
                            return -code error -options $options $error
                        }
                        set PC 0
                    }
                }
            }
        }

        #
        #  Private (autocomplete)
        #

        method AutoGetContext {params} {
            set index [lsearch -exact $params "-target"]
            if {$index >= 0} {
                incr index
                set target [lindex $params $index]
            }
            if {[catch {
                mdbg::cli::Target::getContextIdFromTargetVar target
            } context]} {
                return
            }
            return $context
        }

        method AutoGetFrame {params context} {
            set index [lsearch -exact $params "-frame"]
            if {$index >= 0} {
                incr index
                set frame [lindex $params $index]
            } else {
                set frame 0
            }
            set frameId [tcf::StackTrace::getChildrenRange $context $frame $frame]
            expr {$frameId == {} ? $context : $frameId}
        }

        method AutoGetSymKind {params} {
            if {{-id} in $params} {
                return -id
            }
            foreach {short full} {-V -var -S -sym -X -expr -R -reg -A -addr} {
                if {$short in $params || $full in $params} {
                    return $full
                }
            }
            return ALL
        }

    } ; # oo::objdefine ArgumentParser

} ; # namespace eval mdbg::cli::Memory
