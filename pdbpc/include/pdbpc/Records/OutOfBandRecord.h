//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_OUTOFBANDRECORD_H
#define PDBPC_OUTOFBANDRECORD_H

#include <string>

namespace pdbpc {

    enum class OutOfBandSeverity  {
        error,
        warning,
        idiosyncrasy,
        info
    };

    enum class OutOfBandType  {
        UnhandledPDBLine,
        IncorrectPDBLineFormat,
        IncorrectPDBFileStructure
    };

    enum class OutOfBandSubType  {
        NonRecognizedLinePrefix,
        SupernumerarySpaceAfterLastColumn,
        SupernumeraryContentAfterLastColumn,
        NonCompliantColumnWidth,

        // Model related errors
        unexpectedStringInsteadOfModelNumber,
        negativeModelNumber,
        zeroModelNumber,
        newModelLineWhilePreviousNotClosed,
        duplicateModelNumber,
        EndMdlWithoutOpeningModelStatement

    };

    enum class RecoveryStatus {
        recovered,
        unrecoverable
    };


    std::ostream& operator<<(std::ostream & os, const OutOfBandSeverity & severity);
    std::ostream& operator<<(std::ostream & os, const  OutOfBandType & type);
    std::ostream& operator<<(std::ostream & os, const OutOfBandSubType & subtype);
    std::ostream& operator<<(std::ostream & os, const RecoveryStatus & recoveryStatus);

    std::string OutOfBandRecord_SeverityToString(OutOfBandSeverity severity);
    std::string OutOfBandRecord_TypeToString(OutOfBandType type);
    std::string OutOfBandRecord_SubTypeToString(OutOfBandSubType subtype);
    std::string OutOfBandRecord_RecoveryStatusToString(RecoveryStatus recoveryStatus);

    struct OutOfBandRecord {

        static unsigned int lastOoBRecordId;

        OutOfBandRecord();

        unsigned int OoBRecordId;

        OutOfBandSeverity severity;
        OutOfBandType type;
        OutOfBandSubType subtype;

        int lineNumber;
        std::string line;

        RecoveryStatus recoveryStatus;

        void printRecord();
    };
}


#endif //PDBPC_OUTOFBANDRECORD_H
