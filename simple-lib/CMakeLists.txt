cmake_minimum_required(VERSION 3.9)

project(simple-lib
    VERSION 0.1.0
    LANGUAGES C)

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
    FetchContent_Declare(
        umock
        GIT_REPOSITORY https://github.com/debuti/umock.git
        GIT_SHALLOW    1
    )
    FetchContent_MakeAvailable(umock)
    include(${umock_SOURCE_DIR}/umock/cmake/umock.cmake)

    # Fetch CMocka
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "CMocka: Build as a shared library" FORCE)
    set(WITH_EXAMPLES OFF CACHE BOOL "CMocka: Build examples" FORCE)
    set(UNIT_TESTING OFF CACHE BOOL "CMocka: Build with unit testing" FORCE)
    FetchContent_Declare(
        cmocka
        GIT_REPOSITORY https://git.cryptomilk.org/projects/cmocka.git
        GIT_TAG        cmocka-1.1.6
        GIT_SHALLOW    1
    )
    FetchContent_MakeAvailable(cmocka)

    # Build mylib tester
    add_executable(mylib_test test/mylib.test.c)
    target_compile_features(mylib_test PRIVATE c_std_99)
    target_include_directories(mylib_test PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
    # UMock is not a binary dependency
    target_link_libraries(mylib_test PRIVATE cmocka mylib)
    
    # Tell UMock to create the mocks for this tester
    umock_this(SUT mylib TESTER mylib_test)

    enable_testing()
    add_test(NAME mylib_test COMMAND mylib_test)
endif()
