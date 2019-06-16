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

#ifndef PDBPC_ATOM_H
#define PDBPC_ATOM_H

#include <string>
#include "Residue.h"

namespace pdbpc {

    struct Chain;
    struct Model;

    struct __attribute__ ((visibility ("default")))  Atom {

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

        std::weak_ptr<Model> parentModel;
        std::weak_ptr<Chain> parentChain;

        void printRecord();

    };
}



#endif //PDBPC_ATOM_H
