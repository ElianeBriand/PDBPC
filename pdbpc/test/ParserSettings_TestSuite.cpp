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


#include <pdbpc/pdbpc.h>

#include <pdbpc/Records/OutOfBandRecord.h>

#include "simplePDBFixture.h"


BOOST_AUTO_TEST_SUITE(ParserSettings_testSuite)

    BOOST_FIXTURE_TEST_CASE(DuplicateModelPolicy_Test,SimplePDBFixture,* boost::unit_test::timeout(10)) {

        pdbpc::ParserSettings psetting;
        psetting.duplicateModelPolicy = pdbpc::DuplicateModelPolicy::dropDuplicate;

        pdbpc::ParsedPDB ppdb1 = pdbpc::readPDBBlock(DuplicatedModelPDB, psetting);

        BOOST_TEST(ppdb1.models.size() == 1);
        BOOST_TEST(ppdb1.models.at(0)->modelNumber == 2);

        psetting.duplicateModelPolicy = pdbpc::DuplicateModelPolicy::renumberAfterParsingCompletion;

        pdbpc::ParsedPDB ppdb2 = pdbpc::readPDBBlock(DuplicatedModelPDB, psetting);

        BOOST_TEST(ppdb2.models.size() == 2);
        BOOST_TEST(ppdb2.models.at(0)->modelNumber == 2);
        BOOST_TEST(ppdb2.models.at(1)->modelNumber == 1);



    }

    BOOST_FIXTURE_TEST_CASE(PedanticVerif_Test,SimplePDBFixture,* boost::unit_test::timeout(10)) {

        pdbpc::ParserSettings psetting;
        psetting.doPedanticVerifications = true;

        pdbpc::ParsedPDB ppdb1 = pdbpc::readPDBBlock(Pedantic_ModelNoSameContent, psetting);

        // Check pedantic error
        // When pedantic check are implemented. TODO



    }





BOOST_AUTO_TEST_SUITE_END()