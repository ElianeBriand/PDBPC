//
// Created by eliane on 31/05/19.
//

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