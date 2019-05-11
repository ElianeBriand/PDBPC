//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Records/Model.h>

#include <boost/log/trivial.hpp>


void pdbpc::Model::printModelRecord() {
    BOOST_LOG_TRIVIAL(info) << "";
    BOOST_LOG_TRIVIAL(info) << " * Model #" << this->modelNumber;
    BOOST_LOG_TRIVIAL(info) << " |   Opening Line Number    : " << this->openingLineNumber;
    BOOST_LOG_TRIVIAL(info) << " |   Opening Line : ";
    BOOST_LOG_TRIVIAL(info) << " |   ";
    BOOST_LOG_TRIVIAL(info) << " |         " <<this->openingLine;
    BOOST_LOG_TRIVIAL(info) << " |   ";
    BOOST_LOG_TRIVIAL(info) << " |   Closing Line Number    : " << this->closingLineNumber;
    BOOST_LOG_TRIVIAL(info) << " |   Closing Line : ";
    BOOST_LOG_TRIVIAL(info) << " |   ";
    BOOST_LOG_TRIVIAL(info) << " |         " <<this->closingLine;
    BOOST_LOG_TRIVIAL(info) << " |   ";
    BOOST_LOG_TRIVIAL(info) << " *";
    BOOST_LOG_TRIVIAL(info) << "";
}
