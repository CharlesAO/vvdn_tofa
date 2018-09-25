# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : SymCache.tcl
# Description: Symbol information cache and operations
# Created on : Sep 14, 2017
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::cli::Memory {

    # If Tcf not available
    oo::class create DummySymbolCache {
        method unknown {args} {
            return
        }
    }

    proc SymbolCacheClass {} {
        if {[info object isa class ::tcf::SymbolCache]} {
            return ::tcf::SymbolCache
        }
        return [namespace current]::DummySymbolCache
    }

    proc fillSymTypeName {symInfoVar symCache} {
        upvar 1 $symInfoVar symInfo
        set typeName [cli::Memory::getSymTypeName $symInfo $symCache]
        dict set symInfo TYPE_NAME $typeName
        return "`$typeName {...}`"
    }

    proc getSymTypeName {symInfo symCache} {
        set typeId [dict get $symInfo TYPE_ID]
        getTypeName $typeId $symCache
    }

    proc getTypeName {id symCache} {
        lassign [$symCache getFieldList $id {
            NAME TYPE_CLASS TYPE_ID BASE_TYPE_ID FLAGS}
        ]   name typeClass  typeId  baseTypeId   flags

        if {{TYPEDEF} ni $flags} {
            switch -exact $typeClass {
                COMPOSITE {
                    set composite [expr {
                        {CLASS_TYPE} in $flags ? "class" :
                        {UNION_TYPE} in $flags ? "union" :
                        "struct"
                    }]
                    return [list $composite $name]
                }
                ENUMERATION {
                    return [list enum $name]
                }
            }
        }
        if {$name != {}} {
            return $name
        }

        set isVolatile [expr {{VOLATILE_TYPE} in $flags ? " volatile" : {}}]
        set isConst [expr {{CONST_TYPE} in $flags ? " const" : {}}]

        if {$isVolatile != {} || $isConst != {}} {
            return "[getTypeName $typeId $symCache]$isConst$isVolatile"
        }

        switch -exact $typeClass {
            UNKNOWN {
                return "<unknown>"
            }
            POINTER {
                set OP [expr { {REFERENCE} in $flags ? "&" : "*" }]
                return "[getTypeName $baseTypeId $symCache] $OP"
            }
            FUNCTION {
                return "<function>"
            }
            MEMBER_POINTER {
                return "<ptr-to-member>"
            }
            COMPLEX {
                return "<complex>"
            }
            ARRAY {
                set LEN [$symCache getField $id LENGTH]
                return "[getTypeName $baseTypeId $symCache] \[$LEN\]"
            }
        }
    }

    proc getArrayElemSize {arrayInfo symCache} {
        set arrayType [dict get $arrayInfo TYPE_ID]
        $symCache get $arrayType BASE_TYPE SIZE
    }

    proc getArrayLength {arrayInfo symCache} {
        if {[dict exists $arrayInfo LENGTH]} {
            dict get $arrayInfo LENGTH
        } else {
            set arrayType [dict get $arrayInfo TYPE_ID]
            $symCache getField $arrayType LENGTH
        }
    }


} ; # namespace eval mdbg::cli::Memory