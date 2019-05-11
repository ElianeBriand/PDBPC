//
// Created by eliane on 11/05/19.
//


#include <pdbpc/Parser/parseModelLine.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace b = boost;

#include <iostream>


namespace pdbpc {

    namespace {

        bool doLexicalCastOnTrimmedLine(const std::string& trimmedLine,
                                        int& modelNumber,
                                        ParsedPDB& ppdb,
                                        const std::string& originalModelLine,
                                        int lineNumber) {
            bool castHasSucceeded = true;
            try {
                modelNumber = b::lexical_cast<int>(trimmedLine);
            }
            catch (const b::bad_lexical_cast& e) {
                castHasSucceeded = false;
            }

            if (!castHasSucceeded) {
                auto rec2 = std::make_shared<OutOfBandRecord>();
                rec2->severity = OutOfBandSeverity::error;
                rec2->type = OutOfBandType::IncorrectPDBLineFormat;
                rec2->subtype = OutOfBandSubType::unexpectedStringInsteadOfModelNumber;
                rec2->line = originalModelLine;
                rec2->lineNumber = lineNumber;
                rec2->recoveryStatus = RecoveryStatus::unrecoverable;
                ppdb.outOfBandRecords.push_back(rec2);
                return false;
            }
            return true;
        }

    }


    void parseModelLine(ParsedPDB& ppdb, const std::string& originalModelLine, int lineNumber) {
        // Standard conforming MODEL lines must have 14 columns at least

        // Will be used for field separation
        int modelNumber = -1;

        if (originalModelLine.length() > 14) {
            std::string trimmed_modelLine = b::trim_right_copy(originalModelLine);
            if (trimmed_modelLine.length() == 14) {
                // It was only spaces
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::idiosyncrasy;
                rec->type = OutOfBandType::IncorrectPDBLineFormat;
                rec->subtype = OutOfBandSubType::SupernumerarySpaceAfterLastColumn;
                rec->line = originalModelLine;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::recovered;
                ppdb.outOfBandRecords.push_back(rec);
            }
        }

        std::string expectedModelNumberfield = originalModelLine.substr(5);
        std::string trimmed_modelNumber = b::trim_copy(expectedModelNumberfield);
        bool castHasSucceeded = doLexicalCastOnTrimmedLine(trimmed_modelNumber,
                                                           modelNumber,
                                                           ppdb,
                                                           originalModelLine,
                                                           lineNumber);
        if (!castHasSucceeded) {
            return;
        }

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

        // From this point modelNumber is initialized with the lexically casted model number

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

        ppdb.models.push_back(modelRecord);
    }

}