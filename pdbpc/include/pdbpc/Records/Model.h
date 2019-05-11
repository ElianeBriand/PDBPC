//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_MODEL_H
#define PDBPC_MODEL_H

#include <string>
#include <vector>
#include <memory>

#include <pdbpc/Records/Atom.h>

namespace pdbpc {
    struct Model {

        int modelNumber;

        std::string openingLine;
        int openingLineNumber = -1;

        std::string closingLine;
        int closingLineNumber = -1;


        std::vector<std::shared_ptr<Atom>> atoms_flatlist;


        void printModelRecord();

    };
}




#endif //PDBPC_MODEL_H
