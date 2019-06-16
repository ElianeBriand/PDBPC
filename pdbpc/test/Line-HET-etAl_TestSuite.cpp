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
#include "../src/Parser/parseMasterEndLine.h"
#include <pdbpc/Records/OutOfBandRecord.h>
#include "../src/Utility/internalUtils.h"

struct LineFixture_HET_etAl {
    LineFixture_HET_etAl() {

    }

    ~LineFixture_HET_etAl() {
    }

    std::string conformingLine = "HET    RIT  B 301      50                                                       ";
    std::string NonConformingLine_short = "HET    RIT  B 301      50";
    std::string NonConformingLine_space = "ENDMDL ";
    std::string NonConformingLine_junk = "ENDMDLDHJJD";

    std::shared_ptr<pdbpc::Model> ref_dummyModel;
};

BOOST_AUTO_TEST_SUITE(Line_testSuite)

    BOOST_FIXTURE_TEST_CASE(HET_CorrectLine, LineFixture_HET_etAl, *boost::unit_test::timeout(10)) {

    }
BOOST_AUTO_TEST_SUITE_END()
