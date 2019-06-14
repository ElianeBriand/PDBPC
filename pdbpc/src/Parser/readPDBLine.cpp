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

#include <pdbpc/Parser/readPDBLine.h>

#include <pdbpc/Parser/parseModelLine.h>
#include <pdbpc/Parser/parseEndModelLine.h>
#include <pdbpc/Parser/parseAtomLine.h>
#include <pdbpc/Parser/parseMasterEndLine.h>

#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>



namespace b = boost;

namespace pdbpc {

    void readPDBLine(ParsedPDB& ppdb, const std::string& line, int lineNumber) {

        // Triggered checks go here
        EndRecordChecks(ppdb,line,lineNumber);


        if(b::starts_with(line, "MODEL ")) {

            parseModelLine(ppdb,line,lineNumber);

        }else if (b::starts_with(line, "ENDMDL")) {

            parseEndModelLine(ppdb,line,lineNumber);

        }else if (b::starts_with(line, "END   ")) {

            parseEndLine(ppdb,line,lineNumber);

        }else if (b::starts_with(line, "MASTER")) {

            parseMasterLine(ppdb,line,lineNumber);

        }else if (b::starts_with(line, "ATOM  ")) {

            parseAtomLine(ppdb,line,lineNumber);

        }else if (b::starts_with(line, "HETATM ")) {

        }else {
            // Line type unsupported ?
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::UnhandledPDBLine;
            rec->subtype = OutOfBandSubType::NonRecognizedLinePrefix;
            rec->line = line;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
        }


    }

}