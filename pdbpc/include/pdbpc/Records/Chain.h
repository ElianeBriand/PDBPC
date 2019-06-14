//
// Created by eliane on 31/05/19.
//

#ifndef PDBPC_CHAIN_H
#define PDBPC_CHAIN_H

#include <string>
#include <vector>
#include <memory>

#include <pdbpc/Records/Atom.h>




namespace pdbpc {

    struct Model;

    struct Chain {
        int serialNumber;

        std::string chainIdentifier;

        std::string openingLine;
        int openingLineNumber = -1;

        std::string closingLine;
        int closingLineNumber = -1;

        std::weak_ptr<Model> parentModel;

        std::vector<std::shared_ptr<Residue>> residues;

        std::vector<std::shared_ptr<Atom>> atoms_flatlist;


        void printRecord();


    };

}
#endif //PDBPC_CHAIN_H
