//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Utility/internalUtils.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


namespace b = boost;

namespace pdbpc {


    bool checkPDBPath(ParsedPDB& ppdb, bfs::path pdbPath) {

        if (bfs::exists(pdbPath)) {
            if (bfs::is_regular_file(pdbPath)) {
                if ((pdbPath.extension() != ".pdb") && (pdbPath.extension() != ".PDB")) {
                    auto rec = std::make_shared<OutOfBandRecord>();
                    rec->severity = OutOfBandSeverity::error;
                    rec->type = OutOfBandType::FileAccessError;
                    rec->subtype = OutOfBandSubType::FileExtensionNotPDB;
                    rec->line = std::string("FILE PATH : ") + pdbPath.string();
                    rec->lineNumber = -1;
                    rec->recoveryStatus = RecoveryStatus::recovered;
                    ppdb.outOfBandRecords.push_back(rec);
                }
                return true;
            } else if (bfs::is_directory(pdbPath)) {

                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::FileAccessError;
                rec->subtype = OutOfBandSubType::FilePathIsDirectory;
                rec->line = std::string("FILE PATH : ") + pdbPath.string();
                rec->lineNumber = -1;
                rec->recoveryStatus = RecoveryStatus::unrecoverable;
                ppdb.outOfBandRecords.push_back(rec);

                return false;
            } else {

                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::FileAccessError;
                rec->subtype = OutOfBandSubType::FileNotARegularFile;
                rec->line = std::string("FILE PATH : ") + pdbPath.string();
                rec->lineNumber = -1;
                rec->recoveryStatus = RecoveryStatus::unrecoverable;
                ppdb.outOfBandRecords.push_back(rec);

                return false;
            }
        } else {

            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::FileAccessError;
            rec->subtype = OutOfBandSubType::FileNotFound;
            rec->line = std::string("FILE PATH : ") + pdbPath.string();
            rec->lineNumber = -1;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);

            return false;
        }

    }

    void checkForErrorsWarningsProblems(ParsedPDB& ppdb) {
        ppdb.hasErrors = false;
        ppdb.hasWarnings = false;
        ppdb.hasPedanticProblems = false;
        for (const auto& record: ppdb.outOfBandRecords) {
            if (record->severity == OutOfBandSeverity::error)
                ppdb.hasErrors = true;
            if (record->severity == OutOfBandSeverity::warning)
                ppdb.hasWarnings = true;
            if (record->severity == OutOfBandSeverity::idiosyncrasy)
                ppdb.hasPedanticProblems = true;
        }
    }

    void createPlaceholderModelAndChain(ParsedPDB& ppdb) {
        auto defaultModel = std::make_shared<Model>();
        defaultModel->modelNumber = -1;
        defaultModel->openingLineNumber = 0;
        defaultModel->openingLine = "<Beginning of file>";
        defaultModel->closingLineNumber = ppdb.numberOfParsedLines + 1;
        defaultModel->closingLine = "<End of file>";
        ppdb.models.push_back(defaultModel);
    }

    int findNextFreeModelNumber(ParsedPDB& ppdb) {
        int modelNum = 1;

        while (true) {
            auto find_it = std::find_if(ppdb.models.begin(),
                                        ppdb.models.end(),
                                        [modelNum](const std::shared_ptr<Model>& m_sptr) {
                                            return m_sptr->modelNumber == modelNum;
                                        });
            if (find_it == ppdb.models.end()) // modelNum is not already in use
                return modelNum;
            modelNum++;
        }
    }

    bool inflateShorterLineOrFail(ParsedPDB& ppdb, std::string& line, int lineNumber, unsigned int minimalNumCol, unsigned int targetNumColumn) {
        assert(minimalNumCol < targetNumColumn);

        if(line.length() >= targetNumColumn)
            return true;

        if(line.length() < minimalNumCol)
            return false;

        line.insert(line.begin(), targetNumColumn - line.length(), ' ');
        return true;
    }

    void trimAfterLastColumn(ParsedPDB& ppdb,std::string& line, int lineNumber, unsigned int numColumn) {
        // Preprocessing the line (column number adjustment)
        if (line.length() > numColumn) {
            // Too many columns
            std::string trimmed_modelLine = b::trim_right_copy(line);
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            if (trimmed_modelLine.length() <= numColumn) {
                rec->severity = OutOfBandSeverity::idiosyncrasy;
                rec->subtype = OutOfBandSubType::SupernumerarySpaceAfterLastColumn;
            } else {
                rec->severity = OutOfBandSeverity::error;
                rec->subtype = OutOfBandSubType::SupernumeraryContentAfterLastColumn;
            }
            rec->line = line;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);

            line = line.substr(0, numColumn);
        }
    }





}