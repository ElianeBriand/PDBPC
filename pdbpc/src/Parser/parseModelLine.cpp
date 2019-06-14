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


#include <pdbpc/Parser/parseModelLine.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace b = boost;

#include <iostream>
#include <pdbpc/Utility/internalUtils.h>


namespace pdbpc {



    void parseModelLine(ParsedPDB& ppdb, const std::string& originalModelLine, int lineNumber) {
        // Standard conforming MODEL lines must have 14 columns at least

        // Will be used for field separation
        int modelNumber = -1;
        std::string line = originalModelLine;

        trimAfterLastColumn(ppdb,line,lineNumber,14);

        std::string modelNumberField = line.substr(5);

        std::optional<int> MaybeModelNum =  parseOrUnrecoverableError<int>(ppdb,
                originalModelLine,
                lineNumber,
                modelNumberField,
                OutOfBandSubType::unexpectedStringInsteadOfModelNumber );

        if (! MaybeModelNum.has_value()) {
            return;
        }

        modelNumber = MaybeModelNum.value();

        // If this has succeeded, there was a number in the field.
        // However it might have been a shorter field than expected

        if (originalModelLine.length() < 14) {
            // The line is incorrectly formatted, but there is a recoverable model number
            // We report and move one
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::NonCompliantColumnWidth;
            rec->line = originalModelLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
        }


        if (modelNumber < 0) {
            // Model number should be >0 (0 tolerated)
            // Though we could possibly, continue this indicates serious problem with the formating so we dont
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::negativeModelNumber;
            rec->line = originalModelLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
            return;
        }

        if (modelNumber == 0) {
            // Formally this is disallowed by the standard but it likely does not cause problem to number from 0
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::zeroModelNumber;
            rec->line = originalModelLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
        }

        for (const auto& model_sptr: ppdb.models) {
            if (model_sptr->modelNumber == modelNumber) {
                // Duplicate model ? We report, but this will be handled at post-processing
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::IncorrectPDBFileStructure;
                rec->subtype = OutOfBandSubType::duplicateModelNumber;
                rec->line = originalModelLine;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::recovered;
                ppdb.outOfBandRecords.push_back(rec);
            }
        }

        for (const auto& model_sptr: ppdb.models) {
            if (model_sptr->closingLineNumber == -1) {
                // A previously open model has not been closed with endmdl
                model_sptr->closingLineNumber = lineNumber; // we close it
                model_sptr->closingLine = originalModelLine;
                // We report the error
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::IncorrectPDBFileStructure;
                rec->subtype = OutOfBandSubType::newModelLineWhilePreviousNotClosed;
                rec->line = originalModelLine;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::recovered;
                ppdb.outOfBandRecords.push_back(rec);
            }
        }


        auto modelRecord = std::make_shared<Model>();
        modelRecord->openingLine = originalModelLine;
        modelRecord->openingLineNumber = lineNumber;
        modelRecord->closingLineNumber = -1;
        modelRecord->modelNumber = modelNumber;
        modelRecord->serialNumber = ++Model::lastserialNumber;

        ppdb.models.push_back(modelRecord);
    }

}