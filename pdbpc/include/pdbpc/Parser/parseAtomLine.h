//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_PARSEATOMLINE_H
#define PDBPC_PARSEATOMLINE_H



#include "pdbpc/ParsedPDB.h"

namespace pdbpc {

    void parseAtomLine(ParsedPDB& ppdb, const std::string& originalAtomLine, int lineNumber);

}

#endif //PDBPC_PARSEATOMLINE_H
