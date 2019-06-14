//
// Created by eliane on 11/05/19.
//

#define BOOST_TEST_MODULE main_test_module
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN


#include <boost/test/unit_test.hpp>

#include <pdbpc/pdbpc.h>
#include <pdbpc/Parser/parseEndModelLine.h>
#include <pdbpc/Parser/parseModelLine.h>
#include <pdbpc/Parser/parseAtomLine.h>
#include <pdbpc/Records/OutOfBandRecord.h>
#include <pdbpc/Utility/internalUtils.h>

#include "simplePDBFixture.h"

BOOST_AUTO_TEST_SUITE(Main_testSuite)


    BOOST_FIXTURE_TEST_CASE(SimplePDB_BlockReading,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(SimplePDBBlock);


        BOOST_TEST(ppdb.models.size() == 2);
        BOOST_TEST(ppdb.atoms_flatlist.size() == 109);
        BOOST_TEST(ppdb.residues_flatlist.size() == 9);
        BOOST_TEST(ppdb.chains_flatlist.size() == 4);
    }

    BOOST_FIXTURE_TEST_CASE(SimplePDB_HierarchyWalking,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(SimplePDBBlock);


        // Require the test from the blockreading test
        BOOST_REQUIRE(ppdb.models.size() == 2);
        BOOST_REQUIRE(ppdb.atoms_flatlist.size() == 109);
        BOOST_REQUIRE(ppdb.residues_flatlist.size() == 9);
        BOOST_REQUIRE(ppdb.chains_flatlist.size() == 4);


        BOOST_TEST( ppdb.models[0]->atoms_flatlist.size() == 59);
        BOOST_TEST( ppdb.models[1]->atoms_flatlist.size() == 50);

        BOOST_TEST( ppdb.models[0]->residues_flatlist.size() == 5);
        BOOST_TEST( ppdb.models[1]->residues_flatlist.size() == 4);

        BOOST_TEST( ppdb.models[0]->chains.size() == 2);
        BOOST_TEST( ppdb.models[1]->chains.size() == 2);

        BOOST_TEST( ppdb.models[0]->chains[0]->residues.size() == 3);
        BOOST_TEST( ppdb.models[1]->chains[0]->residues.size() == 2);



    }

    BOOST_FIXTURE_TEST_CASE(DuplicatedModelHandling,SimplePDBFixture,* boost::unit_test::timeout(10)) {
        pdbpc::ParsedPDB ppdb = pdbpc::readPDBBlock(DuplicatedModelPDB);


        BOOST_TEST(ppdb.models.size() == 2);
        BOOST_TEST(ppdb.models.at(0)->modelNumber != ppdb.models.at(1)->modelNumber);
    }

BOOST_AUTO_TEST_SUITE_END()