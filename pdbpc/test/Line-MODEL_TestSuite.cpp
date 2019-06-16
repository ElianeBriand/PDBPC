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

struct LineFixture_MODEL {
    LineFixture_MODEL() {

    }

    ~LineFixture_MODEL() {
    }

    std::string conformingLine = "MODEL        1";
    std::string NonConformingLine_space = "MODEL        1   ";
    std::string NonConformingLine_junk = "MODEL        1dsfsg";
    std::string NonConformingLine_shorterField = "MODEL   1";
};

BOOST_AUTO_TEST_SUITE(Line_testSuite)

    BOOST_FIXTURE_TEST_CASE(MODEL_CorrectLine, LineFixture_MODEL, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb_1;
        pdbpc::parseModelLine(ppdb_1, conformingLine, 12);

        // This should generate no out of band records
        BOOST_TEST(ppdb_1.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb_1.outOfBandRecords.size() == 0);

        // Correctly set opening line number and line
        BOOST_TEST(ppdb_1.models.back()->openingLineNumber == 12);
        BOOST_TEST(ppdb_1.models.back()->openingLine == conformingLine);

        // Closing lines as expected
        BOOST_TEST(ppdb_1.models.back()->closingLineNumber == -1);
    }

    BOOST_FIXTURE_TEST_CASE(MODEL_NonConforming_space, LineFixture_MODEL, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb_2;
        pdbpc::parseModelLine(ppdb_2, NonConformingLine_space, 12);

        // This should generate one out of band record
        BOOST_TEST(ppdb_2.outOfBandRecords.size() == 1);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(
                ppdb_2.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::SupernumerarySpaceAfterLastColumn);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::idiosyncrasy);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb_2.outOfBandRecords.back()->lineNumber == 12);

        // Correctly set opening line number and line
        BOOST_TEST(ppdb_2.models.back()->openingLineNumber == 12);
        BOOST_TEST(ppdb_2.models.back()->openingLine == NonConformingLine_space);

        // Closing lines as expected
        BOOST_TEST(ppdb_2.models.back()->closingLineNumber == -1);
    }

    BOOST_FIXTURE_TEST_CASE(MODEL_NonConforming_junk, LineFixture_MODEL, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb_3;
        pdbpc::parseModelLine(ppdb_3, NonConformingLine_junk, 12);

        // This should generate one out of band record
        BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype ==
                   pdbpc::OutOfBandSubType::SupernumeraryContentAfterLastColumn);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == NonConformingLine_junk);

    }

    BOOST_FIXTURE_TEST_CASE(MODEL_NonConforming_ShorterField, LineFixture_MODEL, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb_4;
        pdbpc::parseModelLine(ppdb_4, NonConformingLine_shorterField, 12);

        // This should generate one out of band record
        BOOST_TEST(ppdb_4.outOfBandRecords.size() == 1);
        BOOST_TEST(ppdb_4.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb_4.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::NonCompliantColumnWidth);
        BOOST_TEST(ppdb_4.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb_4.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb_4.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb_4.outOfBandRecords.back()->line == NonConformingLine_shorterField);

        // Correctly set opening line number and line
        BOOST_TEST(ppdb_4.models.back()->openingLineNumber == 12);
        BOOST_TEST(ppdb_4.models.back()->openingLine == NonConformingLine_shorterField);

        // Closing lines as expected
        BOOST_TEST(ppdb_4.models.back()->closingLineNumber == -1);
    }

BOOST_AUTO_TEST_SUITE_END()