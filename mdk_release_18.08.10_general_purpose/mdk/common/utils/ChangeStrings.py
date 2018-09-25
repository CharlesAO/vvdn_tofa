#!/usr/bin/env python
import argparse
import os
import re
import sys

file_types_pattern = ['.c', '.h', '.ldscript','.asm']

def get_replace_data(file_name):
    """
    Create a dict with old-new string pairs.
    
    Returns a dict containig all new and old strings as
    key-value pairs. Each pair is read from one line of 
    the input file.
    """
    replace_table = {}
    with open(file_name, 'r') as f:
        for line in f.readlines():
            old = new = None
            if line.strip():
                old, new = line.split()
                #print("old: %s new: %s" % (old, new))
            if old and new:
                if old not in replace_table.keys():
                    replace_table[old] = new
                else:
                    print("%s found more than once. Only the first entry will be considered" % old, file = sys.stderr)
    return replace_table
        
def replace_string(file_name, replacement, backup = False):
    """
    Replaces strings in a file.
    
    Replaces all strings which corespond to a key in the replacement
    dict with it's corresponding value in the input file. If the 
    optional backup parameter is true a backup will be made for the
    initial file, otherwise the changed file will overwrite the 
    original one.
    """
    pattern = re.compile('|'.join([r"\b%s\b" % k for k in replacement.keys()]))
    with open(file_name, "r") as f:
        out_file_name = file_name + ".tmp"
        out_file = open(out_file_name, "w", newline = '\n')
        file_changed = True
        if re.search(pattern, f.read()):
            f.seek(0)
            for line in f.readlines():
                new_line = pattern.sub(lambda m: replacement[m.group(0)], line)
                out_file.write(new_line)
            print("File: %s - all strings changed." % file_name)
        else:
            #file not changed; no need for backup
            file_changed = False
            backup = False
        out_file.close()
    if backup:
        backup_file_name = file_name + '.bak'
        #delete backup file if already exists
        if os.path.exists(backup_file_name):
            os.unlink(backup_file_name)
        os.rename(file_name, backup_file_name)
    elif file_changed:
        #delete original file
        os.unlink(file_name)
    if file_changed:
        os.rename(out_file_name, out_file_name[:-4])
    else:
        os.unlink(out_file_name)
    return file_changed
        
def replace_in_dir(dir_name, file_types, replacement, backup = False):
    """
    Replaces strings in a directory.
    
    Replaces all strings which corespond to a key in the replacement
    dict with it's corresponding value in all the files in the inpt
    directory which matches the patterns in the file_types. If the 
    optional backup parameter is true a backup will be made for the
    initial files, otherwise the changed files will overwrite the 
    original ones.
    """
    errfile=open("ErrorFile.lst","w")
    
    for root, _, files in os.walk(dir_name):
        file_list = [f for f in files for ext in file_types if f.endswith(ext)]
        #print(file_list)
        for file in file_list:
            file_path = os.path.join(root, file)
            try:
                replace_string(file_path, replacement, backup)
            except (UnicodeDecodeError) as error:
                errfile.write(file_path+'\n')
                continue
    errfile.close()
    print("Done")
            
    
def main():
    parser = argparse.ArgumentParser(description = "String replacer")
    parser.add_argument('rep_file', action='store', help='file with one line per string change, which contains space separated "old_string NewString"')
    parser.add_argument('-b', '--backup', action='store_true', help='create backup for modified files')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('-f', '--file',  metavar='FILE', nargs='+', help='list of files in which the replacement will be performed')
    group.add_argument('-d', '--dir', nargs=1, help='the directory in which the replacement will be performed')
    args = parser.parse_args()
    #print(args)
    
    backup = False
    if args.backup:
        backup = True
    if args.dir:
        replace_in_dir(os.path.normpath(args.dir[0]), file_types_pattern, get_replace_data(args.rep_file), backup)
    if args.file:
        for f in args.file:
            print("Replacing in file %s..." % f)
            replace_string(f, get_replace_data(os.path.normpath(args.rep_file)), backup)
        print("Done")
    
if __name__ == "__main__":
    main()
