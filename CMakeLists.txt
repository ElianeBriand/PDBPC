cmake_minimum_required(VERSION 3.9)
project(PDBPC VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)


set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)



option(COVERAGE_BUILD "Create a GCov-enabled build." OFF)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COV_CXX_FLAGS} -Wall  -fvisibility=hidden  ")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COV_LDFLAGS} ")

SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
#SET(CMAKE_CXX_COMPILER "/usr/lib/llvm/7/bin/clang++")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

if(COVERAGE_BUILD)
    message("Enabling code coverage ...")
    if(CMAKE_COMPILER_IS_GNUCXX)
        include(CodeCoverage)
        APPEND_COVERAGE_COMPILER_FLAGS()
        set(COVERAGE_LCOV_EXCLUDES '/usr/*' './test/*')
        SETUP_TARGET_FOR_COVERAGE_LCOV(
             NAME ${PROJECT_NAME}_coverage                    # New target name
             EXECUTABLE PDBPC_unittest # Executable in PROJECT_BINARY_DIR
             DEPENDENCIES PDBPC_unittest                     # Dependencies to build first
        )
    endif()
endif()


SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g ")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O3 -march=native -g  ")

#set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_STATIC_LIBS        OFF)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS filesystem unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(./pdbpc/include/)

set(PDBPC_SOURCE_LIST
        pdbpc/src/ParsedPDB.cpp
        pdbpc/src/Records/Model.cpp
        pdbpc/src/Utility/readPDBFile.cpp
        pdbpc/src/Utility/internalUtils.cpp
        pdbpc/src/Parser/readPDBLine.cpp
        pdbpc/src/Records/OutOfBandRecord.cpp
        pdbpc/src/Parser/parseModelLine.cpp
        pdbpc/src/ParserSettings.cpp
        pdbpc/src/Parser/PostProcessing/postProcessParsedPDB.cpp
        pdbpc/src/Parser/parseEndModelLine.cpp
        pdbpc/src/Parser/PostProcessing/pedanticVerifications.cpp
        pdbpc/src/Parser/parseAtomLine.cpp
        pdbpc/src/Records/Atom.cpp
        pdbpc/src/Records/Residue.cpp
        pdbpc/src/Records/Chain.cpp
        pdbpc/src/Utility/readPDBBlock.cpp
        pdbpc/src/Parser/parseMasterEndLine.cpp
        pdbpc/src/Parser/parseHetRelatedLines.cpp
        )



add_library(pdbpc_static STATIC  ${PDBPC_SOURCE_LIST})
set_property(TARGET pdbpc_static PROPERTY POSITION_INDEPENDENT_CODE OFF)
target_include_directories(pdbpc_static PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pdbpc/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE pdbpc/src)

add_library(pdbpc_static_pic STATIC  ${PDBPC_SOURCE_LIST})
set_property(TARGET pdbpc_static_pic PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(pdbpc_static_pic PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pdbpc/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE pdbpc/src)

add_library(pdbpc_shared SHARED  ${PDBPC_SOURCE_LIST})
target_include_directories(pdbpc_shared PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pdbpc/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE pdbpc/src)


# 'make install' to the correct locations (provided by GNUInstallDirs).
install(TARGETS pdbpc_static EXPORT MyLibraryConfig_static
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})




add_executable(PDBPC_example examples/example.cpp)
set_target_properties(PDBPC_example PROPERTIES LINK_FLAGS " -Wl,--no-undefined -Wl,--as-needed  ")
target_link_libraries(PDBPC_example pdbpc_static)
target_link_libraries(PDBPC_example ${Boost_LIBRARIES})
target_link_libraries(PDBPC_example pthread)
target_include_directories(PDBPC_example PRIVATE ./pdbpc/include/)

enable_testing()

add_executable(PDBPC_unittest
        pdbpc/test/Main_TestSuite.cpp
        pdbpc/test/InternalUtils_TestSuite.cpp
        pdbpc/test/Line-ATOM_TestSuite.cpp
        pdbpc/test/Line-MODEL_TestSuite.cpp
        pdbpc/test/line-ENDMDL_TestSuite.cpp
        pdbpc/test/Line-MASTER_END_TestSuite.cpp pdbpc/test/Line-HET-etAl_TestSuite.cpp)

target_link_libraries(PDBPC_unittest -Wl,-Bstatic pdbpc_static -Wl,-Bdynamic)
target_link_libraries(PDBPC_unittest ${Boost_LIBRARIES})
target_link_libraries(PDBPC_unittest pthread)
target_include_directories(PDBPC_unittest PRIVATE ./pdbpc/include/)

add_test(AllUnitTest PDBPC_unittest --catch_system_error=yes --log_level=all )