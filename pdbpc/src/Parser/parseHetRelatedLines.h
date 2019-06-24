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

#ifndef PDBPC_PARSEHETRELATEDLINES_H
#define PDBPC_PARSEHETRELATEDLINES_H

#include <memory>

#include "pdbpc/ParsedPDB.h"

namespace pdbpc {

    void parseHETLine(ParsedPDB& ppdb, const std::string& hetLine, int lineNumber);
    void parseHETNAMLine(ParsedPDB& ppdb, const std::string& hetnamLine, int lineNumber);
    void parseHETSYNLine(ParsedPDB& ppdb, const std::string& hetsynLine, int lineNumber);

    void parseFORMULLine(ParsedPDB& ppdb, const std::string& formulLine, int lineNumber);


    std::shared_ptr<Heterogen> createOrphanedHeterogen(ParsedPDB& ppdb,
            const std::string& line,
            int lineNumber,
            const std::string& hetID);

    std::shared_ptr<Heterogen> findHetOrCreateOrphan(ParsedPDB& ppdb,
                                                       const std::string& line,
                                                       int lineNumber,
                                                       const std::string& hetID);


}

#endif //PDBPC_PARSEHETRELATEDLINES_H
