

#include <pdbpc/pdbpc.h>


int main() {

    pdbpc::ParsedPDB ppdb = pdbpc::readPDBFile("../ExamplePDB/1HXW.pdb");

    for(const auto& record: ppdb.outOfBandRecords) {
        record->printRecord();
    }

//    if(ppdb.hasErrors)
//        return 1;

    for(const auto& record: ppdb.outOfBandRecords) {
        record->printRecord();
    }

//    for(const auto& model: ppdb.models) {
//        for(const auto& chain: model->chains)
//        {
//            for(const auto& residue: chain->residues)
//            {
//                residue->printRecord();
//            }
//        }
//    }


}