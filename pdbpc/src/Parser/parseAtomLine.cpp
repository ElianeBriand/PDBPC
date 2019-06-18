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

#include <Parser/parseAtomLine.h>


#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <pdbpc/Records/Atom.h>
#include <pdbpc/Records/Residue.h>
#include <Utility/internalUtils.h>

namespace b = boost;

namespace pdbpc {

        void parseAtomLine_normalMode(ParsedPDB& ppdb, const std::string& originalAtomLine, int lineNumber, const std::string& atomLine);

        void parseAtomLine_degradedMode(ParsedPDB& ppdb, const std::string& originalAtomLine, int lineNumber);


        bool parseAtomLine_serialNumberString(const std::shared_ptr<Atom>& newAtom,
                                              const std::string& serialNumberString,
                                              ParsedPDB& ppdb,
                                              const std::string& atomLine,
                                              int lineNumber);
        bool parseAtomLine_AtomNameResidueNameString(const std::shared_ptr<Atom>& newAtom,
                                              const std::string& AtomNameString, const std::string& ResNameString,
                                              ParsedPDB& ppdb,
                                              const std::string& atomLine,
                                              int lineNumber);
        bool parseAtomLine_AltLocIDString(const std::shared_ptr<Atom>& newAtom,
                                         const std::string& AltLocIDString,
                                         ParsedPDB& ppdb,
                                         const std::string& atomLine,
                                         int lineNumber);
        bool parseAtomLine_ChainIDString(const std::shared_ptr<Atom>& newAtom,
                                              const std::string& ChainIDString,
                                              ParsedPDB& ppdb,
                                              const std::string& atomLine,
                                              int lineNumber);
        bool parseAtomLine_ResSeqNumString(const std::shared_ptr<Atom>& newAtom,
                                         const std::string& ResSeqNumString,
                                         ParsedPDB& ppdb,
                                         const std::string& atomLine,
                                         int lineNumber);
        bool parseAtomLine_InsertCodeString(const std::shared_ptr<Atom>& newAtom,
                                           const std::string& InsertCodeString,
                                           ParsedPDB& ppdb,
                                           const std::string& atomLine,
                                           int lineNumber);
        bool parseAtomLine_PositionString(const std::shared_ptr<Atom>& newAtom,
                                            const std::string& posXString,const std::string& posYString,const std::string& posZString,
                                            ParsedPDB& ppdb,
                                            const std::string& atomLine,
                                            int lineNumber);
        bool parseAtomLine_occupancyString(const std::shared_ptr<Atom>& newAtom,
                                            const std::string& occupancyString,
                                            ParsedPDB& ppdb,
                                            const std::string& atomLine,
                                            int lineNumber);
        bool parseAtomLine_TempFactorString(const std::shared_ptr<Atom>& newAtom,
                                           const std::string& TempFactorString,
                                           ParsedPDB& ppdb,
                                           const std::string& atomLine,
                                           int lineNumber);
        bool parseAtomLine_ElementString(const std::shared_ptr<Atom>& newAtom,
                                            const std::string& ElementString, const std::string& AtomNameString,
                                            ParsedPDB& ppdb,
                                            const std::string& atomLine,
                                            int lineNumber);
        bool parseAtomLine_ChargeString(const std::shared_ptr<Atom>& newAtom,
                                            const std::string& ChargeString,
                                            ParsedPDB& ppdb,
                                            const std::string& atomLine,
                                            int lineNumber);
        void assignAtomToParentModel(const std::shared_ptr<Atom>& newAtom,ParsedPDB& ppdb);


    void parseAtomLine(ParsedPDB& ppdb, const std::string& originalAtomLine, int lineNumber) {


        std::string atomLine = originalAtomLine;

        // Preprocessing the line (column number adjustment)
        trimAfterLastColumn(ppdb,atomLine,lineNumber, 80);


        // Dispatching for parse
        if (atomLine.length() == 80) {
            parseAtomLine_normalMode(ppdb, atomLine, lineNumber,atomLine);
        } else if (atomLine.length() >= 78) {
            // We add a blank charge field, then parse as usual
            atomLine = (atomLine + "     ").substr(0, 80);
            parseAtomLine_normalMode(ppdb, originalAtomLine, lineNumber,atomLine);
        } else if (atomLine.length() >= 54) {

            // We use degraded mode parsing : information beyond the 54th column (wich is z position) is discarded
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::AtomDegradedModeParsingEvent;
            rec->line = originalAtomLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);

            parseAtomLine_degradedMode(ppdb, atomLine, lineNumber);
        } else {
            // Nothing to do here, missing too much columns

            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::AtomIncompleteLine;
            rec->line = originalAtomLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
        }

    }

    void parseAtomLine_normalMode(ParsedPDB& ppdb, const std::string& originalLine, int lineNumber, const std::string& atomLine) {
        auto it_beg = atomLine.begin();


        std::string serialNumberString(it_beg + 6, it_beg + 11);
        std::string AtomNameString(it_beg + 12, it_beg + 16);
        std::string AltLocIDString(it_beg + 16, it_beg + 17);
        std::string ResNameString(it_beg + 17, it_beg + 20);
        std::string ChainIDString(it_beg + 21, it_beg + 22);
        std::string ResSeqNumString(it_beg + 22, it_beg + 26);
        std::string InsertCodeString(it_beg + 26, it_beg + 27);
        std::string posXString(it_beg + 30, it_beg + 38);
        std::string posYString(it_beg + 38, it_beg + 46);
        std::string posZString(it_beg + 46, it_beg + 54);
        std::string occupancyString(it_beg + 54, it_beg + 60);
        std::string TempFactorString(it_beg + 60, it_beg + 66);
        std::string ElementString(it_beg + 76, it_beg + 78);
        std::string ChargeString(it_beg + 78, it_beg + 80);

        auto newAtom = std::make_shared<Atom>();
        newAtom->lineNumber = lineNumber;
        newAtom->line = originalLine;

        bool hasSucceeded = true;

        hasSucceeded &= parseAtomLine_serialNumberString(newAtom,serialNumberString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_AtomNameResidueNameString(newAtom,AtomNameString,ResNameString,ppdb,atomLine,lineNumber);

        hasSucceeded &=  parseAtomLine_AltLocIDString(newAtom,AltLocIDString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_ChainIDString(newAtom,ChainIDString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_ResSeqNumString(newAtom,ResSeqNumString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_InsertCodeString(newAtom,InsertCodeString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_PositionString(newAtom,posXString,posYString,posZString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_occupancyString(newAtom,occupancyString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_TempFactorString(newAtom,TempFactorString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_ElementString(newAtom,ElementString,AtomNameString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_ChargeString(newAtom,ChargeString,ppdb,atomLine,lineNumber);

        if(hasSucceeded)
        {
            assignAtomToParentModel(newAtom,ppdb);
        }
    }


    void parseAtomLine_degradedMode(ParsedPDB& ppdb, const std::string& atomLine, int lineNumber) {
        auto it_beg = atomLine.begin();

        std::string serialNumberString(it_beg + 6, it_beg + 11);
        std::string AtomNameString(it_beg + 12, it_beg + 16);
        std::string AltLocIDString(it_beg + 16, it_beg + 17);
        std::string ResNameString(it_beg + 17, it_beg + 20);
        std::string ChainIDString(it_beg + 21, it_beg + 22);
        std::string ResSeqNumString(it_beg + 22, it_beg + 26);
        std::string InsertCodeString(it_beg + 26, it_beg + 27);
        std::string posXString(it_beg + 30, it_beg + 38);
        std::string posYString(it_beg + 38, it_beg + 46);
        std::string posZString(it_beg + 46, it_beg + 54);

        auto newAtom = std::make_shared<Atom>();
        newAtom->lineNumber = lineNumber;
        newAtom->line = atomLine;

        bool hasSucceeded = true;

        hasSucceeded &= parseAtomLine_serialNumberString(newAtom,serialNumberString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_AtomNameResidueNameString(newAtom,AtomNameString,ResNameString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_AltLocIDString(newAtom,AltLocIDString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_ChainIDString(newAtom,ChainIDString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_ResSeqNumString(newAtom,ResSeqNumString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_InsertCodeString(newAtom,InsertCodeString,ppdb,atomLine,lineNumber);

        hasSucceeded &= parseAtomLine_PositionString(newAtom,posXString,posYString,posZString,ppdb,atomLine,lineNumber);

        newAtom->occupancy = 1.0;
        newAtom->temperatureFactor = 0.0;
        newAtom->element = b::trim_copy(boost::to_upper_copy<std::string>(AtomNameString)).substr(0,1);
        newAtom->charge = 0;

        if(hasSucceeded)
        {
            assignAtomToParentModel(newAtom,ppdb);
        }

    }

    bool parseAtomLine_serialNumberString(const std::shared_ptr<Atom>& newAtom,
                                          const std::string& serialNumberString,
                                          ParsedPDB& ppdb,
                                          const std::string& atomLine,
                                          int lineNumber) {
        std::string trimmed_atomSerial =  b::trim_copy(serialNumberString);
        int atomSerialNumber = -1;
        try {
            atomSerialNumber = b::lexical_cast<int>(trimmed_atomSerial);
        }
        catch (const b::bad_lexical_cast& e) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::AtomCannotParseSerialNumber;
            rec->line = atomLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
            return false;
        }
        newAtom->serialNumber = atomSerialNumber;
        return true;
    }

    bool parseAtomLine_AtomNameResidueNameString(const std::shared_ptr<Atom>& newAtom,
                                      const std::string& AtomNameString, const std::string& ResNameString,
                                      ParsedPDB& ppdb,
                                      const std::string& atomLine,
                                      int lineNumber) {
        newAtom->atomName = b::trim_copy(boost::to_upper_copy<std::string>(AtomNameString));
        newAtom->residueName = b::trim_copy(boost::to_upper_copy<std::string>(ResNameString));

        newAtom->residueType = ThreeLetterCodeToResidueType(newAtom->residueName);

        if (newAtom->residueType == ResidueType::unknown) {
            // Residue type could not be deduced
            // (This is possible in non-standard amino acid)
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::warning;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::AtomUnknownResidueThreeLetterCode;
            rec->line = atomLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
        } else {
            // We have a valid residue type
            // Checking that the atom belongs to such residue if not hydrogen (not in database, TODO: add them)
            if(newAtom->atomName.at(0) != 'H')
            {
                auto knownAtomIt = residueAtomChargeLookupTable[newAtom->residueType].find(newAtom->atomName);
                if (knownAtomIt == residueAtomChargeLookupTable[newAtom->residueType].end()) {
                    // Atom name unknown for this residue
                    // This may be a misattributed atom

                    auto rec = std::make_shared<OutOfBandRecord>();
                    rec->severity = OutOfBandSeverity::warning;
                    rec->type = OutOfBandType::SurprisingStructure;
                    rec->subtype = OutOfBandSubType::AtomNameNotKnownInSpecifiedResidue;
                    rec->line = atomLine;
                    rec->lineNumber = lineNumber;
                    rec->recoveryStatus = RecoveryStatus::recovered;
                    ppdb.outOfBandRecords.push_back(rec);
                }
            }

        }
        return true;
    }


    bool parseAtomLine_ChainIDString(const std::shared_ptr<Atom>& newAtom,
                                     const std::string& ChainIDString,
                                     ParsedPDB& ppdb,
                                     const std::string& atomLine,
                                     int lineNumber) {
        std::string trimmed_chainID = b::trim_copy(ChainIDString);
        if(trimmed_chainID.length() == 0) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::AtomMissingChainID;
            rec->line = atomLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
            return false;
        }else {
            if (!b::all(ChainIDString, b::is_alnum())) {
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::warning;
                rec->type = OutOfBandType::IncorrectPDBLineFormat;
                rec->subtype = OutOfBandSubType::AtomChainIDIsNotAlphanumerical;
                rec->line = atomLine;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::recovered;
                ppdb.outOfBandRecords.push_back(rec);
            }
            newAtom->chainIdentifier = ChainIDString;
        }
        return true;
    }

    bool parseAtomLine_AltLocIDString(const std::shared_ptr<Atom>& newAtom,
                                      const std::string& AltLocIDString,
                                      ParsedPDB& ppdb,
                                      const std::string& atomLine,
                                      int lineNumber){
        std::string trimmed_altloc = b::trim_copy(AltLocIDString);
        if(trimmed_altloc.length() == 0) {
            newAtom->alternateLocationIdentifier = "";
        }else {
            if (!b::all(AltLocIDString, b::is_alpha())) {
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::warning;
                rec->type = OutOfBandType::IncorrectPDBLineFormat;
                rec->subtype = OutOfBandSubType::AtomAltLocIdContainsNonLetter;
                rec->line = atomLine;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::recovered;
                ppdb.outOfBandRecords.push_back(rec);
            }
            ppdb.hasAlternateLocation = true;
            newAtom->alternateLocationIdentifier = trimmed_altloc;
        }

        return true;
    }

    bool parseAtomLine_ResSeqNumString(const std::shared_ptr<Atom>& newAtom,
                                       const std::string& ResSeqNumString,
                                       ParsedPDB& ppdb,
                                       const std::string& atomLine,
                                       int lineNumber) {
        std::string trimmed_ResSeqNum = b::trim_copy(ResSeqNumString);
        int residueSeqId = -1;
        try {
            residueSeqId = b::lexical_cast<int>(trimmed_ResSeqNum);
        }
        catch (const b::bad_lexical_cast& e) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::AtomCannotParseResidueSeqId;
            rec->line = atomLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
            return false;
        }
        newAtom->residueSequenceNumber = residueSeqId;
        return true;
    }
    bool parseAtomLine_InsertCodeString(const std::shared_ptr<Atom>& newAtom,
                                        const std::string& InsertCodeString,
                                        ParsedPDB& ppdb,
                                        const std::string& atomLine,
                                        int lineNumber) {
        if (
                InsertCodeString != " "
                && (!b::all(InsertCodeString, b::is_alpha()))
                ) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::warning;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::AtomResidueInsertionCodeIsNotALetter;
            rec->line = atomLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
        }

        if (InsertCodeString == " ") {
            newAtom->residueInsertionCode = "";
        } else {
            newAtom->residueInsertionCode = InsertCodeString;
        }
        return true;
    }


    bool parseAtomLine_PositionString(const std::shared_ptr<Atom>& newAtom,
                                      const std::string& posXString,const std::string& posYString,const std::string& posZString,
                                      ParsedPDB& ppdb,
                                      const std::string& atomLine,
                                      int lineNumber) {
        std::string trimmed_posX =  b::trim_copy(posXString);
        std::string trimmed_posY =  b::trim_copy(posYString);
        std::string trimmed_posZ =  b::trim_copy(posZString);

        double posX = -1.0;
        double posY = -1.0;
        double posZ = -1.0;
        try {
            posX = b::lexical_cast<double>(trimmed_posX);
            posY = b::lexical_cast<double>(trimmed_posY);
            posZ = b::lexical_cast<double>(trimmed_posZ);
        }
        catch (const b::bad_lexical_cast& e) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::AtomCannotParseCoordinates;
            rec->line = atomLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::unrecoverable;
            ppdb.outOfBandRecords.push_back(rec);
            return false;
        }
        newAtom->x = posX;
        newAtom->y = posY;
        newAtom->z = posZ;
        return true;
    }

    bool parseAtomLine_occupancyString(const std::shared_ptr<Atom>& newAtom,
                                       const std::string& occupancyString,
                                       ParsedPDB& ppdb,
                                       const std::string& atomLine,
                                       int lineNumber)
    {
        std::string trimmed_occupancy = b::trim_copy(occupancyString);

        if (trimmed_occupancy.length() == 0) {
            newAtom->occupancy = 1.0; // Standard mandates this default
        } else {
            double occupancy = -1.0;
            try {
                occupancy = b::lexical_cast<double>(trimmed_occupancy);
            }
            catch (const b::bad_lexical_cast& e) {
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::IncorrectPDBLineFormat;
                rec->subtype = OutOfBandSubType::AtomCannotParseOccupancy;
                rec->line = atomLine;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::unrecoverable;
                ppdb.outOfBandRecords.push_back(rec);
                return false;
            }
            newAtom->occupancy = occupancy;
        }
        return true;
    }

    bool parseAtomLine_TempFactorString(const std::shared_ptr<Atom>& newAtom,
                                        const std::string& TempFactorString,
                                        ParsedPDB& ppdb,
                                        const std::string& atomLine,
                                        int lineNumber) {
        std::string trimmed_tempFactor = b::trim_copy(TempFactorString);

        if (trimmed_tempFactor.length() == 0) {
            newAtom->temperatureFactor = 0.0; // Standard mandates this default
        } else {
            double temperatureFactor = -1.0;
            try {
                temperatureFactor = b::lexical_cast<double>(trimmed_tempFactor);
            }
            catch (const b::bad_lexical_cast& e) {
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::IncorrectPDBLineFormat;
                rec->subtype = OutOfBandSubType::AtomCannotParseTempFactor;
                rec->line = atomLine;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::unrecoverable;
                ppdb.outOfBandRecords.push_back(rec);
                return false;
            }
            newAtom->temperatureFactor = temperatureFactor;
        }
        return true;
    }

    bool parseAtomLine_ElementString(const std::shared_ptr<Atom>& newAtom,
                                     const std::string& ElementString, const std::string& AtomNameString,
                                     ParsedPDB& ppdb,
                                     const std::string& atomLine,
                                     int lineNumber) {
        std::string trimmed_ElementString = b::trim_copy(ElementString);
        if (trimmed_ElementString.length() == 0) {
            //Taking the first character of the atom name
            std::string atomName = b::trim_copy(boost::to_upper_copy<std::string>(AtomNameString));
            newAtom->element = atomName.substr(0, 1);

            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::warning;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::AtomNoElementName;
            rec->line = atomLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);

        } else {
            if (!b::all(trimmed_ElementString, b::is_alpha())) {
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::IncorrectPDBLineFormat;
                rec->subtype = OutOfBandSubType::AtomElementNameContainsNonLetter;
                rec->line = atomLine;
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::unrecoverable;
                ppdb.outOfBandRecords.push_back(rec);
                return false;
            } else {
                newAtom->element = trimmed_ElementString;
            }
        }
        return true;
    }

    bool parseAtomLine_ChargeString(const std::shared_ptr<Atom>& newAtom,
                                    const std::string& ChargeString,
                                    ParsedPDB& ppdb,
                                    const std::string& atomLine,
                                    int lineNumber) {
        std::string trimmed_charge = b::trim_copy(ChargeString);

        if (trimmed_charge.length() == 0) {
            newAtom->charge = 0.0; // Standard mandates this default
        } else {
            double charge = -1.0;
            try {
                charge = b::lexical_cast<double>(trimmed_charge.substr(0,1));
            }
            catch (const b::bad_lexical_cast& e) {
                auto rec = std::make_shared<OutOfBandRecord>();
                rec->severity = OutOfBandSeverity::error;
                rec->type = OutOfBandType::IncorrectPDBLineFormat;
                rec->subtype = OutOfBandSubType::AtomCannotParseCharge;
                rec->line = atomLine;
                rec->details = "Field -> Untrimmed: \"" +  ChargeString +"\" | Trimmed: \"" +  trimmed_charge +"\"";
                rec->lineNumber = lineNumber;
                rec->recoveryStatus = RecoveryStatus::unrecoverable;
                ppdb.outOfBandRecords.push_back(rec);
                return false;
            }

            if(trimmed_charge.length() == 2) {
                char plusOrMinus = trimmed_charge.at(1);
                if(plusOrMinus == '+') {
                    // Do nothing, already parsed as positive
                }else if (plusOrMinus == '-') {
                    charge = -1 * charge;
                }else {
                    // That's another character, error out
                    auto rec = std::make_shared<OutOfBandRecord>();
                    rec->severity = OutOfBandSeverity::error;
                    rec->type = OutOfBandType::IncorrectPDBLineFormat;
                    rec->subtype = OutOfBandSubType::AtomCannotParseCharge;
                    rec->line = atomLine;
                    rec->details = "Field -> Untrimmed: \"" +  ChargeString +"\" | Trimmed: \"" +  trimmed_charge +"\"";
                    rec->lineNumber = lineNumber;
                    rec->recoveryStatus = RecoveryStatus::unrecoverable;
                    ppdb.outOfBandRecords.push_back(rec);
                    return false;
                }
            }

            newAtom->charge = charge;
        }
        return true;
    }

    void assignAtomToParentModel(const std::shared_ptr<Atom>& newAtom,ParsedPDB& ppdb) {
        ppdb.atoms_flatlist.push_back(newAtom);
        std::shared_ptr<Model> modelForCurrentAtom = ppdb.models.back();
        modelForCurrentAtom->atoms_flatlist.push_back(newAtom);
        newAtom->parentModel = modelForCurrentAtom;
    }


}