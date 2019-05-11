cmake_minimum_required(VERSION 3.9)
project(PDBPC VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 14)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS log filesystem unit_test_framework)
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
        pdbpc/src/Parser/PostProcessing/modelPostProcessing.cpp

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




add_executable(PDBPC_example example.cpp)
set_target_properties(PDBPC_example PROPERTIES LINK_FLAGS " -Wl,--no-undefined -Wl,--as-needed  -static   ")
target_link_libraries(PDBPC_example pdbpc_static)
target_link_libraries(PDBPC_example ${Boost_LIBRARIES})
target_link_libraries(PDBPC_example pthread)
target_include_directories(PDBPC_example PRIVATE ./pdbpc/include/)

enable_testing()

add_executable(PDBPC_unittest
        pdbpc/test/MainTestSuite.cpp)

target_link_libraries(PDBPC_unittest pdbpc_shared)
target_link_libraries(PDBPC_unittest ${Boost_LIBRARIES})
target_link_libraries(PDBPC_unittest pthread)
target_include_directories(PDBPC_unittest PRIVATE ./pdbpc/include/)

add_test(PDBPC_unittest pdbpc_static)