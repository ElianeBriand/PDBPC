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

#include <sstream>

#include <pdbpc/Utility/readPDBFile.h>
#include <Utility/internalUtils.h>

#include <Parser/readPDBLine.h>
#include <Parser/PostProcessing/postProcessParsedPDB.h>
#include <Parser/PostProcessing/pedanticVerifications.h>


namespace pdbpc {


    ParsedPDB readPDBBlock(const std::string& PDBBlock, ParserSettings settings = ParserSettings()) {
        ParsedPDB ppdb;


        ppdb.settings = settings;

        createPlaceholderModelAndChain(ppdb);

        std::stringstream stringStream(PDBBlock);
        std::string line;
        int lineNumber = 1;
        while(getline(stringStream, line)){
            readPDBLine(ppdb,line, lineNumber);
            lineNumber++;
        }

        ppdb.numberOfParsedLines = lineNumber;


        postProcessParsedPDB(ppdb);

        if(settings.doPedanticVerifications)
            doPedanticVerifications(ppdb);

        checkForErrorsWarningsProblems(ppdb);

        return ppdb;
    }

}