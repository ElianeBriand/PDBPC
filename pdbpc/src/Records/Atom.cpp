//
// Created by eliane on 11/05/19.
//

#include "pdbpc/Records/Atom.h"

#include <boost/log/trivial.hpp>


namespace pdbpc {

    void Atom::printAtomRecord() {
        BOOST_LOG_TRIVIAL(info) << "";
        BOOST_LOG_TRIVIAL(info) << " * Atom ";
        BOOST_LOG_TRIVIAL(info) << " |   Chain           : " << this->chainIdentifier;
        BOOST_LOG_TRIVIAL(info) << " |   Serial number   : " << this->serialNumber;
        BOOST_LOG_TRIVIAL(info) << " |   ";
        BOOST_LOG_TRIVIAL(info) << " |   Alt. Loc. ID    : " << this->alternateLocationIdentifier;
        BOOST_LOG_TRIVIAL(info) << " |   ";
        BOOST_LOG_TRIVIAL(info) << " |   Residue serial  : " << this->residueSequenceNumber;
        BOOST_LOG_TRIVIAL(info) << " |   Residue code    : " << this->residueName;
        BOOST_LOG_TRIVIAL(info) << " |   Res. ins. code  : " << this->residueInsertionCode;
        BOOST_LOG_TRIVIAL(info) << " |   Residue name    : " << residueTypeToFullName(this->residueType);
        BOOST_LOG_TRIVIAL(info) << " |   ";
        BOOST_LOG_TRIVIAL(info) << " |   Atom name       : " << this->atomName;
        BOOST_LOG_TRIVIAL(info) << " |   Element         : " << this->element;
        BOOST_LOG_TRIVIAL(info) << " |   Position x      : " << this->x;
        BOOST_LOG_TRIVIAL(info) << " |   Position y      : " << this->y;
        BOOST_LOG_TRIVIAL(info) << " |   Position z      : " << this->z;
        BOOST_LOG_TRIVIAL(info) << " |   ";
        BOOST_LOG_TRIVIAL(info) << " |   Occupancy       : " << this->occupancy;
        BOOST_LOG_TRIVIAL(info) << " |   Temp. factor    : " << this->temperatureFactor;
        BOOST_LOG_TRIVIAL(info) << " |   Charge          : " << this->charge;
        BOOST_LOG_TRIVIAL(info) << " |   ";
        BOOST_LOG_TRIVIAL(info) << " |   Line Number    : " << this->lineNumber;
        BOOST_LOG_TRIVIAL(info) << " |   Line : ";
        BOOST_LOG_TRIVIAL(info) << " |   ";
        BOOST_LOG_TRIVIAL(info) << " |         " <<this->line;
        BOOST_LOG_TRIVIAL(info) << " |   ";
        BOOST_LOG_TRIVIAL(info) << " *";
        BOOST_LOG_TRIVIAL(info) << "";
    }
}


