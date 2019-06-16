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

#ifndef PDBPC_MODEL_H
#define PDBPC_MODEL_H

#include <string>
#include <vector>
#include <memory>

#include <pdbpc/Records/Atom.h>
#include "Chain.h"

namespace pdbpc {
    struct __attribute__ ((visibility ("default")))  Model {

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

        struct __attribute__ ((visibility ("default")))  Details {
            int originalModelNumber = -1;

        } details;

    };
}




#endif //PDBPC_MODEL_H
