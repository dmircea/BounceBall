#!/usr/bin/python3
#############################################################################
#           MAKE PYTHON PROGRAM FOR COMPILING C++ CODE                  #####
#############################################################################
import os

#   FOR DEBUGGING ONLY
def log(output, debug = False):
    if(debug):
        print(output)

#   For future purposes
def get_exclusion_list():
    pass

#   Look through each folder and get .cpp file names in a list
#   This function should return a list of all source files
def get_all_source_files(root = '.'):
    print('Looking for source files...')
    directories = os.listdir(root)

    for i in range(len(directories)):
        directories[i] = root + '/' + directories[i]

    source_files = []

    for listing in directories:
        if os.path.isfile(listing) and len(listing) > 3 and listing[-4:] == '.cpp':
            source_files.append(listing)

        elif os.path.isdir(listing):
            source_files.extend(get_all_source_files(listing))

    return source_files

def main():
    print("Begin make!")
    success = True

    sources = get_all_source_files('.')

    log('The following line describes the sources of this file.')
    log(sources)

    command = 'clang++'
    e_flags = '-o'
    binary = 'main'
    l_flags = '-lsfml-graphics -lsfml-window -lsfml-system'

    print('Compilation start...')

    log(sources)
    log(str(sources)[1:-1])
    log(str(sources)[1:-1].replace(',','').replace('\'',''))
    execution = command + ' ' + str(sources)[1:-1].replace(',','').replace('\'','') + ' ' + e_flags + ' ' + binary + ' ' + l_flags

    log(execution)

    os.system(execution)

    print('End make!')

if __name__ == "__main__":
    main()
