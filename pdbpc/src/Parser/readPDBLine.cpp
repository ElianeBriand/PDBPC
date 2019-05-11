//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Parser/readPDBLine.h>

#include <pdbpc/Parser/parseModelLine.h>
#include <pdbpc/Parser/parseEndModelLine.h>

#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>


namespace b = boost;

namespace pdbpc {

    bool readPDBLine(ParsedPDB& ppdb, const std::string& line, int lineNumber) {

        if(b::starts_with(line, "MODEL ")) {

            parseModelLine(ppdb,line,lineNumber);

        }else if (b::starts_with(line, "ENDMDL")) {

            parseEndModelLine(ppdb,line,lineNumber);

        }else if (b::starts_with(line, "ATOM")) {

        }else if (b::starts_with(line, "HETATM")) {

        }else {
            // Line type unsupported ?
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::UnhandledPDBLine;
            rec->subtype = OutOfBandSubType::NonRecognizedLinePrefix;
            rec->line = line;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
        }


        return true;

    }

}