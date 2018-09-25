namespace eval tcf::Registers {

    namespace eval CONTEXT::FIELDS {
        #  <string> - ID of the context, same as getContext command argument.
        set ID ID
        #  <string> - ID of a parent context.
        set PARENT_ID ParentID
        #  <string> - process ID.
        set PROCESS_ID ProcessID
        #  <string> - context name.
        set NAME Name
        #  <string> - context description.
        set DESCRIPTION Description
        #  <int> - context size in bytes. Byte arrays in get/set commands should be same size. Hardware register can be smaller then this size, for example in case when register size is not an even number of bytes. In such case implementation should add/remove padding that consists of necessary number of zero bits.
        set SIZE Size
        #  <boolean> - true if context value can be read.
        set READABLE Readable
        #  <boolean> - true if reading the context (register) destroys its current value - it can be read only once.
        set READ_ONCE ReadOnce
        #  <boolean> - true if context value can be written.
        set WRITEABLE Writeable
        #  <boolean> - true if register value can not be overwritten - every write counts.
        set WRITEONCE WriteOnce
        #  <boolean> - true if writing the context can change values of other registers.
        set SIDE_EFFECTS SideEffects
        #  <boolean> - true if the register value can change even when target is stopped.
        set VOLATILE Volatile
        #  <boolean> - true if the register value is a floating-point value.
        set FLOAT Float
        #  <boolean> - true if big endian, which means decreasing numeric significance with increasing bit number. If absent default if false, which implies little endianess. The endianess is used to encode and decode values of get, getm, set and setm commands.
        set BIG_ENDIAN BigEndian
        #  <boolean> - true if the lowest numbered bit (i.e. bit #0 or bit #1, depending on "FirstBit" value) should be shown to user as the left-most bit.
        set LEFT_TO_RIGHT LeftToRight
        #  <int> - 0 or 1. If the context has bit field children, bit positions of the fields can be zero-based or 1-based.
        set FIRST_BIT FirstBit
        #  <array of int> - if context is a bit field, contains the field bit numbers in the parent context.
        set BITS Bits
        #  <array of named values> - predefined names (mnemonics) for some of context values.
        set VALUES Values
        #
        #        <array of named values>
        #                ⇒ null
        #                ⇒ [ ]
        #                ⇒ [ <named values list> ]
        #
        #        <named values list>
        #                ⇒ <object: named value properties>
        #                ⇒ <named values list> , <object: named value properties>
        #  <int> - The address of a memory mapped register. If MemoryContext is provided, the address is referring into that context.

        set DWARF_ID    DwarfID
        set EH_FRAME_ID EhFrameID


        #set MemoryAddress MemoryAddress
        #  <string> - The context ID of a memory context in which a memory mapped register is located. Used together with MemoryAddress to inform where in memory memory mapped registers are located. If absent and MemoryAddress is defined, the context ID of this context is used as default.
        #set MemoryContext MemoryContext

        #  <array of strings> - A list of attribute names which can be searched for starting on this context. If absent the context does not support search.
        set CAN_SEARCH CanSearch
        #  <string> - The role the register plays in a program execution.
        set ROLE Role
    }

    # Named value properties are:
    namespace eval NAMED_VALUE::FIELDS {
        #  Name (menemonic) of the value.
        set NAME Name
        #  BASE64 encoded binary bits of the value.
        set VALUE Value
        #  Short, human readable description of the value.
        set DESCRIPTION Description
    }

    namespace eval ROLE {
        # Predefined register role strings are:
        #  Program counter. Defines instruction to execute next.

        set PC PC
        #  Register defining the current stack pointer location.
        set SP SP
        #  Register defining the current frame pointer location.
        set FP FP
        #  Register used to store the return address for calls.
        set RET RET
        #  Indicates register or register groups which belong to the core state. Commonly set for general purpose registers, condition code and other registers which are of special interest for determining the state.
        set CORE CORE
    }

    proc getContext {contextId} {
        tcf::protocol::jsonCommand Registers getContext [json::String $contextId]
    }

    proc getChildren {contextId} {
        tcf::protocol::jsonCommand Registers getChildren [json::String $contextId]
    }

    namespace eval set {
        proc base64 {contextId value} {
            tcf::protocol::jsonCommand Registers set [json::String $contextId] [json::String $value]
        }

        proc bytearray {contextId value} {
            base64 $contextId [binary encode base64 $value]
        }

        proc u32 {contextId value} {
            bytearray [binary format i $value]
        }

        proc u64 {contextId value} {
            bytearray [binary format w $value]
        }
    }

    namespace eval get {
        proc base64 {contextId} {
            tcf::protocol::jsonCommand Registers get [json::String $contextId]
        }

        proc bytearray {contextId} {
            binary decode base64 [base64 $contextId]
        }

        proc u32 {contextId} {
            binary scan [bytearray $contextId] iu result
            format "0x%.8X" $result
        }

        proc u64 {contextId} {
            binary scan [bytearray $contextId] wu result
            format "0x%.16X" $result
        }
    }

    # Each location is represented by 3-element array that consists of context ID,
    # offset in the context in bytes and value size in bytes.
    proc Location {contextId offsetInContext size} {
        json::Array [json::String $contextId] [json::Integer $offsetInContext] [json::Integer $sizeInBytes]
    }

    namespace eval setm {
        proc base64 {arrayOfLocations value} {
            tcf::protocol::jsonCommand Registers setm [json::List $arrayOfLocations] [json::String $value]
        }

        proc bytearray {arrayOfLocations bytearray} {
             base64 $arrayOfLocations [binary encode base64 $bytearray]
        }
    }

    namespace eval getm {
        proc base64 {arrayOfLocations} {
            tcf::protocol::jsonCommand Registers getm [json::List $arrayOfLocations]
        }
        proc bytearray {arrayOfLocations} {
            binary decode base64 [base64 $arrayOfLocations]
        }
    }

    proc FilterProperties {name equalValue} {
        dict create Name [json::String $name] EqualValue [json::String $equalValue]
    }

    proc search {startContextId filterProperties} {
        tcf::protocol::jsonCommand Registers search [json::String $startContextId] [json::Object $filterProperties]
    }

    namespace eval Events {
        ::tcf::protocol::JsonEvent Registers contextChanged
        ::tcf::protocol::JsonEvent Registers registerChanged
    }
}

