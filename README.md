PDB Parsing & Conformity in C++ is a complete and easy to use PDB file parser, that reports and try to correct errors
.


# Basic usage

```c++
pdbpc::ParsedPDB ppdb = pdbpc::readPDBFile("../ExamplePDB/1HXW.pdb");

// While PDBPC can parse through non-conforming/incorrect PDB file format,
// (which can be useful when you can't modify the emitting software)
// It is nonetheless recommended in the general case to check for parsing errors

if(ppdb.hasErrors) {
    std::cout << "PDB file cannot be read" << std::endl;
    return -2
}


// Access the parsed PDB
for(const auto& record: ppdb.models) {
    record->modelNumber;
    // record->printModelRecord(); // Print a summary
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

```