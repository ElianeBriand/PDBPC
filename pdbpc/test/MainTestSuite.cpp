//
// Created by eliane on 11/05/19.
//

#define BOOST_TEST_MODULE main_test_module
#define BOOST_TEST_MAIN


#include <boost/test/unit_test.hpp>

#include <pdbpc/pdbpc.h>
#include <pdbpc/Parser/parseEndModelLine.h>
#include <pdbpc/Parser/parseModelLine.h>
#include <pdbpc/Parser/parseAtomLine.h>
#include <pdbpc/Records/OutOfBandRecord.h>
#include <pdbpc/Utility/internalUtils.h>


BOOST_AUTO_TEST_SUITE(Main_testSuite)

    BOOST_AUTO_TEST_CASE(LineParser_MODEL,* boost::unit_test::timeout(10)) {
        std::string conformingLine = "MODEL        1";
        std::string NonConformingLine_space = "MODEL        1   ";
        std::string NonConformingLine_junk = "MODEL        1dsfsg";
        std::string NonConformingLine_shorterField = "MODEL   1";



        {
            pdbpc::ParsedPDB ppdb_1;
            pdbpc::parseModelLine(ppdb_1,conformingLine,12);

            // This should generate no out of band records
            BOOST_TEST(ppdb_1.outOfBandRecords.size() == 0);
            BOOST_REQUIRE(ppdb_1.outOfBandRecords.size() == 0);

            // Correctly set opening line number and line
            BOOST_TEST(ppdb_1.models.back()->openingLineNumber == 12);
            BOOST_TEST(ppdb_1.models.back()->openingLine == conformingLine);

            // Closing lines as expected
            BOOST_TEST(ppdb_1.models.back()->closingLineNumber == -1);
        }

        {
            pdbpc::ParsedPDB ppdb_2;
            pdbpc::parseModelLine(ppdb_2,NonConformingLine_space,12);

            // This should generate one out of band record
            BOOST_TEST(ppdb_2.outOfBandRecords.size() == 1);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::SupernumerarySpaceAfterLastColumn);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::idiosyncrasy);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->lineNumber == 12);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->line == NonConformingLine_space);

            // Correctly set opening line number and line
            BOOST_TEST(ppdb_2.models.back()->openingLineNumber == 12);
            BOOST_TEST(ppdb_2.models.back()->openingLine == NonConformingLine_space);

            // Closing lines as expected
            BOOST_TEST(ppdb_2.models.back()->closingLineNumber == -1);
        }

        {
            pdbpc::ParsedPDB ppdb_3;
            pdbpc::parseModelLine(ppdb_3,NonConformingLine_junk,12);

            // This should generate one out of band record
            BOOST_TEST(ppdb_3.outOfBandRecords.size() == 1);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::unexpectedStringInsteadOfModelNumber);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == NonConformingLine_junk);

            //  No models are added
            BOOST_TEST(ppdb_3.models.empty());
        }

        {
            pdbpc::ParsedPDB ppdb_4;
            pdbpc::parseModelLine(ppdb_4,NonConformingLine_shorterField,12);

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

    }

    BOOST_AUTO_TEST_CASE(LineParser_ENDMDL,* boost::unit_test::timeout(10)) {
        std::string conformingLine = "ENDMDL";
        std::string NonConformingLine_space = "ENDMDL ";
        std::string NonConformingLine_junk = "ENDMDLDHJJD";


        auto ref_dummyModel = std::make_shared<pdbpc::Model>();
        ref_dummyModel->openingLineNumber = 1;
        ref_dummyModel->openingLine = "MODEL        1";
        ref_dummyModel->closingLineNumber = -1;

        {
            pdbpc::ParsedPDB ppdb_1;
            auto dummyModel_1 = std::make_shared<pdbpc::Model>(*ref_dummyModel);
            ppdb_1.models.push_back(dummyModel_1);
            pdbpc::parseEndModelLine(ppdb_1,conformingLine,12);

            // This should generate no out of band records
            BOOST_TEST(ppdb_1.outOfBandRecords.size() == 0);
            BOOST_REQUIRE(ppdb_1.outOfBandRecords.size() == 0);

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

    BOOST_AUTO_TEST_CASE(LineParser_ATOM,* boost::unit_test::timeout(10)) {
        std::string conformingLine = "ATOM   1786  OE1 GLN B  92      27.358  20.610 -10.667  1.00 20.49           O  ";
        std::string NonConformingLine_degraded = "ATOM   1787  NE2 GLN B  92      27.449  19.137  -8.988";
        std::string NonConformingLine_junk = "ATOM   1787  NE2 GLN B  92      27.nop  19.137  -8.988";


        {
            pdbpc::ParsedPDB ppdb_1;
            createPlaceholderModelAndChain(ppdb_1);
            pdbpc::parseAtomLine(ppdb_1,conformingLine,12);

            // This should generate no out of band records
            BOOST_TEST(ppdb_1.outOfBandRecords.size() == 0);
            BOOST_REQUIRE(ppdb_1.outOfBandRecords.size() == 0);

            // Correctly set record
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->lineNumber == 12);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->line == conformingLine);


            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->serialNumber == 1786);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->atomName == "OE1");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->alternateLocationIdentifier == "");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueType == pdbpc::ResidueType::glutamine);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueName == "GLN");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->chainIdentifier == "B");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueSequenceNumber == 92);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->x == 27.358);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->y == 20.610);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->z == -10.667);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->residueInsertionCode == "");
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->occupancy == 1.0);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->temperatureFactor == 20.49);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->charge == 0.0);
            BOOST_TEST(ppdb_1.atoms_flatlist.at(0)->element == "O");


        }

        {
            pdbpc::ParsedPDB ppdb_2;
            createPlaceholderModelAndChain(ppdb_2);
            pdbpc::parseAtomLine(ppdb_2,NonConformingLine_degraded,12);

            // This should generate no out of band records
            BOOST_TEST(ppdb_2.outOfBandRecords.size() == 1);
            BOOST_REQUIRE(ppdb_2.outOfBandRecords.size() == 1);

            BOOST_TEST(ppdb_2.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomDegradedModeParsingEvent);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->lineNumber == 12);
            BOOST_TEST(ppdb_2.outOfBandRecords.back()->line == NonConformingLine_degraded);


            // Correctly set record
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->lineNumber == 12);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->line == NonConformingLine_degraded);

            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->serialNumber == 1787);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->atomName == "NE2");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->alternateLocationIdentifier == "");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueType == pdbpc::ResidueType::glutamine);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueName == "GLN");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->chainIdentifier == "B");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueSequenceNumber == 92);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->x == 27.449);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->y == 19.137);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->z == -8.988);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->residueInsertionCode == "");
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->occupancy == 1.0);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->temperatureFactor == 0.0);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->charge == 0.0);
            BOOST_TEST(ppdb_2.atoms_flatlist.at(0)->element == "N");

        }

        {
            pdbpc::ParsedPDB ppdb_3;
            createPlaceholderModelAndChain(ppdb_3);
            pdbpc::parseAtomLine(ppdb_3,NonConformingLine_junk,12);

            // This should generate no out of band records
            BOOST_TEST(ppdb_3.outOfBandRecords.size() == 2);
            BOOST_REQUIRE(ppdb_3.outOfBandRecords.size() == 2);

            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->subtype == pdbpc::OutOfBandSubType::AtomDegradedModeParsingEvent);
            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->severity == pdbpc::OutOfBandSeverity::error);
            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->recoveryStatus == pdbpc::RecoveryStatus::recovered);
            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->lineNumber == 12);
            BOOST_TEST(ppdb_3.outOfBandRecords.at(0)->line == NonConformingLine_junk);

            BOOST_TEST(ppdb_3.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::AtomCannotParseCoordinates);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->lineNumber == 12);
            BOOST_TEST(ppdb_3.outOfBandRecords.back()->line == NonConformingLine_junk);
            

        }

    }

}