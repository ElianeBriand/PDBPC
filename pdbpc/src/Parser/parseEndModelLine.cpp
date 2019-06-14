//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Parser/parseEndModelLine.h>


#include <boost/algorithm/string.hpp>
#include <pdbpc/Utility/internalUtils.h>

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