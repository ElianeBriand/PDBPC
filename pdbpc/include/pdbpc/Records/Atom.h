//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_ATOM_H
#define PDBPC_ATOM_H

#include <string>
#include "Residue.h"

namespace pdbpc {



    struct Atom {

        int serialNumber;

        std::string atomName;

        std::string alternateLocationIdentifier;

        ResidueType residueType;
        std::string residueName;

        std::string chainIdentifier;

        int residueSequenceNumber;

        std::string residueInsertionCode;

        double x,y,z;

        double occupancy;
        double temperatureFactor;

        std::string element;

        double charge;


        int lineNumber = -1;
        std::string line;


        void printAtomRecord();

    };
}



#endif //PDBPC_ATOM_H
