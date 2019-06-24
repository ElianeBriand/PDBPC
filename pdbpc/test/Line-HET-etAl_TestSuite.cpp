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
#include "../src/Parser/parseHetRelatedLines.h"
#include <pdbpc/Records/OutOfBandRecord.h>
#include "../src/Utility/internalUtils.h"

struct LineFixture_HET_etAl {
    LineFixture_HET_etAl() {

    }

    ~LineFixture_HET_etAl() {
    }

    std::string conformingLine = "HET    RIT  B 301      50                                                       ";
    std::string NonConformingLine_short = "HET    RIT  B 301 ";
    std::string NonConformingLine_blankField_sequenceNumber = "HET    RIT  B          50             ";
    std::string NonConformingLine_space = "HET    RIT  B 301    50";
    std::string NonConformingLine_blankChaindID = "HET    RIT    301      50               ";
    std::string NonConformingLine_BadChaindID = "HET    RIT  + 301      50                              ";
    std::string NonConformingLine_noHetId =  "HETNAM         RITONAVIR                                                        \n";




    std::string hetBlock =
            "HET    RIT  B 301      50                                                       \n"
            "HETNAM     RIT RITONAVIR                                                        \n"
            "HETSYN     RIT A-84538 ; Norvir                                                 \n"
            "FORMUL   3  RIT    C37 H48 N6 O5 S2                                             \n"
            "FORMUL   4  HOH   *100(H2 O)  \n";

    std::string hetBlock_Implicit =
            "HETNAM     RIT RITONAVIR                                                        \n"
            "HETSYN     RIT A-84538 ; Norvir                                                 \n"
            "FORMUL   3  RIT    C37 H48 N6 O5 S2                                             \n"
            "FORMUL   4  HOH   *100(H2 O)  \n";

};

BOOST_AUTO_TEST_SUITE(Line_testSuite)

    BOOST_FIXTURE_TEST_CASE(HET_CompleteBlock, LineFixture_HET_etAl, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(hetBlock);


        for(const auto& record: ppdb.outOfBandRecords) {
            record->printRecord();
        }
        BOOST_TEST(ppdb.outOfBandRecords.empty());
        BOOST_REQUIRE(ppdb.outOfBandRecords.empty());


        BOOST_TEST(ppdb.heterogens.size() == 2);
        BOOST_REQUIRE(ppdb.heterogens.size() == 2);

        std::shared_ptr<pdbpc::Heterogen> het = ppdb.heterogens.at(0);

        BOOST_TEST(het->hetID == "RIT");
        BOOST_TEST(het->chainID == "B");
        BOOST_TEST(het->sequenceNumber == 301);
        BOOST_TEST(het->insertionCode == "");
        BOOST_TEST(het->description == "");
        BOOST_TEST(het->numHETATM == 50);
        BOOST_TEST(het->chemicalName == "RITONAVIR");
        BOOST_TEST(het->synonyms.size() == 2);
        BOOST_REQUIRE(het->synonyms.size() == 2);

        BOOST_TEST(het->synonyms.at(0) == "A-84538");
        BOOST_TEST(het->synonyms.at(1) == "Norvir");

        BOOST_TEST(het->chemicalFormula == "C37 H48 N6 O5 S2");
        BOOST_TEST(het->isWater == false);

        std::shared_ptr<pdbpc::Heterogen> water = ppdb.heterogens.at(1);

        BOOST_TEST(water->chemicalFormula == "100(H2 O)");
        BOOST_TEST(water->isWater == true);



    }

    BOOST_FIXTURE_TEST_CASE(HET_ImplicitHET, LineFixture_HET_etAl, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(hetBlock_Implicit);

        BOOST_TEST(ppdb.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(!ppdb.outOfBandRecords.empty());

        BOOST_TEST(ppdb.outOfBandRecords.back()->type == pdbpc::OutOfBandType::IncorrectPDBFileStructure);
        BOOST_TEST(ppdb.outOfBandRecords.back()->subtype == pdbpc::OutOfBandSubType::HetIDReferencedButNotDefined);
        BOOST_TEST(ppdb.outOfBandRecords.back()->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb.outOfBandRecords.back()->recoveryStatus == pdbpc::RecoveryStatus::recovered);
        BOOST_TEST(ppdb.outOfBandRecords.back()->lineNumber == 1);

        BOOST_TEST(ppdb.heterogens.size() == 2);
        BOOST_REQUIRE(ppdb.heterogens.size() == 2);

        std::shared_ptr<pdbpc::Heterogen> het = ppdb.heterogens.at(0);

        BOOST_TEST(het->hetID == "RIT");
        BOOST_TEST(het->chainID == "");
        BOOST_TEST(het->sequenceNumber == -1);
        BOOST_TEST(het->insertionCode == "");
        BOOST_TEST(het->description == "");
        BOOST_TEST(het->numHETATM == -1);
        BOOST_TEST(het->chemicalName == "RITONAVIR");
        BOOST_TEST(het->synonyms.size() == 2);
        BOOST_REQUIRE(het->synonyms.size() == 2);

        BOOST_TEST(het->synonyms.at(0) == "A-84538");
        BOOST_TEST(het->synonyms.at(1) == "Norvir");

        std::shared_ptr<pdbpc::Heterogen> water = ppdb.heterogens.at(1);

        BOOST_TEST(water->chemicalFormula == "100(H2 O)");
        BOOST_TEST(water->isWater == true);


    }

    BOOST_FIXTURE_TEST_CASE(HET_CorrectLine, LineFixture_HET_etAl, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb;

        pdbpc::parseHETLine(ppdb,conformingLine,23);

        // This should generate no out of band records
        BOOST_TEST(ppdb.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 0);

        BOOST_TEST(ppdb.heterogens.size() == 1);
        BOOST_REQUIRE(ppdb.heterogens.size() == 1);

        std::shared_ptr<pdbpc::Heterogen> het = ppdb.heterogens.back();

        BOOST_TEST(het->hetID == "RIT");
        BOOST_TEST(het->chainID == "B");
        BOOST_TEST(het->sequenceNumber == 301);
        BOOST_TEST(het->insertionCode == "");
        BOOST_TEST(het->description == "");
        BOOST_TEST(het->numHETATM == 50);

    }

    BOOST_FIXTURE_TEST_CASE(HET_NonConformingLine_space, LineFixture_HET_etAl, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb;

        pdbpc::parseHETLine(ppdb,NonConformingLine_space,23);

        // This should generate no out of band records
        BOOST_TEST(ppdb.outOfBandRecords.size() == 0);
        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 0);

        BOOST_TEST(ppdb.heterogens.size() == 1);
        BOOST_REQUIRE(ppdb.heterogens.size() == 1);

        std::shared_ptr<pdbpc::Heterogen> het = ppdb.heterogens.back();

        BOOST_TEST(het->hetID == "RIT");
        BOOST_TEST(het->chainID == "B");
        BOOST_TEST(het->sequenceNumber == 301);
        BOOST_TEST(het->insertionCode == "");
        BOOST_TEST(het->description == "");
        BOOST_TEST(het->numHETATM == 50);

    }

    BOOST_FIXTURE_TEST_CASE(HET_ShortLine, LineFixture_HET_etAl, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb;

        pdbpc::parseHETLine(ppdb,NonConformingLine_short,23);

        BOOST_TEST(ppdb.heterogens.size() == 1);
        BOOST_REQUIRE(ppdb.heterogens.size() == 1);

        std::shared_ptr<pdbpc::Heterogen> het = ppdb.heterogens.back();

        BOOST_TEST(het->hetID == "RIT");
        BOOST_TEST(het->chainID == "B");
        BOOST_TEST(het->sequenceNumber == 301);
        BOOST_TEST(het->insertionCode == "");
        BOOST_TEST(het->description == "");
        BOOST_TEST(het->numHETATM == -1);

        // This should generate exactly 1 out of band record
        BOOST_TEST(ppdb.outOfBandRecords.size() == 2);
        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 2);

        BOOST_TEST(ppdb.outOfBandRecords.at(0)->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->subtype == pdbpc::OutOfBandSubType::MissingHETATMCountInHETRecord);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->lineNumber == 23);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->line == NonConformingLine_short);

        BOOST_TEST(ppdb.outOfBandRecords.at(1)->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->subtype == pdbpc::OutOfBandSubType::IncompleteHETrecord);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->lineNumber == 23);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->line == NonConformingLine_short);


    }

    BOOST_FIXTURE_TEST_CASE(HET_blankField_sequenceNumber, LineFixture_HET_etAl, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb;

        pdbpc::parseHETLine(ppdb,NonConformingLine_blankField_sequenceNumber,23);

        BOOST_TEST(ppdb.heterogens.size() == 1);
        BOOST_REQUIRE(ppdb.heterogens.size() == 1);

        std::shared_ptr<pdbpc::Heterogen> het = ppdb.heterogens.back();

        BOOST_TEST(het->hetID == "RIT");
        BOOST_TEST(het->chainID == "B");
        BOOST_TEST(het->sequenceNumber == -1);
        BOOST_TEST(het->insertionCode == "");
        BOOST_TEST(het->description == "");
        BOOST_TEST(het->numHETATM == 50);

        // This should generate exactly 1 out of band record
        BOOST_TEST(ppdb.outOfBandRecords.size() == 2);
        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 2);

        BOOST_TEST(ppdb.outOfBandRecords.at(0)->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->subtype == pdbpc::OutOfBandSubType::MissingSequenceNumber);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->lineNumber == 23);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->line == NonConformingLine_blankField_sequenceNumber);

        BOOST_TEST(ppdb.outOfBandRecords.at(1)->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->subtype == pdbpc::OutOfBandSubType::IncompleteHETrecord);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->lineNumber == 23);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->line == NonConformingLine_blankField_sequenceNumber);


    }



    BOOST_FIXTURE_TEST_CASE(HET_blankField_ChainID, LineFixture_HET_etAl, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb;

        pdbpc::parseHETLine(ppdb,NonConformingLine_blankChaindID,23);

        BOOST_TEST(ppdb.heterogens.size() == 1);
        BOOST_REQUIRE(ppdb.heterogens.size() == 1);

        std::shared_ptr<pdbpc::Heterogen> het = ppdb.heterogens.back();

        BOOST_TEST(het->hetID == "RIT");
        BOOST_TEST(het->chainID == "");
        BOOST_TEST(het->sequenceNumber == 301);
        BOOST_TEST(het->insertionCode == "");
        BOOST_TEST(het->description == "");
        BOOST_TEST(het->numHETATM == 50);

        // This should generate exactly 1 out of band record
        BOOST_TEST(ppdb.outOfBandRecords.size() == 2);
        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 2);

        BOOST_TEST(ppdb.outOfBandRecords.at(0)->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->subtype == pdbpc::OutOfBandSubType::MissingChainID);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->lineNumber == 23);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->line == NonConformingLine_blankChaindID);

        BOOST_TEST(ppdb.outOfBandRecords.at(1)->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->subtype == pdbpc::OutOfBandSubType::IncompleteHETrecord);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->lineNumber == 23);
        BOOST_TEST(ppdb.outOfBandRecords.at(1)->line == NonConformingLine_blankChaindID);
    }

    BOOST_FIXTURE_TEST_CASE(HET_BadField_ChainID, LineFixture_HET_etAl, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb;

        pdbpc::parseHETLine(ppdb,NonConformingLine_BadChaindID,23);

        BOOST_TEST(ppdb.heterogens.size() == 1);
        BOOST_REQUIRE(ppdb.heterogens.size() == 1);

        std::shared_ptr<pdbpc::Heterogen> het = ppdb.heterogens.back();

        BOOST_TEST(het->hetID == "RIT");
        BOOST_TEST(het->chainID == "+");
        BOOST_TEST(het->sequenceNumber == 301);
        BOOST_TEST(het->insertionCode == "");
        BOOST_TEST(het->description == "");
        BOOST_TEST(het->numHETATM == 50);

        // This should generate exactly 1 out of band record
        BOOST_TEST(ppdb.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 1);

        BOOST_TEST(ppdb.outOfBandRecords.at(0)->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->subtype == pdbpc::OutOfBandSubType::NonAlphabeticChainID);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->lineNumber == 23);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->line == NonConformingLine_BadChaindID);

    }

    BOOST_FIXTURE_TEST_CASE(HET_noHetId, LineFixture_HET_etAl, *boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb;

        pdbpc::parseHETNAMLine(ppdb, NonConformingLine_noHetId,23);

        BOOST_TEST(ppdb.heterogens.size() == 0);
        BOOST_REQUIRE(ppdb.heterogens.size() == 0);

        BOOST_TEST(ppdb.outOfBandRecords.size() == 1);
        BOOST_REQUIRE(ppdb.outOfBandRecords.size() == 1);

        BOOST_TEST(ppdb.outOfBandRecords.at(0)->type == pdbpc::OutOfBandType::IncorrectPDBLineFormat);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->subtype == pdbpc::OutOfBandSubType::MissingHetID);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->severity == pdbpc::OutOfBandSeverity::error);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->recoveryStatus == pdbpc::RecoveryStatus::unrecoverable);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->lineNumber == 23);
        BOOST_TEST(ppdb.outOfBandRecords.at(0)->line == NonConformingLine_noHetId);

    }


BOOST_AUTO_TEST_SUITE_END()
