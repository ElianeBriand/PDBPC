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

#include "pdbpc/Records/Atom.h"

#include <boost/log/trivial.hpp>

#include <iostream>

namespace pdbpc {

    // Exclude printRecord from LCOV-based test coverage report
    // LCOV_EXCL_START
    void Atom::printRecord() {
        std::cout << "" << std::endl;
        std::cout << " * Atom " << this->serialNumber << std::endl;
        std::cout << " |   Chain           : " << this->chainIdentifier << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |   Alt. Loc. ID    : " << this->alternateLocationIdentifier << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |   Residue serial  : " << this->residueSequenceNumber << std::endl;
        std::cout << " |   Residue code    : " << this->residueName << std::endl;
        std::cout << " |   Res. ins. code  : " << this->residueInsertionCode << std::endl;
        std::cout << " |   Residue name    : " << residueTypeToFullName(this->residueType) << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |   Atom name       : " << this->atomName << std::endl;
        std::cout << " |   Element         : " << this->element << std::endl;
        std::cout << " |   Position x      : " << this->x << std::endl;
        std::cout << " |   Position y      : " << this->y << std::endl;
        std::cout << " |   Position z      : " << this->z << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |   Occupancy       : " << this->occupancy << std::endl;
        std::cout << " |   Temp. factor    : " << this->temperatureFactor << std::endl;
        std::cout << " |   Charge          : " << this->charge << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |   Line Number    : " << this->lineNumber << std::endl;
        std::cout << " |   Line : " << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |         " <<this->line << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " *" << std::endl;
        std::cout << "" << std::endl;
    }

    // LCOV_EXCL_STOP
}


