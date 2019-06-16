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


BOOST_AUTO_TEST_SUITE_END()