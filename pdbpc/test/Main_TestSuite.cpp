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

#define BOOST_TEST_MODULE main_test_module
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <cstdio>

#include <boost/test/unit_test.hpp>

#include <pdbpc/pdbpc.h>
#include "../src/Parser/parseEndModelLine.h"
#include "../src/Parser/parseModelLine.h"
#include "../src/Parser/parseAtomLine.h"
#include <pdbpc/Records/OutOfBandRecord.h>
#include "../src/Utility/internalUtils.h"

#include "simplePDBFixture.h"

BOOST_AUTO_TEST_SUITE(Main_testSuite)


    BOOST_FIXTURE_TEST_CASE(SimplePDB_BlockReading,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(SimplePDBBlock);

        BOOST_TEST(ppdb.outOfBandRecords.empty());


        BOOST_TEST(ppdb.models.size() == 2);
        BOOST_TEST(ppdb.atoms_flatlist.size() == 109);
        BOOST_TEST(ppdb.residues_flatlist.size() == 9);
        BOOST_TEST(ppdb.chains_flatlist.size() == 4);
    }

    BOOST_FIXTURE_TEST_CASE(SimplePDB_HierarchyWalking,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(SimplePDBBlock);

        BOOST_TEST(ppdb.outOfBandRecords.empty());


        // Require the test from the blockreading test
        BOOST_REQUIRE(ppdb.models.size() == 2);
        BOOST_REQUIRE(ppdb.atoms_flatlist.size() == 109);
        BOOST_REQUIRE(ppdb.residues_flatlist.size() == 9);
        BOOST_REQUIRE(ppdb.chains_flatlist.size() == 4);


        BOOST_TEST( ppdb.models[0]->atoms_flatlist.size() == 59);
        BOOST_TEST( ppdb.models[1]->atoms_flatlist.size() == 50);

        BOOST_TEST( ppdb.models[0]->residues_flatlist.size() == 5);
        BOOST_TEST( ppdb.models[1]->residues_flatlist.size() == 4);

        BOOST_TEST( ppdb.models[0]->chains.size() == 2);
        BOOST_TEST( ppdb.models[1]->chains.size() == 2);

        BOOST_TEST( ppdb.models[0]->chains[0]->residues.size() == 3);
        BOOST_TEST( ppdb.models[1]->chains[0]->residues.size() == 2);



    }

    BOOST_FIXTURE_TEST_CASE(DuplicatedModelHandling,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(DuplicatedModelPDB);

        BOOST_TEST(ppdb.outOfBandRecords.size() == 1);

        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 1);

        BOOST_TEST(ppdb.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBFileStructure);
        BOOST_TEST(ppdb.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::duplicateModelNumber);

        BOOST_TEST(ppdb.models.size() == 2);
        BOOST_TEST(ppdb.models.at(0)->modelNumber != ppdb.models.at(1)->modelNumber);
    }

    BOOST_FIXTURE_TEST_CASE(DefaultModel_withOtherModel_Handling,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(SimplePDBBlock_noModel1);


        BOOST_TEST(ppdb.models.size() == 2);
        BOOST_TEST(ppdb.models.at(0)->modelNumber != ppdb.models.at(1)->modelNumber);

        BOOST_TEST( ppdb.models[0]->atoms_flatlist.size() == 59);
        BOOST_TEST( ppdb.models[1]->atoms_flatlist.size() == 50);

        BOOST_TEST( ppdb.models[0]->residues_flatlist.size() == 5);
        BOOST_TEST( ppdb.models[1]->residues_flatlist.size() == 4);

        BOOST_TEST( ppdb.models[0]->chains.size() == 2);
        BOOST_TEST( ppdb.models[1]->chains.size() == 2);

        BOOST_TEST( ppdb.models[0]->chains[0]->residues.size() == 3);
        BOOST_TEST( ppdb.models[1]->chains[0]->residues.size() == 2);

    }

    BOOST_FIXTURE_TEST_CASE(DefaultModel_alone_Handling,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(SimplePDBBlock_noModelAtAll);


        BOOST_TEST(ppdb.models.size() == 1);

        BOOST_TEST( ppdb.models[0]->atoms_flatlist.size() == 59);

        BOOST_TEST( ppdb.models[0]->residues_flatlist.size() == 5);

        BOOST_TEST( ppdb.models[0]->chains.size() == 2);

        BOOST_TEST( ppdb.models[0]->chains[0]->residues.size() == 3);

    }

    BOOST_FIXTURE_TEST_CASE(ReadPDBFile_FileNotFound_test,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBFile("./Fakepath_donotexit.pdb");

        BOOST_TEST(!ppdb.outOfBandRecords.empty());
        BOOST_REQUIRE(!ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.hasErrors);

        BOOST_TEST(ppdb.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::FileNotFound);

    }

    BOOST_FIXTURE_TEST_CASE(ReadPDBFile_ExampleFile_test,SimplePDBFixture,* boost::unit_test::timeout(10)) {

        std::string tempFilename = "./TempFile.pdb";
        std::ofstream out(tempFilename);
        out << SimplePDBBlock;
        out.close();

        pdbpc::ParserSettings psetting;
        psetting.doPedanticVerifications = true;

        pdbpc::ParsedPDB ppdb = pdbpc::readPDBFile(tempFilename, psetting);

        BOOST_TEST(ppdb.outOfBandRecords.empty());
        BOOST_REQUIRE(ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.models.size() == 2);
        BOOST_TEST(ppdb.models.at(0)->modelNumber != ppdb.models.at(1)->modelNumber);

        BOOST_TEST( ppdb.models[0]->atoms_flatlist.size() == 59);
        BOOST_TEST( ppdb.models[1]->atoms_flatlist.size() == 50);

        BOOST_TEST( ppdb.models[0]->residues_flatlist.size() == 5);
        BOOST_TEST( ppdb.models[1]->residues_flatlist.size() == 4);

        BOOST_TEST( ppdb.models[0]->chains.size() == 2);
        BOOST_TEST( ppdb.models[1]->chains.size() == 2);

        BOOST_TEST( ppdb.models[0]->chains[0]->residues.size() == 3);
        BOOST_TEST( ppdb.models[1]->chains[0]->residues.size() == 2);

        std::remove(tempFilename.c_str());

    }

    BOOST_FIXTURE_TEST_CASE(HasWarning_test,SimplePDBFixture,* boost::unit_test::timeout(10)) {

        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(WithUnknownResidueName);

        BOOST_TEST(!ppdb.outOfBandRecords.empty());
        BOOST_REQUIRE(!ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.hasWarnings == true);

    }

    BOOST_FIXTURE_TEST_CASE(Handle_END_line_test,SimplePDBFixture,* boost::unit_test::timeout(10)) {

        std::string SimplePDBBlock_w_END = emitMODELline(1) + Model1_content + "ENDMDL\n" + emitMODELline(2) + Model2_content + "ENDMDL\n" + "END\n";


        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(SimplePDBBlock_w_END);

        BOOST_TEST(ppdb.outOfBandRecords.empty());
        BOOST_REQUIRE(ppdb.outOfBandRecords.empty());


    }

    BOOST_FIXTURE_TEST_CASE(Handle_NonExistingLinePrefix_line_test,SimplePDBFixture,* boost::unit_test::timeout(10)) {

        std::string SimplePDBBlock_w_NONSENSE = emitMODELline(1) + Model1_content + "ATTACH 28 Y477 DH\n" + "ENDMDL\n";

        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(SimplePDBBlock_w_NONSENSE);

        BOOST_REQUIRE(!ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.outOfBandRecords.at(0)->type == pdbpc::OutOfBandType::UnhandledPDBLine);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->subtype == pdbpc::OutOfBandSubType::NonRecognizedLinePrefix);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);

    }



BOOST_AUTO_TEST_SUITE_END()