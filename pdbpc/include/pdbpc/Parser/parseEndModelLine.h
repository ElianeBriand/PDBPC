//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_PARSEENDMODELLINE_H
#define PDBPC_PARSEENDMODELLINE_H

#include "pdbpc/ParsedPDB.h"

namespace pdbpc {

    void parseEndModelLine(ParsedPDB& ppdb, const std::string& endModelLine, int lineNumber);

}


#endif //PDBPC_PARSEENDMODELLINE_H
