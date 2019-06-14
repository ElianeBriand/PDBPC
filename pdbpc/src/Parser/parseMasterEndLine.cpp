//
// Created by eliane on 13/06/19.
//

#include <pdbpc/Parser/parseMasterEndLine.h>
#include <pdbpc/Utility/internalUtils.h>

namespace pdbpc {

    void EndRecordChecks(ParsedPDB& ppdb, const std::string& line, int lineNumber) {
        if (ppdb.details.endRecordReached) {
            ppdb.details.continuePastEndRecord = true;

            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBFileStructure;
            rec->subtype = OutOfBandSubType::FileContinuePastEndRecord;
            rec->line = line;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
        }
    }

    void parseEndLine(ParsedPDB& ppdb, const std::string& endLine, int lineNumber) {
        ppdb.details.endRecordLine = endLine;
        ppdb.details.endRecordLineNumber = lineNumber;
        ppdb.details.endRecordReached = true;

    }

    void parseMasterLine(ParsedPDB& ppdb, const std::string& masterLine, int lineNumber) {

        ppdb.details.master.line = masterLine;
        ppdb.details.master.lineNumber = lineNumber;

        std::string line = masterLine;

        bool hasMinimumColumn = inflateShorterLineOrFail(ppdb,line,lineNumber,70,80);

        if(!hasMinimumColumn) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::MasterRecordTooShort;
            rec->line = masterLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
            return;
        }

        trimAfterLastColumn(ppdb, line, lineNumber, 80);

        auto it_beg = line.begin();

        std::string numRemarkString(it_beg + 10, it_beg + 15);
        std::string mandatedZeroString(it_beg + 15, it_beg + 20);
        std::string numHetString(it_beg + 20, it_beg + 25);
        std::string numHelixString(it_beg + 25, it_beg + 30);
        std::string numSheetString(it_beg + 30, it_beg + 35);
        std::string numTurnString(it_beg + 35, it_beg + 40);
        std::string numSiteString(it_beg + 40, it_beg + 45);
        std::string numXformString(it_beg + 46, it_beg + 50);
        std::string numCoordString(it_beg + 50, it_beg + 55);
        std::string numTerString(it_beg + 55, it_beg + 60);
        std::string numConectString(it_beg + 60, it_beg + 65);
        std::string numSeqString(it_beg + 66, it_beg + 71);

        auto numRemark = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, numRemarkString,
                                                    OutOfBandSubType::MasterRecordFieldParseError);
        auto mandatedZero = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, mandatedZeroString,
                                                       OutOfBandSubType::MasterRecordFieldParseError);
        auto numHet = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, numHetString,
                                                 OutOfBandSubType::MasterRecordFieldParseError);
        auto numHelix = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, numHelixString,
                                                   OutOfBandSubType::MasterRecordFieldParseError);
        auto numSheet = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, numSheetString,
                                                   OutOfBandSubType::MasterRecordFieldParseError);
        auto numTurn = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, numTurnString,
                                                  OutOfBandSubType::MasterRecordFieldParseError);
        auto numSite = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, numSiteString,
                                                  OutOfBandSubType::MasterRecordFieldParseError);
        auto numXform = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, numXformString,
                                                   OutOfBandSubType::MasterRecordFieldParseError);
        auto numCoord = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, numCoordString,
                                                   OutOfBandSubType::MasterRecordFieldParseError);
        auto numTer = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, numTerString,
                                                 OutOfBandSubType::MasterRecordFieldParseError);
        auto numConect = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, numConectString,
                                                    OutOfBandSubType::MasterRecordFieldParseError);
        auto numSeq = parseOrRecoverableError<unsigned int>(ppdb, line, lineNumber, numSeqString,
                                                 OutOfBandSubType::MasterRecordFieldParseError);

        bool noError = checkNoEmptyOptional(numRemark,
                                            mandatedZero,
                                            numHet,
                                            numHelix,
                                            numSheet,
                                            numTurn,
                                            numSite,
                                            numXform,
                                            numCoord,
                                            numTer,
                                            numConect,
                                            numSeq);


        ppdb.details.master.numRemark = numRemark.value_or(-1);
        ppdb.details.master.mandatedZero = mandatedZero.value_or(-1);
        ppdb.details.master.numHet = numHet.value_or(-1);
        ppdb.details.master.numHelix = numHelix.value_or(-1);
        ppdb.details.master.numSheet = numSheet.value_or(-1);
        ppdb.details.master.numTurn = numTurn.value_or(-1);
        ppdb.details.master.numSite = numSite.value_or(-1);
        ppdb.details.master.numXform = numXform.value_or(-1);
        ppdb.details.master.numCoord = numCoord.value_or(-1);
        ppdb.details.master.numTer = numTer.value_or(-1);
        ppdb.details.master.numConect = numConect.value_or(-1);
        ppdb.details.master.numSeq = numSeq.value_or(-1);

        if(!noError) {
            // Signals that the checks will not be as thourough as if every fields were parsed
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::warning;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::MasterRecordCheckIncomplete;
            rec->line = line;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
        }





    }
}
