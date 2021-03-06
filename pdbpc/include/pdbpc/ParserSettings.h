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

#ifndef PDBPC_PARSERSETTINGS_H
#define PDBPC_PARSERSETTINGS_H

namespace pdbpc {

    enum class __attribute__ ((visibility ("default"))) DuplicateModelPolicy  {
        renumberAfterParsingCompletion,
        dropDuplicate
    };

    enum class __attribute__ ((visibility ("default"))) AltLocationPolicy  {
        firstAppearing,
        highestOccupancy,
        leaveAsIs
    };

    struct __attribute__ ((visibility ("default"))) ParserSettings {

        DuplicateModelPolicy duplicateModelPolicy = DuplicateModelPolicy::renumberAfterParsingCompletion;

        AltLocationPolicy altLocationPolicy = AltLocationPolicy::firstAppearing;

        bool doPedanticVerifications = false;
    };

}


#endif //PDBPC_PARSERSETTINGS_H
