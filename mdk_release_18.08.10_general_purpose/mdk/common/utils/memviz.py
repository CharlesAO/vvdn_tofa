#!/usr/bin/env python2
from optparse import OptionParser
import re
import math
import string
import sys
import os
import shutil

class Section:
    def __init__(self, name, v_address, l_address, size, type_info, alignment):
        self.v_address = v_address
        self.l_address = l_address
        self.size = size
        self.type_info = type_info
        self.name = name
        self.alignment = alignment

    def __str__(self):
        return self.section+""


class MemViewSettings:
    def __init__(self, name, address_range, display_unused, select_ranges):
        self.name = name
        self.address_range = address_range
        self.display_unused = display_unused
        self.select_ranges = select_ranges

mem_types_M1 = [
        #               NAME                RANGE                     DISP_UNUSED  SELECT_RANGES
        MemViewSettings('CMX',              (0x10000000, 0x100FFFFF), True,        True),
        MemViewSettings('LRAM',             (0x90100000, 0x90107FFF), True,        True),
        MemViewSettings('DDR',              (0x40000000, 0x4FFFFFFF), True,        True),
        MemViewSettings('Special memory',   (0x00000000, 0x00000000), False,       False)
        ]

mem_types_M2_21xx = [
        #               NAME                RANGE                     DISP_UNUSED  SELECT_RANGES
        MemViewSettings('CMX',              (0x70000000, 0x701FFFFF), True,        True),
        MemViewSettings('DDR',              (0x80000000, 0x87FFFFFF), True,        True),
        MemViewSettings('Special memory',   (0x00000000, 0x00000000), False,       False)
        ]

mem_types_M2_2450 = [
        #               NAME                RANGE                     DISP_UNUSED  SELECT_RANGES
        MemViewSettings('CMX',              (0x70000000, 0x701FFFFF), True,        True),
        MemViewSettings('DDR',              (0x80000000, 0x9FFFFFFF), True,        True),
        MemViewSettings('Special memory',   (0x00000000, 0x00000000), False,       False)
        ]

mem_types_Mx_2480 = [
        #               NAME                RANGE                     DISP_UNUSED  SELECT_RANGES
        MemViewSettings('CMX',              (0x70000000, 0x70280000), True,        True),
        MemViewSettings('DDR',              (0x80000000, 0x9FFFFFFF), True,        True),
        MemViewSettings('Special memory',   (0x00000000, 0x00000000), False,       False)
        ]

mem_types_Kb_2490 = [
        #               NAME                RANGE                     DISP_UNUSED  SELECT_RANGES
        MemViewSettings('CMX',              (0x70000000, 0x70280000), True,        True),
        MemViewSettings('DDR',              (0x80000000, 0x9FFFFFFF), True,        True),
        MemViewSettings('Special memory',   (0x00000000, 0x00000000), False,       False)
        ]

arch_to_mem_types = {
    'ma2150': mem_types_M2_21xx,
    'ma2450': mem_types_M2_2450,
    'ma2480': mem_types_Mx_2480,
    'ma2490': mem_types_Kb_2490,
    }

COLOR_UNUSED = 'F5F3D8'
DATA_SOURCE_MAP = 0
DATA_SOURCE_LST = 1
DEFAULT_ARCH = 'ma2150'

SECTION_STYLE = {}
RANGE_STYLE = {}
SIZE_STYLE = {}
ALIGN_STYLE = {}
UNUSED_STYLE = {}

SECTION_ATTR = {'class' : '\'section\''}
RANGE_ATTR = {'class' : '\'range\''}
SIZE_ATTR = {'class' : '\'size\''}
ALIGN_ATTR = {'class' : '\'align\''}
UNUSED_ATTR = {'class' : '\'unused\''}

UNUSED_SIZE_THRESHOLD = 20

(DATA, CODE, BSS, STACK, FREE) = (0, 1, 2, 3, 4)
SECTION_TYPE = ('data', 'text', 'bss', 'stack', 'free')


GREEN_INTERVAL  = (0, 1023)
YELLOW_INTERVAL = (1023, 10239)
RED_INTERVAL    = (10239, 102399)

# hardcoded section tipes for commonlly used sections
SECTIONS = {
    '.winregs.S0' : DATA,
    '.winregs.S1' : DATA,
    '.winregs.S2' : DATA,
    '.winregs.S3' : DATA,
    '.winregs.S4' : DATA,
    '.winregs.S5' : DATA,
    '.winregs.S6' : DATA,
    '.winregs.S7' : DATA,
    '.l2.mode'    : DATA,
    '.cmx.ctrl'   : DATA,
    '.sys.traps'  : CODE
}

#Load section list
def parse_lst(lst_file):
    sections = []
    try:
        f = open(lst_file, 'r')
        try:
            while True:
                #read two lines at a time
                line1 = f.readline()
                if not line1:
                    break
                section_info = []
                m = re.search('^\s*([0-9]+)\s+(.*?)\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]+)\s+(([1-9]+)\*\*([0-9]+))\s*', line1)

                if m:
                    name = m.group(2)
                    #ignore debug sections
                    if not name.startswith('.debug'):
                        size = int(m.group(3), base = 16)
                        v_address = int(m.group(4), base = 16)
                        l_address = int(m.group(5), base = 16)
                        #view CMX as a single address space
                        if 0xA0000000 <= l_address < 0xA0100000:
                            l_address -=  0x90000000
                        align = m.group(8), m.group(9)

                        #second line contains information about section type
                        line2 = f.readline()
                        section_info = line2.split(',')

                        for i in range(len(section_info)):
                            section_info[i] = section_info[i].strip()

                        #skip LROM
                        if not (0x90000000 <= l_address < 0x90010000):
                            sections.append(Section(name, v_address, l_address, size, section_info, align))
        finally:
            f.close()
    except IOError:
        print('File %s could not be found.\nTry running "make all" to generate the lst file and then run "make report" again.' % lst_file)
        sys.exit()

    return sections


#try to find out the section type from it's name
def get_type_from_name(name):
    if ".free" in name:
        return FREE
    elif ".text" in name:
        return CODE
    elif (".rodata" in name) or (".data" in name):
        return DATA
    elif ".bss" in name:
        return BSS
    elif ".stack" in name:
        return STACK
    return None

#parses a map file for section information
def parse_map(map_file):
    sections = []
    subsections = {}

    try:
        f = open(map_file, 'r')

        try:
            while True:
                line1 = f.readline()
                if not line1:
                    break
                pattern_found = False

                m = re.search('^\s*([^\*\s]+?)\s+0x([0-9A-Fa-f]+)\s+0x([0-9A-Fa-f]+)\s*$', line1)

                if m:
                    pattern_found = True
                    #only name address and size available for map file
                    name = m.group(1)
                    vaddress = int(m.group(2), base = 16)
                    laddress = int(m.group(2), base = 16)
                    size = int(m.group(3), base = 16)
                else:
                    m1 = re.search('^\s*([^\*\s]+?)\s*$', line1)
                    if m1:
                        line2 = f.readline()
                        if not line2:
                            break
                        m2 = re.search('^\s+0x([0-9A-Fa-f]+)\s+0x([0-9A-Fa-f]+)\s*(\s*load\s*address\s*0x)*([0-9A-Fa-f]+)*$', line2)
                        if m2:
                            pattern_found = True
                            name = m1.group(1)
                            vaddress = int(m2.group(1), base = 16)
                            if(m2.group(4)):
                                laddress = int(m2.group(4), base = 16)
                            else:
                                laddress = vaddress
                            size = int(m2.group(2), base = 16)

                if pattern_found:
                    if not name.startswith('.debug') and (size > 0):
                        #try to guess section type

                        #first try to guess from section name
                        section_type = get_type_from_name(name)
                        if not section_type:
                            #if not yet found try to find the type in a table of well known sections
                            if name in SECTIONS:
                                section_type = SECTIONS[name]
                            else:
                                #in none of the above succeded assume the section to be a data section
                                section_type = DATA

                        #main section found
                        sections.append(Section(name, vaddress, laddress, size, section_type, None))
                        #now search for subsections
                        section_end = False
                        while not section_end:
                            line1 = f.readline()
                            if line1[0] != ' ':
                                section_end = True
                            else:
                                m = re.match('\s*([^\*\s]+?)\s+0x([0-9A-Fa-f]+)\s+0x([0-9A-Fa-f]+)\s+(.*?)$', line1)
                                if m:
                                    sname = m.group(1)
                                    vaddress = int(m.group(2), base = 16)
                                    laddress = int(m.group(2), base = 16)
                                    size = int(m.group(3), base = 16)
                                    subs = Section(sname, vaddress, laddress, size, section_type, None)
                                    if name in subsections:
                                        subsections[name].append(subs)
                                    else:
                                        subsections[name] = [subs]
                                else:
                                    m1 = re.match('\s*([^\*\s]+?)\s*$', line1)
                                    if m1:
                                        line2 = f.readline()
                                        if line2[0] != ' ':
                                            section_end = True
                                        else:
                                            sname = m1.group(1)
                                            m2 = re.match('^\s+0x([0-9A-Fa-f]+)\s+0x([0-9A-Fa-f]+)\s+(.*?)$', line2)
                                            if m2:
                                                #found a new subsection
                                                vaddress = int(m2.group(1), base = 16)
                                                laddress = int(m2.group(1), base = 16)
                                                size = int(m2.group(2), base = 16)
                                                subs = Section(sname, vaddress, laddress, size, section_type, None)
                                                #print(name, address, size)
                                                if name in subsections:
                                                    subsections[name].append(subs)
                                                else:
                                                    subsections[name] = [subs]
        finally:
            f.close()

    except IOError:
        print('File %s could not be found. \nTry running "make all" to generate the map file and then run "make report" again.' % map_file)
        sys.exit()


    return sections, subsections

def table_cell(text, attributes = None, style = None):
    attr = []
    if attributes:
        for a in attributes.keys():
            attr += "%s=%s" % (a, attributes[a])

    st = []
    if style:
        st = ['style=\'']
        for s in style.keys():
            st += "%s:%s;" % (s, style[s])
        st.append('\'')

    return "<td %s %s>%s</td>\n" % (''.join(attr), ''.join(st), text)


def get_total_size(sections):
    size = 0
    for s in sections:
        size += s.size
    return size

def get_total_size_not_in_range(sections, mem_types):
    size = 0
    for s in sections:
        found = False
        for mem in mem_types:
            if mem.address_range[0] < s.l_address < mem.address_range[1]:
                found = True
        if not found:
            size += s.size
    return size

def get_section_type(type_info):
    if 'DATA' in type_info:
       return DATA
    elif 'CODE' in type_info:
       return CODE
    elif ('CONTENTS' in type_info) and ('READONLY' in type_info):
       return DATA
    elif 'FREE' in type_info:
       return FREE
    return BSS

def table_start(f, header):
    f.write("<table cellspacing='1px'>\n")
    f.write("<tr class='table_header'>\n")
    for column in header:
        f.write("<th>%s</th>\n" % column)

def table_end(f):
    f.write("</table>\n")


def gen_html_table_view_not_in_range(f, section_list, mem_table, data_source):
    count = 0
    total_height = 32.0
    total_size = get_total_size_not_in_range(section_list, mem_table)

    f.write("<div class='range_view'>\n")

    f.write("<div class='top'><h1>Special memory</h1></div>\n")
    #section table
    f.write("<div class='view'>\n")
    table_start(f, ['Section', 'Address', 'Size'])

    if data_source == DATA_SOURCE_LST:
        #alingn information is not available in map files
        f.write("<th>Align</th>\n")

    for s in section_list:
        found = False
        for mem in mem_table:
            if mem.address_range[0] <= s.l_address < mem.address_range[1]:
                found = True
        if not found:
            height = total_height * (s.size / total_size) + 1.0
            if data_source == DATA_SOURCE_LST:
                section_type = SECTION_TYPE[get_section_type(s.type_info)]
            else:
                section_type = SECTION_TYPE[s.type_info]
            f.write("<tr class='section_row %s' style='height:%gem;line-height:%gem'>\n" % (section_type, height, height))
            f.write(table_cell(s.name, SECTION_ATTR, SECTION_STYLE))
            f.write(table_cell("0x%X - 0x%X" % (s.l_address, s.l_address + s.size), RANGE_ATTR, RANGE_STYLE))
            f.write(table_cell("%s bytes" % s.size, SIZE_ATTR, SIZE_STYLE))
            if data_source == DATA_SOURCE_LST:
                f.write(table_cell("%s bytes" % (int(s.alignment[0])**int(s.alignment[1])), ALIGN_ATTR, ALIGN_STYLE))
            f.write("</tr>\n")
            count += 1

    table_end(f)
    f.write("</div>\n")

    f.write("</div>\n")


def gen_addr_range_table_view(f, section_list, mem, data_source):
    range = mem.address_range
    current_addr = range[0]
    total_height = 32.0
    total_size = range[1] - range[0]
    count = 0
    for s in section_list:
        #check if address in range
        if range[0] <= s.l_address < range[1]:
            if s.l_address > current_addr:
                if mem.display_unused and ((s.l_address - current_addr) >= UNUSED_SIZE_THRESHOLD):
                    height = (total_height/total_size) * (s.l_address - current_addr) + 1
                    if height > 1.0:
                        font_size = 1.0
                    else:
                        font_size = height
                    #we have an empty section - add a table row for it
                    f.write("<tr class='unused' style='height:%gem;line-height:%gem'>\n" % (height, height))
                    f.write(table_cell('Unused', UNUSED_ATTR, UNUSED_STYLE))
                    f.write(table_cell("0x%X - 0x%X" % (current_addr, s.l_address), UNUSED_ATTR, UNUSED_STYLE))
                    f.write(table_cell("0x%X - 0x%X" % (current_addr, s.l_address), UNUSED_ATTR, UNUSED_STYLE))
                    f.write(table_cell("%s bytes" % (s.l_address - current_addr), UNUSED_ATTR, UNUSED_STYLE))
                    if data_source == DATA_SOURCE_LST:
                        f.write(table_cell("-", UNUSED_ATTR, UNUSED_STYLE))
                    f.write("</tr>\n")
                    current_addr = s.l_address
            #not empty section
            height = (total_height/total_size) * s.size + 1
            if height > 1.0:
                font_size = 1.0
            else:
                font_size = height

            if data_source == DATA_SOURCE_LST:
                section_type = SECTION_TYPE[get_section_type(s.type_info)]
            else:
                section_type = SECTION_TYPE[s.type_info]

            f.write("<tr class='section_row %s' style='height:%gem;line-height:%gem'>\n" % (section_type, height, height))
            f.write(table_cell(s.name, SECTION_ATTR, SECTION_STYLE))
            f.write(table_cell("0x%X - 0x%X" % (s.v_address, s.v_address + s.size), RANGE_ATTR, RANGE_STYLE))
            f.write(table_cell("0x%X - 0x%X" % (s.l_address, s.l_address + s.size), RANGE_ATTR, RANGE_STYLE))
            f.write(table_cell("%s bytes" % (s.l_address + s.size - current_addr), SIZE_ATTR, SIZE_STYLE))
            if data_source == DATA_SOURCE_LST:
                align = int(s.alignment[0])**int(s.alignment[1])
                if align == 1:
                    align_one_or_more = ''
                else:
                    align_one_or_more = 's'
                f.write(table_cell("%s byte%s" % (align, align_one_or_more), ALIGN_ATTR, ALIGN_STYLE))
            f.write("</tr>\n")
            current_addr = s.l_address + s.size

            count += 1
    #check for empty sections at the end of the address range
    if current_addr < range[1]:
        if mem.display_unused  and ((range[1] - current_addr) >= UNUSED_SIZE_THRESHOLD):
            height = (total_height/total_size) * (range[1] - current_addr) + 1
            if height > 1.0:
                font_size = 1.0
            else:
                font_size = height
            #we have an empty section - add a table row for it
            f.write("<tr class='unused' style='height:%gem;line-height:%gem'>\n" % (height, height))
            f.write(table_cell("Unused", UNUSED_ATTR, UNUSED_STYLE))
            f.write(table_cell("0x%X - 0x%X" % (current_addr, range[1]), UNUSED_ATTR, UNUSED_STYLE))
            f.write(table_cell("0x%X - 0x%X" % (current_addr, range[1]), UNUSED_ATTR, UNUSED_STYLE))
            f.write(table_cell("%s bytes" % (range[1] - current_addr), UNUSED_ATTR, UNUSED_STYLE))
            if data_source == DATA_SOURCE_LST:
                f.write(table_cell("-", UNUSED_STYLE, UNUSED_STYLE))
            f.write("</tr>\n")

def gen_html_table_view_range(f, section_list, mem, data_source):
        f.write("<div class='range_view'>\n")

        f.write("<div class='top'><h1>%s</h1></div>\n" % mem.name)
        #section table
        f.write("<div class='view'>\n")

        table_header = ['Section', 'VirtualAddress', 'LoadAddress', 'Size']
        if data_source == DATA_SOURCE_LST:
            table_header.append('Align')

        table_start(f, table_header)
        f.write("</tr>\n")
        gen_addr_range_table_view(f, section_list, mem, data_source)
        table_end(f)
        f.write("</div>\n")

        f.write("</div>\n")


def gen_html_table_view(html_file, section_list, subsection_list, mem_types, data_source, css_file):
    html_path , html_filename = os.path.split(html_file)
    css_path , css_filename = os.path.split(css_file)

    shutil.copyfile(css_file, os.path.join(html_path, css_filename))

    f = open(html_file, 'w')
    try:
        # html header
        f.write("<html><head>\n")
        f.write("<link rel='stylesheet' type='text/css' href='%s' />\n" % (css_filename))
        f.write("<body>\n")

        f.write("<div id='container'>\n")

        #list of views for all ranges
        for mem in mem_types:
            if mem.select_ranges:
                gen_html_table_view_range(f, section_list, mem, data_source)
            else:
                gen_html_table_view_not_in_range(f, section_list, mem_types, data_source)

        f.write("</div>\n")

        f.write("<div id='stat_container'>\n")
        for mem in mem_types:
            if mem.select_ranges:
                gen_statistics_view(f, section_list, subsection_list, mem)
            else:
                gen_statistics_view_special(f, section_list, subsection_list, mem_types)
        f.write("</div>\n")

        f.write("</body></html>\n")
    finally:
        f.close()


def color_interpolate(in_size, min_size, max_size, min_color, max_color):
    if in_size <= min_size:
        return min_color
    elif in_size >= max_size:
        return max_color
    else:
        ratio = (in_size - min_size) / (max_size - min_size)
        return int(min_color + (max_color - min_color) * ratio)

def gen_statistics_view(f, section_list, subsection_list, mem):
    range = mem.address_range
    total_size = get_total_size(section_list)
    total_height = 32.0

    f.write("<div id='stat_view'>\n")
    f.write("<div class='top'><h1>%s</h1></div>\n" % mem.name)
    section_list.sort(key = lambda s: s.size, reverse = True)

    table_start(f, ['Section', 'Address', 'Size'])
    all_subsections = []
    for s in section_list:
        if s.name in subsection_list:
            all_subsections += subsection_list[s.name]

    for subs in sorted(all_subsections, key = lambda a: a.size, reverse = True):
        if range[0] <= subs.l_address < range[1]:
            height = (total_height/total_size) * subs.size + 1
            color = ''
            #get color depending on the section size
            if GREEN_INTERVAL[0] <= subs.size < GREEN_INTERVAL[1]:
                color_val = color_interpolate(subs.size, 0.0, 1023.0, 150.0, 255.0)
                color = '00%x00' % color_val
            elif YELLOW_INTERVAL[0] <= subs.size < YELLOW_INTERVAL[1]:
                color_val = color_interpolate(subs.size, 1023.0, 10239.0, 200.0, 255.0)
                color = '%x%x00' % (color_val, color_val)
            elif RED_INTERVAL[0] <= subs.size < RED_INTERVAL[1]:
                color_val = color_interpolate(subs.size, 10239.0, 102399.0, 200.0, 255.0)
                color = '%x0000' % color_val
            else:
                color = 'FF0000'

            f.write("<tr class='section_row' style='background-color:#%s'>\n" % color)
            f.write(table_cell(subs.name, SECTION_ATTR, SECTION_STYLE))
            f.write(table_cell("0x%X - 0x%X" % (subs.l_address, subs.l_address + subs.size), RANGE_ATTR, RANGE_STYLE))
            f.write(table_cell("%s bytes" % subs.size, SIZE_ATTR, SIZE_STYLE))
    table_end(f)

    f.write("</div>\n")

def gen_statistics_view_special(f, section_list, subsection_list, mem_table):
    f.write("<div id='stat_view'>\n")
    f.write("<div class='top'><h1>Special memory</h1></div>\n")
    section_list.sort(key = lambda s: s.size, reverse = True)

    table_start(f, ['Section', 'Address', 'Size'])
    for s in section_list:
        found = False
        for mem in mem_table:
            if mem.address_range[0] <= s.l_address < mem.address_range[1]:
                found = True
        if not found:
            if s.name in subsection_list:
                for subs in subsection_list[s.name]:
                    color = ''
                    #get color depending on the section size
                    if GREEN_INTERVAL[0] <= subs.size < GREEN_INTERVAL[1]:
                        color_val = color_interpolate(subs.size, 0.0, 1023.0, 150.0, 255.0)
                        color = '00%x00' % color_val
                        #print(s.name, "green: ", color)
                    elif YELLOW_INTERVAL[0] <= subs.size < YELLOW_INTERVAL[1]:
                        color_val = color_interpolate(subs.size, 1023.0, 10239.0, 200.0, 255.0)
                        color = '%x%x00' % (color_val, color_val)
                        #print("yellow: ", color)
                    elif RED_INTERVAL[0] <= subs.size < RED_INTERVAL[1]:
                        color_val = color_interpolate(subs.size, 10239.0, 102399.0, 200.0, 255.0)
                        color = '%x0000' % color_val
                        #print(s.name, "red: ", color)
                    else:
                        color = 'FF0000'
                        #print(s.name, "red: ", color)
                    f.write("<tr class='section_row' style='background-color:#%s'>\n" % color)
                    f.write(table_cell(subs.name, SECTION_ATTR, SECTION_STYLE))
                    f.write(table_cell("0x%X - 0x%X" % (subs.l_address, subs.l_address + subs.size), RANGE_ATTR, RANGE_STYLE))
                    f.write(table_cell("%s bytes" % subs.size, SIZE_ATTR, SIZE_STYLE))

    table_end(f)

    f.write("</div>\n")


def main():
    #parser = argparse.ArgumentParser(description='Memory viewer')
    parser = OptionParser()
    parser.add_option('-i', metavar='INPUT_FILE', dest='in_file', help='Input file containing section information')
    parser.add_option('-p', '--platform', metavar='PLATFORM', dest='platform', default='M2', help='Used platform')
    parser.add_option('-a', '--arch', metavar='ARCH', dest='arch', default=DEFAULT_ARCH, help='Chip architecture')
    parser.add_option('-o', '--output-file',  metavar='OUTPUT_FILE', dest='out_file', default='memviz.html', help='Output html file')
    parser.add_option('-c', '--css-file',  metavar='CSS_FILE', dest='css_file', default='memviz_style.css', help='CSS file used for styling html output')
    parser.add_option('-d', '--data-source',  metavar='DATA_SOURCE', dest='data_source', default='map',
                        help='Chose style for output display (available options: table and bar_chart)')
    (options, args) = parser.parse_args()

    if options.css_file:
       css_file = options.css_file

    if options.arch:
    	mem_types = arch_to_mem_types[options.arch]

    if options.in_file:
        if options.data_source == 'map':
            sections, subsections = parse_map(options.in_file)
            if len(sections) > 0:
                sections.sort(key=lambda a: a.l_address)
                gen_html_table_view(options.out_file, sections, subsections, mem_types, DATA_SOURCE_MAP, css_file)
            else:
                print("Input file contains no section info or has a wrong format.")
        else:
            sections = parse_lst(options.in_file)
            if len(sections) > 0:
                sections.sort(key=lambda a: a.l_address)
                gen_html_table_view(options.out_file, sections, subsections, mem_types, DATA_SOURCE_LST, css_file)
            else:
                print("Input file contains no section info or has a wrong format.")

if __name__ == '__main__':
    main()

