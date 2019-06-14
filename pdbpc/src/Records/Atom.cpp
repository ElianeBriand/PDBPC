//
// Created by eliane on 11/05/19.
//

#include "pdbpc/Records/Atom.h"

#include <boost/log/trivial.hpp>

#include <iostream>

namespace pdbpc {

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
}


