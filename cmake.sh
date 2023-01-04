#/bin/bash

DIR=`dirname $0`
cd $DIR

set -o errexit
set -o nounset
USAGE="
1) Build with cmake:\n
      \t[-v | --verbose]\n       
      \t[debug | released]\n
      \t[reset]                  \t\t-- remove build dir\n
      \t[generate]               \t\t-- generate buildtree from CMakeLists.txt\n
      \t[target=value]\n            
      \t[clean]                  \t\t-- run cmake clean target\n
      \t[(\`key | key=value\`)*]   \t-- cmake variables\n
\n
2) Test with cmake ctest:\n
      \t[-v | --verbose]\n
      \t[debug | released]\n
      \ttest\n
      \t[sep]                    \t\t-- tests are printed separately
"

CMAKE=cmake
BUILD=./build

TARGET_BUILD=1 # run always target
TYPE=Debug
BUILD_DIR=$BUILD/debug

TARGET_CLEAN=
TARGET_RESET=
VERBOSE=
TARGET_GENERATE=
JOBS="-j8"

CMAKE_BUILDTREE_VARIABLES="-DPROGRAM_LINUX=''" 
# CMAKE_BUILDTREE_VARIABLES="-DPROGRAM_WINDOWS=''" 
CMAKE_BUILDTREE_OPTIONS="" # --warn-uninitialized
CMAKE_BUILD_OPTIONS=""

TARGET_TEST=
SEPARATE=
TEST_EXEC=bin/dbms_test

TARGET_VALGRIND=
VALGRIND_ARGS=--leak-check=yes
VALGRIND_PROG=$TEST_EXEC

function echosc() {
  echo -n "[CMAKE.SH]: Running "
  echo $@
}

for arg; do
  key=${arg%%=*}
  value=${arg#*=} # file arg without = then [value = key]
  case "$key" in
    --help|-h|help)    echo -e $USAGE; exit 0;;
    -v|--verbose) VERBOSE='-v'  ;;
    debug)        TYPE=Debug;   BUILD_DIR=$BUILD/debug ;;
    released)      TYPE=Released; BUILD_DIR=$BUILD/released ;;
    sanitized)    TYPE=Sanitized; BUILD_DIR=$BUILD/sanitized ;;
    clean)        TARGET_CLEAN=1  ;;
    reset)        TARGET_RESET=1 ;;
    gen|generate) TARGET_GENERATE=1 ;;
    test)         TARGET_TEST=1 ;;
    sep)          SEPARATE=1 ;;
    val|valgrind) TARGET_VALGRIND=1 ;;
    t|target)       CMAKE_BUILD_OPTIONS="$CMAKE_BUILD_OPTIONS --target $value";;
    *)            if [ "$key" = "$value" ]; then
                    CMAKE_BUILDTREE_VARIABLES="$CMAKE_BUILDTREE_VARIABLES $key=''"
                  else
                    CMAKE_BUILDTREE_VARIABLES="$CMAKE_BUILDTREE_VARIABLES $key=$value"
                  fi ;;
  esac
done

mkdir -p $BUILD_DIR

# COMMANDS
# Reset
if [[ -n $TARGET_RESET && -d $BUILD_DIR ]]; then
  echosc "RESET"
  rm -rf $BUILD_DIR
fi
# Generate CMake
if [[ -n $TARGET_RESET || -n $TARGET_GENERATE ]]; then
  GENERATE=1
  echosc "GENERATE"
  $CMAKE -S . -B $BUILD_DIR $CMAKE_BUILDTREE_OPTIONS -DCMAKE_BUILD_TYPE=$TYPE $CMAKE_BUILDTREE_VARIABLES
fi
# Clean
if [[ -z $TARGET_GENERATE && -n $TARGET_CLEAN ]]; then
  echosc "CLEAN"
  $CMAKE --build $BUILD_DIR --target clean
fi
# Build
if [[ -n $TARGET_BUILD ]]; then
  echosc "BUILD"
  $CMAKE --build $BUILD_DIR $VERBOSE $JOBS $CMAKE_BUILD_OPTIONS
fi
# Test
if [[ -n $TARGET_TEST ]]; then
  cd $DIR
  if [[ -n $SEPARATE ]]; then
    echosc "TEST SEPARATE"
    ./$BUILD_DIR/$TEST_EXEC --gtest_color=yes
  else
    echosc "TEST GROUPED"
    cd $BUILD_DIR && ctest --gtest-color=yes
  fi
fi
# Valgrind checks
if [[ -n $TARGET_VALGRIND ]]; then
  echosc "VALGRIND"
  valgrind $VALGRIND_ARGS ./$BUILD_DIR/$VALGRIND_PROG
fi
