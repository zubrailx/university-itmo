#!/bin/bash

DIR=`dirname $0`
cd $DIR

set -o errexit
set -o nounset
USAGE="
1) Build with cmake:\n
      \t[-v | --verbose]\n       
      \t[debug | release]\n
      \t[reset]                  \t\t-- remove build dir\n
      \t[generate]               \t\t-- generate buildtree from CMakeLists.txt\n
      \t[target=value]\n            
      \t[clean]                  \t\t-- run cmake clean target\n
      \t[(\`key | key=value\`)*]   \t-- cmake variables\n
\n
2) Test with cmake ctest:\n
      \t[-v | --verbose]\n
      \t[debug | release]\n
      \ttest\n
"

CMAKE=cmake
BUILD=./build
TYPE=DEBUG
BUILD_DIR=$BUILD/debug
CLEAN=
RESET=
VERBOSE=
GENERATE=
JOBS="-j1"
CMAKE_BUILDTREE_VARIABLES="-DPROGRAM_LINUX=''" 
# CMAKE_BUILDTREE_VARIABLES="-DPROGRAM_WINDOWS=''" 
CMAKE_BUILDTREE_OPTIONS="" # --warn-uninitialized
CMAKE_BUILD_OPTIONS=""
TEST=

for arg; do
  key=${arg%%=*}
  value=${arg#*=} # file arg without = then [value = key]
  case "$key" in
    --help|-h|help)    echo -e $USAGE; exit 0;;
    -v|--verbose) VERBOSE='-v'  ;;
    debug)        TYPE=DEBUG;   BUILD_DIR=$BUILD/debug ;;
    release)      TYPE=RELEASE; BUILD_DIR=$BUILD/release ;;
    clean)        CLEAN=1  ;;
    reset)        RESET=1 ;;
    gen|generate) GENERATE=1 ;;
    test)         TEST=1 ;;
    target)       CMAKE_BUILD_OPTIONS="$CMAKE_BUILD_OPTIONS --target $value";;
    *)            if [ "$key" = "$value" ]; then
                    CMAKE_BUILDTREE_VARIABLES="$CMAKE_BUILDTREE_VARIABLES $key=''"
                  else
                    CMAKE_BUILDTREE_VARIABLES="$CMAKE_BUILDTREE_VARIABLES $key=$value"
                  fi ;;
  esac
done

# COMMANDS
if [[ -n $TEST ]]; then
  cd $BUILD_DIR && ctest
else
  # Reset
  [[ -n $RESET && -d $BUILD_DIR ]] && rm -rf $BUILD_DIR
  # Generate
  [[ "$GENERATE" -ne 0 ]] && $CMAKE -S . -B $BUILD_DIR $CMAKE_BUILDTREE_OPTIONS -DCMAKE_BUILD_TYPE=$TYPE $CMAKE_BUILDTREE_VARIABLES
  # Clean
  [[ -n $CLEAN ]] && $CMAKE --build $BUILD_DIR --target clean
  # Build
  [[ -d $BUILD_DIR ]] && $CMAKE --build $BUILD_DIR $VERBOSE $JOBS $CMAKE_BUILD_OPTIONS
fi
