//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_RESIDUE_H
#define PDBPC_RESIDUE_H

#include <string>
#include <map>

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

    struct Residue {




        int lineNumber = -1;
        std::string line;


        void printResidueRecord();

    };
}


#endif //PDBPC_RESIDUE_H
