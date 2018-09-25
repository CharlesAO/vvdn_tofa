namespace eval mdbg::cpr {

set MV_SOC_REV  [mdbg::MV_SOC_REV]

proc constExists {constant_name} {
    dict exists $::mdbg::REGISTER_MAP $constant_name
}

proc const {constant_name} {
	::mdbg::getRegAddr $constant_name
}

proc peek {address} {
	::mdbg::getReg32 $address
}

proc poke {address value} {
	::mdbg::setReg32 $address $value
}

# TODO: this can be done way simpler
proc extractBits {word start end} {
  set bitMask 0
  for {set i $start} {$i <= $end} {incr i} {
      set bitMask [expr [expr 1 << $i] | $bitMask]
  }
  set maskedValue [expr $word & $bitMask]
  return [expr $maskedValue >> $start]
}

# TODO: this can be done way simpler
proc isBitSet {bitNum word} {
  set bitMask [expr 1 << $bitNum]
  set result [expr $bitMask & $word]
  if { $result == $bitMask} {
    return 1;
  } else {
    return 0;
  }
}

# TODO: this can be done way simpler
proc setBit {regAddr bitNum} {
    set regVal [trim32 [peek $regAddr]]
#    puts "$regAddr $bitNum [hex $regVal]"
    set regVal [expr [expr 1 << $bitNum] | $regVal]
#    puts "$regAddr $bitNum [hex $regVal]"
    poke $regAddr $regVal
}

# TODO: this can be done way simpler
# This function trims a 64 bit value down to 32 bits
proc trim32 {word} {
    set bitMask [extractBits 0xFFFFFFFF 0 31]
    set word [expr ($bitMask & $word)]
    return $word
}

# TODO: this can be done way simpler
proc clearBit {regAddr bitNum} {
    set regVal [trim32 [peek $regAddr]]
#    puts "$regAddr $bitNum [hex $regVal]"
    set regVal [expr [expr [expr ~[expr 1 << $bitNum]] & $regVal] & 0xFFFFFFFF]
#    puts "$regAddr $bitNum [hex $regVal]"
    poke $regAddr $regVal
}

# TODO: this can be done way simpler
proc setBitRange {word start end value} {
#   Make sure that value isn't too big for its destination
    set truncatedValue  [extractBits $value 0 [expr $end - $start]]
    if {$truncatedValue != $value} {
      puts "WARNING: setBitRange: value too big, truncated!"
    }
#   Create an appropriate bitmask to clear the bits of interest
    set bitMask [expr ~[expr [extractBits -1 $start $end] << $start]]
#   Clear the masked portion of the target word
    set word [expr ($bitMask & $word)]
#   Or in the new value
    set word [expr [expr $value << $start] | $word]
    return $word
}

# TODO: this can be done way simpler
proc isBitClear {bitNum word} {
  set bitMask [expr 1 << $bitNum]
  set result [expr $bitMask & $word]
  if { $result == $bitMask} {
     return 0;
  } else {
     return 1;
  }
}

proc setBitRangeInReg {regAddr start end value} {
    set originalValue [peek $regAddr]
    set newValue [setBitRange $originalValue $start $end $value]
    poke $regAddr $newValue
}

proc getNumberOfTerminalColumns {} {
    if {[catch {
        mdbg::shell::console cols
    } cols]} {
        return 80
    }
    return $cols
}

# ------------------------------------------------

# adds a list of key-value pairs to a dictionary
# removes line comments
proc AddListToDict {name list} {
    upvar 1 $name var
    if {[regsub -all -line -lineanchor -expanded {
        (?: ^ | ; ) \s* \# .* $
    } $list {} listWithoutComments]} {
        set list $listWithoutComments
    }
    foreach {k v} $list {
        dict set var $k $v
    }
}
# converts a list of key-value pairs to a dictionary
# removes line comments
proc ListToDict {list} {
    AddListToDict result $list
    return $result
}

proc DictToArray {arrayVar dct} {
    upvar 1 $arrayVar arr
    dict for {k v} $dct {
        set arr($k) $v
    }
}

proc ArrayToDict {arrayVar} {
    upvar 1 $arrayVar arr
    foreach k [array names arr] {
        dict set dct $k $arr($k)
    }
    return $dct
}

} ; # namespace mdbg::cpr