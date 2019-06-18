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


struct LineFixture_ENDMDL {
    LineFixture_ENDMDL() {
        ref_dummyModel = std::make_shared<pdbpc::Model>();
        ref_dummyModel->openingLineNumber = 1;
        ref_dummyModel->openingLine = "MODEL        1";
        ref_dummyModel->closingLineNumber = -1;
    }

    ~LineFixture_ENDMDL() {
    }

    std::string conformingLine = "ENDMDL";
    std::string NonConformingLine_space = "ENDMDL ";
    std::string NonConformingLine_junk = "ENDMDLDHJJD";

    std::shared_ptr<pdbpc::Model> ref_dummyModel;
};

BOOST_AUTO_TEST_SUITE(Line_testSuite)

    BOOST_FIXTURE_TEST_CASE(ENDMDL_CorrectLine, LineFixture_ENDMDL, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb_1;
        auto dummyModel_1 = std::make_shared<pdbpc::Model>(*ref_dummyModel);
        ppdb_1.models.push_back(dummyModel_1);
        pdbpc::parseEndModelLine(ppdb_1, conformingLine, 12);

        // This should generate no out of band records
        BOOST_TEST(ppdb_1.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb_1.outOfBandRecords.size() == 0);

        // Correctly set closing line number and line
        BOOST_TEST(ppdb_1.models.back()->closingLineNumber == 12);
        BOOST_TEST(ppdb_1.models.back()->closingLine == conformingLine);

        // Opening lines untouched
        BOOST_TEST(ppdb_1.models.back()->openingLineNumber == ref_dummyModel->openingLineNumber);
        BOOST_TEST(ppdb_1.models.back()->openingLine == ref_dummyModel->openingLine);
    }

    BOOST_FIXTURE_TEST_CASE(ENDMDL_NonConforming_space, LineFixture_ENDMDL, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb_2;
        auto dummyModel_2 = std::make_shared<pdbpc::Model>(*ref_dummyModel);
        ppdb_2.models.push_back(dummyModel_2);
        pdbpc::parseEndModelLine(ppdb_2, NonConformingLine_space, 12);

        // This should generate one out of band record
        BOOST_TEST(ppdb_2.outOfBandRecords.size() == 1);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(
                ppdb_2.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::SupernumerarySpaceAfterLastColumn);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::idiosyncrasy);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->line == NonConformingLine_space);

        // Correctly set closing line number and line
        BOOST_TEST(ppdb_2.models.back()->closingLineNumber == 12);
        BOOST_TEST(ppdb_2.models.back()->closingLine == NonConformingLine_space);

        // Opening lines untouched
        BOOST_TEST(ppdb_2.models.back()->openingLineNumber == ref_dummyModel->openingLineNumber);
        BOOST_TEST(ppdb_2.models.back()->openingLine == ref_dummyModel->openingLine);
    }

    BOOST_FIXTURE_TEST_CASE(ENDMDL_NonConforming_junk, LineFixture_ENDMDL, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb_3;
        auto dummyModel_3 = std::make_shared<pdbpc::Model>(*ref_dummyModel);
        ppdb_3.models.push_back(dummyModel_3);
        pdbpc::parseEndModelLine(ppdb_3, NonConformingLine_junk, 12);

        // This should generate one out of band record
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype ==
                   pdbpc::OutOfBandSubType::SupernumeraryContentAfterLastColumn);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == NonConformingLine_junk);

        // Correctly set closing line number and line
        BOOST_TEST(ppdb_3.models.back()->closingLineNumber == 12);
        BOOST_TEST(ppdb_3.models.back()->closingLine == NonConformingLine_junk);

        // Opening lines untouched
        BOOST_TEST(ppdb_3.models.back()->openingLineNumber == ref_dummyModel->openingLineNumber);
        BOOST_TEST(ppdb_3.models.back()->openingLine == ref_dummyModel->openingLine);
    }

    BOOST_FIXTURE_TEST_CASE(ENDMDL_ClosingWhileNoModelAreOpened, SimplePDBFixture, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(IncorrectENDML_NoModelOpen);

        BOOST_TEST(ppdb.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 1);

        BOOST_TEST(ppdb.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBFileStructure);
        BOOST_TEST(ppdb.outOfBandRecords.back()->subtype ==
                   pdbpc::OutOfBandSubType::EndMdlWithoutOpeningModelStatement);



    }

    BOOST_FIXTURE_TEST_CASE(ENDMDL_ModelDoubleClose, SimplePDBFixture, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb1 = pdbpc::readPDBBlock(IncorrectENDML_ModelDoubleClose1);


        BOOST_TEST(ppdb1.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb1.outOfBandRecords.size() == 1);

        BOOST_TEST(ppdb1.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBFileStructure);
        BOOST_TEST(ppdb1.outOfBandRecords.back()->subtype ==
                   pdbpc::OutOfBandSubType::EndMdlModelDoubleClose);

        pdbpc::ParsedPDB ppdb2 = pdbpc::readPDBBlock(IncorrectENDML_ModelDoubleClose2);

        BOOST_TEST(ppdb2.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb2.outOfBandRecords.size() == 1);

        BOOST_TEST(ppdb2.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBFileStructure);
        BOOST_TEST(ppdb2.outOfBandRecords.back()->subtype ==
                   pdbpc::OutOfBandSubType::EndMdlModelDoubleClose);


    }



BOOST_AUTO_TEST_SUITE_END()