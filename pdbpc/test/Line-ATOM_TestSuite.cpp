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

#include <boost/test/unit_test.hpp>



#include <pdbpc/pdbpc.h>
#include "../src/Parser/parseEndModelLine.h"
#include "../src/Parser/parseModelLine.h"
#include "../src/Parser/parseAtomLine.h"
#include <pdbpc/Records/OutOfBandRecord.h>
#include "../src/Utility/internalUtils.h"
#include "simplePDBFixture.h"


struct LineFixture_ATOM {
    LineFixture_ATOM() = default;

    ~LineFixture_ATOM() = default;

    std::string conformingLine                  = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  1.00 21.62           C  ";
    std::string NonConformingLine_degraded      = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314";
    std::string NonConformingLine_noChargeField = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  1.00 21.62           C";
    std::string NonConformingLine_junk          = "ATOM    440  CB  MET A  46      -0.301  24.err  12.314  1.00 21.62           C  ";
    std::string NonConformingLine_tooShort      = "ATOM    440  CB  MET A  46      -0.301  ";

    std::string badSerial                  = "ATOM    4d0  CB  MET A  46      -0.301  24.090  12.314  1.00 21.62           C  ";
    std::string badAtomInRes               = "ATOM    440  CE1 MET A  46      -0.301  24.090  12.314  1.00 21.62           C  ";
    std::string missingChainId             = "ATOM    440  CB  MET    46      -0.301  24.090  12.314  1.00 21.62           C  ";
    std::string badChainId                 = "ATOM    440  CB  MET +  46      -0.301  24.090  12.314  1.00 21.62           C  ";

    std::string withAltLocId               = "ATOM    440  CB JMET A  46      -0.301  24.090  12.314  1.00 21.62           C  ";
    std::string badAltLocId                = "ATOM    440  CB +MET A  46      -0.301  24.090  12.314  1.00 21.62           C  ";

    std::string badResSeq1                 = "ATOM    440  CB  MET A          -0.301  24.090  12.314  1.00 21.62           C  ";
    std::string badResSeq2                 = "ATOM    440  CB  MET A  4a      -0.301  24.090  12.314  1.00 21.62           C  ";

    std::string withInsCode                = "ATOM    440  CB  MET A  46D     -0.301  24.090  12.314  1.00 21.62           C  ";
    std::string badResInsCode              = "ATOM    440  CB  MET A  46?     -0.301  24.090  12.314  1.00 21.62           C  ";

    std::string missingOccupancy           = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314       21.62           C  ";
    std::string badOccupancy               = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  AAAA 21.62           C  ";

    std::string missingTempFactor          = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  1.00                 C  ";
    std::string badTempFactor              = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  1.00 ABCDE           C  ";


    std::string missingElement             = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  1.00 21.62              ";
    std::string badElement                 = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  1.00 21.62           %  ";

    std::string withCharge                 = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  1.00 21.62           C2+";
    std::string withNegCharge              = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  1.00 21.62           C2-";
    std::string badCharge1                  = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  1.00 21.62           C2%";
    std::string badCharge2                  = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  1.00 21.62           Cd-";
    std::string badCharge3                  = "ATOM    440  CB  MET A  46      -0.301  24.090  12.314  1.00 21.62           Cd ";

};

BOOST_AUTO_TEST_SUITE(Line_testSuite)

    BOOST_FIXTURE_TEST_CASE(ATOM_CorrectLine,LineFixture_ATOM,*boost::unit_test::timeout(10)) {
            pdbpc::ParsedPDB ppdb_1;
            createPlaceholderModelAndChain(ppdb_1);
            pdbpc::parseAtomLine(ppdb_1,conformingLine,12);

            // This should generate no out of band records
            BOOST_TEST(ppdb_1.outOfBandRecords.empty());
            BOOST_REQUIRE(ppdb_1.outOfBandRecords.empty());

            // Correctly set record
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->lineNumber == 12);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->line == conformingLine);


            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->serialNumber == 440);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->atomName == "CB");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->alternateLocationIdentifier == "");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueType == pdbpc::ResidueType::methionine);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueName == "MET");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->chainIdentifier == "A");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueSequenceNumber == 46);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->x == -0.301);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->y == 24.090);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->z == 12.314);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueInsertionCode == "");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->occupancy == 1.0);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->temperatureFactor == 21.62);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->charge == 0.0);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->element == "C");


        }

        BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_degraded,LineFixture_ATOM,*boost::unit_test::timeout(10)) {
            pdbpc::ParsedPDB ppdb_2;
            createPlaceholderModelAndChain(ppdb_2);
            pdbpc::parseAtomLine(ppdb_2,NonConformingLine_degraded,12);

            // This should generate no out of band records
            BOOST_TEST(ppdb_2.outOfBandRecords.size() == 1);
            BOOST_REQUIRE(ppdb_2.outOfBandRecords.size() == 1);

            BOOST_TEST(ppdb_2.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomDegradedModeParsingEvent);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->lineNumber == 12);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->line == NonConformingLine_degraded);


            // Correctly set record
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->lineNumber == 12);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->line == NonConformingLine_degraded);

            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->serialNumber == 440);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->atomName == "CB");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->alternateLocationIdentifier == "");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueType == pdbpc::ResidueType::methionine);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueName == "MET");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->chainIdentifier == "A");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueSequenceNumber == 46);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->x == -0.301);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->y == 24.090);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->z == 12.314);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueInsertionCode == "");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->occupancy == 1.0);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->temperatureFactor == 0.0);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->charge == 0.0);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->element == "C");

        }

    BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_NoChargeField,LineFixture_ATOM,*boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb_2;
        createPlaceholderModelAndChain(ppdb_2);
        pdbpc::parseAtomLine(ppdb_2,NonConformingLine_noChargeField,12);

        for(const auto& record: ppdb_2.outOfBandRecords) {
            record->printRecord();
        }

        // This should generate no out of band records
        BOOST_TEST(ppdb_2.outOfBandRecords.empty());



        // Correctly set record
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->lineNumber == 12);
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->line == NonConformingLine_noChargeField);

        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->serialNumber == 440);
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->atomName == "CB");
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->alternateLocationIdentifier == "");
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueType == pdbpc::ResidueType::methionine);
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueName == "MET");
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->chainIdentifier == "A");
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueSequenceNumber == 46);
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->x == -0.301);
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->y == 24.090);
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->z == 12.314);
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueInsertionCode == "");
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->occupancy == 1.0);
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->temperatureFactor == 21.62);
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->charge == 0.0);
        BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->element == "C");

    }

    BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_junk,LineFixture_ATOM,*boost::unit_test::timeout(10)){
            pdbpc::ParsedPDB ppdb_3;
            createPlaceholderModelAndChain(ppdb_3);
            pdbpc::parseAtomLine(ppdb_3,NonConformingLine_junk,12);



            // This should generate no out of band records
            BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
            BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


            BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomCannotParseCoordinates);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == NonConformingLine_junk);


        }

    BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_tooShort,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,NonConformingLine_tooShort,12);



        // This should generate no out of band records
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomIncompleteLine);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == NonConformingLine_tooShort);


    }


    BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_badSerial,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,badSerial,12);



        // This should generate no out of band records
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomCannotParseSerialNumber);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == badSerial);


    }

    BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_badAtomInRes,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,badAtomInRes,12);



        // This should generate no out of band records
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::SurprisingStructure);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomNameNotKnownInSpecifiedResidue);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::warning);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == badAtomInRes);


    }

    BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_MissingChainID,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,missingChainId,12);



        // This should generate no out of band records
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::MissingChainID);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == missingChainId);


    }

    BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_BadID,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,badChainId,12);



        // This should generate no out of band records
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::NonAlphabeticChainID);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::warning);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == badChainId);


    }

    BOOST_FIXTURE_TEST_CASE(ATOM_WithAltLoc,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,withAltLocId,12);



        // This should generate no out of band records
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.empty());

        BOOST_TEST(ppdb_3.hasAlternateLocation);

        BOOST_REQUIRE(!ppdb_3.atoms_flatlist.empty());


        BOOST_TEST(ppdb_3.atoms_flatlist.back()->alternateLocationIdentifier == "J");

    }

    BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_BadAltLoc,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,badAltLocId,12);


        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::NonAlphabeticAltLocId);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::warning);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == badAltLocId);

        BOOST_TEST(ppdb_3.hasAlternateLocation);

        BOOST_REQUIRE(!ppdb_3.atoms_flatlist.empty());


        BOOST_TEST(ppdb_3.atoms_flatlist.back()->alternateLocationIdentifier == "+");


    }

    BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_missingResSeqNum,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,badResSeq1,12);


        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomCannotParseResidueSeqId);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == badResSeq1);


    }

    BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_badResSeqNum,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,badResSeq2,12);

        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomCannotParseResidueSeqId);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == badResSeq2);


    }

    BOOST_FIXTURE_TEST_CASE(ATOM_withInsCode,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,withInsCode,12);



        // This should generate no out of band records
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 0);


        BOOST_REQUIRE(!ppdb_3.atoms_flatlist.empty());


        BOOST_TEST(ppdb_3.atoms_flatlist.back()->residueInsertionCode == "D");

    }

    BOOST_FIXTURE_TEST_CASE(ATOM_NonConformingbadResInsCode,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,badResInsCode,12);

        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::NonAlphabeticInsertionCode);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::warning);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == badResInsCode);



        BOOST_REQUIRE(!ppdb_3.atoms_flatlist.empty());


        BOOST_TEST(ppdb_3.atoms_flatlist.back()->residueInsertionCode == "?");

    }

    BOOST_FIXTURE_TEST_CASE(ATOM_missingOccupancy,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,missingOccupancy,12);



        // This should generate no out of band records
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 0);


        BOOST_REQUIRE(!ppdb_3.atoms_flatlist.empty());


        BOOST_TEST(ppdb_3.atoms_flatlist.back()->occupancy == 1.00);

    }

    BOOST_FIXTURE_TEST_CASE(ATOM_badOccupancy,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,badOccupancy,12);


        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomCannotParseOccupancy);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == badOccupancy);

    }


    BOOST_FIXTURE_TEST_CASE(ATOM_missingTempFactor,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,missingTempFactor,12);



        // This should generate no out of band records
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 0);


        BOOST_REQUIRE(!ppdb_3.atoms_flatlist.empty());


        BOOST_TEST(ppdb_3.atoms_flatlist.back()->temperatureFactor == 0.00);

    }

    BOOST_FIXTURE_TEST_CASE(ATOM_badTempFactor,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,badTempFactor,12);


        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomCannotParseTempFactor);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == badTempFactor);

    }

    BOOST_FIXTURE_TEST_CASE(ATOM_missingElement,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,missingElement,12);

        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomNoElementName);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::warning);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == missingElement);


        BOOST_REQUIRE(!ppdb_3.atoms_flatlist.empty());


        BOOST_TEST(ppdb_3.atoms_flatlist.back()->element == "C");

    }

    BOOST_FIXTURE_TEST_CASE(ATOM_badElement,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,badElement,12);

        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomElementNameContainsNonLetter);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == badElement);

    }


    BOOST_FIXTURE_TEST_CASE(ATOM_withCharge,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,withCharge,12);



        // This should generate no out of band records
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 0);


        BOOST_REQUIRE(!ppdb_3.atoms_flatlist.empty());


        BOOST_TEST(ppdb_3.atoms_flatlist.back()->charge == 2.0);

    }

    BOOST_FIXTURE_TEST_CASE(ATOM_withNegCharge,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,withNegCharge,12);



        // This should generate no out of band records
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 0);


        BOOST_REQUIRE(!ppdb_3.atoms_flatlist.empty());


        BOOST_TEST(ppdb_3.atoms_flatlist.back()->charge == -2.0);

    }

    BOOST_FIXTURE_TEST_CASE(ATOM_badCharge,LineFixture_ATOM,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb_3;
        createPlaceholderModelAndChain(ppdb_3);
        pdbpc::parseAtomLine(ppdb_3,badCharge3,12);

        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomCannotParseCharge);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == badCharge3);

        pdbpc::ParsedPDB ppdb_1;
        createPlaceholderModelAndChain(ppdb_1);
        pdbpc::parseAtomLine(ppdb_1,badCharge1,12);

        BOOST_TEST(ppdb_1.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_1.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_1.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_1.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomCannotParseCharge);
        BOOST_TEST(ppdb_1.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_1.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb_1.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_1.outOfBandRecords.back()->line == badCharge1);

        pdbpc::ParsedPDB ppdb_2;
        createPlaceholderModelAndChain(ppdb_2);
        pdbpc::parseAtomLine(ppdb_2,badCharge2,12);

        BOOST_TEST(ppdb_2.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb_2.outOfBandRecords.size() == 1);


        BOOST_TEST(ppdb_2.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomCannotParseCharge);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->line == badCharge2);



    }




    BOOST_FIXTURE_TEST_CASE(ATOM_ResidueTypeUnknown,SimplePDBFixture,*boost::unit_test::timeout(10)){
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(WithUnknownResidueName);

        BOOST_TEST(!ppdb.outOfBandRecords.empty());
        BOOST_REQUIRE(!ppdb.outOfBandRecords.empty());


        BOOST_TEST(ppdb.outOfBandRecords.at(0)->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->subtype == pdbpc::OutOfBandSubType::AtomUnknownResidueThreeLetterCode);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->severity == pdbpc::OutOfBandSeverity::warning);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->recoveryStatus == pdbpc::RecoveryStatus::recovered);


    }


BOOST_AUTO_TEST_SUITE_END()