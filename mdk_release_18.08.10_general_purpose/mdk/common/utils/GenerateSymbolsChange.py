#!/usr/bin/env python
import argparse
import os
import re
from subprocess import Popen, PIPE, STDOUT, call
   
asm_file_types_pattern = ['.asm']
   
c_file_types_pattern = ['.hpp','.h','.cpp','.c']
   
def format_filter_labels(labels):
    fmtlabels=[]
    index=0
    for x in labels:
        y=x.split()
        for word in y:
            label=word.replace(":","")
            #determine this is indeed a underscore label (at least one underscore)
            if re.search('^_',label):
                #determine if this is not a local label
                if not re.search('^___',label):
                    #determine if this is a label at all or an empty string
                    if re.search('[a-zA-Z0-9]',label):
                        fmtlabels.append([])
                        fmtlabels[index]=label
                        index=index+1
    return fmtlabels
        

def find_labels(file_name, outfile):
    """
    Finds labels in a file
    
    Finds all labels in a file that are not local labels.
    """
    
    outf=open(outfile,"a")
    with open(file_name, "r") as f:
        #Generate list with all labels inside the file
        labelscommand='cat '+file_name+' | grep "_*:"'
        labels=os.popen(labelscommand).read().split('\n')
        labels=format_filter_labels(labels)
        #and now, for each label, print out a line for all possible replacements
        for label in labels:
            #Build up the new symbol by stripping the leading underscore
            newlabel=label.lstrip('_')
            #First the shave assembly replacement:
            outf.write(label+' '+newlabel+'\n')
            #then all of the possible Leon sysmbols
            for shvsym in ['SVE0_','SVE1_','SVE2_','SVE3_','SVE4_','SVE5_','SVE6_','SVE7_']:
                outf.write(shvsym+label+' '+shvsym+newlabel+'\n')
    outf.close()
        
def find_labels_in_c(file_name, outfile):
    """
    Finds labels in a file
    
    Finds all labels in a file that are not local labels.
    """
    
    outf=open(outfile,"a")
    with open(file_name, "r") as f:
        #Generate list with all labels inside the file
        labelscommand='cat '+file_name+' | grep "SVE[01234567]__" | grep "extern"'
        lines=os.popen(labelscommand).read().split('\n')
        labels=[]
        index=0
        #identify the label in the selected strings
        for line in lines:
            words=line.split()
            #Build up the labels list
            for word in words:
                if re.search("SVE",word):
                    #Strip any extra paranthesis existing in there
                    x=word.replace("(","")
                    y=x.replace(")","")
                    z=y.replace(";","")
                    t=z.replace("[","")
                    u=t.replace("]","")
                    labels.append([])
                    labels[index]=u
                    index=index+1
        #Now we have labels generated. We need to generate the file replacements
        for label in labels:
            outf.write(label+' '+label.replace("__","_",1)+'\n')
    outf.close()
        
def find_in_dir(dir_name, outfile):
    """
    Finds labels in a directory.
    
    Finds labels in a directory. Checks they are global labels, not local ones.
    """
    for root, _, files in os.walk(dir_name):
        asm_file_list = [f for f in files for ext in asm_file_types_pattern if f.endswith(ext)]
        #print(file_list)
        for file in asm_file_list:
            file_path = os.path.join(root, file)
            print(file_path)
            find_labels(file_path, outfile)
        c_file_list = [f for f in files for ext in c_file_types_pattern if f.endswith(ext)]
        #print(file_list)
        for file in c_file_list:
            file_path = os.path.join(root, file)
            print(file_path)
            find_labels_in_c(file_path, outfile)
    print("Done")
            
    
def main():
    parser = argparse.ArgumentParser(description = "Label generator")
    parser.add_argument('out_file', action='store', help='out file with one line per string change, which contains space separated "old_string NewString"')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('-f', '--file',  metavar='FILE', nargs='+', help='list of files in which the replacement will be performed')
    group.add_argument('-d', '--dir', nargs=1, help='the directory in which the replacement will be performed')
    args = parser.parse_args()
    #print(args)
    
    if args.dir:
        find_in_dir(os.path.normpath(args.dir[0]), args.out_file)
    if args.file:
        for f in args.file:
            print("Generating labels to file %s..." % f)
            find_labels(f, os.path.normpath(args.out_file))
            find_labels_in_c(f, os.path.normpath(args.out_file))
        print("Done")
    
if __name__ == "__main__":
    main()
