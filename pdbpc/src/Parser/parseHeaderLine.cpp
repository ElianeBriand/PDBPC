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

#include <locale>
#include <iostream>
#include <sstream>

#include <boost/date_time.hpp>
#include <boost/algorithm/string.hpp>


#include <Parser/parseHeaderLine.h>
#include <Utility/internalUtils.h>

namespace b = boost;

namespace pdbpc {


    void parseHeaderLine(ParsedPDB& ppdb, const std::string& headerLine, int lineNumber) {

        ppdb.details.headerLine = headerLine;
        ppdb.details.headerLineNumber = lineNumber;

        std::string line = headerLine;

        bool hasMinimumColumn = inflateShorterLineOrFail(ppdb, line, lineNumber, 66, 80);

        if (!hasMinimumColumn) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::HeaderRecordTooShort;
            rec->line = headerLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);
            return;
        }

        trimAfterLastColumn(ppdb, line, lineNumber, 80);

        auto it_beg = line.begin();

        std::string classificationString(it_beg + 10, it_beg + 50);
        std::string depositionDateString(it_beg + 50, it_beg + 59);
        std::string PDBCodeString(it_beg + 62, it_beg + 66);


        b::gregorian::date_input_facet* pdbInputFacet = new b::gregorian::date_input_facet("%d-%b-%y");

        boost::gregorian::date depositionDate(boost::gregorian::not_a_date_time);
        std::stringstream ss;
        ss.imbue(std::locale(ss.getloc(), pdbInputFacet));
        ss << depositionDateString;
        ss >> depositionDate;


        if(ss.fail()) {
            auto rec = std::make_shared<OutOfBandRecord>();
            rec->severity = OutOfBandSeverity::error;
            rec->type = OutOfBandType::IncorrectPDBLineFormat;
            rec->subtype = OutOfBandSubType::DateCannotBeParsed;
            rec->line = headerLine;
            rec->lineNumber = lineNumber;
            rec->recoveryStatus = RecoveryStatus::recovered;
            ppdb.outOfBandRecords.push_back(rec);

            ppdb.details.depositionDate = boost::gregorian::date(boost::gregorian::not_a_date_time);
        }else{
            b::gregorian::date today = b::gregorian::day_clock::local_day();

            if (depositionDate > today) {
                // Assume there was a mismatch between two digit years (ie 20XX instead of 19XX)
                boost::gregorian::years oneCentury(100);
                depositionDate -= oneCentury;
            }


            ppdb.details.depositionDate = depositionDate;
        }


        ppdb.details.classification =  b::trim_copy(classificationString);
        ppdb.details.PDBCode = b::trim_copy(PDBCodeString);


    }


} // namespace pdbpc