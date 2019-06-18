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

#include <boost/test/output_test_stream.hpp>
using b_ostream = boost::test_tools::output_test_stream;


#include <pdbpc/pdbpc.h>

#include <pdbpc/Records/OutOfBandRecord.h>

BOOST_AUTO_TEST_SUITE(Records_testSuite)

    BOOST_AUTO_TEST_CASE(OutOfBand_Severity_Output_Test) {
        std::string res = pdbpc::OutOfBandRecord_SeverityToString(pdbpc::OutOfBandSeverity::error);

        BOOST_TEST(res == "Error");

        res = pdbpc::OutOfBandRecord_SeverityToString(pdbpc::OutOfBandSeverity::idiosyncrasy);

        BOOST_TEST(res == "Idiosyncrasy");

        res = pdbpc::OutOfBandRecord_SeverityToString(static_cast<pdbpc::OutOfBandSeverity>(142));

        BOOST_TEST(res == "???");

        b_ostream testStream;

        testStream << "Status=" << pdbpc::OutOfBandSeverity::error;
        BOOST_TEST( !testStream.is_empty( false ) );
        BOOST_TEST( testStream.is_equal( "Status=Error" ) );

    }

    BOOST_AUTO_TEST_CASE(OutOfBand_Type_Output_Test) {
        std::string res = pdbpc::OutOfBandRecord_TypeToString(pdbpc::OutOfBandType::FileAccessError);

        BOOST_TEST(res == "File access error");

        res = pdbpc::OutOfBandRecord_TypeToString(pdbpc::OutOfBandType::UnhandledPDBLine);

        BOOST_TEST(res == "Unhandled PDB line");

        res = pdbpc::OutOfBandRecord_TypeToString(static_cast<pdbpc::OutOfBandType>(142));

        BOOST_TEST(res == "???");

        b_ostream testStream;

        testStream << "Type: " << pdbpc::OutOfBandType::SurprisingStructure;
        BOOST_TEST( !testStream.is_empty( false ) );
        BOOST_TEST( testStream.is_equal( "Type: Line specifies structure which, while possible, is surprising" ) );


    }

    BOOST_AUTO_TEST_CASE(OutOfBand_Subtype_Output_Test) {
        std::string res = pdbpc::OutOfBandRecord_SubTypeToString(pdbpc::OutOfBandSubType::FileNotFound);

        BOOST_TEST(res == "FileNotFound");

        res = OutOfBandRecord_SubTypeToExplainationString(pdbpc::OutOfBandSubType::FileNotFound);

        BOOST_TEST(res == "File not found");

        res = pdbpc::OutOfBandRecord_SubTypeToString(pdbpc::OutOfBandSubType::FilePathIsDirectory);

        BOOST_TEST(res == "FilePathIsDirectory");

        res = pdbpc::OutOfBandRecord_SubTypeToString(static_cast<pdbpc::OutOfBandSubType>(142));

        BOOST_TEST(res == "???");

        res = OutOfBandRecord_SubTypeToExplainationString(static_cast<pdbpc::OutOfBandSubType>(142));

        BOOST_TEST(res == "???");

        b_ostream testStream;

        testStream << "Subtype: " << pdbpc::OutOfBandSubType::AtomCannotParseTempFactor;
        BOOST_TEST( !testStream.is_empty( false ) );
        BOOST_TEST( testStream.is_equal( "Subtype: AtomCannotParseTempFactor" ) );


    }

    BOOST_AUTO_TEST_CASE(OutOfBand_RecoveryStatus_Output_Test) {
        std::string res = pdbpc::OutOfBandRecord_RecoveryStatusToString(pdbpc::RecoveryStatus::recovered);

        BOOST_TEST(res == "Recovered");

        res = pdbpc::OutOfBandRecord_RecoveryStatusToString(pdbpc::RecoveryStatus::unrecoverable);

        BOOST_TEST(res == "Unrecoverable");

        res = pdbpc::OutOfBandRecord_RecoveryStatusToString(static_cast<pdbpc::RecoveryStatus>(142));

        BOOST_TEST(res == "???");

        b_ostream testStream;

        testStream << "Recovery status: " << pdbpc::RecoveryStatus::recovered;
        BOOST_TEST( !testStream.is_empty( false ) );
        BOOST_TEST( testStream.is_equal( "Recovery status: Recovered" ) );


    }

    BOOST_AUTO_TEST_CASE(Residue_Output_Test) {

        // Type to three-letter codes

        std::string res = pdbpc::residueTypeToThreeLetterCode(pdbpc::ResidueType::alanine);

        BOOST_TEST(res == "ALA");

        res = pdbpc::residueTypeToThreeLetterCode(pdbpc::ResidueType::asparagine);

        BOOST_TEST(res == "ASN");

        res = pdbpc::residueTypeToThreeLetterCode(static_cast<pdbpc::ResidueType>(1142));

        BOOST_TEST(res == "???");

        b_ostream testStream;

        testStream << pdbpc::ResidueType::cysteine;
        BOOST_TEST( !testStream.is_empty( false ) );
        BOOST_TEST( testStream.is_equal( "cysteine (CYS)" ) );

        // Type to full name

        res = pdbpc::residueTypeToFullName(pdbpc::ResidueType::alanine);

        BOOST_TEST(res == "alanine");

        res = pdbpc::residueTypeToFullName(pdbpc::ResidueType::asparagine);

        BOOST_TEST(res == "asparagine");

        res = pdbpc::residueTypeToFullName(static_cast<pdbpc::ResidueType>(1142));

        BOOST_TEST(res == "???");

        // Three-letter code to type

        pdbpc::ResidueType t = pdbpc::ThreeLetterCodeToResidueType("ASP");

        BOOST_TEST(t == pdbpc::ResidueType::aspartate);


        t = pdbpc::ThreeLetterCodeToResidueType("SLD");

        BOOST_TEST(t == pdbpc::ResidueType::unknown);

    }

BOOST_AUTO_TEST_SUITE_END()