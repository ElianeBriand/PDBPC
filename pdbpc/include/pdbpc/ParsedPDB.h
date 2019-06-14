//
// Created by eliane on 11/05/19.
//

#ifndef PDBPC_PARSEDPDB_H
#define PDBPC_PARSEDPDB_H

#include <vector>
#include <memory>
#include <pdbpc/Records/Atom.h>

#include "ParserSettings.h"
#include "pdbpc/Records/Model.h"
#include "pdbpc/Records/OutOfBandRecord.h"

namespace pdbpc {
    struct ParsedPDB {

        ParsedPDB();

        ParserSettings settings;

        std::vector<std::shared_ptr<Model>> models;

        std::vector<std::shared_ptr<Chain>> chains_flatlist;
        std::vector<std::shared_ptr<Residue>> residues_flatlist;
        std::vector<std::shared_ptr<Atom>> atoms_flatlist;

        std::vector<std::shared_ptr<OutOfBandRecord>> outOfBandRecords;


        int numberOfParsedLines;


        bool hasErrors;
        bool hasWarnings;
        bool hasPedanticProblems;


        struct Details {

            // END record
            bool endRecordReached = false;
            int endRecordLineNumber = -1;
            std::string endRecordLine;
            bool continuePastEndRecord = false;

            struct MasterRecord {
                int lineNumber;
                std::string line;

                int numRemark = -1;
                int mandatedZero = -1;
                int numHet = -1;
                int numHelix = -1;
                int numSheet = -1;
                int numTurn = -1;
                int numSite = -1;
                int numXform = -1;
                int numCoord = -1;
                int numTer = -1;
                int numConect = -1;
                int numSeq = -1;
            } master;

        } details;


    };
}



#endif //PDBPC_PARSEDPDB_H
