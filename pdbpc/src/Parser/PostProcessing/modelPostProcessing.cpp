//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Parser/PostProcessing/modelPostProcessing.h>


namespace pdbpc {

    void modelPostProcessing_handleNoModel(ParsedPDB& ppdb) {
        auto defaultModel = std::make_shared<Model>();
        defaultModel->modelNumber = 1;
        defaultModel->openingLineNumber = 0;
        defaultModel->openingLine = "<Beginning of file>";
        defaultModel->closingLineNumber = ppdb.numberOfParsedLines+1;
        defaultModel->closingLine = "<End of file>";
        ppdb.models.push_back(defaultModel);
    }

}