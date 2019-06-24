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


#include "pdbpc/Records/Heterogen.h"


#include <iostream>

namespace pdbpc {

    /*
        std::string hetID;

        std::string chainID;
        int sequenceNumber;
        std::string insertionCode;

        std::string description;

        int numHETATM;*/

    // Exclude printRecord from LCOV-based test coverage report
    // LCOV_EXCL_START
    void Heterogen::printRecord() {
        std::cout << "" << std::endl;
        std::cout << " * Heterogen " << std::endl;
        std::cout << " |   HetID           : " << this->hetID << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |   Chain           : " << this->chainID << std::endl;
        std::cout << " |   Sequence Num.   : " << this->sequenceNumber << std::endl;
        std::cout << " |   Insertion code  : " << this->insertionCode << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |   Description  : " << this->description << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " |   Num HETATM      : " << this->numHETATM << std::endl;
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