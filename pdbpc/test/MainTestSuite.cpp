//
// Created by eliane on 11/05/19.
//

#define BOOST_TEST_MODULE main_test_module
#define BOOST_TEST_MAIN


#include <boost/test/unit_test.hpp>

#include <pdbpc/pdbpc.h>
#include <pdbpc/Parser/parseEndModelLine.h>
#include <pdbpc/Records/OutOfBandRecord.h>

BOOST_AUTO_TEST_SUITE(Main_testSuite)

    BOOST_AUTO_TEST_CASE(LineParser_ENDMDL,* boost::unit_test::timeout(10)) {
        std::string conformingLine = "ENDMDL";
        std::string NonConformingLine_space = "ENDMDL ";
        std::string NonConformingLine_junk = "ENDMDLDHJJD";


        auto ref_dummyModel = std::make_shared<pdbpc::Model>();
        ref_dummyModel->openingLineNumber = 1;
        ref_dummyModel->openingLine = "MODEL     1";
        ref_dummyModel->closingLineNumber = -1;

        {
            pdbpc::ParsedPDB ppdb_1;
            auto dummyModel_1 = std::make_shared<pdbpc::Model>(*ref_dummyModel);
            ppdb_1.models.push_back(dummyModel_1);
            pdbpc::parseEndModelLine(ppdb_1,conformingLine,12);

            // This should generate no out of band records
            BOOST_TEST(ppdb_1.outOfBandRecords.size() == 0);

            // Correctly set closing line number and line
            BOOST_TEST(ppdb_1.models.back()->closingLineNumber == 12);
            BOOST_TEST(ppdb_1.models.back()->closingLine == conformingLine);

            // Opening lines untouched
            BOOST_TEST(ppdb_1.models.back()->openingLineNumber == ref_dummyModel->openingLineNumber);
            BOOST_TEST(ppdb_1.models.back()->openingLine == ref_dummyModel->openingLine);
        }

        {
            pdbpc::ParsedPDB ppdb_2;
            auto dummyModel_2 = std::make_shared<pdbpc::Model>(*ref_dummyModel);
            ppdb_2.models.push_back(dummyModel_2);
            pdbpc::parseEndModelLine(ppdb_2,NonConformingLine_space,12);

            // This should generate one out of band record
            BOOST_TEST(ppdb_2.outOfBandRecords.size() == 1);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::SupernumerarySpaceAfterLastColumn);
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

        {
            pdbpc::ParsedPDB ppdb_3;
            auto dummyModel_3 = std::make_shared<pdbpc::Model>(*ref_dummyModel);
            ppdb_3.models.push_back(dummyModel_3);
            pdbpc::parseEndModelLine(ppdb_3,NonConformingLine_junk,12);

            // This should generate one out of band record
            BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::SupernumeraryContentAfterLastColumn);
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

    }
}