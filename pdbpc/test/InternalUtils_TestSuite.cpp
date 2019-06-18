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

#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;

#include <pdbpc/pdbpc.h>
#include "../src/Parser/parseEndModelLine.h"
#include "../src/Parser/parseModelLine.h"
#include "../src/Parser/parseAtomLine.h"
#include <pdbpc/Records/OutOfBandRecord.h>
#include "../src/Utility/internalUtils.h"
#include "simplePDBFixture.h"


BOOST_AUTO_TEST_SUITE(InternalUtils_testSuite)

    BOOST_AUTO_TEST_CASE(FindNextModel_Test,* boost::unit_test::timeout(10)) {
        auto ref_dummyModel = std::make_shared<pdbpc::Model>();
        ref_dummyModel->modelNumber = 1;
        ref_dummyModel->openingLineNumber = 1;
        ref_dummyModel->openingLine = "MODEL        1";
        ref_dummyModel->closingLineNumber = -1;

        pdbpc::ParsedPDB ppdb_1;
        ppdb_1.models.push_back(ref_dummyModel);

        int nextModel = findNextFreeModelNumber(ppdb_1);

        BOOST_TEST(nextModel == 2);


    }

    BOOST_AUTO_TEST_CASE(trimAfterLastColumn_Test,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb;
        std::string line = "MODEL        1   ";

        trimAfterLastColumn(ppdb,line,23,14);

        BOOST_TEST(line.length() == 14);
        BOOST_TEST(line == "MODEL        1");



    }

    BOOST_AUTO_TEST_CASE(inflateShorterLineOrFail_Test,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb;
        std::string line_ref = "MODEL        1   "; // This is 17 char exactly

        // Starting string too short for minimalRow
        std::string line1 = line_ref; // This is 17 char exactly
        bool ret1 = inflateShorterLineOrFail(ppdb,line1,12,23,80);
        BOOST_TEST(!ret1);

        // Starting string just right for minimalRow
        std::string line2 = line_ref; // This is 17 char exactly
        bool ret2 = inflateShorterLineOrFail(ppdb,line2,12,17,80);
        BOOST_TEST(ret2);

        // Starting string longer than minimalRow
        std::string line3 = line_ref; // This is 17 char exactly
        bool ret3 = inflateShorterLineOrFail(ppdb,line3,12,13,80);
        BOOST_TEST(ret3);

        // Starting string longer than minimalRow, and longer than target lenght, should not be modifier
        std::string line4 = line_ref; // This is 17 char exactly
        bool ret4 = inflateShorterLineOrFail(ppdb,line4,12,13,15);
        BOOST_TEST(ret4);
        BOOST_TEST(line4 == line_ref);

        // Starting string longer than minimalRow, and shorter than target lenght, should be inflated
        std::string line5 = line_ref; // This is 17 char exactly
        bool ret5 = inflateShorterLineOrFail(ppdb,line5,12,13,19);
        BOOST_TEST(ret5);
        BOOST_TEST(line5 != line_ref);
        BOOST_TEST(line5.length() == 19);
        BOOST_TEST(line5 == "MODEL        1     ");



    }

    BOOST_AUTO_TEST_CASE(parseOrError_int_Test,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb;
        std::string line = "DUMMYLINE        1   ";

        std::string intField1 = " 173 ";

        std::optional<int> MaybeInt1 =  pdbpc::parseOrRecoverableError<int>( ppdb,
                line,
                12,
                intField1,
                pdbpc::OutOfBandSubType::GenericFieldParseError);

        BOOST_TEST(MaybeInt1.has_value());
        BOOST_TEST(MaybeInt1.value() == 173);
        BOOST_TEST(ppdb.outOfBandRecords.empty());

        std::string intField2 = " -17423 ";

        std::optional<int> MaybeInt2 =  pdbpc::parseOrRecoverableError<int>( ppdb,
                                                                             line,
                                                                             12,
                                                                             intField2,
                                                                             pdbpc::OutOfBandSubType::GenericFieldParseError);

        BOOST_TEST(MaybeInt2.has_value());
        BOOST_TEST(MaybeInt2.value() == -17423);
        BOOST_TEST(ppdb.outOfBandRecords.empty());

        std::string intField3 = " -17423.44 ";

        std::optional<int> MaybeInt3 =  pdbpc::parseOrRecoverableError<int>( ppdb,
                                                                             line,
                                                                             12,
                                                                             intField3,
                                                                             pdbpc::OutOfBandSubType::GenericFieldParseError);

        BOOST_TEST(!MaybeInt3.has_value());
        BOOST_TEST(ppdb.outOfBandRecords.size() == 1);


    }


    BOOST_AUTO_TEST_CASE(parseOrError_double_Test,* boost::unit_test::tolerance(0.00001)) {
        pdbpc::ParsedPDB ppdb;
        std::string line = "DUMMYLINE        1   ";

        std::string floatField1 = " 173 ";

        std::optional<double> MaybeFloat1 =  pdbpc::parseOrRecoverableError<double>( ppdb,
                                                                                     line,
                                                                                     12,
                                                                                     floatField1,
                                                                                     pdbpc::OutOfBandSubType::GenericFieldParseError);

        BOOST_TEST(MaybeFloat1.has_value());
        BOOST_TEST(MaybeFloat1.value() == 173.0);
        BOOST_TEST(ppdb.outOfBandRecords.empty());

        std::string floatField2 = " -173.4 ";

        std::optional<double> MaybeFloat2 =  pdbpc::parseOrRecoverableError<double>( ppdb,
                                                                                     line,
                                                                                     12,
                                                                                     floatField2,
                                                                                     pdbpc::OutOfBandSubType::GenericFieldParseError);

        BOOST_TEST(MaybeFloat2.has_value());
        BOOST_TEST(MaybeFloat2.value() == -173.4);
        BOOST_TEST(ppdb.outOfBandRecords.empty());


    }

    BOOST_AUTO_TEST_CASE(parseOrError_RecoverableUnrecoverable_Test,* boost::unit_test::tolerance(0.00001)) {
        pdbpc::ParsedPDB ppdb1;
        std::string line = "DUMMYLINE        1   ";

        std::string floatField1 = " 17ehf3 ";

        std::optional<double> MaybeFloat1 =  pdbpc::parseOrRecoverableError<double>( ppdb1,
                                                                                     line,
                                                                                     12,
                                                                                     floatField1,
                                                                                     pdbpc::OutOfBandSubType::GenericFieldParseError);

        BOOST_TEST(!MaybeFloat1.has_value());
        BOOST_TEST(!ppdb1.outOfBandRecords.empty());
        BOOST_REQUIRE(!ppdb1.outOfBandRecords.empty());

        BOOST_TEST(ppdb1.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);

        pdbpc::ParsedPDB ppdb2;

        std::optional<double> MaybeFloat2 =  pdbpc::parseOrUnrecoverableError<double>( ppdb2,
                                                                                     line,
                                                                                     12,
                                                                                     floatField1,
                                                                                     pdbpc::OutOfBandSubType::GenericFieldParseError);

        BOOST_TEST(!MaybeFloat2.has_value());
        BOOST_TEST(!ppdb2.outOfBandRecords.empty());
        BOOST_REQUIRE(!ppdb2.outOfBandRecords.empty());

        BOOST_TEST(ppdb2.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);


    }

    BOOST_FIXTURE_TEST_CASE(CheckPDBPath_test,SimplePDBFixture,* boost::unit_test::timeout(10)) {



        std::string tempFilename = "./TempFile.notpdb";
        std::string tempDirectoryName = "./TmpDirectory";

        bfs::remove(tempFilename);
        bfs::remove(tempDirectoryName);

        std::ofstream out(tempFilename);
        out << SimplePDBBlock;
        out.close();

        bfs::create_directory(tempDirectoryName);

        std::function<void(void)> cleanUp = [tempFilename,tempDirectoryName]() {
            bfs::remove(tempFilename);
            bfs::remove(tempDirectoryName);
        };

        pdbpc::ParsedPDB ppdb1;

        bool res1 = pdbpc::checkPDBPath(ppdb1,"./NonExistant");

        BOOST_TEST(!ppdb1.outOfBandRecords.empty());


        BOOST_TEST(ppdb1.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::FileNotFound);

        pdbpc::ParsedPDB ppdb2;

        bool res2 = pdbpc::checkPDBPath(ppdb2,tempFilename);

        BOOST_TEST(res2);

        BOOST_TEST(!ppdb2.outOfBandRecords.empty());

        BOOST_TEST(ppdb2.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::FileExtensionNotPDB);

        pdbpc::ParsedPDB ppdb3;

        bool res3 = pdbpc::checkPDBPath(ppdb3,tempDirectoryName);

        BOOST_TEST(!res3);

        BOOST_TEST(!ppdb3.outOfBandRecords.empty());

        BOOST_TEST(ppdb3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::FilePathIsDirectory);

        cleanUp();

    }

    BOOST_FIXTURE_TEST_CASE(Internal_DeleteAtomFromHierarchy,SimplePDBFixture,*boost::unit_test::timeout(10)){

        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(smallATOMBlock);


        BOOST_TEST(ppdb.atoms_flatlist.size() == 9);
        BOOST_TEST(ppdb.models.at(0)->atoms_flatlist.size() == 9);
        BOOST_REQUIRE(ppdb.atoms_flatlist.size() != 0);


        deleteAtomFromHierarchy(ppdb,ppdb.atoms_flatlist.at(0));



        BOOST_TEST(ppdb.atoms_flatlist.size() == 8);
        BOOST_TEST(ppdb.models.at(0)->atoms_flatlist.size() == 8);
        BOOST_TEST(ppdb.models.at(0)->chains.at(0)->atoms_flatlist.size() == 8);
        BOOST_TEST(ppdb.models.at(0)->chains.at(0)->residues.at(0)->atoms.size() == 8);

    }


BOOST_AUTO_TEST_SUITE_END()