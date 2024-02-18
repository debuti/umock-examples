cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)

project(google-test
    VERSION 0.1.0
    LANGUAGES C CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Build mylib
add_library(mylib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mylib.c
)
target_compile_features(mylib PRIVATE c_std_99)

# Build mylib_app
add_executable(mylib_app
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)
target_compile_features(mylib_app PRIVATE c_std_99)
target_link_libraries(mylib_app PRIVATE mylib)

if (TESTING)
    include(FetchContent)

    # Fetch UMock
    # FetchContent_Declare(
    #     umock
    #     SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../umock/"
    # )
    FetchContent_Declare(
        umock
        GIT_REPOSITORY https://github.com/debuti/umock.git
        GIT_SHALLOW    1
    )
    FetchContent_MakeAvailable(umock)
    include(${umock_SOURCE_DIR}/umock/cmake/umock.cmake)

    # Fetch GoogleTest
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.10.0
    )
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(BUILD_GTEST ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Build mylib tester
    SET_SOURCE_FILES_PROPERTIES(test/mylib.test.c PROPERTIES LANGUAGE CXX)
    add_executable(mylib_test test/mylib.test.c)
    target_compile_features(mylib_test PRIVATE c_std_99)
    target_include_directories(mylib_test PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
    # UMock is not a binary dependency
    target_link_libraries(mylib_test PRIVATE gtest_main mylib)
    
    # Tell UMock to create the mocks for this tester
    umock_this(SUT mylib TESTER mylib_test)

    enable_testing()

    include(GoogleTest)
    gtest_discover_tests(mylib_test)

    add_test(NAME mylib_test COMMAND mylib_test)
endif()
