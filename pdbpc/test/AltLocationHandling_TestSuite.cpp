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


BOOST_AUTO_TEST_SUITE(AltLocation_testSuite)

    BOOST_FIXTURE_TEST_CASE(AltLoc_GeneralSmokeTest,SimplePDBFixture,*boost::unit_test::timeout(10)){

        pdbpc::ParserSettings settings;
        settings.altLocationPolicy = pdbpc::AltLocationPolicy::highestOccupancy;

        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(smallATOMBlock_WithAltLoc, settings);

        BOOST_TEST(ppdb.outOfBandRecords.empty());
        BOOST_REQUIRE(ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.hasAlternateLocation);
        BOOST_TEST(ppdb.details.alternateLocationWereRemoved);

        BOOST_TEST(ppdb.atoms_flatlist.size() == 9);

        BOOST_REQUIRE(ppdb.atoms_flatlist.size() != 0);

        BOOST_TEST(ppdb.atoms_flatlist.at(0)->alternateLocationIdentifier == "B");



    }

    BOOST_FIXTURE_TEST_CASE(AltLoc_highestOccupancy,SimplePDBFixture,*boost::unit_test::timeout(10)){

        pdbpc::ParserSettings settings;
        settings.altLocationPolicy = pdbpc::AltLocationPolicy::highestOccupancy;

        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(smallATOMBlock_WithAltLoc, settings);

        BOOST_TEST(ppdb.outOfBandRecords.empty());
        BOOST_REQUIRE(ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.hasAlternateLocation);
        BOOST_TEST(ppdb.details.alternateLocationWereRemoved);

        BOOST_TEST(ppdb.atoms_flatlist.size() == 9);

        BOOST_REQUIRE(ppdb.atoms_flatlist.size() != 0);

        // This checks that the ATOM line are the correct one (abitrary, see the fixture)
        BOOST_TEST(ppdb.atoms_flatlist.at(0)->temperatureFactor == 36.25);
        BOOST_TEST(ppdb.atoms_flatlist.at(0)->temperatureFactor != 36.45);

        for(const auto& atom: ppdb.atoms_flatlist) {
            BOOST_TEST(atom->alternateLocationIdentifier == "B");
        }

    }

    BOOST_FIXTURE_TEST_CASE(AltLoc_firstAppearing,SimplePDBFixture,*boost::unit_test::timeout(10)){

        pdbpc::ParserSettings settings;
        settings.altLocationPolicy = pdbpc::AltLocationPolicy::firstAppearing;

        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(smallATOMBlock_WithAltLoc, settings);

        BOOST_TEST(ppdb.outOfBandRecords.empty());
        BOOST_REQUIRE(ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.hasAlternateLocation);
        BOOST_TEST(ppdb.details.alternateLocationWereRemoved);

        BOOST_TEST(ppdb.atoms_flatlist.size() == 9);

        BOOST_REQUIRE(ppdb.atoms_flatlist.size() != 0);

        // This checks that the ATOM line are the correct one (abitrary, see the fixture)
        BOOST_TEST(ppdb.atoms_flatlist.at(0)->temperatureFactor != 36.25);
        BOOST_TEST(ppdb.atoms_flatlist.at(0)->temperatureFactor == 36.45);

        for(const auto& atom: ppdb.atoms_flatlist) {
            BOOST_TEST(atom->alternateLocationIdentifier == "A");
        }

    }

    BOOST_FIXTURE_TEST_CASE(AltLoc_LeaveAsIs,SimplePDBFixture,*boost::unit_test::timeout(10)){

        pdbpc::ParserSettings settings;
        settings.altLocationPolicy = pdbpc::AltLocationPolicy::leaveAsIs;

        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(smallATOMBlock_WithAltLoc, settings);

        BOOST_TEST(ppdb.outOfBandRecords.empty());
        BOOST_REQUIRE(ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.hasAlternateLocation);
        BOOST_TEST(!ppdb.details.alternateLocationWereRemoved);

        BOOST_TEST(ppdb.atoms_flatlist.size() == 18);

        BOOST_REQUIRE(ppdb.atoms_flatlist.size() != 0);

        BOOST_TEST(ppdb.atoms_flatlist.at(0)->alternateLocationIdentifier == "A");
        BOOST_TEST(ppdb.atoms_flatlist.at(9)->alternateLocationIdentifier == "B");

    }

BOOST_AUTO_TEST_SUITE_END()