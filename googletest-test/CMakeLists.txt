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

if (NOT TESTING)
    # Build mylib_app
    add_executable(mylib_app
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    )
    target_compile_features(mylib_app PRIVATE c_std_99)
    target_link_libraries(mylib_app PRIVATE mylib)

else()
    include(FetchContent)

    # Fetch UMock
    set(UMOCK_VERBOSE ON CACHE BOOL "UMock: Verbose mock configuration" FORCE)
    if (UMOCK_DEBUG)
        FetchContent_Declare(
            umock
            SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../umock/"
        )
    else ()
        FetchContent_Declare(
            umock
            GIT_REPOSITORY  https://github.com/debuti/umock.git
            GIT_TAG         0.1.2
        )
    endif()
    FetchContent_MakeAvailable(umock)

    # Fetch GoogleTest
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY  https://github.com/google/googletest.git
        GIT_TAG         v1.14.0
    )
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(BUILD_GTEST ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Build mylib tester
    set_source_files_properties(test/mylib.test.c PROPERTIES LANGUAGE CXX)
    add_executable(mylib_test test/mylib.test.c)
    target_compile_features(mylib_test PRIVATE c_std_99)
    target_include_directories(mylib_test PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
    # UMock only adds one extra file to the tester automatically
    target_link_libraries(mylib_test PRIVATE gtest_main mylib)

    # Tell UMock to create the mocks for this tester
    umock_this(SUTS mylib TESTER mylib_test)    

    enable_testing()

    include(GoogleTest)
    gtest_discover_tests(mylib_test)

    add_test(NAME mylib_test COMMAND mylib_test)
endif()
