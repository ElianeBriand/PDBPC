#!/bin/bash
set -ev

THREAD=1
SEND=1

while getopts t:s: option
do
case "${option}"
in
t) THREAD=${OPTARG};;
s) SEND=${OPTARG};;
esac
done


mkdir build_cov
export GCOV_PREFIX=`pwd`/build_cov/data
cd build_cov
export GCOV_PREFIX_STRIP=`pwd | awk -F"/" '{print NF-1}'`
cmake .. -DCOVERAGE_BUILD=ON -DCMAKE_BUILD_TYPE=Debug
make -j$THREAD PDBPC_unittest
./PDBPC_unittest
cd CMakeFiles
cp --parents `find . -name '*.gcno'` ../data
cd ..
export GCOV_PREFIX=`pwd`
export GCOV_PREFIX_STRIP=`pwd | awk -F"/" '{print NF-1}'`
make PDBPC_coverage
if [[ $SEND != 0 ]] ;
then
bash <(curl -s https://codecov.io/bash) -f PDBPC_coverage.info || echo "Codecov did not collect coverage reports";
fi
