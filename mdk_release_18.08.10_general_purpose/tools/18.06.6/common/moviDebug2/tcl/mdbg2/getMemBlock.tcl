namespace eval mdbg {

    # obsolete
    proc hex2bin {args} {
        binary decode hex [string cat {*}$args]
    }

    # obsolete
    proc bin2hex {args} {
        string toupper [
        binary encode hex [string cat {*}$args]
        ]
    }

    # obsolete
    proc getMemBlock {address size args} {
        set words [mvproto::getBurst32 [getRegAddr $address] [
        expr ( ( $size {*}$args ) + 3 ) / 4
        ]]
        binary format i* $words
    }

    mdbg::shell::autocomplete::addScript {
        getMemBlock
    } {
        ::mdbg::findReg $PREFIX*
    }

    namespace export getMemBlock hex2bin bin2hex
}