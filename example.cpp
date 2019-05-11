

#include <pdbpc/pdbpc.h>

#include <pdbpc/Utility/LogPrettyPrint.h>

int main() {

    pdbpc::setupLogPrinting(); // Log pretty printing

    pdbpc::ParsedPDB ppdb = pdbpc::readPDBFile("../ExamplePDB/1HXW.pdb");

    for(const auto& record: ppdb.outOfBandRecords) {
        record->printRecord();
    }

    for(const auto& record: ppdb.models) {
        record->printModelRecord();
    }
}