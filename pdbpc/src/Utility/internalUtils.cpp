//
// Created by eliane on 11/05/19.
//

#include <pdbpc/Utility/internalUtils.h>

#include <boost/log/trivial.hpp>


namespace pdbpc {


    void checkPDBPath(bfs::path pdbPath) {

        if (bfs::exists(pdbPath)) {
            if (bfs::is_regular_file(pdbPath)) {
                if ((pdbPath.extension() != ".pdb") && (pdbPath.extension() != ".PDB")) {
                    BOOST_LOG_TRIVIAL(warning) << "Expected .pdb file, got : " << pdbPath.filename();
                    BOOST_LOG_TRIVIAL(warning) << "Will proceed nonetheless.";
                }
                return;
            } else if (bfs::is_directory(pdbPath)) {
                BOOST_LOG_TRIVIAL(error) << "Expected file, got directory : " << pdbPath;
                exit(2);
            } else {
                BOOST_LOG_TRIVIAL(error) << "Not a regular file : " << pdbPath;
                exit(2);
            }
        } else {
            BOOST_LOG_TRIVIAL(error) << "File not found : " << pdbPath;
            exit(2);
        }

    }

    void checkForErrorsWarningsProblems(ParsedPDB& ppdb) {
        ppdb.hasErrors = false;
        ppdb.hasWarnings = false;
        ppdb.hasPedanticProblems = false;
        for (const auto& record: ppdb.outOfBandRecords) {
            if (record->severity == OutOfBandSeverity::error)
                ppdb.hasErrors = true;
            if (record->severity == OutOfBandSeverity::warning)
                ppdb.hasWarnings = true;
            if (record->severity == OutOfBandSeverity::idiosyncrasy)
                ppdb.hasPedanticProblems = true;
        }
    }

    void createPlaceholderModelAndChain(ParsedPDB& ppdb) {
        auto defaultModel = std::make_shared<Model>();
        defaultModel->modelNumber = -1;
        defaultModel->openingLineNumber = 0;
        defaultModel->openingLine = "<Beginning of file>";
        defaultModel->closingLineNumber = ppdb.numberOfParsedLines + 1;
        defaultModel->closingLine = "<End of file>";
        ppdb.models.push_back(defaultModel);
    }

    int findNextFreeModelNumber(ParsedPDB& ppdb) {
        int modelNum = 1;

        while (true) {
            auto find_it = std::find_if(ppdb.models.begin(),
                                        ppdb.models.end(),
                                        [modelNum](const std::shared_ptr<Model>& m_sptr) {
                                            return m_sptr->modelNumber == modelNum;
                                        });
            if (find_it == ppdb.models.end()) // modelNum is not already in use
                return modelNum;
            modelNum++;
        }
    }


}