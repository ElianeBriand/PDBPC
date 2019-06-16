PDB Parsing & Conformity in C++ is a complete and easy to use PDB file parser,
 that reports and tries to correct file format errors.
 
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.com/ElianeBriand/PDBPC.svg?branch=master)](https://travis-ci.com/ElianeBriand/PDBPC)
[![codecov](https://codecov.io/gh/ElianeBriand/PDBPC/branch/master/graph/badge.svg)](https://codecov.io/gh/ElianeBriand/PDBPC)

PDBPC is licenced under the GNU GPL v3 or later version. See GPLv3.txt for the terms of this licence.

# Basic usage

```c++
pdbpc::ParsedPDB ppdb = pdbpc::readPDBFile("../ExamplePDB/1HXW.pdb");

// While PDBPC can parse through non-conforming/incorrect PDB file format,
// (which can be useful when you can't modify the emitting software)
// It is nonetheless recommended to check and report errors

if(ppdb.hasErrors) {
    std::cout << "Error while parsing PDB file :" << std::endl;
    for(const auto& record: ppdb.outOfBandRecords) {
        if(record->severity != pdbpc::OutOfBandSeverity::error)
            continue;
        std::cout << record->type << ": " << record->subtype << std::end;
    }
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
   // However, relaxing the requirement allow for information extraction
   // eg: some columns were too long or too short, but as they were at the end of the line
   //     the content could be recovered
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
*  The `ParsedPDB` structure contains mostly array of `std::shared_ptr`, you do not have to fear copies too much.

# Bugs and improvements

If you find a standard-conforming PDB file that fails to parse, please report it as a bug. Same if
you have a non-compliant file that generate no warnings.


If you find an emitting software with a non-compliant quirk in its PDBs, file a feature 
request, we are interested in making the parser as robust as possible, even to not-quite-correct
formatting.