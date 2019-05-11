//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Parser/PostProcessing/postProcessParsedPDB.h>

#include <pdbpc/Parser/PostProcessing/modelPostProcessing.h>


namespace pdbpc {

    void postProcessParsedPDB(ParsedPDB& ppdb) {

        // TODO: Handle duplicate models

        // Handle no model record
        //  --> We legitimize the default model (model num -1) to be our only model
         modelPostProcessing_handleNoModel(ppdb);



    }

}