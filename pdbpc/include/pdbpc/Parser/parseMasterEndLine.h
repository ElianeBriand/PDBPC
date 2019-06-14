/*
 * Copyright (c) 2019 Eliane Briand
 *
 * This file is part of PDBPC.
 *
 * PDBPC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PDBPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PDBPC.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef PDBPC_PARSEMASTERENDLINE_H
#define PDBPC_PARSEMASTERENDLINE_H



#include "pdbpc/ParsedPDB.h"

namespace pdbpc {

    void EndRecordChecks(ParsedPDB& ppdb, const std::string& line, int lineNumber);

    void parseMasterLine(ParsedPDB& ppdb, const std::string& masterLine, int lineNumber);

    void parseEndLine(ParsedPDB& ppdb, const std::string& endLine, int lineNumber);

}



#endif //PDBPC_PARSEMASTERENDLINE_H
