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

#ifndef PDBPC_OUTOFBANDRECORD_H
#define PDBPC_OUTOFBANDRECORD_H

#include <string>

namespace pdbpc {

    enum class __attribute__ ((visibility ("default")))  OutOfBandSeverity  {
        error,
        warning,
        idiosyncrasy,
        info
    };

    enum class __attribute__ ((visibility ("default")))  OutOfBandType  {
        FileAccessError,
        UnhandledPDBLine,
        IncorrectPDBLineFormat,
        IncorrectPDBFileStructure,
        SurprisingStructure // Does not imply incorrect, but should probably be checked
    };

    enum class __attribute__ ((visibility ("default")))  OutOfBandSubType  {
        //File access error
        FileNotFound,
        FilePathIsDirectory,
        FileExtensionNotPDB,
        FileNotARegularFile,


        //Generic errors
        GenericFieldParseError,
        NonRecognizedLinePrefix,
        SupernumerarySpaceAfterLastColumn,
        SupernumeraryContentAfterLastColumn,
        NonCompliantColumnWidth,
        MissingChainID,
        NonAlphabeticChainID,
        MissingSequenceNumber,
        NonAlphabeticInsertionCode,
        NonAlphabeticAltLocId,
        CannotParseComponentNumber,

        // Model related errors
        unexpectedStringInsteadOfModelNumber,
        negativeModelNumber,
        zeroModelNumber,
        newModelLineWhilePreviousNotClosed,
        duplicateModelNumber,
        EndMdlWithoutOpeningModelStatement,
        EndMdlModelDoubleClose,

        // Atom related errors
        AtomDegradedModeParsingEvent,
        AtomIncompleteLine,
        AtomCannotParseSerialNumber,
        AtomUnknownResidueThreeLetterCode,
        AtomNameNotKnownInSpecifiedResidue,
        AtomCannotParseResidueSeqId,
        AtomCannotParseCoordinates,
        AtomCannotParseOccupancy,
        AtomCannotParseTempFactor,
        AtomCannotParseCharge,
        AtomNoElementName,
        AtomElementNameContainsNonLetter,
        AtomAltLocButNoOccupancy,

        //Residue Related error
        ResidueInconsistentInsertionCode,
        ResidueInconsistentResidueName,
        ResidueInconsistentInsertionCodeAndResidueName,

        // HET related records
        MissingHETATMCountInHETRecord,
        IncompleteHETrecord,
        MissingHetID,
        HetIDReferencedButNotDefined,

        // MASTER RECORD error
        MasterRecordFieldParseError,
        MasterRecordCheckIncomplete,
        MasterRecordTooShort,

        // END record error
        FileContinuePastEndRecord

    };

    enum class __attribute__ ((visibility ("default")))  RecoveryStatus {
        recovered,
        unrecoverable
    };


    __attribute__ ((visibility ("default"))) std::ostream& operator<<(std::ostream & os, const OutOfBandSeverity & severity);
    __attribute__ ((visibility ("default"))) std::ostream& operator<<(std::ostream & os, const  OutOfBandType & type);
    __attribute__ ((visibility ("default"))) std::ostream& operator<<(std::ostream & os, const OutOfBandSubType & subtype);
    __attribute__ ((visibility ("default"))) std::ostream& operator<<(std::ostream & os, const RecoveryStatus & recoveryStatus);

    __attribute__ ((visibility ("default"))) std::string OutOfBandRecord_SeverityToString(OutOfBandSeverity severity);
    __attribute__ ((visibility ("default"))) std::string OutOfBandRecord_TypeToString(OutOfBandType type);
    __attribute__ ((visibility ("default"))) std::string OutOfBandRecord_SubTypeToString(OutOfBandSubType subtype);
    __attribute__ ((visibility ("default"))) std::string OutOfBandRecord_SubTypeToExplainationString(OutOfBandSubType subtype);
    __attribute__ ((visibility ("default"))) std::string OutOfBandRecord_RecoveryStatusToString(RecoveryStatus recoveryStatus);

    struct __attribute__ ((visibility ("default")))  OutOfBandRecord {

        static unsigned int lastOoBRecordId;

        OutOfBandRecord();

        unsigned int OoBRecordId;

        OutOfBandSeverity severity;
        OutOfBandType type;
        OutOfBandSubType subtype;

        int lineNumber;
        std::string line;
        std::string details;

        RecoveryStatus recoveryStatus;

        void printRecord();
    };
}


#endif //PDBPC_OUTOFBANDRECORD_H
