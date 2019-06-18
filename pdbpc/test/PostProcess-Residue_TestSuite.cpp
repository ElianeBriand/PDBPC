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

#include <boost/test/output_test_stream.hpp>
using b_ostream = boost::test_tools::output_test_stream;

#include "simplePDBFixture.h"

#include <pdbpc/pdbpc.h>

#include <pdbpc/Records/OutOfBandRecord.h>

BOOST_AUTO_TEST_SUITE(PostProcess_testSuite)

    BOOST_FIXTURE_TEST_CASE(Residue_IncorrectInsertionCode_Test,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(incorrectResidueInsertionCode);

        BOOST_TEST(ppdb.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 1);

        BOOST_TEST(ppdb.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBFileStructure);
        BOOST_TEST(ppdb.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::ResidueInconsistentInsertionCode);


    }

    BOOST_FIXTURE_TEST_CASE(Residue_IncorrectName_Test,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(incorrectResidueName);

        BOOST_TEST(ppdb.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 1);

        BOOST_TEST(ppdb.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBFileStructure);
        BOOST_TEST(ppdb.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::ResidueInconsistentResidueName);

    }

    BOOST_FIXTURE_TEST_CASE(Residue_IncorrectNameAndInsCode_Test,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(incorrectResidueNameAndInsertionCode);

        BOOST_TEST(ppdb.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 1);

        BOOST_TEST(ppdb.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBFileStructure);
        BOOST_TEST(ppdb.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::ResidueInconsistentInsertionCodeAndResidueName);

    }


BOOST_AUTO_TEST_SUITE_END()