//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_PARSEDPDB_H
#define PDBPC_PARSEDPDB_H

#include <vector>
#include <memory>

#include "ParserSettings.h"
#include "pdbpc/Records/Model.h"
#include "pdbpc/Records/OutOfBandRecord.h"

namespace pdbpc {
    struct ParsedPDB {

        ParsedPDB();

        ParserSettings settings;

        std::vector<std::shared_ptr<Model>> models;


        std::vector<std::shared_ptr<OutOfBandRecord>> outOfBandRecords;


        int numberOfParsedLines;

    };
}



#endif //PDBPC_PARSEDPDB_H
