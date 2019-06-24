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

#include "parseHetRelatedLines.h"

#include "Utility/internalUtils.h"


#include <boost/algorithm/string.hpp>
#include <pdbpc/Records/Heterogen.h>

namespace b = boost;

namespace pdbpc {

    void parseHETLine(ParsedPDB& ppdb, const std::string& hetLine, int lineNumber) {
        std::string line = hetLine;

        inflateShorterLineOrFail(ppdb, line, lineNumber, 0, 70);

        // Field separation
        auto it_beg = line.begin();
        std::string hetIDField(it_beg + 7, it_beg + 10);
        std::string chainIDField(it_beg + 12, it_beg + 13);
        std::string sequenceNumberField(it_beg + 13, it_beg + 17);
        std::string insertionCodeField(it_beg + 17, it_beg + 18);
        std::string numHETATMField(it_beg + 20, it_beg + 25);
        std::string descriptionField(it_beg + 30, it_beg + 70);

        // Filling the record
        auto heterogen = std::make_shared<Heterogen>();
        bool incompleteRecord = false;

        heterogen->line = line;
        heterogen->lineNumber = lineNumber;

        heterogen->hetID = b::trim_copy(hetIDField);
        heterogen->chainID = b::trim_copy(chainIDField);

        if (heterogen->chainID.length() == 0) {
            incompleteRecord = true;
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::MissingChainID;
            rec->line = hetLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
            ppdb.details.hasMisformatedHET = true;
        } else if (!b::all(heterogen->chainID, b::is_alpha())) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::NonAlphabeticChainID;
            rec->line = hetLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
            ppdb.details.hasMisformatedHET = true;
        }

        auto sequenceNumber = parseOrUnrecoverableError<int>(ppdb,
                                                             hetLine,
                                                             lineNumber,
                                                             sequenceNumberField,
                                                             OutOfBandSubType::MissingSequenceNumber);
        heterogen->sequenceNumber = sequenceNumber.value_or(-1);
        incompleteRecord |= !sequenceNumber;


        auto numHETATM = parseOrUnrecoverableError<int>(ppdb,
                                                        hetLine,
                                                        lineNumber,
                                                        numHETATMField,
                                                        OutOfBandSubType::MissingHETATMCountInHETRecord);
        heterogen->numHETATM = numHETATM.value_or(-1);
        incompleteRecord |= !numHETATM;

        heterogen->insertionCode = b::trim_copy(insertionCodeField); // This can be "", insertionCode is not mandatory

        heterogen->description = b::trim_copy(descriptionField);


        // Signalling incomplete record
        if (incompleteRecord) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::IncompleteHETrecord;
            rec->line = hetLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
            ppdb.details.hasMisformatedHET = true;
        }

        // Adding to the relevant vector

        ppdb.heterogens.push_back(heterogen);

    }


    void parseHETNAMLine(ParsedPDB& ppdb, const std::string& hetnamLine, int lineNumber) {
        std::string line = hetnamLine;

        inflateShorterLineOrFail(ppdb, line, lineNumber, 0, 70);

        // Field separation
        auto it_beg = line.begin();
        std::string continuationField(it_beg + 8, it_beg + 10);
        std::string hetIDField(it_beg + 11, it_beg + 14);
        std::string chemicalNameField(it_beg + 15, it_beg + 70);

        std::string hetID = b::trim_copy(hetIDField);
        std::string continuation = b::trim_copy(continuationField);
        std::string chemicalName = b::trim_copy(chemicalNameField);

        if (hetID.length() == 0) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::MissingHetID;
            rec->line = hetnamLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
            return;
        }

        // We don't care if this is a continuation : as the default name string is "",
        // and we append to it, this results in the same name
        // Only if continuation are not in the right order, would that matter.
        // This particular case was not seen in the wild, but if needs be can be implemented

        //bool continuationMode = (continuation.length() != 0);


        std::shared_ptr<Heterogen> het = findHetOrCreateOrphan(ppdb,hetnamLine,lineNumber,hetID);

        het->chemicalName += chemicalName;

    }

    std::shared_ptr<Heterogen> createOrphanedHeterogen(ParsedPDB& ppdb,
                                                       const std::string& line,
                                                       int lineNumber,
                                                       const std::string& hetID) {
        auto heterogen = std::make_shared<Heterogen>();
        heterogen->orphaned = true;
        heterogen->hetID = hetID;

        heterogen->line = line;
        heterogen->lineNumber = lineNumber;

        ppdb.heterogens.push_back(heterogen);

        return heterogen;
    }

    std::shared_ptr<Heterogen> findHetOrCreateOrphan(ParsedPDB& ppdb,
                                                     const std::string& line,
                                                     int lineNumber,
                                                     const std::string& hetID) {
        auto found_it = std::find_if(ppdb.heterogens.begin(),
                                     ppdb.heterogens.end(),
                                     [&hetID](const std::shared_ptr<Heterogen>& het) {
                                         return het->hetID == hetID;
                                     });

        std::shared_ptr<Heterogen> het;

        if(found_it == ppdb.heterogens.end()) {
            if(hetID != "HOH") {
                // According to standard, this normal to not have HET for water :
                // """PDB entries do not have HET records for water molecules, deuterated water, or methanol (when used as solvent)."""
                // But we need to create it when we encounter FORMUL for water...
                // (Sound pretty strange not to have consistent HET records, but whatever)

                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::IncorrectPDBFileStructure;
                rec->subtype = OutOfBandSubType::HetIDReferencedButNotDefined;
                rec->line = line;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::recovered;
                ppdb.outOfBandRecords.push_back(rec);
            }


            // We create an orphaned Heterogen
            het = createOrphanedHeterogen(ppdb,line,lineNumber,hetID);
        } else {
            het = *found_it;
        }

        return het;
    }

    void parseHETSYNLine(ParsedPDB& ppdb, const std::string& hetsynLine, int lineNumber) {
        std::string line = hetsynLine;

        inflateShorterLineOrFail(ppdb, line, lineNumber, 0, 70);

        // Field separation
        auto it_beg = line.begin();
        std::string continuationField(it_beg + 8, it_beg + 10);
        std::string hetIDField(it_beg + 11, it_beg + 14);
        std::string hetSynonymsField(it_beg + 15, it_beg + 70);

        std::string hetID = b::trim_copy(hetIDField);
        std::string continuation = b::trim_copy(continuationField);
        std::string hetSynonyms = b::trim_copy(hetSynonymsField);


        std::shared_ptr<Heterogen> het = findHetOrCreateOrphan(ppdb,hetsynLine,lineNumber,hetID);

        std::vector<std::string> synonymsToAdd;
        boost::split(synonymsToAdd, hetSynonyms, boost::is_any_of(";"));

        for(std::string& rawSynonyms : synonymsToAdd) {
            b::trim(rawSynonyms);
        }

        het->synonyms.insert(het->synonyms.end(), synonymsToAdd.begin(), synonymsToAdd.end());



    }

    void parseFORMULLine(ParsedPDB& ppdb, const std::string& formulLine, int lineNumber) {
        std::string line = formulLine;

        inflateShorterLineOrFail(ppdb, line, lineNumber, 0, 70);

        // Field separation
        auto it_beg = line.begin();
        std::string componentNumberField(it_beg + 8, it_beg + 10);
        std::string hetIDField(it_beg + 12, it_beg + 15);
        std::string waterSignalField(it_beg + 18, it_beg + 19);
        std::string continuationField(it_beg + 16, it_beg + 18);
        std::string formulaField(it_beg + 19, it_beg + 70);

        std::string componentNumberString = b::trim_copy(componentNumberField);
        std::string hetID = b::trim_copy(hetIDField);
        std::string continuation = b::trim_copy(continuationField);
        std::string formulaString = b::trim_copy(formulaField);

        // It is unclear to me how component number are actually assigned ? - E. B.

        bool isWater = (waterSignalField == "*");

        auto componentNumber = parseOrRecoverableError<int>(ppdb,
                                                      formulLine,
                                                        lineNumber,
                                                            componentNumberString,
                                                        OutOfBandSubType::CannotParseComponentNumber);

        std::shared_ptr<Heterogen> het = findHetOrCreateOrphan(ppdb,formulLine,lineNumber,hetID);

        het->chemicalFormula += formulaString;
        het->componentNumber = componentNumber.value_or(-1);
        het->isWater = isWater;

    }



}