//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_MODEL_H
#define PDBPC_MODEL_H

#include <string>
#include <vector>
#include <memory>

#include <pdbpc/Records/Atom.h>
#include "Chain.h"

namespace pdbpc {
    struct Model {

        static unsigned int lastserialNumber;

        int serialNumber;

        int modelNumber;

        std::string openingLine;
        int openingLineNumber = -1;

        std::string closingLine;
        int closingLineNumber = -1;

        std::vector<std::shared_ptr<Chain>> chains;

        std::vector<std::shared_ptr<Atom>> atoms_flatlist;
        std::vector<std::shared_ptr<Residue>> residues_flatlist;

        void printRecord();

        struct Details {
            int originalModelNumber = -1;

        } details;

    };
}




#endif //PDBPC_MODEL_H
