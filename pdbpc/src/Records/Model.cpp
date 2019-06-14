//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Records/Model.h>

#include <boost/log/trivial.hpp>

#include <iostream>

unsigned int pdbpc::Model::lastserialNumber = 0;

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
