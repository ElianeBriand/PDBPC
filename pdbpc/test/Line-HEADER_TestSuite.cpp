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

#include <boost/date_time.hpp>

#include <pdbpc/pdbpc.h>
#include "../src/Parser/parseHeaderLine.h"


#include "simplePDBFixture.h"


struct LineFixture_HEADER {
    LineFixture_HEADER() {
    }

    ~LineFixture_HEADER() {
    }

    std::string conformingLine = "HEADER    HYDROLASE/HYDROLASE INHIBITOR           24-JAN-97   1HXW              ";
    std::string NonConformingLine_tooShort = "HEADER    HYDROLASE/HYDROLASE INHIBITOR           24-PRR-97   1H";

    std::string NonConformingLine_junkDate_1 = "HEADER    HYDROLASE/HYDROLASE INHIBITOR           24-PRR-97   1HXW              ";
    std::string NonConformingLine_junkDate_2 = "HEADER    HYDROLASE/HYDROLASE INHIBITOR           66-JAN-97   1HXW              ";


};

BOOST_AUTO_TEST_SUITE(Line_testSuite)

    BOOST_FIXTURE_TEST_CASE(HEADER_CorrectLine, LineFixture_HEADER, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb;
        pdbpc::parseHeaderLine(ppdb, conformingLine, 12);
        for(const auto& record: ppdb.outOfBandRecords) {
            record->printRecord();
        }
        BOOST_TEST(ppdb.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.details.PDBCode == "1HXW");
        BOOST_TEST(ppdb.details.classification == "HYDROLASE/HYDROLASE INHIBITOR");
        BOOST_TEST(ppdb.details.depositionDate == boost::gregorian::date(1997,boost::gregorian::Jan,24));

    }

    BOOST_FIXTURE_TEST_CASE(HEADER_CompleteBlock, SimplePDBFixture, *boost::unit_test::timeout(10)) {
        std::string SimplePDBBlock_w_HEADER = "HEADER    HYDROLASE/HYDROLASE INHIBITOR           24-JAN-97   1HXW              \n" +
                emitMODELline(1) + Model1_content + "ENDMDL\n" + "END\n";

        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(SimplePDBBlock_w_HEADER);




        BOOST_TEST(ppdb.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.details.PDBCode == "1HXW");
        BOOST_TEST(ppdb.details.classification == "HYDROLASE/HYDROLASE INHIBITOR");
        BOOST_TEST(ppdb.details.depositionDate == boost::gregorian::date(1997,boost::gregorian::Jan,24));
    }

    BOOST_FIXTURE_TEST_CASE(HEADER_JunkDate, LineFixture_HEADER, *boost::unit_test::timeout(10)) {
        {
            pdbpc::ParsedPDB ppdb;
            pdbpc::parseHeaderLine(ppdb, NonConformingLine_junkDate_1, 12);

            BOOST_TEST(ppdb.outOfBandRecords.size() == 1);
            BOOST_REQUIRE(!ppdb.outOfBandRecords.empty());

            BOOST_TEST(ppdb.details.PDBCode == "1HXW");
            BOOST_TEST(ppdb.details.classification == "HYDROLASE/HYDROLASE INHIBITOR");
            BOOST_TEST(ppdb.details.depositionDate == boost::gregorian::date(boost::date_time::not_a_date_time));

            BOOST_TEST(ppdb.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::DateCannotBeParsed);
            BOOST_TEST(ppdb.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
            BOOST_TEST(ppdb.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
            BOOST_TEST(ppdb.outOfBandRecords.back()->lineNumber == 12);
            BOOST_TEST(ppdb.outOfBandRecords.back()->line == NonConformingLine_junkDate_1);
        }

        {
            pdbpc::ParsedPDB ppdb;
            pdbpc::parseHeaderLine(ppdb, NonConformingLine_junkDate_2, 12);

            BOOST_TEST(ppdb.outOfBandRecords.size() == 1);
            BOOST_REQUIRE(!ppdb.outOfBandRecords.empty());

            BOOST_TEST(ppdb.details.PDBCode == "1HXW");
            BOOST_TEST(ppdb.details.classification == "HYDROLASE/HYDROLASE INHIBITOR");
            BOOST_TEST(ppdb.details.depositionDate == boost::gregorian::date(boost::date_time::not_a_date_time));

            BOOST_TEST(ppdb.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::DateCannotBeParsed);
            BOOST_TEST(ppdb.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
            BOOST_TEST(ppdb.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
            BOOST_TEST(ppdb.outOfBandRecords.back()->lineNumber == 12);
            BOOST_TEST(ppdb.outOfBandRecords.back()->line == NonConformingLine_junkDate_2);

        }
    }

    BOOST_FIXTURE_TEST_CASE(HEADER_tooShort, LineFixture_HEADER, *boost::unit_test::timeout(10)) {

        pdbpc::ParsedPDB ppdb;
        pdbpc::parseHeaderLine(ppdb, NonConformingLine_tooShort, 12);

        BOOST_TEST(ppdb.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(!ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.details.PDBCode == "");
        BOOST_TEST(ppdb.details.classification == "");
        BOOST_TEST(ppdb.details.depositionDate == boost::gregorian::date(boost::date_time::not_a_date_time));

        BOOST_TEST(ppdb.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::HeaderRecordTooShort);
        BOOST_TEST(ppdb.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb.outOfBandRecords.back()->lineNumber == 12);
        BOOST_TEST(ppdb.outOfBandRecords.back()->line == NonConformingLine_tooShort);
    }

BOOST_AUTO_TEST_SUITE_END()