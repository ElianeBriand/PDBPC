//
// Created by eliane on 13/06/19.
//

#ifndef PDBPC_PARSEMASTERENDLINE_H
#define PDBPC_PARSEMASTERENDLINE_H



#include "pdbpc/ParsedPDB.h"

namespace pdbpc {

    void EndRecordChecks(ParsedPDB& ppdb, const std::string& line, int lineNumber);

    void parseMasterLine(ParsedPDB& ppdb, const std::string& masterLine, int lineNumber);

    void parseEndLine(ParsedPDB& ppdb, const std::string& endLine, int lineNumber);

}



#endif //PDBPC_PARSEMASTERENDLINE_H
