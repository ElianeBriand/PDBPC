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

#include <pdbpc/Records/Model.h>


#include <iostream>

unsigned int pdbpc::Model::lastserialNumber = 0;

// Exclude printRecord from LCOV-based test coverage report
// LCOV_EXCL_START

void pdbpc::Model::printRecord() {
    std::cout << "" << std::endl;
    std::cout << " * Model #" << this->serialNumber << std::endl;
    std::cout << " |   Opening Line Number    : " << this->openingLineNumber << std::endl;
    std::cout << " |   Opening Line : " << std::endl;
    std::cout << " |   " << std::endl;
    std::cout << " |         " <<this->openingLine << std::endl;
    std::cout << " |   " << std::endl;
    std::cout << " |   Closing Line Number    : " << this->closingLineNumber << std::endl;
    std::cout << " |   Closing Line : " << std::endl;
    std::cout << " |   " << std::endl;
    std::cout << " |         " <<this->closingLine << std::endl;
    std::cout << " |   " << std::endl;
    std::cout << " *" << std::endl;
    std::cout << "" << std::endl;
}

// LCOV_EXCL_STOP
