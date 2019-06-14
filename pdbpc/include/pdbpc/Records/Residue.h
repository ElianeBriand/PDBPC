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

#ifndef PDBPC_RESIDUE_H
#define PDBPC_RESIDUE_H

#include <string>
#include <map>
#include <memory>
#include <vector>

namespace pdbpc {

    enum class ResidueType {
        alanine,
        arginine,
        asparagine,
        aspartate,
        cysteine,
        glutamate,
        glutamine,
        glycine,
        histidine,
        isoleucine,
        leucine,
        lysine,
        methionine,
        phenylalanine,
        proline,
        serine,
        threonine,
        tryptophan,
        tyrosine,
        valine,
        selenocysteine,
        pyrrolysine,
        heteroatom, /* Used for convenience */
        unknown
    };

    std::ostream& operator<<(std::ostream & os, const ResidueType & restype);

    std::string residueTypeToThreeLetterCode(ResidueType t);
    std::string residueTypeToFullName(ResidueType t);
    ResidueType ThreeLetterCodeToResidueType(const std::string &shorthand);

    extern std::map<ResidueType, std::map<std::string, double> > residueAtomChargeLookupTable;

    struct Model;
    struct Chain;
    struct Atom;

    struct Residue {

        static unsigned int lastserialNumber;


        int serialNumber;


        std::string name;
        ResidueType type;

        int sequenceNumber;
        std::string insertionCode;

        std::weak_ptr<Model> parentModel;
        std::weak_ptr<Chain> parentChain;

        std::vector<std::shared_ptr<Atom>> atoms;

        std::string openingLine;
        int openingLineNumber = -1;

        std::string closingLine;
        int closingLineNumber = -1;




        void printRecord();

    };
}


#endif //PDBPC_RESIDUE_H
