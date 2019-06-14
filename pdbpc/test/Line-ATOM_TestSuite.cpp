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
#include <pdbpc/Parser/parseEndModelLine.h>
#include <pdbpc/Parser/parseModelLine.h>
#include <pdbpc/Parser/parseAtomLine.h>
#include <pdbpc/Records/OutOfBandRecord.h>
#include <pdbpc/Utility/internalUtils.h>

struct LineFixture_ATOM {
    LineFixture_ATOM()
    {

    }
    ~LineFixture_ATOM()
    {
    }

    std::string conformingLine = "ATOM   1786  OE1 GLN B  92      27.358  20.610 -10.667  1.00 20.49           O  ";
    std::string NonConformingLine_degraded = "ATOM   1787  NE2 GLN B  92      27.449  19.137  -8.988";
    std::string NonConformingLine_junk = "ATOM   1787  NE2 GLN B  92      27.nop  19.137  -8.988";
};

BOOST_AUTO_TEST_SUITE(Line_testSuite)

    BOOST_FIXTURE_TEST_CASE(ATOM_CorrectLine,LineFixture_ATOM,*boost::unit_test::timeout(10)) {
            pdbpc::ParsedPDB ppdb_1;
            createPlaceholderModelAndChain(ppdb_1);
            pdbpc::parseAtomLine(ppdb_1,conformingLine,12);

            // This should generate no out of band records
            BOOST_TEST(ppdb_1.outOfBandRecords.size() == 0);
            BOOST_REQUIRE(ppdb_1.outOfBandRecords.size() == 0);

            // Correctly set record
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->lineNumber == 12);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->line == conformingLine);


            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->serialNumber == 1786);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->atomName == "OE1");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->alternateLocationIdentifier == "");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueType == pdbpc::ResidueType::glutamine);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueName == "GLN");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->chainIdentifier == "B");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueSequenceNumber == 92);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->x == 27.358);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->y == 20.610);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->z == -10.667);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueInsertionCode == "");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->occupancy == 1.0);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->temperatureFactor == 20.49);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->charge == 0.0);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->element == "O");


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

            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->serialNumber == 1787);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->atomName == "NE2");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->alternateLocationIdentifier == "");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueType == pdbpc::ResidueType::glutamine);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueName == "GLN");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->chainIdentifier == "B");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueSequenceNumber == 92);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->x == 27.449);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->y == 19.137);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->z == -8.988);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueInsertionCode == "");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->occupancy == 1.0);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->temperatureFactor == 0.0);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->charge == 0.0);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->element == "N");

        }

    BOOST_FIXTURE_TEST_CASE(ATOM_NonConforming_junk,LineFixture_ATOM,*boost::unit_test::timeout(10)){
            pdbpc::ParsedPDB ppdb_3;
            createPlaceholderModelAndChain(ppdb_3);
            pdbpc::parseAtomLine(ppdb_3,NonConformingLine_junk,12);

            // This should generate no out of band records
            BOOST_TEST(ppdb_3.outOfBandRecords.size() == 2);
            BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 2);

            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->subtype == pdbpc::OutOfBandSubType::AtomDegradedModeParsingEvent);
            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->severity == pdbpc::OutOfBandSeverity::error);
            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->recoveryStatus == pdbpc::RecoveryStatus::recovered);
            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->lineNumber == 12);
            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->line == NonConformingLine_junk);

            BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomCannotParseCoordinates);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == NonConformingLine_junk);


        }


BOOST_AUTO_TEST_SUITE_END()