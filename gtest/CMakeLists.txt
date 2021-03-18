project(google_test)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name

SET(TEST_SRC
        mekhail_test.cpp
        )

add_executable(Google_Tests_run ${TEST_SRC})

# PLACE STATIC LIBRARY IN 'libs_for_testing' FOLDER AS 'vector_lib.a'
find_library(LIB
        NAMES vector_lib
        HINTS ../libs_for_testing)

target_link_libraries(Google_Tests_run ${LIB})

target_link_libraries(Google_Tests_run gtest gtest_main)
