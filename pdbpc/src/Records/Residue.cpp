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
#include <pdbpc/Records/Residue.h>



#include <set>
#include <string>
#include <map>
#include <tuple>
#include <algorithm>
#include <iostream>

namespace pdbpc {


    unsigned int Residue::lastserialNumber = 0;

    std::set<std::tuple<ResidueType, std::string, std::string> > residueShorthandSet = {
                {ResidueType::heteroatom,     "heteroatom",     "HET"},
                {ResidueType::unknown,        "unknown",        "UNK"},
                {ResidueType::alanine,        "alanine",        "ALA"},
                {ResidueType::arginine,       "arginine",       "ARG"},
                {ResidueType::asparagine,     "asparagine",     "ASN"},
                {ResidueType::aspartate,      "aspartate",      "ASP"},
                {ResidueType::cysteine,       "cysteine",       "CYS"},
                {ResidueType::glutamate,      "glutamate",      "GLU"},
                {ResidueType::glutamine,      "glutamine",      "GLN"},
                {ResidueType::glycine,        "glycine",        "GLY"},
                {ResidueType::histidine,      "histidine",      "HIS"},
                {ResidueType::isoleucine,     "isoleucine",     "ILE"},
                {ResidueType::leucine,        "leucine",        "LEU"},
                {ResidueType::lysine,         "lysine",         "LYS"},
                {ResidueType::methionine,     "methionine",     "MET"},
                {ResidueType::phenylalanine,  "phenylalanine",  "PHE"},
                {ResidueType::proline,        "proline",        "PRO"},
                {ResidueType::serine,         "serine",         "SER"},
                {ResidueType::threonine,      "threonine",      "THR"},
                {ResidueType::tryptophan,     "tryptophan",     "TRP"},
                {ResidueType::tyrosine,       "tyrosine",       "TYR"},
                {ResidueType::valine,         "valine",         "VAL"},
                {ResidueType::selenocysteine, "selenocysteine", "SEC"},
                {ResidueType::pyrrolysine,    "pyrrolysine",    "PYL"}
        };

    std::map<ResidueType, std::map<std::string, double> > residueAtomChargeLookupTable = {
                {ResidueType::alanine,       {{"CA", 0.033700},
                                                           {"C", 0.597300},
                                                           {"N", -0.415700},
                                                           {"O", -0.567900},
                                                           {"OXT", -0.805500},
                                                           {"CB", -0.182500}}},
                {ResidueType::arginine,      {{"CA", -0.263700},
                                                           {"C", 0.734100},
                                                           {"N", -0.347900},
                                                           {"O", -0.589400},
                                                           {"OXT", -0.826600},
                                                           {"CB", -0.000700},
                                                           {"CG", 0.039000},
                                                           {"CD", 0.048600},
                                                           {"NE", 0.345600},
                                                           {"CZ", 0.807600},
                                                           {"NH1", -0.862700},
                                                           {"NH2", -0.862700}}},
                {ResidueType::asparagine,    {{"CA", 0.01430},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.81470},
                                                           {"CB", -0.20410},
                                                           {"CG", 0.71300},
                                                           {"OD1", -0.59310},
                                                           {"ND2", -0.91910}}},
                {ResidueType::aspartate,     {{"CA", 0.03810},
                                                           {"C", 0.53660},
                                                           {"N", -0.51630},
                                                           {"O", -0.58190},
                                                           {"OXT", -0.78870},
                                                           {"CB", -0.03030},
                                                           {"CG", 0.79940},
                                                           {"OD1", -0.80140},
                                                           {"OD2", -0.80140}}},
                {ResidueType::cysteine,      {{"CA", 0.02130},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.79810},
                                                           {"CB", -0.12310},
                                                           {"SG", -0.31190}}},
                {ResidueType::glutamate,     {{"CA", 0.03970},
                                                           {"C", 0.53660},
                                                           {"N", -0.51630},
                                                           {"O", -0.58190},
                                                           {"OXT", -0.79300},
                                                           {"CB", 0.05600},
                                                           {"CG", 0.01360},
                                                           {"CD", 0.80540},
                                                           {"OE1", -0.81880},
                                                           {"OE2", -0.81880}}},
                {ResidueType::glutamine,     {{"CA", -0.00310},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.80420},
                                                           {"CB", -0.00360},
                                                           {"CG", -0.06450},
                                                           {"CD", 0.69510},
                                                           {"OE1", -0.60860},
                                                           {"NE2", -0.94070}}},
                {ResidueType::glycine,       {{"CA", -0.02520},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.7855}}},
                {ResidueType::histidine,     {{"CA", 0.01880},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.80160},
                                                           {"CB", -0.04620},
                                                           {"CG", -0.02660},
                                                           {"ND1", -0.38110},
                                                           {"CD2", 0.12920},
                                                           {"CE1", 0.20570},
                                                           {"NE2",  -0.57270}}},
                {ResidueType::isoleucine,    {{"CA", -0.05970},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.81900},
                                                           {"CB", 0.13030},
                                                           {"CG1", -0.04300},
                                                           {"CG2", -0.32040},
                                                           {"CD1", -0.06600}}},
                {ResidueType::leucine,       {{"CA", -0.05180},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.81990},
                                                           {"CB", -0.11020},
                                                           {"CG", 0.35310},
                                                           {"CD1", -0.41210},
                                                           {"CD2", -0.41210}}},
                {ResidueType::lysine,        {{"CA", -0.24000},
                                                           {"C", 0.73410},
                                                           {"N", -0.34790},
                                                           {"O", -0.58940},
                                                           {"OXT", -0.82520},
                                                           {"CB",  -0.00940},
                                                           {"CG", 0.01870},
                                                           {"CD", -0.04790},
                                                           {"CE", -0.01430},
                                                           {"NZ", -0.38540}}},
                {ResidueType::methionine,    {{"CA", -0.02370},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.41570},
                                                           {"OXT", -0.81050},
                                                           {"CB", 0.03420},
                                                           {"CG", 0.00180},
                                                           {"SD", -0.27370},
                                                           {"CE", -0.05360}}},
                {ResidueType::phenylalanine, {{"CA", -0.00240},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.80260},
                                                           {"CB", -0.03430},
                                                           {"CG", 0.01180},
                                                           {"CD1", -0.12560},
                                                           {"CD2", -0.12560},
                                                           {"CE1", -0.17040},
                                                           {"CE2", -0.17040},
                                                           {"CZ", -0.10720}}},
                {ResidueType::proline,       {{"CA", -0.02660},
                                                           {"C", 0.58960},
                                                           {"N", -0.25480},
                                                           {"O", -0.57480},
                                                           {"OXT", -0.76970},
                                                           {"CB", -0.00700},
                                                           {"CG", 0.01890},
                                                           {"CD", 0.01920}}},
                {ResidueType::serine,        {{"CA", -0.02490},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.81320},
                                                           {"CB", 0.21170},
                                                           {"OG", -0.65460}}},
                {ResidueType::threonine,     {{"CA", -0.03890},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.80440},
                                                           {"CB", 0.36540},
                                                           {"OG1", -0.67610},
                                                           {"CG2", -0.24380}}},
                {ResidueType::tryptophan,    {{"CA", -0.02750},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O",  -0.56790},
                                                           {"OXT", -0.80110},
                                                           {"CB", -0.00500},
                                                           {"CG", -0.14150},
                                                           {"CD1", -0.16380},
                                                           {"CD2", 0.12430},
                                                           {"NE1", -0.34180},
                                                           {"CE2", 0.13800},
                                                           {"CE3", -0.23870},
                                                           {"CZ2", -0.26010},
                                                           {"CZ3", -0.19720},
                                                           {"CH2", -0.11340}}},
                {ResidueType::tyrosine,      {{"CA", -0.00140},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.80700},
                                                           {"CB", -0.01520},
                                                           {"CG", -0.00110},
                                                           {"CD1", -0.19060},
                                                           {"CD2", -0.19060},
                                                           {"CE1", -0.23410},
                                                           {"CE2", -0.23410},
                                                           {"CZ", 0.32260},
                                                           {"OH", -0.55790}}},
                {ResidueType::valine,        {{"CA", -0.08750},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.81730},
                                                           {"CB", 0.29850},
                                                           {"CG1", -0.31920},
                                                           {"CG2", -0.31920},}},
                {ResidueType::selenocysteine,
                                                   {{"CA", 0.02130},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.56790},
                                                           {"OXT", -0.79810},
                                                           {"CB", -0.12310},
                                                           {"SEG", -0.31190}}}, // TODO: add apropriate charges (currently sec=cys)
                {ResidueType::pyrrolysine,   {{"CA", 0.02130},
                                                           {"C", 0.59730},
                                                           {"N", -0.41570},
                                                           {"O", -0.5679},
                                                           {"OXT", -0.79810}}}
        };
    




    void Residue::printRecord() {
        std::cout << "" << std::endl;
        std::cout << " * Residue " << this->serialNumber << std::endl;
        std::cout << " |   Name              : " << this->name << std::endl;
        std::cout << " |   Type              : " << this->type << std::endl;
        std::cout << " |   Sequence number   : " << this->sequenceNumber << std::endl;
        std::cout << " |   Insertion code    : " << this->insertionCode << std::endl;
        std::cout << " |   Number of atom    : " << this->atoms.size() << std::endl;
        std::cout << " |   " << std::endl;
        std::cout << " *" << std::endl;
        std::cout << "" << std::endl;
    }

    std::string residueTypeToThreeLetterCode(ResidueType t) {
        auto it = std::find_if(residueShorthandSet.begin(), residueShorthandSet.end(),
                               [&](const std::tuple<ResidueType, std::string, std::string> &e) {
                                   return std::get<0>(e) == t;
                               });
        if (it != residueShorthandSet.end()) {
            return std::get<2>(*it);
        } else {
            return "???";
        }
    }

    ResidueType ThreeLetterCodeToResidueType(const std::string &shorthand) {
        auto it = std::find_if(residueShorthandSet.begin(), residueShorthandSet.end(),
                               [&](const std::tuple<ResidueType, std::string, std::string> &e) {
                                   return (std::get<2>(e) == shorthand);
                               });
        if (it != residueShorthandSet.end()) {
            return std::get<0>(*it);
        } else {
            return ResidueType::unknown;
        }
    }

    std::string residueTypeToFullName(ResidueType t) {
        auto it = std::find_if(residueShorthandSet.begin(), residueShorthandSet.end(),
                               [&](const std::tuple<ResidueType, std::string, std::string> &e) {
                                   return std::get<0>(e) == t;
                               });
        if (it != residueShorthandSet.end()) {
            return std::get<1>(*it);
        } else {
            return "???";
        }
    }

    std::ostream& operator<<(std::ostream& os, const ResidueType& restype) {
        os << residueTypeToFullName(restype) + std::string(" (") + residueTypeToThreeLetterCode(restype) + std::string(") ");
        return os;
    }
}