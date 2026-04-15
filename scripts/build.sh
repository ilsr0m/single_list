#!/usr/bin/env bash

set -e

BUILD_TYPE="Release"
GENERATOR="MinGW Makefiles"
JOBS=2
CLEAN=false
RUN=false
VALGRIND=false

# Color for printed text
MAGENTA="\033[35m"
RED="\033[31m"
GREEN="\033[32m"
RESET="\033[0m"

while [[ $# -gt 0 ]]; do
	case $1 in
		-b|--build-type)
			BUILD_TYPE="$2"
			shift 2
			;;
		-j|--jobs)
			JOBS="$2"
			shift 2
			;;
		-c|--clean)
			CLEAN=true
			shift 
			;;
		-g|--generator)
			GENERATOR="$2"
			shift 2
			;;
		-r|--run)
			RUN=true
			shift
			;;
		-v|--valgrind)
			VALGRIND=true
			shift
			;;
		*)
			echo -e "${RED}Unknown option: $1 ${RESET}"
			shift
			exit 1
			;;
	esac
done

BUILD_DIRECTORY="build-bash"
EXE_TEST="slist_test" 

# Return error if try valgrind with non-debug option
if [[ "$BUILD_TYPE" != "Debug" && "$VALGRIND" = true ]]; then
	echo -e "${RED}Valgrind is supported only for Debug builds.${RESET}"
	exit 1
fi

if [ "$CLEAN" = true ]; then
	echo -e "${MAGENTA}Removing build directory...${RESET}"
	rm -rf $BUILD_DIRECTORY
fi

echo -e "${MAGENTA}Configuring project...${RESET}"
cmake -S . -B $BUILD_DIRECTORY -G $GENERATOR -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

if [ $? -ne 0 ]; then
	echo -e "${RED}CMake configure failed.${RESET}"
	exit $?
fi

cmake --build $BUILD_DIRECTORY --verbose --parallel $JOBS

if [ $? -ne 0 ]; then
	echo -e "${RED}Build failed.${RESET}"
	exit $?
fi
echo -e "${GREEN}Build completed successfully.${RESET}"

if [ "$RUN" = true ]; then
	EXE_PATH=$(pwd)/"$BUILD_DIRECTORY"/"$EXE_TEST"
	if [ -f "$EXE_PATH" ]; then
		if [ "$VALGRIND" = true ]; then
			echo -e "${MAGENTA}Running ${EXE_PATH} with Valgrind...${RESET}"
			valgrind --leak-check=full --track-origins=yes "$EXE_PATH"
		else
			echo -e "${MAGENTA}Running ${EXE_PATH}...${RESET}"
			$EXE_PATH &
		fi
		exit 0
 	fi
 	echo -e "${RED}Executable not found: ${EXE_PATH}.${RESET}"
 	exit 
fi

