namespace eval json {

    proc Null {args} {
        return null
    }

    proc String {str} {
        set command ::tcf::protocol::formatJsonString
        if [llength [info commands $command]] {
            $command $str
        } else {
            format {"%s"} [string map {
                "\b" "\\b"
                "\f" "\\f"
                "\n" "\\n"
                "\r" "\\r"
                "\t" "\\t"
                "\"" "\\\""
                "\\" "\\\\"
            } $str]
        }
    }

    proc NullableString {value {nullValue ""}} {
        expr {$value == $nullValue ? [Null] : [String $value]}
    }

    proc StringList {list} {
        set command ::tcf::protocol::formatJsonStringArray
        if [llength [info commands $command]] {
            $command $list
        } else {
            set result [list]
            foreach str $list {
                lappend result [String $str]
            }
            List $result
        }
    }

    proc List {items} {
        format "\[%s\]" [join $items ,]
    }

    proc StringArray {args} {
        StringList $args
    }

    proc Array {args} {
        List $args
    }

    proc Object {dict} {
        set result {}
        dict for {key value} $dict {
            lappend result "[String $key]:$value"
        }
        format "{%s}" [join $result ,]
    }

    proc ObjectList {listOfDict} {
        set result {}
        foreach dict $listOfDict {
            lappend result [Object $dict]
        }
        List $result
    }

    proc Integer {number} {
        if {[string is integer $number]} {
            if {$number >= 0} {
               return [format %u $number]
            }
            return [format %d $number]
        }
        return -code error "JSON format error: `$number` is not a valid integer"
    }

    proc Number {number} {
        if {[string is integer $number]} {
            if {$number >= 0} {
               return [format %u $number]
            }
            return [format %d $number]
        }
        if {[string is entier $number]} {
            if {$number >= 0} {
                return [format %lu $number]
            }
            return [format %ld $number]
        }
        if {[string is double $number]} {
            return [format %g $number]
        }
        return -code error "JSON format error: `$number` is not a valid number"
    }

    proc NullableNumber {value {nullValue ""}} {
        expr {$value == $nullValue ? [Null] : [Number $value]}
    }

    proc Boolean {value} {
        if [string is true $value] {
            return true
        } elseif [string is false $value] {
            return false
        }
        return -code error "JSON format error: `$value` is not a valid Boolean"
    }

    proc Base64 {data} {
        String [binary encode base64 $data]
    }

    proc ByteArray {data} {
        binary scan $data cu* bytes
        List $bytes
    }

    proc ApplyLayout {layout value} {
        switch -nocase [lindex $layout 0] {
            Bool -
            Boolean {
                tailcall Boolean $value
            }
            Base64 {
                tailcall Base64 $value
            }
            ByteArray {
                tailcall ByteArray $value
            }
            Int -
            Integer {
                tailcall Integer $value
            }
            Number {
                tailcall Number $value
            }
            String {
                tailcall String $value
            }
            NullableString {
                tailcall NullableString $value [lindex $layout 1]
            }
            Array -
            List {
                set result [list]
                set layout [lrange $layout 1 end]
                set len [llength $layout]
                if {!$len} {
                    set layout String
                    set len 1
                }
                set index 0
                foreach item $value {
                    lappend result [ApplyLayout [lindex $layout $index] $item]
                    set index [expr {( $index + 1 ) % $len}]
                }
                tailcall List $result
            }
            Object {
                set result [dict create]
                set layout [lindex $layout 1]
                dict for {key layout} $layout {
                    if {![string match {*[*?]*} $key]} {
                        if {![regexp {^(.+)->(.+)$} $key -> key name]} {
                            set name $key
                        }
                        if {[dict exists $value $key]} {
                            set item [dict get $value $key]
                            dict unset value $key
                            dict set result $name [ApplyLayout $layout $item]
                        } else {
                            dict set result $name [Null]
                        }
                    } else {
                        set pattern $key
                        dict for {key item} $value {
                            if {[string match $pattern $key]} {
                                dict set result $key [ApplyLayout $layout $item]
                                dict unset value $key
                            }
                        }
                    }
                }
                tailcall Object $result
            }
            Dict -
            Dictionary {
                set result [dict create]
                set layout [lindex $layout 1]
                if {$layout == ""} {
                    set layout String
                }
                dict for {key value} $value {
                    dict set result $key [ApplyLayout $layout $value]
                }
                tailcall Object $result
            }
            Switch -
            Enum -
            Enumeration {
                set layout [lindex $layout 1]
                tailcall dict get $layout $value
            }
            Command {
                set command [lrange $layout 1 end]
                tailcall {*}$command $value
            }
            Apply {
                set lambda [lrange $layout 1 end]
                tailcall apply $lambda $value
            }
            Json {
                return $value
            }
            default {
                return -code error "invalid layout \"[lindex $layout 0]\""
            }
        }
    }

    # return  namespace to help format the object as json
    # returns absolute namespace from relative.
    # @param ObjectType absolute namespace name or relative namespace name
    # @param up number of uplevels to check for current namespace in case of relative namespace
    proc ObjectTypeNS {ObjectType {up 1}} {
        if {[string match ::* $ObjectType]} {
            return $ObjectType
        }
        set upNS [uplevel $up namespace current]
        if {![string length $ObjectType]} {
            return $upNS
        }
        if {![string match *:: $upNS]} {
            append upNS ::
        }
        return ${upNS}$ObjectType
    }

    # format json object from dictionary and Object type
    # @param dict dictionary of field names/value
    # @param ObjectType type of object (determines helper namespace @see setObjectFieldsFromDict)
    proc ObjectFromFieldDict {dict {ObjectType {}}} {
        set result [dict create]
        setObjectFieldsFromDict result $dict [ObjectTypeNS $ObjectType 2]
        json::Object $result
    }

    # set object from field list
    # @param ObjectType object type (determines helper namespace, @see setObjectFieldsFromDict)
    # @param args : field_name value ?field_name value...?
    # @return json object
    proc ObjectFromFields {ObjectType args} {
        set result [dict create]
        setObjectFieldsFromDict result $dict [ObjectTypeNS $ObjectType 2]
        json::Object $result
    }

    # Set Json object fields from dictionary
    # @param objectVar name of object dictionary variable
    # @param dict dictionary of fields
    # @param ObjectTypeNS parent of formatting helper namespace
    # @note The helper namespaces follow the following structure:
    # @pre namespace eval ObjectType::FIELDS {
    # @pre     set FIELD_NAME "fieldName"   # name of the Json object member
    # @pre     set FIELD_NAME.TYPE Number # default is String, can be name of any json::proc
    # @pre }
    # @pre namespace eval ObjectType::FIELD_NAME::VALUES {
    # @pre     set ONE 1.0
    # @pre     set TWO 2.0
    # @pre     set THREE 3.0
    # @pre }
    proc setObjectFieldsFromDict {objectVar dict ObjectTypeNS} {
        set NS ${ObjectTypeNS}::FIELDS
        upvar $objectVar object
        if {[namespace exists $NS]} {
            dict for {key value} $dict {

                set keyVar ${NS}::$key

                if {[info exists $keyVar]} {
                    set key [set $keyVar]
                } else {
                    if {[info exists mdbg::WARNING_JSON_MISSING_OBJECT_KEY_NAMES] &&
                        $mdbg::WARNING_JSON_MISSING_OBJECT_KEY_NAMES
                    } {
                        puts stderr "Warning: \"$keyVar\" does not exist!"
                    }
                }

                set valueNsVar ${keyVar}.VALUES
                if {[info exists $valueNsVar]} {
                    set valueNS [set $valueNsVar]
                    if {![string match ::* $valueNS]} {
                        set valueNS [namespace qualifiers $keyVar]::$valueNS
                    }
                } else {
                    set valueNS ${keyVar}::VALUES
                }

                set valueVar ${valueNS}::$value
                if {[info exists $valueVar]} {
                    set value [set $valueVar]
                }

                set flagsNS ${keyVar}::FLAGS
                if {[namespace exists $flagsNS]} {
                    set value [getNumericValueFromFlagsList $value $flagsNS]
                }

                set typeVar ${keyVar}.TYPE
                if {[info exists $typeVar]} {
                    set type [set $typeVar]
                } else {
                    set type json::String
                }
                dict set object $key [$type $value]
            }
        } else {
            puts stderr "Warning: namespace `$NS` does not exist."
            dict for {key value} $dict {
                dict set object $key [json::String $value]
            }
        }
        return $object
    }

    proc getNumericValueFromFlagsList {flagList {flagsNS}} {
        if {[string is integer -strict $flagList]} {
            return $flagList
        }
        set result 0
        foreach flag $flagList {
            set flagVar ${flagsNS}::$flag
            if {[info exists $flagVar]} {
                set flagValue [getNumericValueFromFlagsList [set $flagVar] $flagsNS]
                set result [expr {$result | $flagValue}]
            }
        }
        return $result
    }

}

