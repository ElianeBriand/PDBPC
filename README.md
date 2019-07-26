PDB Parsing & Conformity in C++ is a complete and easy to use PDB file parser,
 that reports and tries to parse through common PDB format errors.
 
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.com/ElianeBriand/PDBPC.svg?branch=master)](https://travis-ci.com/ElianeBriand/PDBPC)
[![codecov](https://codecov.io/gh/ElianeBriand/PDBPC/branch/master/graph/badge.svg)](https://codecov.io/gh/ElianeBriand/PDBPC)

PDBPC is licenced under the GNU GPL v3 or later version. See GPLv3.txt for the terms of this licence.

# Installing

```sh
 #git clone or download release
 mkdir build
 cd build
 
 # INSTALL_PREFIX recommended for ease of installing/unistalling, choose wherever you prefer
 cmake .. -DCMAKE_INSTALL_PREFIX=/home/user/local/
 make -j4
 make install
```

Then inform your compiler of the header location (eg `-I/home/user/local/include`) and your linker of the
library file location (eg `-L/home/user/local/lib`), and the library to use (eg `-lpdbpc_static`). You can also copy `pdbpc/include/pdbpc`
and the generated library file wherever you want, and point your toolchain there.

The library version, which contain the same code, are the following :

- **pdbpc_static.a** : static library, no runtime dependencies on pdbpc. Can be used in static executable, or executable which links with
other libraries static or not, but not in position-independant executable (`-fPIE`) or shared libraries (`.so`)
- **pdbpc_shared.so** : dynamic library, must be shipped with the executable. Can be used in static or position-independant executable, in shared
libraries. Does not have transitive dependencies, except on libstdc++ and system runtimes, so no need to have Boost on the target system.
- **pdbpc_static_pic.a** : static library compiled with -fPIC. You will want to use this if you are building shared libraries
 (including python module as `.so`), but do not want to introduce transitive dependencies on pdbpc. Can also be used in 
 position-independant executable.

Basically, try to use pdbpc_static. If you encounter errors that sound like `relocation R_X86_64_32S against XXX can not
 be used when making a shared object; recompile with -fPIC`, or `symbol cannot be emitted on the output`, you need to switch
 to pdbpc_static_pic or pdbpc_shared. 

If you want to statically link to pdbpc, but dynamically to other libraries, `-Wl,-Bstatic -lpdbpc_static -Wl,-Bdynamic` or, if
talking directly to the linker, `-Bstatic -lpdbpc_static -Bdynamic`, or if using CMake target_link_library `-Wl,-Bstatic pdbpc_static -Wl,-Bdynamic`


# Basic usage

```c++
#include <pdbpc/pdbpc.h> 

pdbpc::ParsedPDB ppdb = pdbpc::readPDBFile("../ExamplePDB/1HXW.pdb");

// While PDBPC can parse through non-conforming/incorrect PDB file format,
// (which can be useful when you can't modify the emitting software)
// It is nonetheless recommended to check and report errors

if(ppdb.hasErrors) {
    std::cout << "Error while parsing PDB file :" << std::endl;
    for(const auto& record: ppdb.outOfBandRecords) {
        if(record->severity != pdbpc::OutOfBandSeverity::error)
            continue;
        std::cout << "Line " << record->lineNumber << ": " 
        << record->type << " -- " << record->subtype << std::end;
    }
    return 1;
}


// Access the parsed PDB :

// As a flat atom list
for(const auto& atom : ppdb.atoms_flatlist) {
    std::cout << "Atom " << atom->atomName << " !" << std::endl;
}

// As a hierarchy
for(const auto& model: ppdb.models) {
    for(const auto& chain : model->chains) {
        for(const auto& residue: chain->residues) {
            for(const auto& atom: residue->atoms) {
                std::cout << "Atom " << atom->atomName << " !" << std::endl;
            }
        }
    }
}

// As a mix : flat list from any hierarchical level
for(const auto& model: ppdb.models) {
    for(const auto& chain : model->chains) {
        auto atom = chain.atoms_flatlist.at(23);
        std::cout << "Atom #23 is " << atom->atomName << " !" << std::endl;
    }
}

```
    
# Conformity checking

```c++
pdbpc::ParsedPDB ppdb = pdbpc::readPDBFile("../ExamplePDB/1HXW.pdb");

// More strict flags :
if(ppdb.hasErrors) {
   // Some line were totally un-parseable, and were skipped
   // This means there is missing information
   // Generally, this should lead to early termination of the program
}
if(ppdb.hasWarnings) {
   // Some lines were misformatted according to the standard
   // However, relaxing the requirement allowed for information extraction
   // eg: some columns were too long or too short, but as they were at the end of the line
   //     so the content could be recovered
   // This is a bad sign, but you could proceed anyway (maybe display a message)
}

if(ppdb.hasPedanticProblems) {
   // Some lines were misformatted according to the standard. 
   // However, parsing could continue without any problem/risk.
   // Even the slightest additional space at end of line triggers this
   // Also, not-always-relevant constraints (like, that MODEL should have exactly the same ATOM content)
   // are enforced.
   // Can be used as a test suite if you have a program emitting PDB file.
}

// Print all errors/warning/pedant remarks generated when parsing, with line, line number,
// and offences.
for(const auto& record: ppdb.outOfBandRecords) {
    record->printRecord();
}

```

# Tips
*  The `ParsedPDB` structure contains mostly vectors of `std::shared_ptr`, you do not have to fear copies too much.

# Bugs and improvements

If you find a standard-conforming PDB file that fails to parse, please report it as a bug. Same if
you have a non-compliant file that generate no warnings.


If you find an emitting software with a non-compliant quirk in its PDBs, file a feature 
request, we are interested in making the parser as robust as possible, even to not-quite-correct
formatting.

PDB formatting "standard" is here : [https://www.wwpdb.org/documentation/file-format-content/format33/v3.3.html](https://www.wwpdb.org/documentation/file-format-content/format33/v3.3.html)