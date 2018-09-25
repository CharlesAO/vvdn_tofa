namespace eval tcf::MemoryMap {

    # TODO field types, set from fields
    namespace eval FIELDS {
        #    Predefined memory map properties are:
        #
        # <string> - Memory region ID.
        set ID ID
        # <string> - Memory region context query.
        set QUERY ContextQuery

        # <int> - region address in memory.
        set ADDR Addr
        set ADDR.FORMAT 0x%.8X
        # <int> - region size.
        set SIZE Size
        # <int> - Region offset in the file.
        set OFFS Offs
        # <boolean> - True if the region represents BSS.
        set BSS BSS
        # <int> - Region memory protection flags, see predefined flags.
        set FLAGS Flags
        # <string> - Name of the file.
        set FILE_NAME FileName
        # <string> - Name of the object file section.
        set SECTION_NAME SectionName
        # <string> - Size in the file.
        set FILE_SIZE FileSize

        namespace eval FLAGS::FLAGS {
            #    Predefined flags are:
            #
            #    1 - Read access is allowed.
            set READ 1
            #    2 - Write access is allowed.
            set WRITE 2
            #    4 - Instruction fetch access is allowed.
            set EXEC 4
        }
    }

    proc get {contextId} {
        ::tcf::protocol::jsonCommand \
            MemoryMap get \
            [json::String $contextId]
    }

    proc setMap {contextId memoryMapObjectList} {
        ::tcf::protocol::jsonCommand \
            MemoryMap "set" \
            [json::String $contextId] \
            [json::ObjectList $memoryMapObjecList]
    }

    namespace eval Events {
        ::tcf::protocol::JsonEvent2 MemoryMap changed
    }
}

namespace eval tcf {

    proc getMemoryMapTable {contextId fieldList {defaults {}}} {
        fields::getNamedFieldTable \
            MemoryMap \
            [tcf::MemoryMap::get $contextId] \
            $fieldList \
            $defaults
    }

    proc getFullMemoryMap {} {
        set map {}
        foreach context [tcf::queryContext] {
            foreach range [MemoryMap::get $context] {
                set key [dict get $range SectionName]
                if {![dict exists $map $key]} {
                    dict set map $key $range
                }
            }
        }
        dict values $map
    }

    proc getFullMemoryMapFields {fieldList {defaults {}}} {
        fields::getNamedFieldTable MemoryMap [getFullMemoryMap] $fieldList $defaults
    }

}

