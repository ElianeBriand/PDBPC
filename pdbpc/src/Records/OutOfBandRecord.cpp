//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Records/OutOfBandRecord.h>

#include <map>

#include <boost/log/trivial.hpp>


namespace pdbpc {

    unsigned int OutOfBandRecord::lastOoBRecordId = 0;

    void OutOfBandRecord::printRecord() {
        BOOST_LOG_TRIVIAL(info) << "";
        BOOST_LOG_TRIVIAL(info) << " * Out of band record #" << this->OoBRecordId;
        BOOST_LOG_TRIVIAL(info) << " |   Severity     : " << OutOfBandRecord_SeverityToString(this->severity);
        BOOST_LOG_TRIVIAL(info) << " |   Type         : " << OutOfBandRecord_TypeToString(this->type);
        BOOST_LOG_TRIVIAL(info) << " |   Subtype      : " << OutOfBandRecord_SubTypeToString(this->subtype);
        BOOST_LOG_TRIVIAL(info) << " |   Recovery     : " << OutOfBandRecord_RecoveryStatusToString(this->recoveryStatus);
        BOOST_LOG_TRIVIAL(info) << " |   ";
        BOOST_LOG_TRIVIAL(info) << " |   Line number  : " << this->lineNumber;
        BOOST_LOG_TRIVIAL(info) << " |   Line : ";
        BOOST_LOG_TRIVIAL(info) << " |   ";
        BOOST_LOG_TRIVIAL(info) << " |         " <<this->line;
        BOOST_LOG_TRIVIAL(info) << " |   ";
        BOOST_LOG_TRIVIAL(info) << " *";
        BOOST_LOG_TRIVIAL(info) << "";
    }

    OutOfBandRecord::OutOfBandRecord() {
        this->OoBRecordId = (++lastOoBRecordId);
    }

    static std::map<OutOfBandSeverity,std::string> SeverityToString =
            {{OutOfBandSeverity::error,"Error"},
             {OutOfBandSeverity::warning,"Warning"},
             {OutOfBandSeverity::idiosyncrasy,"Idiosyncrasy"},
             {OutOfBandSeverity::info,"Info"},
             };
    static std::map<OutOfBandType,std::string> TypeToString =
            {{OutOfBandType::UnhandledPDBLine,"Unhandled PDB line"},
             {OutOfBandType::IncorrectPDBLineFormat,"Incorrect PDB line Format"},
             {OutOfBandType::IncorrectPDBFileStructure,"Incorrect PDB file structure"},
             {OutOfBandType::SurprisingStructure, "Line specifies structure which, while possible, is surprising"}
            };

    static std::map<OutOfBandSubType,std::string> SubTypeToString =
            {{OutOfBandSubType::NonRecognizedLinePrefix,"Non-recognized line prefix"},
             {OutOfBandSubType::SupernumerarySpaceAfterLastColumn,"Supernumerary space after last column"},
             {OutOfBandSubType::SupernumeraryContentAfterLastColumn,"Supernumerary content after last column"},
             {OutOfBandSubType::NonCompliantColumnWidth,"Non-compliant column width"},
             {OutOfBandSubType::unexpectedStringInsteadOfModelNumber,"Unexpected string instead of model number"},
             {OutOfBandSubType::negativeModelNumber,"Negative model number"},
             {OutOfBandSubType::zeroModelNumber,"Number model is zero"},
             {OutOfBandSubType::newModelLineWhilePreviousNotClosed,"new MODEL line encountered while previous model not closed"},
             {OutOfBandSubType::duplicateModelNumber,"Duplicate model number"},
             {OutOfBandSubType::EndMdlWithoutOpeningModelStatement,"ENDMDL line without previous MODEl line"},
             {OutOfBandSubType::AtomDegradedModeParsingEvent, "ATOM degraded mode parsing triggered"},
             {OutOfBandSubType::AtomIncompleteLine,"ATOM line too incomplete for even degraded mode parsing"},
             {OutOfBandSubType::AtomCannotParseSerialNumber, "ATOM Serial number could not be parsed"},
             {OutOfBandSubType::AtomUnknownResidueThreeLetterCode, "Residue three letter code is unknown"},
             {OutOfBandSubType::AtomNameNotKnownInSpecifiedResidue, "ATOM atom name not usually found in specified residue"},
             {OutOfBandSubType::AtomChainIDIsNotAlphanumerical, "ATOM chain identifier should be alphanumerical"},
             {OutOfBandSubType::AtomCannotParseResidueSeqId, "ATOM has unparseable residue sequence identifier"},
             {OutOfBandSubType::AtomResidueInsertionCodeIsNotALetter, "ATOM residue insertion code is not a letter"},
             {OutOfBandSubType::AtomCannotParseCoordinates, "ATOM coordinate could not be parsed"},
             {OutOfBandSubType::AtomCannotParseTempFactor, "Present but unparseable ATOM temperature factor"},
             {OutOfBandSubType::AtomCannotParseCharge, "Present but unparseable ATOM charge"},
             {OutOfBandSubType::AtomNoElementName, "ATOM element name is missing"},
             {OutOfBandSubType::AtomElementNameContainsNonLetter, "ATOM element name contains non-alphabetic characters"},
             {OutOfBandSubType::AtomAltLocIdContainsNonLetter,"ATOM alternate location identifier contains non-alphabetic characters"},
             {OutOfBandSubType::AtomMissingChainID, "ATOM has missing chain identifier"}
            };
    static std::map<RecoveryStatus,std::string> RecoveryStatusToString =
            {{RecoveryStatus::recovered,"Recovered"},
             {RecoveryStatus::unrecoverable,"Unrecoverable"}
            };
    std::string OutOfBandRecord_SeverityToString(OutOfBandSeverity severity) {
        auto recordIt = SeverityToString.find(severity);
        if(recordIt == SeverityToString.end())
        {
            // Not found
            return "???";
        }else {
            return recordIt->second;
        }
    }

    std::string OutOfBandRecord_TypeToString(OutOfBandType type) {
        auto recordIt = TypeToString.find(type);
        if(recordIt == TypeToString.end())
        {
            // Not found
            return "???";
        }else {
            return recordIt->second;
        }
    }

    std::string OutOfBandRecord_SubTypeToString(OutOfBandSubType subtype) {
        auto recordIt = SubTypeToString.find(subtype);
        if(recordIt == SubTypeToString.end())
        {
            // Not found
            return "???";
        }else {
            return recordIt->second;
        }
    }

    std::string OutOfBandRecord_RecoveryStatusToString(RecoveryStatus recoveryStatus) {
        auto recordIt = RecoveryStatusToString.find(recoveryStatus);
        if(recordIt == RecoveryStatusToString.end())
        {
            // Not found
            return "???";
        }else {
            return recordIt->second;
        }
    }

    std::ostream &operator<<(std::ostream &os,const  OutOfBandSeverity &severity) {
        os << OutOfBandRecord_SeverityToString(severity);
        return os;
    }

    std::ostream &operator<<(std::ostream &os,const  OutOfBandType &type) {
        os << OutOfBandRecord_TypeToString(type);

        return os;
    }

    std::ostream &operator<<(std::ostream &os, const OutOfBandSubType &subtype) {
        os << OutOfBandRecord_SubTypeToString(subtype);
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const RecoveryStatus &recoveryStatus) {
        os << OutOfBandRecord_RecoveryStatusToString(recoveryStatus);
        return os;
    }
}