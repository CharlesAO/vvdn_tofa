# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : LazyFactory.tcl
# Description: mixin class for Lazy sub-object and field creation inside objects
# Created on : Aug 14, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
package require TclOO

namespace eval mdbg::oo {

    oo::class create LazyFactory {

        variable Objects
        variable Fields

        destructor {
            if {[info exists Objects]} {
                foreach object [lreverse [dict values $Objects]] {
                    catch {
                        $object destroy
                    }
                }
            }
        }

        method LazyObject {name class args} {

            if {[info exists Objects] && [dict exists $Objects $name]} {
                return [dict get $Objects $name]
            }

            set object [uplevel 1 [list $class new {*}$args]]

            dict set Objects $name $object

            return $object
        }

        method LazyField {name script} {
            if {[info exists Fields] && [dict exists $Fields $name]} {
                return [dict get $Fields $name]
            }

            set field uplevel 1 $script

            dict set Fields $field

            return $field
        }

        method GetObject {name} {
            dict get $Objects $name
        }

        method GetField {name} {
            dict get $Objects $name
        }

        method <cloned> {src} {
            return -code error [
                format {cannot copy objects of type "%s"} [self class]
            ]
        }

    } ; # oo::class LazyCreateMembers

} ; # namespace eval mdbg