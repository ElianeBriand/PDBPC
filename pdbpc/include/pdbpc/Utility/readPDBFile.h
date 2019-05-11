//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_READPDBFILE_H
#define PDBPC_READPDBFILE_H

#include <string>

#include "pdbpc/ParsedPDB.h"

namespace pdbpc {


    ParsedPDB readPDBFile(const std::string& filepath, ParserSettings settings = ParserSettings());


}


#endif //PDBPC_READPDBFILE_H
