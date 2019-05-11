//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_INTERNALUTILS_H
#define PDBPC_INTERNALUTILS_H

#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;


namespace pdbpc {


    void checkPDBPath(bfs::path pdbPath);

}


#endif //PDBPC_INTERNALUTILS_H
