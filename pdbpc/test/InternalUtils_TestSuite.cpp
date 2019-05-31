#include <boost/test/unit_test.hpp>


#include <pdbpc/pdbpc.h>
#include <pdbpc/Parser/parseEndModelLine.h>
#include <pdbpc/Parser/parseModelLine.h>
#include <pdbpc/Parser/parseAtomLine.h>
#include <pdbpc/Records/OutOfBandRecord.h>
#include <pdbpc/Utility/internalUtils.h>

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

BOOST_AUTO_TEST_SUITE_END()