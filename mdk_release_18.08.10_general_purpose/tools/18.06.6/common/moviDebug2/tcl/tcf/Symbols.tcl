#Symbol information can change at any time as result of target background activities.
#Clients should not cache symbol information, and should not retain the information
#longer than one dispatch cycle.
namespace eval tcf::Symbols {

    namespace eval SYMBOL::FIELDS {
        set ID ID
        set OWNER_ID OwnerID
        set UPDATE_POLICY UpdatePolicy
        set NAME Name
        set CLASS Class
        set TYPE_CLASS TypeClass
        set TYPE_ID TypeID
        set BASE_TYPE_ID BaseTypeID
        set INDEX_TYPE_ID IndexTypeID
        set CONTAINER_ID ContainerID
        set SIZE Size
        set LENGTH Length
        set LOWER_BOUND LowerBound
        set UPPER_BOUND UpperBound
        set OFFSET Offset
        set ADDRESS Address
        set ADDRESS.FORMAT 0x%.8X
        set VALUE Value
        set BIG_ENDIAN BigEndian
        set REGISTER Register
        set FLAGS Flags
        set BIT_STRIDE BitStride
        set BINARY_SCALE BinaryScale
        set DECIMAL_SCALE DecimalScale

        set FRAME Frame

        namespace eval CLASS::VALUES {
            set UNKNOWN       0
            set VALUE         1;# Symbol represents a constant value
            set REFERENCE     2;# Symbol is an address of an object (variable) in memory
            set FUNCTION      3;# Symbol is an address of a function
            set TYPE          4;# Symbol represents a type declaration
            set COMP_UNIT     5;# Symbol represents a compilation unit
            set BLOCK         6;# Symbol represents a block of code
            set NAMESPACE     7;# Symbol represents a namespace
            set VARIANT_PART  8;# Symbol represents a variant part of a structure
            set VARIANT       9;# Symbol represents a member of a variant part of a structure
        }

        namespace eval TYPE_CLASS::VALUES {
            set UNKNOWN      0
            set CARDINAL     1
            set INTEGER      2
            set REAL         3
            set POINTER      4
            set ARRAY        5
            set COMPOSITE    6
            set ENUMERATION  7
            set FUNCTION     8
            set MEMBER_PTR   9
            set COMPLEX      10
        }

        namespace eval FLAGS::FLAGS {
            set PARAMETER      0x00000001
            set TYPEDEF        0x00000002
            set CONST_TYPE     0x00000004
            set PACKET_TYPE    0x00000008
            set SUBRANGE_TYPE  0x00000010
            set VOLATILE_TYPE  0x00000020
            set RESTRICT_TYPE  0x00000040
            set UNION_TYPE     0x00000080
            set CLASS_TYPE     0x00000100
            set INTERFACE_TYPE 0x00000200
            set SHARED_TYPE    0x00000400
            set REFERENCE      0x00000800
            set BIG_ENDIAN     0x00001000
            set LITTLE_ENDIAN  0x00002000
            set OPTIONAL       0x00004000
            set EXTERNAL       0x00008000
            set VARARG         0x00010000
            set ARTIFICIAL     0x00020000
            set TYPE_PARAMETER 0x00040000
            set PRIVATE        0x00080000
            set PROTECTED      0x00100000
            set PUBLIC         0x00200000
            set ENUM_TYPE      0x00400000
            set STRUCT_TYPE    0x00800000
            set STRING_TYPE    0x01000000
            set INHERITANCE    0x02000000

            set BOOL_TYPE      0x04000000
            set INDIRECT       0x08000000
            set RVALUE         0x10000000
        }

        namespace eval UPDATE_POLICY::VALUES {
            set ON_MEMORY_MAP_CHANGES 0
            set ON_EXE_STATE_CHANGES  1
        }
    }

    # Retrieve symbol context info for given symbol ID.
    proc getContext {id} {
        tcf::protocol::jsonCommand Symbols getContext [json::String $id]
    }

    # Retrieve children IDs for given parent ID.
    proc getChildren {id} {
        tcf::protocol::jsonCommand Symbols getChildren [json::String $id]
    }

    # Search symbol with given name in given context.
    # Return first symbol that matches.
    # The context can be memory space, process, thread or stack frame.
    proc find {contextId ip name} {
        tcf::protocol::jsonCommand Symbols find [json::String $contextId] [json::Number $ip] [json::String $name]
    }

    # Search symbol with given name in given context.
    # Return all symbol that matches, starting with current scope and going up to compilation unit global scope.
    # The context can be memory space, process, thread or stack frame
    proc findByName {contextId ip name} {
        tcf::protocol::jsonCommand Symbols findByName [json::String $contextId] [json::Number $ip] [json::String $name]
    }

    # Search symbol with given address in given context.
    # Return all matching symbols @since 1.3
    # The context can be memory space, process, thread or stack frame.
    # @param context_id - a search scope.
    # @param addr - symbol address.
    proc findByAddr {context_id addr} {
        tcf::protocol::jsonCommand Symbols findByAddr [json::String $context_id] [json::Number $addr]
    }

    #
    # Search symbol with given address in given context.
    # The context can be memory space, process, thread or stack frame.
    #
    # @param context_id - a search scope.
    # @param ip - instruction pointer - ignored if context_id is a stack frame ID
    # @param scope_id - a symbols ID of visibility scope.
    # @param name - symbol name.
    #
    # Return all matching symbols
    proc findInScope {context_id ip scope_id name} {
        tcf::protocol::jsonCommand Symbols findInScope [json::String $context_id] \
            [json::Number $ip] [json::String $scope_id] [json::String $name]
    }

    # List all symbols in given context.
    # The context can be a stack frame.
    # returns list of symbol IDs
    proc listStackSymbols {contextId} {
        tcf::protocol::jsonCommand Symbols "list" [json::String $contextId]
    }

    # Undocumented
    proc getArrayType {sym_type_id length} {
        tcf::protocol::jsonCommand Symbols getArrayType [json::String $sym_type_id] [json::Number $length]
    }

    # Retrieve symbol location information.
    # returns location information object (dictionary)
    #
    proc getLocationInfo {id} {
        tcf::protocol::jsonCommand Symbols getLocationInfo [json::String $id]
    }

    # Retrieve stack tracing commands for given instruction address in a context memory.
    # @param context_id - executable context ID.
    # @param address - instruction address.
    # return [list $address $size [list $fp_cmds] [dict reg_cmds]
    proc findFrameInfo {context_id address} {
        tcf::protocol::jsonCommand Symbols findFrameInfo [json::String $context_id] [json::Number $address]
    }

    # Undocumented
    proc findFrameProps {context_id address} {
        tcf::protocol::jsonCommand Symbols findFrameProps [json::String $context_id] [json::Number $address]
    }

    # Get symbol file info for a module that contains given address in a memory space.
    # @param context_id - a memory space (process) ID.
    # @param address - an address in the memory space.
    # @return - symbol file properties
    proc getSymFileInfo {context_id address} {
        tcf::protocol::jsonCommand Symbols getSymFileInfo [json::String $context_id] [json::Number $address]
    }

    # Undocumented
    proc getAddressInfo {context_id address} {
        tcf::protocol::jsonCommand Symbols getAddressInfo [json::String $context_id] [json::Number $address]
    }


} ; # namespace eval tcf::Symbols

namespace eval tcf {

    proc getSymbolField {symContextDict field {default {}} } {
        fields::getNamedField Symbols::SYMBOL $symContextDict $field $default
    }

    proc getSymbolFieldList {symContextDict fieldList {defaults {}} } {
        fields::getNamedFieldList Symbols::SYMBOL $symContextDict $fieldList $defaults
    }

    proc getSymbolFieldDict {symContextDict} {
        fields::getNamedFieldDict Symbols::SYMBOL $symContextDict
    }

} ; # namespace eval tcf

