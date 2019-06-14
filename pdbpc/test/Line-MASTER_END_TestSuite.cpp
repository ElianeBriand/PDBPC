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
#include <pdbpc/Parser/parseMasterEndLine.h>
#include <pdbpc/Records/OutOfBandRecord.h>
#include <pdbpc/Utility/internalUtils.h>

struct LineFixture_MASTER_END {
    LineFixture_MASTER_END() {

    }

    ~LineFixture_MASTER_END() {
    }

    std::string conformingLine = "MASTER      312    0    1    2   16    0    5    6 1670    2   50   16          ";
    std::string NonConformingLine_short = "MASTER      312    0    1    2   16    0    5    6 1670    2   50   16";
    std::string NonConformingLine_space = "ENDMDL ";
    std::string NonConformingLine_junk = "ENDMDLDHJJD";

    std::shared_ptr<pdbpc::Model> ref_dummyModel;
};

BOOST_AUTO_TEST_SUITE(Line_testSuite)

    BOOST_FIXTURE_TEST_CASE(MASTER_CorrectLine, LineFixture_MASTER_END, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb_1;
        createPlaceholderModelAndChain(ppdb_1);
        pdbpc::parseMasterLine(ppdb_1,conformingLine,12);


        // This should generate no out of band records
        BOOST_TEST(ppdb_1.outOfBandRecords.empty());
        BOOST_REQUIRE(ppdb_1.outOfBandRecords.empty());

        // Correctly set line number and line
        BOOST_TEST(ppdb_1.details.master.lineNumber == 12);
        BOOST_TEST(ppdb_1.details.master.line == conformingLine);

        // Currectly set attributes
        BOOST_TEST(ppdb_1.details.master.numRemark == 312);
        BOOST_TEST(ppdb_1.details.master.mandatedZero == 0);
        BOOST_TEST(ppdb_1.details.master.numHet == 1);
        BOOST_TEST(ppdb_1.details.master.numHelix == 2);
        BOOST_TEST(ppdb_1.details.master.numSheet == 16);
        BOOST_TEST(ppdb_1.details.master.numTurn == 0);
        BOOST_TEST(ppdb_1.details.master.numSite == 5);
        BOOST_TEST(ppdb_1.details.master.numXform == 6);
        BOOST_TEST(ppdb_1.details.master.numCoord == 1670);
        BOOST_TEST(ppdb_1.details.master.numTer == 2);
        BOOST_TEST(ppdb_1.details.master.numConect == 50);
        BOOST_TEST(ppdb_1.details.master.numSeq == 16);
    }

    BOOST_FIXTURE_TEST_CASE(MASTER_NonConforming_space, LineFixture_MASTER_END, *boost::unit_test::timeout(10)) {

    }

    BOOST_FIXTURE_TEST_CASE(MASTER_NonConforming_junk, LineFixture_MASTER_END, *boost::unit_test::timeout(10)) {

    }

BOOST_AUTO_TEST_SUITE_END()