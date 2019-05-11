//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Parser/PostProcessing/modelPostProcessing.h>
#include <pdbpc/Utility/internalUtils.h>


namespace pdbpc {

    void modelPostProcessing_handleNoModel(ParsedPDB& ppdb) {

        if(ppdb.models.size() > 1) {
            // We have explicit models
            if(ppdb.models.at(0)->atoms_flatlist.size() == 0) {
                // No atoms in the default model : we can just remove it
                ppdb.models.erase(ppdb.models.begin());
            }else {
                // We find a free model num to legitimize the default model
                int newDefaultModelmodelNum = findNextFreeModelNumber(ppdb);
                ppdb.models.at(0)->modelNumber = newDefaultModelmodelNum;
            }
        }else {
            // We only have the implicit default model
            ppdb.models.at(0)->modelNumber = 1;
        }
    }

}