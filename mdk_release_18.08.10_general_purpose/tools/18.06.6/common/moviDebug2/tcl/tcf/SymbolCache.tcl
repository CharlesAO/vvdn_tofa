# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : tcf/SymbolCache.tcl
# Description: TCF Symbol cache + navigation class
# Created on : Sep 17, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval tcf {

    #  Class to cache TCF symbol context objects
    #  - allows retrieving symbols and/or symbol fields/or child ids by SymbolID
    #  - each TCF Symbol is queried at most once per SymbolID
    #  - each getChildren is carried out at most once per SymbolID
    #  - allows navigation from starting SymbolID
    #       through user specified path, and optionally
    #       retrieving field(s) from destination symbol
    oo::class create SymbolCache {

        # Internal cache
        variable Cache

        constructor {} {
            set Cache [dict create]
        }

        destructor {
        }

        method getContext {id} {
            if {![dict exists $Cache $id CONTEXT]} {
                dict set Cache $id CONTEXT [
                    set context [tcf::Symbols::getContext $id]
                ]
                return $context
            }
            dict get $Cache $id CONTEXT
        }

        method getChildren {id} {
            if {![dict exists $Cache $id CHILD_IDS]} {
                dict set Cache $id CHILD_IDS [
                    set children [tcf::Symbols::getChildren $id]
                ]
            }
            dict get $Cache $id CHILD_IDS
        }

        method getField {id name {default {}}} {
            set context [my getContext $id]
            tcf::getSymbolField $context $name $default
        }

        method getFieldList {id names {defaults {}}} {
            set context [my getContext $id]
            tcf::getSymbolFieldList $context $names $defaults
        }

        method getDict {id} {
            set context [my getContext $id]
            tcf::getSymbolFieldDict $context
        }

        method getChildByIndex {id index} {
            lindex [my getChildren $id] $index
        }

        method getChildByName {id name} {
            if {![dict exists $Cache $id CHILD_NAMES $name]} {
                foreach childId [my getChildren $id] {
                    set childName [my getField $childId NAME]
                    if {$childName == {} || [
                        dict exists $Cache $id CHILD_NAMES $childName
                    ]} {
                        continue
                    }
                    dict set Cache $id CHILD_NAMES $childName $childId
                    if {$name == $childName} {
                        return $childId
                    }
                }
                dict set Cache $id CHILD_NAMES $name {}
                return
            }
            dict get $Cache $id CHILD_NAMES $name
        }

        method get {id {path {}} {field {}} args} {
            set qid [my queryId $id $path]
            if {$field == {}} {
                my getDict $qid
            } elseif {$args == {}} {
                if {$field != {CHILDREN}} {
                    my getField $qid $field
                } else {
                    my getChildren $qid
                }
            } else {
                my getFieldList $qid [list $field {*}$args]
            }
        }

        method resolveTypedef {id} {
            set id_list [list]
            while {$id ni $typeList} {
                lassign [my getFieldList $id {TYPE_ID FLAGS}] type_id flags
                if {"TYPEDEF" ni $flags} {
                    return $id
                }
                lappend id_list $id
                set id $type_id
            }
            return -code error "Recursive TYPEDEF detected"
        }

        method queryId {startid {path {}}} {
            set id $startid
            foreach elem $path {
                set previd $id
                switch -regexp -matchvar grp $elem {
                    {^OWNER(?:_ID)?$} {
                        set id [my getField $id OWNER_ID]
                    }
                    {^TYPE(?:_ID)?$} {
                        set id [my getField $id TYPE_ID]
                    }
                    {^BASE_TYPE(?:_ID)?$} {
                        set id [my getField $id BASE_TYPE_ID]
                    }
                    {^INDEX_TYPE(?:_ID)?$} {
                        set id [my getField $id INDEX_TYPE_ID]
                    }
                    {^CONTAINER(?:_ID)?$} {
                        set id [my getField $id CONTAINER_ID]
                    }
                    {^CHILD_INDEX\((.*)\)$} {
                        set index [lindex grp 1]
                        set id [my getChildByIndex $id $index]
                    }
                    {^CHILD_NAME\((.*)\)$} {
                        set name [lindex grp 1]
                        set id [my getChildByName $id $name]
                    }
                    default {
                        return -code error -options [
                            dict create PREV $previd ELEM $elem
                        ] "invalid path element \"$elem\" "
                    }
                }
                if {$id == {}} {
                    return -code error "symbol \"$previd\" does not have \"$elem\""
                }
            }
            set id
        }
} ; # oo::class SymbolCache

}
