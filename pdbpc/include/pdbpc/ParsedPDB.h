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

#ifndef PDBPC_PARSEDPDB_H
#define PDBPC_PARSEDPDB_H

#include <vector>
#include <memory>

#include <boost/date_time.hpp>


#include <pdbpc/Records/Atom.h>
#include <pdbpc/Records/Heterogen.h>

#include "ParserSettings.h"
#include "pdbpc/Records/Model.h"
#include "pdbpc/Records/OutOfBandRecord.h"

namespace pdbpc {
    struct __attribute__ ((visibility ("default"))) ParsedPDB {


        ParserSettings settings;

        std::vector<std::shared_ptr<Model>> models;

        std::vector<std::shared_ptr<Chain>> chains_flatlist;
        std::vector<std::shared_ptr<Residue>> residues_flatlist;
        std::vector<std::shared_ptr<Atom>> atoms_flatlist;

        std::vector<std::shared_ptr<Heterogen>> heterogens;

        std::vector<std::shared_ptr<OutOfBandRecord>> outOfBandRecords;


        int numberOfParsedLines;


        bool hasErrors = false;
        bool hasWarnings = false ;
        bool hasPedanticProblems = false;

        bool hasAlternateLocation = false;


        struct __attribute__ ((visibility ("default"))) Details {

            std::string headerLine;
            int headerLineNumber;
            boost::gregorian::date depositionDate;
            std::string PDBCode;
            std::string classification;


            // END record
            bool endRecordReached = false;
            int endRecordLineNumber = -1;
            std::string endRecordLine;
            bool continuePastEndRecord = false;

            bool alternateLocationWereRemoved = false; ///>  True if a altLocPolicy handler removed ATOM/HETATM records.

            bool hasMisformatedHET = false; ///> At least some HET record were non-conformant. Expect subpar HET data.

            struct __attribute__ ((visibility ("default"))) MasterRecord {
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
