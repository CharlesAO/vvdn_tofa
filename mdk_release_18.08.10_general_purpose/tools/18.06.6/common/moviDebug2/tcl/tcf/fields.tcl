# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : tcf/fields.tcl
# Description: Functions to manipulate fields of objects returned from
#              or passed to TCF. It's sort of reverse mechanism(s)
#              of what happens in json::Object-related functions
# Created on : Sep 15, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval tcf::fields {

    proc TypeNS {namespace} {
        if {[string match ::* $namespace]} {
            return $namespace
        }
        return ::tcf::${namespace}
    }

    proc getNamedFieldDict {namespace context} {
        set keysNS [TypeNS $namespace]::FIELDS
        set result [dict create]
        dict for {key value} $context {
            set fieldName [getNameOfValue $key $keysNS]
            dict set result $fieldName [getNamedField \
                $namespace $context $fieldName $value
            ]
        }
        set result
    }

    proc getNamedFieldDictList {namespace dictList} {
        set result [list]
        foreach dict $dictList {
            lappend result [getNamedFieldDict $namespace $dict]
        }
        set result
    }

    proc getNamedField {namespace context fieldOrIndex {default {}}} {
        if {[string is integer -strict $fieldOrIndex]} {
            set value [lindex $context $fieldOrIndex]
            set indexNS [TypeNS $namespace]::INDEX
            set var ${indexNS}::[getNameOfValue ${fieldOrIndex} $indexNS]
            if {![info exists $var]} {
                return $value
            }
        } elseif {[dict exists $context $fieldOrIndex]} {
            set value [dict get $context $fieldOrIndex]
            set fieldNS [TypeNS $namespace]::FIELDS
            set var ${fieldNS}::${fieldOrIndex}
            if {[info exists $var]} {
                if {[set $var] != $fieldOrIndex} {
                    return $value
                }
            } else {
                set var ${fieldNS}::[getNameOfValue ${fieldOrIndex} $fieldNS]
                if {![info exists $var]} {
                    return $value
                }
            }
        } elseif {[namespace exists [set fieldNS [TypeNS $namespace]::FIELDS]]} {
            set var "${fieldNS}::${fieldOrIndex}"
            if {[info exists $var]} {
                set field [set $var]
                if {[dict exists $context $field]} {
                    set value [dict get $context $field]
                }
            }
        } elseif {[namespace exists [set indexNS [TypeNS $namespace]::INDEX]]} {
            set var "${indexNS}::${fieldOrIndex}"
            if {[info exists $var]} {
                set index [set $var]
                if {$index >= 0 && $index < [llength $context]} {
                    set value [lindex $context $index]
                }
            }
        }

        if {![info exists value]} {
            return $default
        }

        if {[info exists ${var}.FORMAT]} {
            set value [format [set $var.FORMAT] $value]
        }

        set flagsNS ${var}::FLAGS
        if {[namespace exists $flagsNS]} {
            set value [getFlagListFromInteger $value $flagsNS]
        }

        if {[info exists ${var}.VALUES]} {
            set valuesNS [set $var.VALUES]
            if {![string match ::* $valuesNS]} {
                set valuesNS [namespace qualifiers $var]::$valuesNS
            }
        } else {
            set valuesNS ${var}::VALUES
        }
        if {[namespace exists $valuesNS]} {
            set value [getNameOfValue $value $valuesNS]
        }

        if {[info exists ${var}.TYPE]} {
            set type [set ${var}.TYPE]
            switch -exact [namespace tail $type] {
                OBJECT {
                    set typeNS [namespace qualifiers $type]
                    if {[llength $value] > 1} {
                        set value [getNamedFieldDict $typeNS $value]
                    }
                }
                OBJECT_LIST {
                    set typeNS [namespace qualifiers $type]
                    set result [list]
                    foreach $object $value {
                        if {[llength $object] > 1} {
                            lappend result [getNamedFieldDict $typeNS $object]
                        }
                    }
                    set value $result
                }
            }
        }

        return $value
    }

    proc getNameOfValue { value valueNS } {
        set valueVarList [info vars ${valueNS}::*]
        foreach valueVarName $valueVarList {
            set valueVarValue [set $valueVarName]
            if {$value == $valueVarValue} {
                return [namespace tail $valueVarName]
            }
        }
        set value
    }

    proc getFlagListFromInteger { value flagNS } {
        if {![string is integer -strict $value]} {
            return $value
        }
        set flagVarList [info vars ${flagNS}::*]
        set flagList [list]
        foreach flagVarName $flagVarList {
            if {$value == 0} {
                break
            }
            set flagValue [set $flagVarName]
            if {![string is integer -strict $flagValue]} {
                continue
            }
            if {$value & $flagValue} {
                lappend flagList [namespace tail $flagVarName]
                set value [expr {$value & ~$flagValue}]
            }
        }
        if {$value != 0} {
            lappend flagList [format 0x%X $value]
        } else {
            if {![llength flagList]} {
                lappend flagList 0
            }
        }
        set flagList
    }

    proc getFieldList {dictList} {
        set fieldDict {}
        foreach object $dictList {
            dict for {key value} $object {
                dict set fieldDict $key {}
            }
        }
        dict keys $fieldDict
    }

    proc getNamedFieldList {namespace object fieldList {defaults {}} } {
        set index 0
        set item [list]
        foreach field $fieldList {
            set default [lindex $defaults $index]
            incr index
            lappend item [getNamedField $namespace $object $field $default]
        }
        return $item
    }

    proc getNamedFieldTable {namespace dictList fieldList {defaults {}} } {
        set result [list]
        if [string equal $fieldList *] {
            set fieldList [getFieldList $dictList]
        }
        foreach object $dictList {
            lappend result [getNamedFieldList \
                $namespace $object $fieldList $defaults \
            ]
        }
        return $result
    }

    proc importVars {sourceNs {recursive 1}} {
        set destNs [uplevel 1 namespace current]
        if {![string match ::* $destNs]} {
            set destNs ::$destNs
        }
        if {![string match ::* $sourceNs]} {
            set fullNs ${destNs}::${sourceNs}
            if {[namespace exists $fullNs]} {
                set sourceNs $fullNs
            } else {
                set sourceNs ::$sourceNs
            }
        }

        if {![namespace exists $sourceNs]} {
            return -code error "namespace \"$sourceNs\" does not exist"
        }

        set varList [info vars ${sourceNs}::*]
        foreach var $varList {
            set name [namespace tail $var]
            uplevel #0 [list \
                namespace eval $destNs [list upvar 0 $var $name] \
            ]
        }

        if {$recursive} {
            set importVars [namespace current]::importVars
            foreach childNs [namespace children $sourceNs] {
                set destChildNs ${destNs}::[namespace tail $childNs]
                set body [list $importVars $childNs $recursive]
                uplevel #0 [list namespace eval $destChildNs $body]
            }
        }
    }

    # imports the field name variables to the caller's stack
    proc Using {namespace fieldVarName args} {
        foreach fieldVarName [list $fieldVarName {*}$args] {
            uplevel 1 [list \
                upvar 0 [TypeNS $namespace]::FIELDS::${fieldVarName} $fieldVarName \
            ]
        }
    }

} ; # namespace eval tcf::fields
