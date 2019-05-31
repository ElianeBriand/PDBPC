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

//
//    BOOST_AUTO_TEST_CASE(LineParser_ENDMDL,* boost::unit_test::timeout(10)) {
//
//    }



BOOST_AUTO_TEST_SUITE_END()