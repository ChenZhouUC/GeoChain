# define the minimum required version of cmake
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# assign the project title
PROJECT(GeoChain)

# assign building flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# find required package jsoncpp
FIND_PACKAGE(jsoncpp CONFIG REQUIRED)

# find required package glog
FIND_PACKAGE(glog REQUIRED)

# find optional package openmp
FIND_PACKAGE(OpenMP REQUIRED)
IF(OPENMP_FOUND)
    MESSAGE("######!!!!!! OPENMP Found !!!!!!######")
    # adding build flags for openmp
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE()
    MESSAGE("######!!!!!! OPENMP Not Found !!!!!!######")
ENDIF()

# find OpenCV package
SET(OpenCV_DIR "/usr/local/opencv3.4.5/share/OpenCV")
SET(OpenCV_INCLUDE_DIRS "/usr/local/opencv3.4.5/include")
FIND_PACKAGE(OpenCV REQUIRED)

# define path alias
SET(SOURCE_ROOT .)
# SET(SOURCE_CPP ${SOURCE_ROOT}/src)
# SET(SOURCE_HEADER ${SOURCE_ROOT}/include)
# SET(SOURCE_BUILD ${SOURCE_ROOT}/build)
# SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
# SET(CMAKE_BINARY_DIR ${SOURCE_BUILD})
# SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# store all files under AUX_SOURCE_DIRECTORY into variable DIR_ROOT_SRCS
AUX_SOURCE_DIRECTORY(${SOURCE_ROOT} DIR_ROOT_SRCS)

# store all related files into variable SOURCE_FILES recursively
# FILE(GLOB_RECURSE SOURCE_FILES ${SOURCE_CPP}/*.cpp)
MESSAGE("==============================================")
MESSAGE("------------- Main Source Files -------------")
MESSAGE("${DIR_ROOT_SRCS}")
# MESSAGE("------------- Other Source Files -------------")
# MESSAGE("${SOURCE_FILES}")
MESSAGE("==============================================")

# assign header file path
INCLUDE_DIRECTORIES(
    /usr/local/include
    /usr/include
    ${OpenCV_INCLUDE_DIRS}
    ${SOURCE_ROOT}/Arithmetics
    ${SOURCE_ROOT}/Elements
    ${SOURCE_ROOT}/Utils
    ${SOURCE_ROOT}/Vessels
)

# assign shared lib path
LINK_DIRECTORIES(
    /usr/lib/x86_64-linux-gnu
    /usr/local/lib/
)

# SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
# SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)
# SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)

# define executable file
ADD_EXECUTABLE(geochain main.cpp ${SOURCE_FILES})

# link libs required
TARGET_LINK_LIBRARIES(geochain ${OpenCV_LIBS} ${CURL_LIBRARY} jsoncpp_lib glog)

SET_TARGET_PROPERTIES(
    geochain
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

MESSAGE("######!!!!!! CMake Succeed !!!!!!######")
