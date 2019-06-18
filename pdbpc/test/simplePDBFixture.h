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

#ifndef PDBPC_SIMPLEPDBFIXTURE_H
#define PDBPC_SIMPLEPDBFIXTURE_H

// Only for modelNumber < 10, but that's enough for our tests
inline std::string emitMODELline(int modelNumber) {
    std::string ret = "MODEL        " + std::to_string(modelNumber) + "\n";
    return ret;
}

struct SimplePDBFixture {




    std::string Model1_content =
            "ATOM      1  N   PRO A   1      29.071  39.248   5.491  1.00 36.45           N  \n"
            "ATOM      2  CA  PRO A   1      29.983  38.171   5.136  1.00 35.67           C  \n"
            "ATOM      3  C   PRO A   1      29.540  37.373   3.917  1.00 31.34           C  \n"
            "ATOM      4  O   PRO A   1      28.380  37.476   3.533  1.00 34.24           O  \n"
            "ATOM      5  CB  PRO A   1      30.126  37.270   6.375  1.00 35.64           C  \n"
            "ATOM      6  CG  PRO A   1      28.910  37.672   7.181  1.00 38.30           C  \n"
            "ATOM      7  CD  PRO A   1      28.927  39.170   6.913  1.00 39.93           C  \n"
            "ATOM      8  H2  PRO A   1      29.246  40.197   5.100  1.00  0.00           H  \n"
            "ATOM      9  H3  PRO A   1      28.124  39.059   5.075  1.00  0.00           H  \n"
            "ATOM     10  N   GLN A   2      30.417  36.603   3.299  1.00 30.20           N  \n"
            "ATOM     11  CA  GLN A   2      30.037  35.792   2.165  1.00 32.99           C  \n"
            "ATOM     12  C   GLN A   2      30.067  34.364   2.629  1.00 33.44           C  \n"
            "ATOM     13  O   GLN A   2      31.097  33.938   3.141  1.00 34.15           O  \n"
            "ATOM     14  CB  GLN A   2      30.999  35.913   1.046  1.00 36.81           C  \n"
            "ATOM     15  CG  GLN A   2      30.349  35.299  -0.134  1.00 44.73           C  \n"
            "ATOM     16  CD  GLN A   2      31.238  35.322  -1.346  1.00 51.11           C  \n"
            "ATOM     17  OE1 GLN A   2      32.218  34.584  -1.345  1.00 54.41           O  \n"
            "ATOM     18  NE2 GLN A   2      30.989  36.146  -2.352  1.00 49.42           N  \n"
            "ATOM     19  H   GLN A   2      31.340  36.518   3.602  1.00  0.00           H  \n"
            "ATOM     20 HE21 GLN A   2      31.567  36.076  -3.121  1.00  0.00           H  \n"
            "ATOM     21 HE22 GLN A   2      30.232  36.770  -2.292  1.00  0.00           H  \n"
            "ATOM     22  N   ILE A   3      28.952  33.657   2.537  1.00 30.28           N  \n"
            "ATOM     23  CA  ILE A   3      28.864  32.290   3.024  1.00 26.83           C  \n"
            "ATOM     24  C   ILE A   3      28.747  31.269   1.863  1.00 25.88           C  \n"
            "ATOM     25  O   ILE A   3      27.788  31.410   1.070  1.00 22.73           O  \n"
            "ATOM     26  CB  ILE A   3      27.646  32.277   3.994  1.00 27.13           C  \n"
            "ATOM     27  CG1 ILE A   3      27.885  33.296   5.129  1.00 27.34           C  \n"
            "ATOM     28  CG2 ILE A   3      27.429  30.848   4.533  1.00 24.72           C  \n"
            "ATOM     29  CD1 ILE A   3      26.899  33.242   6.316  1.00 31.20           C  \n"
            "ATOM     30  H   ILE A   3      28.179  34.033   2.064  1.00  0.00           H  \n"
            "ATOM     65  N   GLN B   1      29.262  23.791   3.255  1.00 18.43           N  \n"
            "ATOM     66  CA  GLN B   1      28.827  23.345   4.541  1.00 18.15           C  \n"
            "ATOM     67  C   GLN B   1      27.530  24.042   4.830  1.00 16.06           C  \n"
            "ATOM     68  O   GLN B   1      27.108  24.948   4.137  1.00 18.09           O  \n"
            "ATOM     69  CB  GLN B   1      29.818  23.723   5.608  1.00 28.80           C  \n"
            "ATOM     70  CG  GLN B   1      31.200  23.111   5.391  1.00 44.09           C  \n"
            "ATOM     71  CD  GLN B   1      32.375  23.984   5.867  1.00 53.77           C  \n"
            "ATOM     72  OE1 GLN B   1      32.568  25.167   5.554  1.00 61.48           O  \n"
            "ATOM     73  NE2 GLN B   1      33.255  23.399   6.646  1.00 56.32           N  \n"
            "ATOM     74  H   GLN B   1      29.312  24.762   3.138  1.00  0.00           H  \n"
            "ATOM     75 HE21 GLN B   1      34.048  23.898   6.907  1.00  0.00           H  \n"
            "ATOM     76 HE22 GLN B   1      33.103  22.473   6.906  1.00  0.00           H  \n"
            "ATOM     77  N   ARG B   2      26.834  23.541   5.844  1.00 18.56           N  \n"
            "ATOM     78  CA  ARG B   2      25.590  24.126   6.309  1.00 19.32           C  \n"
            "ATOM     79  C   ARG B   2      25.877  25.558   6.729  1.00 20.01           C  \n"
            "ATOM     80  O   ARG B   2      26.858  25.719   7.478  1.00 18.56           O  \n"
            "ATOM     81  CB  ARG B   2      25.039  23.350   7.512  1.00 21.88           C  \n"
            "ATOM     82  CG  ARG B   2      24.307  22.051   7.237  1.00 29.35           C  \n"
            "ATOM     83  CD  ARG B   2      23.948  21.359   8.528  1.00 39.12           C  \n"
            "ATOM     84  NE  ARG B   2      22.815  20.429   8.288  1.00 52.84           N  \n"
            "ATOM     85  CZ  ARG B   2      21.927  20.143   9.268  1.00 64.39           C  \n"
            "ATOM     86  NH1 ARG B   2      21.979  20.814  10.472  1.00 70.05           N  \n"
            "ATOM     87  NH2 ARG B   2      20.873  19.330   9.064  1.00 68.82           N  \n"
            "ATOM     88  H   ARG B   2      27.161  22.745   6.271  1.00  0.00           H  \n"
            "ATOM     89  HE  ARG B   2      22.749  19.897   7.464  1.00  0.00           H  \n"
            "ATOM     90 HH11 ARG B   2      22.763  21.433  10.658  1.00  0.00           H  \n"
            "ATOM     91 HH12 ARG B   2      21.314  20.601  11.210  1.00  0.00           H  \n"
            "ATOM     92 HH21 ARG B   2      20.741  18.836   8.224  1.00  0.00           H  \n"
            "ATOM     93 HH22 ARG B   2      20.266  19.172   9.843  1.00  0.00           H  \n";

    std::string Model2_content =
            "ATOM      1  N   PRO A   1      29.071  39.248   5.491  1.00 36.45           N  \n"
            "ATOM      2  CA  PRO A   1      29.983  38.171   5.136  1.00 35.67           C  \n"
            "ATOM      3  C   PRO A   1      29.540  37.373   3.917  1.00 31.34           C  \n"
            "ATOM      4  O   PRO A   1      28.380  37.476   3.533  1.00 34.24           O  \n"
            "ATOM      5  CB  PRO A   1      30.126  37.270   6.375  1.00 35.64           C  \n"
            "ATOM      6  CG  PRO A   1      28.910  37.672   7.181  1.00 38.30           C  \n"
            "ATOM      7  CD  PRO A   1      28.927  39.170   6.913  1.00 39.93           C  \n"
            "ATOM      8  H2  PRO A   1      29.246  40.197   5.100  1.00  0.00           H  \n"
            "ATOM      9  H3  PRO A   1      28.124  39.059   5.075  1.00  0.00           H  \n"
            "ATOM     10  N   GLN A   2      30.417  36.603   3.299  1.00 30.20           N  \n"
            "ATOM     11  CA  GLN A   2      30.037  35.792   2.165  1.00 32.99           C  \n"
            "ATOM     12  C   GLN A   2      30.067  34.364   2.629  1.00 33.44           C  \n"
            "ATOM     13  O   GLN A   2      31.097  33.938   3.141  1.00 34.15           O  \n"
            "ATOM     14  CB  GLN A   2      30.999  35.913   1.046  1.00 36.81           C  \n"
            "ATOM     15  CG  GLN A   2      30.349  35.299  -0.134  1.00 44.73           C  \n"
            "ATOM     16  CD  GLN A   2      31.238  35.322  -1.346  1.00 51.11           C  \n"
            "ATOM     17  OE1 GLN A   2      32.218  34.584  -1.345  1.00 54.41           O  \n"
            "ATOM     18  NE2 GLN A   2      30.989  36.146  -2.352  1.00 49.42           N  \n"
            "ATOM     19  H   GLN A   2      31.340  36.518   3.602  1.00  0.00           H  \n"
            "ATOM     20 HE21 GLN A   2      31.567  36.076  -3.121  1.00  0.00           H  \n"
            "ATOM     21 HE22 GLN A   2      30.232  36.770  -2.292  1.00  0.00           H  \n"
            "ATOM     65  N   GLN B   1      29.262  23.791   3.255  1.00 18.43           N  \n"
            "ATOM     66  CA  GLN B   1      28.827  23.345   4.541  1.00 18.15           C  \n"
            "ATOM     67  C   GLN B   1      27.530  24.042   4.830  1.00 16.06           C  \n"
            "ATOM     68  O   GLN B   1      27.108  24.948   4.137  1.00 18.09           O  \n"
            "ATOM     69  CB  GLN B   1      29.818  23.723   5.608  1.00 28.80           C  \n"
            "ATOM     70  CG  GLN B   1      31.200  23.111   5.391  1.00 44.09           C  \n"
            "ATOM     71  CD  GLN B   1      32.375  23.984   5.867  1.00 53.77           C  \n"
            "ATOM     72  OE1 GLN B   1      32.568  25.167   5.554  1.00 61.48           O  \n"
            "ATOM     73  NE2 GLN B   1      33.255  23.399   6.646  1.00 56.32           N  \n"
            "ATOM     74  H   GLN B   1      29.312  24.762   3.138  1.00  0.00           H  \n"
            "ATOM     75 HE21 GLN B   1      34.048  23.898   6.907  1.00  0.00           H  \n"
            "ATOM     76 HE22 GLN B   1      33.103  22.473   6.906  1.00  0.00           H  \n"
            "ATOM     77  N   ARG B   2      26.834  23.541   5.844  1.00 18.56           N  \n"
            "ATOM     78  CA  ARG B   2      25.590  24.126   6.309  1.00 19.32           C  \n"
            "ATOM     79  C   ARG B   2      25.877  25.558   6.729  1.00 20.01           C  \n"
            "ATOM     80  O   ARG B   2      26.858  25.719   7.478  1.00 18.56           O  \n"
            "ATOM     81  CB  ARG B   2      25.039  23.350   7.512  1.00 21.88           C  \n"
            "ATOM     82  CG  ARG B   2      24.307  22.051   7.237  1.00 29.35           C  \n"
            "ATOM     83  CD  ARG B   2      23.948  21.359   8.528  1.00 39.12           C  \n"
            "ATOM     84  NE  ARG B   2      22.815  20.429   8.288  1.00 52.84           N  \n"
            "ATOM     85  CZ  ARG B   2      21.927  20.143   9.268  1.00 64.39           C  \n"
            "ATOM     86  NH1 ARG B   2      21.979  20.814  10.472  1.00 70.05           N  \n"
            "ATOM     87  NH2 ARG B   2      20.873  19.330   9.064  1.00 68.82           N  \n"
            "ATOM     88  H   ARG B   2      27.161  22.745   6.271  1.00  0.00           H  \n"
            "ATOM     89  HE  ARG B   2      22.749  19.897   7.464  1.00  0.00           H  \n"
            "ATOM     90 HH11 ARG B   2      22.763  21.433  10.658  1.00  0.00           H  \n"
            "ATOM     91 HH12 ARG B   2      21.314  20.601  11.210  1.00  0.00           H  \n"
            "ATOM     92 HH21 ARG B   2      20.741  18.836   8.224  1.00  0.00           H  \n"
            "ATOM     93 HH22 ARG B   2      20.266  19.172   9.843  1.00  0.00           H  \n"
    ;

    std::string smallATOMBlock =
            "ATOM      1  N   PRO A   1      29.071  39.248   5.491  1.00 36.45           N  \n"
            "ATOM      2  CA  PRO A   1      29.983  38.171   5.136  1.00 35.67           C  \n"
            "ATOM      3  C   PRO A   1      29.540  37.373   3.917  1.00 31.34           C  \n"
            "ATOM      4  O   PRO A   1      28.380  37.476   3.533  1.00 34.24           O  \n"
            "ATOM      5  CB  PRO A   1      30.126  37.270   6.375  1.00 35.64           C  \n"
            "ATOM      6  CG  PRO A   1      28.910  37.672   7.181  1.00 38.30           C  \n"
            "ATOM      7  CD  PRO A   1      28.927  39.170   6.913  1.00 39.93           C  \n"
            "ATOM      8  H2  PRO A   1      29.246  40.197   5.100  1.00  0.00           H  \n"
            "ATOM      9  H3  PRO A   1      28.124  39.059   5.075  1.00  0.00           H  \n";

    std::string smallATOMBlock_WithAltLoc =
            "ATOM      1  N  APRO A   1      29.071  39.248   5.491  0.33 36.45           N  \n"
            "ATOM      2  CA APRO A   1      29.983  38.171   5.136  0.33 35.67           C  \n"
            "ATOM      3  C  APRO A   1      29.540  37.373   3.917  0.33 31.34           C  \n"
            "ATOM      4  O  APRO A   1      28.380  37.476   3.533  0.33 34.24           O  \n"
            "ATOM      5  CB APRO A   1      30.126  37.270   6.375  0.33 35.64           C  \n"
            "ATOM      6  CG APRO A   1      28.910  37.672   7.181  0.33 38.30           C  \n"
            "ATOM      7  CD APRO A   1      28.927  39.170   6.913  0.33 39.93           C  \n"
            "ATOM      8  H2 APRO A   1      29.246  40.197   5.100  0.33  0.00           H  \n"
            "ATOM      9  H3 APRO A   1      28.124  39.059   5.075  0.33  0.00           H  \n"
            "ATOM      1  N  BPRO A   1      29.071  39.248   5.491  0.66 36.25           N  \n"
            "ATOM      2  CA BPRO A   1      29.983  38.171   5.136  0.66 35.27           C  \n"
            "ATOM      3  C  BPRO A   1      29.540  37.373   3.917  0.66 31.24           C  \n"
            "ATOM      4  O  BPRO A   1      28.380  37.476   3.533  0.66 34.24           O  \n"
            "ATOM      5  CB BPRO A   1      30.126  37.270   6.375  0.66 35.24           C  \n"
            "ATOM      6  CG BPRO A   1      28.910  37.672   7.181  0.66 38.20           C  \n"
            "ATOM      7  CD BPRO A   1      28.927  39.170   6.913  0.66 39.23           C  \n"
            "ATOM      8  H2 BPRO A   1      29.246  40.197   5.100  0.66  0.20           H  \n"
            "ATOM      9  H3 BPRO A   1      28.124  39.059   5.075  0.66  0.20           H  \n";

    std::string ATOM_residueTypeUnknown  =
            "ATOM     10  N   ABA B   2      29.262  23.791   3.255  1.00 18.43           N  \n";

    std::string aBitBiggerAtomBlock =
            "ATOM      1  N   PRO A   1      29.071  39.248   5.491  1.00 36.45           N  \n"
            "ATOM      2  CA  PRO A   1      29.983  38.171   5.136  1.00 35.67           C  \n"
            "ATOM      3  C   PRO A   1      29.540  37.373   3.917  1.00 31.34           C  \n"
            "ATOM      4  O   PRO A   1      28.380  37.476   3.533  1.00 34.24           O  \n"
            "ATOM      5  CB  PRO A   1      30.126  37.270   6.375  1.00 35.64           C  \n"
            "ATOM      6  CG  PRO A   1      28.910  37.672   7.181  1.00 38.30           C  \n"
            "ATOM      7  CD  PRO A   1      28.927  39.170   6.913  1.00 39.93           C  \n"
            "ATOM      8  H2  PRO A   1      29.246  40.197   5.100  1.00  0.00           H  \n"
            "ATOM      9  H3  PRO A   1      28.124  39.059   5.075  1.00  0.00           H  \n"
            "ATOM     10  N   GLN A   2      30.417  36.603   3.299  1.00 30.20           N  \n"
            "ATOM     11  CA  GLN A   2      30.037  35.792   2.165  1.00 32.99           C  \n"
            "ATOM     12  C   GLN A   2      30.067  34.364   2.629  1.00 33.44           C  \n"
            "ATOM     13  O   GLN A   2      31.097  33.938   3.141  1.00 34.15           O  \n"
            "ATOM     14  CB  GLN A   2      30.999  35.913   1.046  1.00 36.81           C  \n"
            "ATOM     15  CG  GLN A   2      30.349  35.299  -0.134  1.00 44.73           C  \n"
            "ATOM     16  CD  GLN A   2      31.238  35.322  -1.346  1.00 51.11           C  \n"
            "ATOM     17  OE1 GLN A   2      32.218  34.584  -1.345  1.00 54.41           O  \n"
            "ATOM     18  NE2 GLN A   2      30.989  36.146  -2.352  1.00 49.42           N  \n"
            "ATOM     19  H   GLN A   2      31.340  36.518   3.602  1.00  0.00           H  \n"
            "ATOM     20 HE21 GLN A   2      31.567  36.076  -3.121  1.00  0.00           H  \n"
            "ATOM     21 HE22 GLN A   2      30.232  36.770  -2.292  1.00  0.00           H  \n"
            "ATOM     65  N   GLN B   1      29.262  23.791   3.255  1.00 18.43           N  \n";




    std::string SimplePDBBlock = emitMODELline(1) + Model1_content + "ENDMDL\n" + emitMODELline(2) + Model2_content + "ENDMDL\n";

    std::string SimplePDBBlock_noModel1 = Model1_content + emitMODELline(2) + Model2_content + "ENDMDL\n";

    std::string SimplePDBBlock_noModelAtAll = Model1_content;





    std::string DuplicatedModelPDB =
            emitMODELline(2) + smallATOMBlock + "ENDMDL\n" +
            emitMODELline(2) + aBitBiggerAtomBlock + "ENDMDL\n"
        ;

    std::string IncorrectENDML_NoModelOpen =
            smallATOMBlock + "ENDMDL\n" +
            emitMODELline(2) + aBitBiggerAtomBlock + "ENDMDL\n"
    ;

    std::string IncorrectENDML_ModelDoubleClose1 =
            emitMODELline(1) + smallATOMBlock + "ENDMDL\n" + "ENDMDL\n" +
            emitMODELline(2) + aBitBiggerAtomBlock + "ENDMDL\n"
    ;

    std::string IncorrectENDML_ModelDoubleClose2 =
            emitMODELline(1) +smallATOMBlock + "ENDMDL\n" +
            emitMODELline(2) + aBitBiggerAtomBlock + "ENDMDL\n" + "ENDMDL\n"
    ;


    std::string Pedantic_ModelNoSameContent =
            emitMODELline(1) + smallATOMBlock + "ENDMDL\n" + emitMODELline(2) + Model2_content + "ENDMDL\n";

    std::string WithUnknownResidueName =
            emitMODELline(1) + smallATOMBlock + ATOM_residueTypeUnknown + "ENDMDL\n";

    std::string incorrectResidueInsertionCode =
            "ATOM      1  N   PRO A   1      29.071  39.248   5.491  1.00 36.45           N  \n"
            "ATOM      2  CA  PRO A   1A     29.983  38.171   5.136  1.00 35.67           C  \n"
            "ATOM      3  C   PRO A   1      29.540  37.373   3.917  1.00 31.34           C  \n";

    std::string incorrectResidueName =
            "ATOM      1  N   PRO A   1      29.071  39.248   5.491  1.00 36.45           N  \n"
            "ATOM      2  CA  ALA A   1      29.983  38.171   5.136  1.00 35.67           C  \n"
            "ATOM      3  C   PRO A   1      29.540  37.373   3.917  1.00 31.34           C  \n";

    std::string incorrectResidueNameAndInsertionCode =
            "ATOM      1  N   PRO A   1      29.071  39.248   5.491  1.00 36.45           N  \n"
            "ATOM      2  CA  ALA A   1C     29.983  38.171   5.136  1.00 35.67           C  \n"
            "ATOM      3  C   PRO A   1      29.540  37.373   3.917  1.00 31.34           C  \n";

    // Our primary clue is the sequence number, so we (intentionally) dont detect mixup where the sequence number is
    // incorrect but the name and insertion code are fine.

};

#endif //PDBPC_SIMPLEPDBFIXTURE_H
