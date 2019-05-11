//
// Created by eliane on 11/05/19.
//


#include <pdbpc/Parser/parseModelLine.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace b = boost;


namespace pdbpc {

    bool parseModelLine(ParsedPDB& ppdb, const std::string& originalModelLine, int lineNumber) {
        // Standard conforming MODEL lines must have 14 columns at least

        // Will be used for field separation
        std::string modelLine = originalModelLine;
        int modelNumber = -1;

        // Preprocessing the model line (column number check)
        if(modelLine.length() >= 14) {
            //We have at least the 14 required columns

            if(modelLine.length() > 14)
            {
                // Too many columns
                std::string trimmed_modelLine = b::trim_right_copy(modelLine);
                if(trimmed_modelLine.length() == 14) {
                    // Spaces were added after the model number, not a serious problem
                    auto rec = std::make_shared<OutOfBandRecord>();
                    rec->severity = OutOfBandSeverity::idiosyncrasy;
                    rec->type = OutOfBandType::IncorrectPDBLineFormat;
                    rec->subtype = OutOfBandSubType::SupernumerarySpaceAfterLastColumn;
                    rec->line = modelLine;
                    rec->lineNumber = lineNumber;
                    rec->recoveryStatus = RecoveryStatus::recovered;
                    ppdb.outOfBandRecords.push_back(rec);

                    modelLine = trimmed_modelLine;
                }else{
                    // Something else going on:
                    std::string expectedModelNumberfield = modelLine.substr(5);
                    int tentativeModelNumber = -1;
                    bool castHasSucceeded = true;
                    try
                    {
                        tentativeModelNumber = b::lexical_cast<int>(expectedModelNumberfield);
                    }
                    catch(const b::bad_lexical_cast& e)
                    {
                        castHasSucceeded = false;
                    }

                    if(castHasSucceeded)
                    {
                        // The line is incorrectly formatted, but there is a recoverable model number
                        // We report and move one
                        auto rec = std::make_shared<OutOfBandRecord>();
                        rec->severity = OutOfBandSeverity::error;
                        rec->type = OutOfBandType::IncorrectPDBLineFormat;
                        rec->subtype = OutOfBandSubType::NonCompliantColumnWidth;
                        rec->line = modelLine;
                        rec->lineNumber = lineNumber;
                        rec->recoveryStatus = RecoveryStatus::recovered;
                        ppdb.outOfBandRecords.push_back(rec);

                    }else {
                        // There is something not convertible to int in the field
                        // We give up and report
                        auto rec = std::make_shared<OutOfBandRecord>();
                        rec->severity = OutOfBandSeverity::error;
                        rec->type = OutOfBandType::IncorrectPDBLineFormat;
                        rec->subtype = OutOfBandSubType::unexpectedStringInsteadOfModelNumber;
                        rec->line = modelLine;
                        rec->lineNumber = lineNumber;
                        rec->recoveryStatus = RecoveryStatus::unrecoverable;
                        ppdb.outOfBandRecords.push_back(rec);
                        return false;
                    }

                    modelNumber = tentativeModelNumber;
                    goto modelNumberAlreadyDetermined;
                }
            }

            // Else the model line is exactly 14 columns wide, nothing to do

        }else {
            // We are missing some columns
            // We know we have at least column 1-6 "MODEL " (because we have been routed to this function)
            // We log the problem and try to recover (by searching for a valid model number in the remaining columns)

            auto rec = std::make_shared<OutOfBandRecord>();
            // We will fill severity/recoveryStatus based on whether we could recover
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->line = originalModelLine;
            rec->lineNumber = lineNumber;


            ppdb.outOfBandRecords.push_back(rec);
        }

        {
            std::string expectedModelNumberfield = modelLine.substr(5);

            bool castHasSucceeded = true;
            try
            {
                modelNumber = b::lexical_cast<int>(expectedModelNumberfield);
            }
            catch(const b::bad_lexical_cast& e)
            {
                castHasSucceeded = false;
            }

            if(!castHasSucceeded) {
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::IncorrectPDBLineFormat;
                rec->subtype = OutOfBandSubType::unexpectedStringInsteadOfModelNumber;
                rec->line = modelLine;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::unrecoverable;
                ppdb.outOfBandRecords.push_back(rec);
                return false;
            }
        }

        // From this point modelNumber is initialized with the lexically casted model number
        modelNumberAlreadyDetermined:

        if(modelNumber < 0) {
            // Model number should be >0 (0 tolerated)
            // Though we could possibly, continue this indicates serious problem with the formating so we dont
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::negativeModelNumber;
            rec->line = modelLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
            return false;
        }

        if(modelNumber == 0) {
            // Formally this is disallowed by the standard but it likely does not cause problem to number from 0
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::zeroModelNumber;
            rec->line = modelLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
        }

        for(const auto& model_sptr: ppdb.models) {
            if(model_sptr->modelNumber == modelNumber) {
                // Duplicate model ? We report, but this will be handled at post-processing
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::IncorrectPDBFileStructure;
                rec->subtype = OutOfBandSubType::duplicateModelNumber;
                rec->line = modelLine;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::recovered;
                ppdb.outOfBandRecords.push_back(rec);
            }
        }

        for(const auto& model_sptr: ppdb.models) {
            if(model_sptr->closingLineNumber == -1) {
                // A previously open model has not been closed with endmdl
                model_sptr->closingLineNumber = lineNumber; // we close it
                model_sptr->closingLine = modelLine;
                // We report the error
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::IncorrectPDBFileStructure;
                rec->subtype = OutOfBandSubType::newModelLineWhilePreviousNotClosed;
                rec->line = modelLine;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::recovered;
                ppdb.outOfBandRecords.push_back(rec);
            }
        }


        auto modelRecord = std::make_shared<Model>();
        modelRecord->openingLine = modelLine;
        modelRecord->openingLineNumber = lineNumber;
        modelRecord->closingLineNumber = -1;
        modelRecord->modelNumber = modelNumber;

        ppdb.models.push_back(modelRecord);
        return true;
    }

}