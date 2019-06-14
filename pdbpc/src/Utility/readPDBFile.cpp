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

#include <pdbpc/Utility/readPDBFile.h>
#include <pdbpc/Utility/internalUtils.h>

#include <pdbpc/Parser/readPDBLine.h>
#include <pdbpc/Parser/PostProcessing/postProcessParsedPDB.h>
#include <pdbpc/Parser/PostProcessing/pedanticVerifications.h>

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

namespace bfs = boost::filesystem;

namespace pdbpc {


    ParsedPDB readPDBFile(const std::string& filepath, ParserSettings settings) {
        ParsedPDB ppdb;

        bfs::path pdbPath(filepath);

        if(!checkPDBPath(ppdb,pdbPath))
        {
            ppdb.hasErrors = true;
            return ppdb;
        }

        ppdb.settings = settings;

        createPlaceholderModelAndChain(ppdb);

        bfs::ifstream fileStream(pdbPath);
        std::string line;
        int lineNumber = 1;
        while(getline(fileStream, line)){
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