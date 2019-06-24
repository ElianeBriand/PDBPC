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
#include <pdbpc/Records/OutOfBandRecord.h>

#include <map>


#include <iostream>


namespace pdbpc {

    unsigned int OutOfBandRecord::lastOoBRecordId = 0;

    // Exclude printRecord from LCOV-based test coverage report
    // LCOV_EXCL_START
    void OutOfBandRecord::printRecord() {
        std::cout << "" << std::endl;
        std::cout << " * Out of band record #" << this->OoBRecordId << std::endl;
        std::cout << " |   Severity     : " << OutOfBandRecord_SeverityToString(this->severity) << std::endl;
        std::cout << " |   Type         : " << OutOfBandRecord_TypeToString(this->type) << std::endl;
        std::cout << " |   Subtype      : " << OutOfBandRecord_SubTypeToString(this->subtype) << std::endl;
        std::cout << " |   Explanation  : " << OutOfBandRecord_SubTypeToExplainationString(this->subtype) << std::endl;
        std::cout << " |   Recovery     : " << OutOfBandRecord_RecoveryStatusToString(this->recoveryStatus)
                  << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |   Line number  : " << this->lineNumber << std::endl;
        std::cout << " |   Line : " << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |         " << this->line << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |   Details : " << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |         " << this->details << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " *" << std::endl;
        std::cout << "" << std::endl;
    }

    // LCOV_EXCL_STOP

    OutOfBandRecord::OutOfBandRecord() {
        this->OoBRecordId = (++lastOoBRecordId);
    }


    static std::map<OutOfBandSeverity, std::string> SeverityToString =
            {{OutOfBandSeverity::error,        "Error"},
             {OutOfBandSeverity::warning,      "Warning"},
             {OutOfBandSeverity::idiosyncrasy, "Idiosyncrasy"},
             {OutOfBandSeverity::info,         "Info"},
            };
    static std::map<OutOfBandType, std::string> TypeToString =
            {{OutOfBandType::FileAccessError,           "File access error"},
             {OutOfBandType::UnhandledPDBLine,          "Unhandled PDB line"},
             {OutOfBandType::IncorrectPDBLineFormat,    "Incorrect PDB line Format"},
             {OutOfBandType::IncorrectPDBFileStructure, "Incorrect PDB file structure"},
             {OutOfBandType::SurprisingStructure,       "Line specifies structure which, while possible, is surprising"}
            };

    static std::map<OutOfBandSubType, std::tuple<std::string, std::string>> SubTypeToString =
            {{OutOfBandSubType::FileNotFound,                                   std::tuple<std::string, std::string>(
                    "FileNotFound", "File not found")},
             {OutOfBandSubType::FilePathIsDirectory,                            std::tuple<std::string, std::string>(
                     "FilePathIsDirectory", "Given file path is actually a directory")},
             {OutOfBandSubType::FileExtensionNotPDB,                            std::tuple<std::string, std::string>(
                     "FileExtensionNotPDB", "File extension is not .pdb or .PDB")},
             {OutOfBandSubType::FileNotARegularFile,                            std::tuple<std::string, std::string>(
                     "FileNotARegularFile", "File is not a regular file (ie is a special system file)")},
             {OutOfBandSubType::NonRecognizedLinePrefix,                        std::tuple<std::string, std::string>(
                     "NonRecognizedLinePrefix", "Beginning of line not recognised as valid PDB line")},
             {OutOfBandSubType::SupernumerarySpaceAfterLastColumn,              std::tuple<std::string, std::string>(
                     "SupernumerarySpaceAfterLastColumn", "Additional blank characters after last column")},
             {OutOfBandSubType::SupernumeraryContentAfterLastColumn,            std::tuple<std::string, std::string>(
                     "SupernumeraryContentAfterLastColumn", "Additional content after last line column (ignored)")},
             {OutOfBandSubType::NonCompliantColumnWidth,                        std::tuple<std::string, std::string>(
                     "NonCompliantColumnWidth", "Non-compliant column width")},
             {OutOfBandSubType::unexpectedStringInsteadOfModelNumber,           std::tuple<std::string, std::string>(
                     "unexpectedStringInsteadOfModelNumber",
                     "Unexpected string instead of model number in MODEL record")},
             {OutOfBandSubType::negativeModelNumber,                            std::tuple<std::string, std::string>(
                     "negativeModelNumber", "Model number is negative")},
             {OutOfBandSubType::zeroModelNumber,                                std::tuple<std::string, std::string>(
                     "zeroModelNumber", "Model number is zero")},
             {OutOfBandSubType::newModelLineWhilePreviousNotClosed,             std::tuple<std::string, std::string>(
                     "newModelLineWhilePreviousNotClosed",
                     "A new MODEL line was encountered but the previous model was not closed")},
             {OutOfBandSubType::duplicateModelNumber,                           std::tuple<std::string, std::string>(
                     "duplicateModelNumber", "Multiple model with the same number were found")},
             {OutOfBandSubType::EndMdlWithoutOpeningModelStatement,             std::tuple<std::string, std::string>(
                     "EndMdlWithoutOpeningModelStatement", "ENDMDL line without opening MODEl line")},
             {OutOfBandSubType::EndMdlModelDoubleClose,                         std::tuple<std::string, std::string>(
                     "EndMdlModelDoubleClose", "Two ENDMDL lines without intervening MODEL line")},
             {OutOfBandSubType::AtomDegradedModeParsingEvent,                   std::tuple<std::string, std::string>(
                     "AtomDegradedModeParsingEvent",
                     "Malformed yet partially recovered ATOM line encountered")},
             {OutOfBandSubType::AtomIncompleteLine,                             std::tuple<std::string, std::string>(
                     "AtomIncompleteLine",
                     "Malformed ATOM line too incomplete to parse")},
             {OutOfBandSubType::AtomCannotParseSerialNumber,                    std::tuple<std::string, std::string>(
                     "AtomCannotParseSerialNumber", "ATOM serial number could not be parsed to integer")},
             {OutOfBandSubType::AtomUnknownResidueThreeLetterCode,              std::tuple<std::string, std::string>(
                     "AtomUnknownResidueThreeLetterCode",
                     "Residue three letter code is unknown, possibly non-standard residue")},
             {OutOfBandSubType::AtomNameNotKnownInSpecifiedResidue,             std::tuple<std::string, std::string>(
                     "AtomNameNotKnownInSpecifiedResidue",
                     "An ATOM not usually found in the current residue was found")},
             {OutOfBandSubType::NonAlphabeticChainID,                           std::tuple<std::string, std::string>(
                     "NonAlphabeticChainID", "Chain identifier should be alphabetic")},
             {OutOfBandSubType::AtomCannotParseResidueSeqId,                    std::tuple<std::string, std::string>(
                     "AtomCannotParseResidueSeqId", "ATOM has residue sequence identifier not parsable to integer")},
             {OutOfBandSubType::NonAlphabeticInsertionCode,                     std::tuple<std::string, std::string>(
                     "NonAlphabeticInsertionCode", "Insertion code should be a letter")},
             {OutOfBandSubType::AtomCannotParseCoordinates,                     std::tuple<std::string, std::string>(
                     "AtomCannotParseCoordinates", "ATOM coordinate could not be parsed to float")},
             {OutOfBandSubType::AtomCannotParseOccupancy,                       std::tuple<std::string, std::string>(
                     "AtomCannotParseOccupancy", "Present but unparseable ATOM occupancy factor")},
             {OutOfBandSubType::AtomAltLocButNoOccupancy,                       std::tuple<std::string, std::string>(
                     "AtomAltLocButNoOccupancy",
                     "Alternate location indicator were used without occupancy factor. Depending on the AltLocPolicy used,"
                     " suprising selection location may occur.")},
             {OutOfBandSubType::AtomCannotParseTempFactor,                      std::tuple<std::string, std::string>(
                     "AtomCannotParseTempFactor", "Present but unparseable ATOM temperature factor")},
             {OutOfBandSubType::AtomCannotParseCharge,                          std::tuple<std::string, std::string>(
                     "AtomCannotParseCharge", "Present but unparseable ATOM charge")},
             {OutOfBandSubType::AtomNoElementName,                              std::tuple<std::string, std::string>(
                     "AtomNoElementName", "ATOM element name is missing")},
             {OutOfBandSubType::AtomElementNameContainsNonLetter,               std::tuple<std::string, std::string>(
                     "AtomElementNameContainsNonLetter", "ATOM element name contains non-alphabetic characters")},
             {OutOfBandSubType::NonAlphabeticAltLocId,                          std::tuple<std::string, std::string>(
                     "NonAlphabeticAltLocId",
                     "Alternate location identifier contains non-alphabetic characters")},
             {OutOfBandSubType::ResidueInconsistentInsertionCode,               std::tuple<std::string, std::string>(
                     "ResidueInconsistentInsertionCode",
                     "Residue has same sequence identifier but different insertion code, in the same chain")},
             {OutOfBandSubType::ResidueInconsistentResidueName,                 std::tuple<std::string, std::string>(
                     "ResidueInconsistentResidueName",
                     "Residue has same sequence identifier but different residue name, in the same chain")},
             {OutOfBandSubType::ResidueInconsistentInsertionCodeAndResidueName, std::tuple<std::string, std::string>(
                     "ResidueInconsistentInsertionCodeAndResidueName",
                     "Residue has same sequence identifier but different residue name and insertion code, in the same chain")},
             {OutOfBandSubType::FileContinuePastEndRecord,                      std::tuple<std::string, std::string>(
                     "FileContinuePastEndRecord",
                     "File continue past the END record.")},
             {OutOfBandSubType::MasterRecordFieldParseError,                    std::tuple<std::string, std::string>(
                     "MasterRecordFieldParseError",
                     "One or several field from the MASTER record were not parsed to unsigned int")},
             {OutOfBandSubType::MasterRecordCheckIncomplete,                    std::tuple<std::string, std::string>(
                     "MasterRecordCheckIncomplete",
                     "Unparsed fields in the MASTER record only allow for partial checking of the file structure")},
             {OutOfBandSubType::MasterRecordTooShort,
                                                                                std::tuple<std::string, std::string>(
                                                                                        "MasterRecordTooShort",
                                                                                        "The MASTER line is too short")},
             {OutOfBandSubType::GenericFieldParseError,
                                                                                std::tuple<std::string, std::string>(
                                                                                        "GenericFieldParseError",
                                                                                        "Unspecified error while parsing a field")},
             {OutOfBandSubType::MissingChainID,                                 std::tuple<std::string, std::string>(
                     "MissingChainID", "Expected chain ID but got blank/unreadable field")},
             {OutOfBandSubType::MissingSequenceNumber,                          std::tuple<std::string, std::string>(
                     "MissingSequenceNumber", "Expected sequence number, got blank/unreadable field")},
             {OutOfBandSubType::MissingHETATMCountInHETRecord,                  std::tuple<std::string, std::string>(
                     "MissingHETATMCountInHETRecord",
                     "HET record does not include a HETATM count field (or unreadable field)")},
             {OutOfBandSubType::IncompleteHETrecord,                            std::tuple<std::string, std::string>(
                     "IncompleteHETrecord", "HET line does not contain all mandatory fields")},
             {OutOfBandSubType::MissingHetID,                                   std::tuple<std::string, std::string>(
                     "MissingHetID", "Missing or unreadable HetID field (three-letter code for heterogen) in record")},
             {OutOfBandSubType::HetIDReferencedButNotDefined,                   std::tuple<std::string, std::string>(
                     "HetIDReferencedButNotDefined",
                     "Record is referencing a HetID (three-letter code for heterogens), but no HET line defining it were parsed before")},
             {OutOfBandSubType::CannotParseComponentNumber,                     std::tuple<std::string, std::string>(
                     "CannotParseComponentNumber",
                     "Expected a component number field, but field is empty or unreadable")}
            };
    static std::map<RecoveryStatus, std::string> RecoveryStatusToString =
            {{RecoveryStatus::recovered,     "Recovered"},
             {RecoveryStatus::unrecoverable, "Unrecoverable"}
            };

    std::string OutOfBandRecord_SeverityToString(OutOfBandSeverity severity) {
        auto recordIt = SeverityToString.find(severity);
        if (recordIt == SeverityToString.end()) {
            // Not found
            return "???";
        } else {
            return recordIt->second;
        }
    }

    std::string OutOfBandRecord_TypeToString(OutOfBandType type) {
        auto recordIt = TypeToString.find(type);
        if (recordIt == TypeToString.end()) {
            // Not found
            return "???";
        } else {
            return recordIt->second;
        }
    }

    std::string OutOfBandRecord_SubTypeToString(OutOfBandSubType subtype) {
        auto recordIt = SubTypeToString.find(subtype);
        if (recordIt == SubTypeToString.end()) {
            // Not found
            return "???";
        } else {
            return std::get<0>(recordIt->second);
        }
    }

    std::string OutOfBandRecord_SubTypeToExplainationString(OutOfBandSubType subtype) {
        auto recordIt = SubTypeToString.find(subtype);
        if (recordIt == SubTypeToString.end()) {
            // Not found
            return "???";
        } else {
            return std::get<1>(recordIt->second);
        }
    }

    std::string OutOfBandRecord_RecoveryStatusToString(RecoveryStatus recoveryStatus) {
        auto recordIt = RecoveryStatusToString.find(recoveryStatus);
        if (recordIt == RecoveryStatusToString.end()) {
            // Not found
            return "???";
        } else {
            return recordIt->second;
        }
    }

    std::ostream& operator<<(std::ostream& os, const OutOfBandSeverity& severity) {
        os << OutOfBandRecord_SeverityToString(severity);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const OutOfBandType& type) {
        os << OutOfBandRecord_TypeToString(type);

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const OutOfBandSubType& subtype) {
        os << OutOfBandRecord_SubTypeToString(subtype);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const RecoveryStatus& recoveryStatus) {
        os << OutOfBandRecord_RecoveryStatusToString(recoveryStatus);
        return os;
    }
}