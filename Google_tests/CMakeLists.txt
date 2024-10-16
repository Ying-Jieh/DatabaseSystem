project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target

file(GLOB_RECURSE SOURCES "../src/*/*.cpp")  # Note: DON'T include main.cpp!

file(GLOB_RECURSE HEADERS "../include/*.hpp")

file(GLOB testSRC
        "dbTests/*.cpp"
)

add_executable(Google_Tests_run ${HEADERS} ${SOURCES} ${testSRC})
#add_executable(Google_Tests_run ${testSRC})

# linking Google_Tests_run with DateConverter_lib which will be tested
#add_subdirectory(..)
#link_directories("../cmake-build-debug/CMakeFiles/")
#target_link_libraries(Google_Tests_run ECE141_lib)

target_link_libraries(Google_Tests_run gtest gtest_main)

