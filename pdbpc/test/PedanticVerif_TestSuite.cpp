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

#include "../src/Parser/PostProcessing/pedanticVerifications.h"

BOOST_AUTO_TEST_SUITE(Pedantic_testSuite)

    BOOST_AUTO_TEST_CASE(Pedantic_call_Test) {
        pdbpc::ParsedPDB ppdb;

        pdbpc::doPedanticVerifications(ppdb);
    }

BOOST_AUTO_TEST_SUITE_END()