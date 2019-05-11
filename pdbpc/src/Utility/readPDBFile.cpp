//
// Created by eliane on 11/05/19.
//

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

        checkPDBPath(pdbPath);

        ppdb.settings = settings;

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

        return ppdb;
    }

}