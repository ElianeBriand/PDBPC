//
// Created by eliane on 31/05/19.
//

#include <sstream>

#include <pdbpc/Utility/readPDBFile.h>
#include <pdbpc/Utility/internalUtils.h>

#include <pdbpc/Parser/readPDBLine.h>
#include <pdbpc/Parser/PostProcessing/postProcessParsedPDB.h>
#include <pdbpc/Parser/PostProcessing/pedanticVerifications.h>


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