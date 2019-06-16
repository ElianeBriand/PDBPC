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

#ifndef PDBPC_INTERNALUTILS_H
#define PDBPC_INTERNALUTILS_H

#include <optional>

#include <boost/filesystem.hpp>
#include <pdbpc/ParsedPDB.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


namespace b = boost;
namespace bfs = boost::filesystem;


namespace pdbpc {


    bool checkPDBPath(ParsedPDB& ppdb, bfs::path pdbPath);

    void checkForErrorsWarningsProblems(ParsedPDB& ppdb);

    void createPlaceholderModelAndChain(ParsedPDB& ppdb);

    int findNextFreeModelNumber(ParsedPDB& ppdb);

    bool inflateShorterLineOrFail(ParsedPDB& ppdb, std::string& line, int lineNumber, unsigned int minimalNumCol, unsigned int targetNumColumn);

    void trimAfterLastColumn(ParsedPDB& ppdb, std::string& line, int lineNumber, unsigned int numColumn);


    template<typename T>
    std::optional<T> parseOrRecoverableError(ParsedPDB& ppdb,
                                               const std::string& line,
                                               int lineNumber,
                                               const std::string& field, OutOfBandSubType subtypeIfFail) {
        std::string trimmedField =  b::trim_copy(field);
        T presumptiveResult = -1;
        try {
            presumptiveResult = b::lexical_cast<T>(trimmedField);
        }
        catch (const b::bad_lexical_cast& e) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = subtypeIfFail;
            rec->line = line + std::string(" | FIELD=") +  field;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
            return std::nullopt;
        }
        return presumptiveResult;
    }

    template<typename T>
    std::optional<T> parseOrUnrecoverableError(ParsedPDB& ppdb,
                                  const std::string& line,
                                  int lineNumber,
                                  const std::string& field, OutOfBandSubType subtypeIfFail) {
        std::string trimmedField =  b::trim_copy(field);
        T presumptiveResult = -1;
        try {
            presumptiveResult = b::lexical_cast<T>(trimmedField);
        }
        catch (const b::bad_lexical_cast& e) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = subtypeIfFail;
            rec->line = line + std::string(" | FIELD=") +  field;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
            return std::nullopt;
        }
        return presumptiveResult;
    }


    namespace {
        template<typename T>
        void checkNoEmptyOptional_internal(bool& noEmpty, std::optional<T> value) {

        }

        template<typename T, typename... Args>
        void checkNoEmptyOptional_internal(bool& noEmpty, std::optional<T> value, Args... args) {
            noEmpty = noEmpty && value.has_value();
            checkNoEmptyOptional_internal(noEmpty, args...);
        }
    }



    template<typename... Args>
    bool checkNoEmptyOptional(Args... values) {
        bool noEmpty = true;
        // Compile time loop to insert the constructor arguments in the vector
        checkNoEmptyOptional_internal(noEmpty, values...);

        return noEmpty;
    }

}


#endif //PDBPC_INTERNALUTILS_H
