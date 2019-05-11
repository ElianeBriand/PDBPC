//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_PARSERSETTINGS_H
#define PDBPC_PARSERSETTINGS_H

namespace pdbpc {

    enum class DuplicateModelRecovery {
        renumberAfterParsingCompletion,
        dropDuplicate
    };

    struct ParserSettings {

        DuplicateModelRecovery duplicateModelRecovery = DuplicateModelRecovery::dropDuplicate;

        bool doPedanticVerifications = false;
    };

}


#endif //PDBPC_PARSERSETTINGS_H
