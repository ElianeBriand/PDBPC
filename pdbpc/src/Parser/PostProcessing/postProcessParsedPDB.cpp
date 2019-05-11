//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Parser/PostProcessing/postProcessParsedPDB.h>

#include <pdbpc/Parser/PostProcessing/modelPostProcessing.h>


namespace pdbpc {

    void postProcessParsedPDB(ParsedPDB& ppdb) {

        // TODO: Handle duplicate models

        // Handle no model record
        //  --> We create a default model where all records will go

        if(ppdb.models.empty())
            modelPostProcessing_handleNoModel(ppdb);



    }

}