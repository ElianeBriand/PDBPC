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
#include <Parser/parseEndModelLine.h>


#include <boost/algorithm/string.hpp>
#include <Utility/internalUtils.h>

namespace b = boost;


namespace pdbpc {

    void parseEndModelLine(ParsedPDB& ppdb, const std::string& originalEndModelLine, int lineNumber) {

        // Standard conforming ENDMDL lines must have 6 columns, composing the keyword ENDMDL

        std::string line = originalEndModelLine;

        trimAfterLastColumn(ppdb, line, lineNumber, 6);


        if(ppdb.models.empty()) {
            // ENDMDL but no MODEL statement were encountered before
            // We ignore this ENDMDL, and report the error
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBFileStructure;
            rec->subtype = OutOfBandSubType::EndMdlWithoutOpeningModelStatement;
            rec->line = originalEndModelLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
            return;
        }

        std::shared_ptr<Model> lastModel = ppdb.models.back();

        if(lastModel->closingLineNumber != -1) {
            // last model is already closed : report and ignore
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBFileStructure;
            rec->subtype = OutOfBandSubType::EndMdlWithoutOpeningModelStatement;
            rec->line = originalEndModelLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
            return;
        }

        lastModel->closingLine = originalEndModelLine;
        lastModel->closingLineNumber = lineNumber;

    }

}