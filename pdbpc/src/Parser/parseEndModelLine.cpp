//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Parser/parseEndModelLine.h>


#include <boost/algorithm/string.hpp>

namespace b = boost;


namespace pdbpc {

    void parseEndModelLine(ParsedPDB& ppdb, const std::string& originalEndModelLine, int lineNumber) {

        // Standard conforming ENDMDL lines must have 6 columns, composing the keyword ENDMDL

        // Column preprocessing
        if(originalEndModelLine.length() > 6) {
            // Too many columns
            std::string trimmed_endModelLine = b::trim_right_copy(originalEndModelLine);

            auto rec = std::make_shared<OutOfBandRecord>();

            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            if(trimmed_endModelLine.length() == 6) {
                rec->severity = OutOfBandSeverity::idiosyncrasy;
                rec->subtype = OutOfBandSubType::SupernumerarySpaceAfterLastColumn;
            }else {
                rec->severity = OutOfBandSeverity::error;
                rec->subtype = OutOfBandSubType::SupernumeraryContentAfterLastColumn;
            }
            rec->line = originalEndModelLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
        }

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

        return;
    }

}