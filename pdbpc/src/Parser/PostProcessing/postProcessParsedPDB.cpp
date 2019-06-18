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


#include <set>
#include <algorithm>
#include <iostream>

#include <Parser/PostProcessing/postProcessParsedPDB.h>
#include <Utility/internalUtils.h>
#include <pdbpc/Records/Chain.h>

namespace pdbpc {

    void modelPostProcessing_handleNoModel(ParsedPDB& ppdb);

    void modelPostProcessing_handleDuplicatedModelNumber(ParsedPDB& ppdb);

    void populateChains(ParsedPDB& ppdb);

    void populateResidues(ParsedPDB& ppdb);

    void checkMasterRecord(ParsedPDB& ppdb);

    void handleAltLocation(ParsedPDB& ppdb);

    void postProcessParsedPDB(ParsedPDB& ppdb) {

        checkMasterRecord(ppdb);

        // TODO: Handle duplicate models
        modelPostProcessing_handleDuplicatedModelNumber(ppdb);
        // Handle no model record
        //  --> We legitimize the default model (model num -1) to be our only model
        modelPostProcessing_handleNoModel(ppdb);

        // Generate and populate chains, then residues
        populateChains(ppdb);
        populateResidues(ppdb);


        handleAltLocation(ppdb);

    }

    void modelPostProcessing_handleDuplicatedModelNumber(ParsedPDB& ppdb) {
        std::vector<int> encounteredModelNumber;

        for (int i = 0; i < ppdb.models.size(); i++) {
            std::shared_ptr<Model> model = ppdb.models.at(i);
            auto found_it = std::find(encounteredModelNumber.begin(), encounteredModelNumber.end(), model->modelNumber);
            if (found_it == encounteredModelNumber.end()) { // We have never seen this modelNumber before
                encounteredModelNumber.push_back(model->modelNumber);
                continue;
            }
            // else we have a duplicated model number

            if (ppdb.settings.duplicateModelPolicy == DuplicateModelPolicy::dropDuplicate) {
                ppdb.models.erase(std::next(ppdb.models.begin(), i));
            } else if (ppdb.settings.duplicateModelPolicy == DuplicateModelPolicy::renumberAfterParsingCompletion) {
                int freeModelNumber = findNextFreeModelNumber(ppdb);
                model->details.originalModelNumber = model->modelNumber;
                model->modelNumber = freeModelNumber;
                encounteredModelNumber.push_back(model->modelNumber);


            } // This branch should not be reached. // LCOV_EXCL_START
            else {

                // This should not be reached.
                std::cout << "Unexpected parser DuplicateModelPolicy setting.\n"
                             "  integer value=" << static_cast<int>(ppdb.settings.duplicateModelPolicy) <<
                          "Now terminating. " << std::endl;
                std::terminate();

            } // LCOV_EXCL_STOP
        }

    }


    void modelPostProcessing_handleNoModel(ParsedPDB& ppdb) {

        if (ppdb.models.size() > 1) {
            // We have explicit models
            if (ppdb.models.at(0)->atoms_flatlist.size() == 0) {
                // No atoms in the default model : we can just remove it
                ppdb.models.erase(ppdb.models.begin());
            } else {
                // We find a free model num to legitimize the default model
                int newDefaultModelmodelNum = findNextFreeModelNumber(ppdb);
                ppdb.models.at(0)->modelNumber = newDefaultModelmodelNum;
            }
        } else {
            // We only have the implicit default model
            ppdb.models.at(0)->modelNumber = 1;
        }
    }

    void populateChains(ParsedPDB& ppdb) {
        int lastUsedChainNumber = -1;
        for (auto& model : ppdb.models) {
            std::set<std::string> foundChainNames;
            for (auto& atom: model->atoms_flatlist) {
                foundChainNames.insert(atom->chainIdentifier);
            }

            for (const auto& chainName : foundChainNames) {
                auto chain = std::make_shared<Chain>();
                chain->serialNumber = ++lastUsedChainNumber;
                chain->chainIdentifier = chainName;

                int firstEncounteredLineNumber = -1;
                std::string firstEncounteredLine;
                int lastEncounteredLineNumber = -1;
                std::string* lastEncounteredLine = nullptr;

                for (auto& atom: model->atoms_flatlist) {
                    if (atom->chainIdentifier == chainName) {
                        if (firstEncounteredLineNumber == -1) {
                            firstEncounteredLineNumber = atom->lineNumber;
                            firstEncounteredLine = atom->line;
                        }

                        chain->atoms_flatlist.push_back(atom);
                        atom->parentChain = chain;

                        // Such that when we process the last line, these will be set to the expected value
                        lastEncounteredLineNumber = atom->lineNumber;
                        lastEncounteredLine = &(atom->line);
                    }

                }
                assert(firstEncounteredLineNumber != -1);
                assert(!firstEncounteredLine.empty());
                assert(lastEncounteredLineNumber != -1);
                assert(lastEncounteredLine != nullptr);
                assert(!lastEncounteredLine->empty());

                chain->parentModel = model;
                chain->closingLineNumber = lastEncounteredLineNumber;
                chain->closingLine = *lastEncounteredLine;
                model->chains.push_back(chain);
                ppdb.chains_flatlist.push_back(chain);
            }
        }
    }

    void populateResidues(ParsedPDB& ppdb) {
        for (auto& model : ppdb.models) {
            for (const auto& chain : model->chains) {
                std::vector<std::shared_ptr<Residue>> foundResidue;

                int lastResidueSeqNum = -1;
                std::string lastResidueName = "";
                std::string lastResidueInsertionCode = "";

                std::shared_ptr<Atom> lastSeenAtom = nullptr;
                for (auto& atom: chain->atoms_flatlist) {
                    if (atom->residueSequenceNumber != lastResidueSeqNum) {
                        // That's a new residue
                        auto residue = std::make_shared<Residue>();

                        residue->serialNumber = ++Residue::lastserialNumber;

                        residue->parentModel = model;
                        residue->parentChain = chain;

                        residue->openingLineNumber = atom->lineNumber;
                        residue->openingLine = atom->line;

                        residue->sequenceNumber = atom->residueSequenceNumber;
                        residue->type = atom->residueType;
                        residue->name = atom->residueName;
                        residue->insertionCode = atom->residueInsertionCode;

                        if (lastSeenAtom != nullptr) {
                            // This is a new residue, and we have already seen an atom => this is not the first atom/residue
                            // Then we have to assign the closing line of the last residue
                            auto lastResidue = foundResidue.back();
                            lastResidue->closingLineNumber = lastSeenAtom->lineNumber;
                            lastResidue->closingLine = lastSeenAtom->line;
                        }

                        foundResidue.push_back(residue);
                        lastResidueSeqNum = residue->sequenceNumber;
                        lastResidueName = residue->name;
                        lastResidueInsertionCode = residue->insertionCode;

                    } else {
                        // It is alledgedly the same residue
                        // We sanity check that it has the same name and insertion code

                        bool residueNameNotMatching = (atom->residueName != lastResidueName);
                        bool residueInsertionCodeNotMatching = (atom->residueInsertionCode != lastResidueInsertionCode);
                        if (residueNameNotMatching || residueInsertionCodeNotMatching) {
                            auto rec = std::make_shared<OutOfBandRecord>();
                            rec->severity = OutOfBandSeverity::error;
                            rec->type = OutOfBandType::IncorrectPDBFileStructure;

                            if (residueNameNotMatching && residueInsertionCodeNotMatching)
                                rec->subtype = OutOfBandSubType::ResidueInconsistentInsertionCodeAndResidueName;
                            else if (residueInsertionCodeNotMatching)
                                rec->subtype = OutOfBandSubType::ResidueInconsistentInsertionCode;
                            else
                                rec->subtype = OutOfBandSubType::ResidueInconsistentResidueName;

                            rec->line = atom->line;
                            rec->lineNumber = atom->lineNumber;
                            rec->recoveryStatus = RecoveryStatus::unrecoverable; // We will ignore, but it means the structure is not nearly correct
                            ppdb.outOfBandRecords.push_back(rec);
                        }

                    }

                    assert(!foundResidue.empty());
                    auto currResidue = foundResidue.back();

                    atom->parentResidue = currResidue;
                    currResidue->atoms.push_back(atom);

                    lastSeenAtom = atom;
                }

                // Attaching the residues to the chain and model and whole ppdb
                for (const auto& res: foundResidue) {
                    chain->residues.push_back(res);
                    model->residues_flatlist.push_back(res);
                    ppdb.residues_flatlist.push_back(res);
                }
            }
        }
    }

    void checkMasterRecord(ParsedPDB& ppdb) {
        // TODO: Add actual master record check
    }

    void handleAltLocation(ParsedPDB& ppdb) {

        if (ppdb.settings.altLocationPolicy == AltLocationPolicy::leaveAsIs) {
            return;
        }

        // Find all locus of AltLocations

        for (auto& model : ppdb.models) {
            // Each model is independant for the purpose of alt locations

            // Map (ChainIdentifier,ResidueSeqNumber,AtomName) --> Atom in a AltLoc group
            std::map<std::tuple<std::string, int, std::string>, std::vector<std::shared_ptr<Atom>>> altLocationLocusMap;

            for (auto& atom: model->atoms_flatlist) {

                if (atom->alternateLocationIdentifier == "") {
                    continue;
                } else {
                    std::tuple<std::string, int, std::string> key(atom->chainIdentifier, atom->residueSequenceNumber,
                                                                  atom->atomName);
                    if (altLocationLocusMap.count(key) == 0) {
                        // First atom in the group
                        std::vector<std::shared_ptr<Atom>> value;
                        value.push_back(atom);
                        altLocationLocusMap.insert(std::pair(key, value));
                    } else {
                        // We insert in the existing vector
                        altLocationLocusMap.at(key).push_back(atom);
                    }
                }

            } // for(auto& atom: model->atoms_flatlist)

            for (auto& pair: altLocationLocusMap) {

                if (ppdb.settings.altLocationPolicy == AltLocationPolicy::firstAppearing) {
                    for (auto it = pair.second.begin() + 1; it != pair.second.end(); ++it) {
                        deleteAtomFromHierarchy(ppdb,*it);
                        ppdb.details.alternateLocationWereRemoved = true;

                    }
                } else if (ppdb.settings.altLocationPolicy == AltLocationPolicy::highestOccupancy) {
                    auto it_max = std::max_element(pair.second.begin(),
                                                   pair.second.end(),
                                                   [](const std::shared_ptr<Atom>& a_sptr1,const std::shared_ptr<Atom>& a_sptr2) {
                                                        return a_sptr1->occupancy < a_sptr2->occupancy;
                                                   });
                    assert(it_max != pair.second.end());
                    for (auto & it : pair.second) {
                        if(it == *it_max)
                            continue;
                        deleteAtomFromHierarchy(ppdb,it);
                        ppdb.details.alternateLocationWereRemoved = true;
                    }
                }// This branch should not be reached. // LCOV_EXCL_START
                else {
                    std::cout << "Unexpected parser AltLocationPolicy setting.\n"
                                 "  integer value=" << static_cast<int>(ppdb.settings.duplicateModelPolicy) <<
                              "Now terminating. " << std::endl;
                    std::terminate();

                } // LCOV_EXCL_STOP
            } //for(auto& pair: altLocationLocusMap)


        } // for (auto& model : ppdb.models)



    }

}