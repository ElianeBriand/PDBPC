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

#include "pdbpc/Records/Chain.h"

#include <boost/log/trivial.hpp>

#include <iostream>

namespace pdbpc {

    void Chain::printRecord() {
        std::cout << "" << std::endl;
        std::cout << " * Chain " << this->serialNumber << std::endl;
        std::cout << " |   Chain identifier   : " << this->chainIdentifier << std::endl;
        std::cout << " |   Number of residue  : " << this->residues.size() << std::endl;
        std::cout << " |   Number of atom     : " << this->atoms_flatlist.size() << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " *" << std::endl;
        std::cout << "" << std::endl;
    }
}