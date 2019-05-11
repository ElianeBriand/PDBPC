//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_INTERNALUTILS_H
#define PDBPC_INTERNALUTILS_H

#include <boost/filesystem.hpp>
#include <pdbpc/ParsedPDB.h>

namespace bfs = boost::filesystem;


namespace pdbpc {


    void checkPDBPath(bfs::path pdbPath);

    void checkForErrorsWarningsProblems(ParsedPDB& ppdb);

    void createPlaceholderModelAndChain(ParsedPDB& ppdb);

    int findNextFreeModelNumber(ParsedPDB& ppdb);

    }


#endif //PDBPC_INTERNALUTILS_H
