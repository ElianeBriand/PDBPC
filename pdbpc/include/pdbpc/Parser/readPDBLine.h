//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_READPDBLINE_H
#define PDBPC_READPDBLINE_H

#include "pdbpc/ParsedPDB.h"

namespace pdbpc {

    void readPDBLine(ParsedPDB& ppdb, const std::string& line, int lineNumber);

}


#endif //PDBPC_READPDBLINE_H
