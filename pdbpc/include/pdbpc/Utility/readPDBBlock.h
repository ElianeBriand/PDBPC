//
// Created by eliane on 31/05/19.
//

#ifndef PDBPC_READPDBBLOCK_H
#define PDBPC_READPDBBLOCK_H

#include <string>

#include "pdbpc/ParsedPDB.h"

namespace pdbpc {


    ParsedPDB readPDBBlock(const std::string& PDBBlock, ParserSettings settings = ParserSettings());


}


#endif //PDBPC_READPDBBLOCK_H
