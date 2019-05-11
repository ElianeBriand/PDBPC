//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_PARSEMODELLINE_H
#define PDBPC_PARSEMODELLINE_H

#include "pdbpc/ParsedPDB.h"

namespace pdbpc {

    bool parseModelLine(ParsedPDB& ppdb, const std::string& originalModelLine, int lineNumber);

}

#endif //PDBPC_PARSEMODELLINE_H
