import os, re

list_of_controllers = [
    #[ FILENAME, CONTROLLER_VERSION, MEMC_BURST_LENGTH, MEMC_DRAM_DATA_WIDTH ]
    ["../../../drivers/myriad2/socDrivers/leon/bm/arch/ma2x5x/include/DrvDdrAddrMapMa2x5x.h", "2.40a", 8, 64, "ADDRESS_MAPPINGS_MA2X5X_H",
        [
            ["R12..R0 , B2..B0 , C8..C0", 32, "LPDDR2", "1Gb"],
            ["B2..B0 , R12..R0 , C8..C0", 32, "LPDDR2", "1Gb"],
            ["B2..B1 , R12..R0 , B0 , C8..C0", 32, "LPDDR2", "1Gb"],
            ["B2 , R12..R0 , B1..B0 , C8..C0", 32, "LPDDR2", "1Gb"],
            ["R13..R0 , B2..B0 , C9..C0", 32, "LPDDR3", "4Gb"],
            ["B2..B0 , R13..R0 , C9..C0", 32, "LPDDR3", "4Gb"],
            ["B2..B1 , R13..R0 , B0 , C9..C0", 32, "LPDDR3", "4Gb"],
            ["B2 , R13..R0 , B1..B0 , C9..C0", 32, "LPDDR3", "4Gb"],
        ],
    ],
]

element_matcher = re.compile(r"^([^0-9]+)([0-9]+)$")

def expand_spec(spec_string, outfile):
    """
        spec_string: a specification string that specifies the assignment
        of each address bit of a memory word to a column (C*), row (R*), bank (B*), chip select (CS*)
        
        C0 is never transmitted on the LPDDR2 interface, because all transactions are some kind of burst,
        and the first clock cycle is always even, while the second clock cycles is always an odd address.
        Therfore C0 must be the last element in the spec.
        
        C0 is assigned to the least significant memory word address bit. For a 32-bit DDR die this is assigned
        to byte address bit 2.
        
        Spaces are ignored
        Ranges are separated by ","
        High and Low end of a range is inclusive and separated by ".."
    """
    spec = [i.split("..") for i in spec_string.replace(" ", "").split(",")]
    expanded_spec = []
    for elem in spec:
        if len(elem) == 1:
            expanded_spec.append(elem[0])
        elif len(elem) == 2:
            high_result = element_matcher.match(elem[0])
            element_type = high_result.group(1)
            element_high = int(high_result.group(2))
            low_result = element_matcher.match(elem[1])
            if element_type != low_result.group(1):
                outfile.write("// ERROR: spec string format not recognized: different element types in range\n")
                return
            element_low = int(low_result.group(2))
            for i in range(element_high, element_low - 1, -1):
                expanded_spec.append(element_type + str(i))
        else:
            outfile.write("// ERROR: spec string format not recognized\n")
            return
    
    if len(set(expanded_spec)) != len(expanded_spec):
        outfile.write("// ERROR: spec string format invalid: one ore more elements are assigned to more then one address bit\n")
        return
    
    if "C0" != expanded_spec[-1]:
        outfile.write("// ERROR: C0 must be the last element of the spec\n")
        return
    
    return spec, expanded_spec

def gen_symbol_name(spec, die_width):
    return "__".join([("_").join(i) for i in spec]) + ("_%d_bit_die" % (die_width, ))

def handle_controller(controller):
    f = open(controller[0], "w")
    bare_fn = os.path.split(controller[0])[-1]
    no_ext = bare_fn[:-2]
    f.write("///\n")
    f.write("/// @file %s\n" % (bare_fn,))
    f.write("/// @copyright All code copyright Movidius Ltd 2014, all rights reserved\n")
    f.write("///            For License Warranty see: common/license.txt\n")
    f.write("/// @defgroup %s %s Driver\n" % (no_ext, no_ext))
    f.write("/// @{\n")
    f.write("///\n")
    f.write("\n")

    f.write("#ifndef %s\n" % (controller[4],))
    f.write("#define %s\n\n" % (controller[4],))
    f.write("// Generated file %s having controller version %s using %s\n\n" % (controller[0], controller[1], __file__,) )
    f.write("#ifdef __cplusplus\n")
    f.write("extern \"C\" {\n")
    f.write("#endif\n\n")
    list_of_specs = controller[5]
    for spec_str, die_width, lpddr_version, size in list_of_specs:
        f.write("\n\n// Address mapping for spec \"%s\" with %d-bit die\n" % (spec_str, die_width))
        f.write("//------------------------------------------------------------------\n\n")
        spec, expanded_spec = expand_spec(spec_str, f)
        if not expanded_spec:
            continue
        handle_address_mapping(gen_symbol_name(spec, die_width), expanded_spec, controller, die_width, lpddr_version, size, f)
    f.write("\n#ifdef __cplusplus\n")
    f.write("}\n")
    f.write("#endif\n\n")
    f.write("#endif\n")
    f.write("\n")
    f.write("///@}\n")
    f.close()

def set_addrmap_field(field, value, addrmaplist):
    ind = field[0]
    msb = field[1]
    lsb = field[2]
    mask = (1 << (msb + 1)) - (1 << lsb)
    invmask = mask ^ 0xffffffff
    addrmaplist[ind] = (addrmaplist[ind] & invmask) | ((value << lsb) & mask)

def handle_address_mapping(symbol, espec, controller, die_width, lpddr_version, size, f):

    if controller[1] == "2.40a":
        mapregs = 12
    else:
        mapregs = 13

    addrmap = mapregs * [0]
    
    if controller[3] == die_width:
        colshift = 0
    elif controller[3]/2  == die_width:
        colshift = 1
    elif controller[3]/4  == die_width:
        colshift = 2
    else:
        f.write("// ERROR: unsupported die width: %d\n" % (die_width, ))
        return
        
    if "R10" in espec:
        r10ind = espec.index("R10")
        if tuple(espec[r10ind+1: r10ind + 9]) == ("R9", "R8", "R7", "R6", "R5", "R4", "R3", "R2"):
            espec = espec[:r10ind] + ["R10-2"] + espec[r10ind+9:]
    
    f.write("// INFO: C%d matches HIF address bit 0\n" % (colshift,))
    if die_width == 64:
        f.write("// INFO: C0 matches byte address bit 3\n\n")
    elif die_width == 32:
        f.write("// INFO: C0 matches byte address bit 2\n\n")
    elif die_width == 16:
        f.write("// INFO: C0 matches byte address bit 1\n\n")
    elif die_width == 8:
        f.write("// INFO: C0 matches byte address bit 0\n\n")
    
    if controller[1] == "2.40a":
      fields = {
                           #name: register, msb, lsb, lowrange, highrange, base, disabled_value
                           "DCH": (      0,  20,  16,        0,        27,    2, 31),
                           "CS1": (      0,  12,   8,        0,        26,    7, 31),
                           "CS0": (      0,   4,   0,        0,        27,    6, 31),
                            "B2": (      1,  20,  16,        0,        29,    4, 31),
                            "B1": (      1,  12,   8,        0,        30,    3, -1),
                            "B0": (      1,   4,   0,        0,        30,    2, -1),
         "C" + str(5 + colshift): (      2,  27,  24,        0,         7,    5, 15),
         "C" + str(4 + colshift): (      2,  19,  16,        0,         7,    4, 15),
         "C" + str(3 + colshift): (      2,  11,   8,        0,         7,    3, -1),
         "C" + str(2 + colshift): (      2,   3,   0,        0,         7,    2, -1),
         "C" + str(9 + colshift): (      3,  27,  24,        0,         7,    9, 15),
         "C" + str(8 + colshift): (      3,  19,  16,        0,         7,    8, 15),
         "C" + str(7 + colshift): (      3,  11,   8,        0,         7,    7, 15),
         "C" + str(6 + colshift): (      3,   3,   0,        0,         7,    6, 15),
         "C" + str(11+ colshift): (      4,  11,   8,        0,         7,   11, 15),
         "C" + str(10+ colshift): (      4,   3,   0,        0,         7,   10, 15),
                           "R11": (      5,  27,  24,        0,        11,   17, 15),
                         "R10-2": (      5,  19,  16,        0,        11,    8, -1),
                            "R1": (      5,  11,   8,        0,        11,    7, -1),
                            "R0": (      5,   3,   0,        0,        11,    6, -1),
                           "R15": (      6,  27,  24,        0,        11,   21, 15),
                           "R14": (      6,  19,  16,        0,        11,   20, 15),
                           "R13": (      6,  11,   8,        0,        11,   19, 15),
                           "R12": (      6,   3,   0,        0,        11,   18, 15),
                           "R17": (      7,  11,   8,        0,        10,   23, 15),
                           "R16": (      7,   3,   0,        0,        11,   22, 15),
                           "BG1": (      8,  12,   8,        0,        30,    3, 31),
                           "BG0": (      8,   4,   0,        0,        30,    2, -1),
                         "R10-2": (      5,  19,  16,        0,        11,    8, 15), # it can be disabled and replaced with individual settings per row
                            "R5": (      9,  27,  24,        0,        11,   11, -1),
                            "R4": (      9,  19,  16,        0,        11,   10, -1),
                            "R3": (      9,  11,   8,        0,        11,    9, -1),
                            "R2": (      9,   3,   0,        0,        11,    8, -1),
                            "R9": (     10,  27,  24,        0,        11,   15, -1),
                            "R8": (     10,  19,  16,        0,        11,   14, -1),
                            "R7": (     10,  11,   8,        0,        11,   13, -1),
                            "R6": (     10,   3,   0,        0,        11,   12, -1),
                           "R10": (     11,   3,   0,        0,        11,   16, -1),
      }
    else:
      fields = {
                           #name: register, msb, lsb, lowrange, highrange, base, disabled_value
                           "DCH": (      0,  20,  16,        0,        30,    2, 31),
                           "CS1": (      0,  12,   8,        0,        27,    7, 31),
                           "CS0": (      0,   4,   0,        0,        28,    6, 31),
                            "B2": (      1,  21,  16,        0,        31,    4, 63),
                            "B1": (      1,  13,   8,        0,        32,    3, 63),
                            "B0": (      1,   5,   0,        0,        32,    2, 63),
         "C" + str(5 + colshift): (      2,  27,  24,        0,         7,    5, 15),
         "C" + str(4 + colshift): (      2,  19,  16,        0,         7,    4, 15),
         "C" + str(3 + colshift): (      2,  11,   8,        0,         7,    3, -1),
         "C" + str(2 + colshift): (      2,   3,   0,        0,         7,    2, -1),
         "C" + str(9 + colshift): (      3,  27,  24,        0,         7,    9, 15),
         "C" + str(8 + colshift): (      3,  19,  16,        0,         7,    8, 15),
         "C" + str(7 + colshift): (      3,  11,   8,        0,         7,    7, 15),
         "C" + str(6 + colshift): (      3,   3,   0,        0,         7,    6, 15),
         "C" + str(11+ colshift): (      4,  11,   8,        0,         7,   11, 15),
         "C" + str(10+ colshift): (      4,   3,   0,        0,         7,   10, 15),
                           "R11": (      5,  27,  24,        0,        11,   17, 15),
                         "R10-2": (      5,  19,  16,        0,        11,    8, -1),
                            "R1": (      5,  11,   8,        0,        11,    7, -1),
                            "R0": (      5,   3,   0,        0,        11,    6, -1),
                           "R15": (      6,  27,  24,        0,        11,   21, 15),
                           "R14": (      6,  19,  16,        0,        11,   20, 15),
                           "R13": (      6,  11,   8,        0,        11,   19, 15),
                           "R12": (      6,   3,   0,        0,        11,   18, 15),
                           "R17": (      7,  11,   8,        0,        11,   23, 15),
                           "R16": (      7,   3,   0,        0,        11,   22, 15),
                           "BG1": (      8,  13,   8,        0,        32,    3, 63),
                           "BG0": (      8,   4,   0,        0,        32,    2, 63),
                         "R10-2": (      5,  19,  16,        0,        11,    8, 15), # it can be disabled and replaced with individual settings per row
                            "R5": (      9,  27,  24,        0,        11,   11, -1),
                            "R4": (      9,  19,  16,        0,        11,   10, -1),
                            "R3": (      9,  11,   8,        0,        11,    9, -1),
                            "R2": (      9,   3,   0,        0,        11,    8, -1),
                            "R9": (     10,  27,  24,        0,        11,   15, -1),
                            "R8": (     10,  19,  16,        0,        11,   14, -1),
                            "R7": (     10,  11,   8,        0,        11,   13, -1),
                            "R6": (     10,   3,   0,        0,        11,   12, -1),
                          "CID1": (     11,  20,  16,        0,        31,    5, -1),
                          "CID0": (     11,  12,   8,        0,        30,    4, 31),
                           "R10": (     11,   3,   0,        0,        11,   16, -1),
                           "CS3": (     12,  12,   8,        0,        26,    9, 31),
                           "CS2": (     12,   4,   0,        0,        27,    8, 31),
      }
    
    for i in range(2 + colshift):
        if espec[-1-i] != "C" + str(i):
            f.write("// ERROR: must end with contiguous C%d..C0\n" % (2 + colshift - 1,))
            return
    
    for fieldn in fields:
        dvalue = fields[fieldn][6]
        if dvalue>=0:
            set_addrmap_field(fields[fieldn], dvalue, addrmap)
    
    cspec = espec[:-colshift-2]
    hifbit = 2
    while cspec:
        if cspec[-1] not in fields:
            f.write("//ERROR: unable to handle spec (no such element?)\n")
            return
        
        field = fields[cspec[-1]]
        if (field[5] + field[3] > hifbit) or (field[5] + field[4] < hifbit):
            f.write("//ERROR: unable to assign %s to HIF bit %d\n" % (cspec[-1], hifbit))
            return
        
        new_value = hifbit - field[5]
        set_addrmap_field(field, new_value, addrmap)
            
        if cspec[-1] == "R10-2":
            hifbit += 9
        else:
            hifbit += 1
        del cspec[-1]
    
    if lpddr_version=="LPDDR4":
        if size == "6Gb":
            addrmap[6] |= 1 << 29
        if size == "12Gb":
            addrmap[6] |= 2 << 29
        if size == "24Gb":
            addrmap[6] |= 3 << 29

    if lpddr_version=="LPDDR3":
        if size in ("6Gb", "12Gb"):
            addrmap[6] |= 1 << 31
    
    f.write("static const int address_map_%s[%d] = {\n" % (symbol, mapregs))
    for i in range(len(addrmap)):
        mapvalue = addrmap[i]
        commentstring="ADDRMAP%d:" % i
        f.write("    /*%-10s*/ 0x%08x,\n" % (commentstring, mapvalue))
    f.write("};\n")

for controller in list_of_controllers:
    handle_controller(controller)
