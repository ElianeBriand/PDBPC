PDB Parsing & Conformity in C++ is a complete and easy to use PDB file parser, that reports and try to correct errors
.


Basic usage :

```c++
pdbpc::ParsedPDB ppdb = pdbpc::readPDBFile("../ExamplePDB/1HXW.pdb");

// Access the parsed PDB

for(const auto& record: ppdb.models) {
    record->modelNumber;
    // record->printModelRecord(); // Print a summary
}

```
    