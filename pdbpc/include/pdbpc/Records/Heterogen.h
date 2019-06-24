//
// Created by eliane on 21/06/19.
//

#ifndef PDBPC_HETEROGEN_H
#define PDBPC_HETEROGEN_H

#include "Atom.h"

namespace pdbpc {

    struct Chain;
    struct Model;

    struct __attribute__ ((visibility ("default")))  Heterogen {

        /**
         * This flag is set when the Heterogen was implicitly defined without a HET record
         *
         * This can occurs if a record make reference to a HetID (three letter code for Heterogen),
         * for example in HETATM lines, but do not define it in a HET record. Often, chemo-informatic
         * suite will output PDB file with only HETATM (+/- CONECT), without any other line. We somewhat
         * handle this case by creating an orphaned Heterogen.
         *
         */
        bool orphaned = false;

        std::string hetID;

        std::string chainID;
        int sequenceNumber = -1;
        std::string insertionCode;

        int componentNumber = -1;
        std::string chemicalFormula;
        bool isWater = false;

        std::string description;

        std::string chemicalName;
        std::vector<std::string> synonyms;

        int numHETATM = -1;


        int lineNumber = -1;
        std::string line;

        void printRecord();


    };
}


#endif //PDBPC_HETEROGEN_H
