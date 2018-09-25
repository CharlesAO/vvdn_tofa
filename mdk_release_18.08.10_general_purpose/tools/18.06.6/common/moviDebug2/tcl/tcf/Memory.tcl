namespace eval tcf::Memory {

    namespace eval FIELDS {
        # String, ID of the context, same as getContext command argument
        set ID "ID"
        # String, ID of a parent context
        set PARENT_ID "ParentID"
        # String, process ID, see Processes service
        set PROCESS_ID "ProcessID"
        # Boolean, true if memory is big-endian
        set BIG_ENDIAN "BigEndian"
        # Number, size of memory address in bytes
        set ADDRESS_SIZE "AddressSize"
        # String, name of the context, can be used for UI purposes
        set NAME "Name"
        # Number, lowest address (inclusive) which is valid for the context
        set START_BOUND "StartBound"
        # Number, highest address (inclusive) which is valid for the context
        set END_BOUND "EndBound"
        # Array of String, the access types allowed for this context
        set ACCESS_TYPES "AccessTypes"
    }
    # @since 1.3*/
    namespace eval FIELDS {
        # Number, addressable unit size in number of bytes
        set ADDRESSABLE_UNIT_SIZE "AddressableUnitSize"
        # Number, default word size in number of bytes
        set DEFAULT_WORD_SIZE "DefaultWordSize"
    }

    # Values of "AccessTypes".
    # Target system can support multiple different memory access types, like instruction and data access.
    # Different access types can use different logic for address translation and memory mapping, so they can
    # end up accessing different data bits, even if address is the same.
    # Each distinct access type should be represented by separate memory context.
    # A memory context can represent multiple access types if they are equivalent - all access same memory bits.
    # Same data bits can be exposed through multiple memory contexts.
    namespace eval ACCESS_TYPES::VALUES {
        # Context represent instructions fetch access
        set INSTRUCTION "instruction"
        # Context represents data access
        set DATA "data"
        # Context represents IO peripherals
        set IO "io"
        # Context represents a user (e.g. application running in Linux) view to memory
        set USER "user"
        # Context represents a supervisor (e.g. Linux kernel) view to memory
        set SUPERVISOR "supervisor"
        # Context represents a hypervisor view to memory
        set HYPERVISOR "hypervisor"
        # Context uses virtual addresses
        set VIRTUAL "virtual"
        # Context uses physical addresses
        set PHYSICAL "physical"
        # Context is a cache
        set CACHE "cache"
        # Context is a TLB memory
        set TLB "tlb"
    }

    #/**
    #* Retrieve context info for given context ID.
    #*
    #* @param id - context ID.
    #* @param done - call back interface called when operation is completed.
    #* @return - context data.
    #*/
    proc getContext {id} {
        tcf::protocol::jsonCommand \
            Memory getContext [json::String $id]
    }

    #/**
    #* Retrieve contexts available for memory commands.
    #* A context corresponds to an execution thread, process, address space, etc.
    #* A context can belong to a parent context. Contexts hierarchy can be simple
    #* plain list or it can form a tree. It is up to target agent developers to choose
    #* layout that is most descriptive for a given target. Context IDs are valid across
    #* all services. In other words, all services access same hierarchy of contexts,
    #* with same IDs, however, each service accesses its own subset of context's
    #* attributes and functionality, which is relevant to that service.
    #*
    #* @param parent_context_id - parent context ID. Can be null -
    #* to retrieve top level of the hierarchy, or one of context IDs retrieved
    #* by previous getChildren commands.
    #* @param done - call back interface called when operation is completed.
    #* @return - array of available context IDs.
    #*/
    proc getChildren {parentContextId} {
        tcf::protocol::jsonCommand \
            Memory getChildren [json::String $parentContextId]
    }

    namespace eval MEMORY_ACCESS_MODE {
        #/**
        #* Memory access mode:
        #* Carry on when some of the memory cannot be accessed and
        #* return MemoryError at the end if any of the bytes
        #* were not processed correctly.
        #*/
        set CONTINUEONERROR 0x1
        #/**
        #* Memory access mode:
        #* Verify result of memory operations (by reading and comparing).
        #*/
        set VERIFY 0x2
    }

    # TODO: make this simpler by not expecting operators and such, only list of flag values
    proc getMemoryAccessMode {modeExpression} {
        if {$modeExpression == {}} {
            return 0
        }
        if {[string is integer $modeExpression]} {
            return $modeExpression
        }
        set expression [list]
        foreach mode $modeExpression {
            set modeVarName [namespace current]::MEMORY_ACCESS_MODE::$mode
            if {[info exists $modeVarName]} {
                lappend expression [set $modeVarName]
                continue
            }
            lappend expression $mode
        }
        expr $expression
    }

    # /**
    #  Writes data bytes at given address in memory, "word size" bytes at a time.
    # Address should be aligned by "word size"
    # * If 'word_size' is 0 it means client does not care about word size.
    # */
    #   C • <token> • Memory • set •
    #   <string: context ID> • <int: address> • <int: word size> •
    #   <int: byte count> • <int: mode> • <string: BASE64 encoded byte array> •
    # @return array of error addresses
    proc setBytes {contextID address byteArray {wordSize 0} {modeExpr 0}} {
        # Note: this is how you measure the byte length of a tcl bytearray
        set byteCount [string length $byteArray]
        tcf::protocol::jsonCommand \
            Memory set \
            [json::String $contextID] \
            [json::Integer $address] \
            [json::Integer $wordSize] \
            [json::Integer $byteCount] \
            [json::Integer [getMemoryAccessMode $modeExpr]] \
            [json::Base64 $byteArray]
    }

    # Reads data bytes at given address in memory, "word size" bytes at a time.
    # Address should be aligned by "word size"
    #  C • <token> • Memory • get •
    #  <string: context ID> • <int: address> • <int: word size> •
    #  <int: byte count> • <int: mode> •
    #  R • <token> • <string: BASE64 encoded byte array> • <error report> • <array of error addresses> •
    # @return byte array
    proc getBytes {contextId address byteCount {wordSize 0} {modeExpr 0}} {
        set resultList [tcf::protocol::jsonListCommand \
            2 1 \
            Memory get \
            [json::String $contextId] \
            [json::Integer $address] \
            [json::Integer $wordSize] \
            [json::Integer $byteCount] \
            [json::Integer [getMemoryAccessMode $modeExpr]] \
        ]
        binary decode base64 [lindex $resultList 0]
    }

    # Writes pattern bytes at given address in memory, "word size" bytes at a time.
    # Address should be aligned by "word size". If "byte count" is bigger then pattern size,
    # then pattern is repeated necessary number of times.
    # If 'word_size' is 0 it means client does not care about word size.
    # C • <token> • Memory • fill •
    # <string: context ID> • <int: address> • <int: word size> •
    # <int: byte count> • <int: mode> • <array: array of pattern bytes> •
    # @return array of error addresses
    proc fillBytes {contextID address byteCount byteArray {wordSize 0} {modeExpr 0}} {
        tcf::protocol::jsonCommand \
            Memory fill \
            [json::String $contextID] \
            [json::Integer $address] \
            [json::Integer $wordSize] \
            [json::Integer $byteCount] \
            [json::Integer [getMemoryAccessMode $modeExpr]] \
            [json::ByteArray $byteArray]
    }

    namespace eval Events {
        # E • Memory • contextAdded • <array of context data> •
        tcf::protocol::JsonEvent Memory contextAdded
        # E • Memory • contextChanged • <array of context data> •
        tcf::protocol::JsonEvent Memory contextChanged
        # E • Memory • contextRemoved • <array of context IDs> •
        tcf::protocol::JsonEvent Memory contextRemoved
        # E • Memory • memoryChanged • <string: context ID> • <array of address ranges> •
        tcf::protocol::JsonEvent2 Memory memoryChanged
    }

}