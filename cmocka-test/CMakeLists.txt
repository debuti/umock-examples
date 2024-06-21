cmake_minimum_required(VERSION 3.9)

project(cmocka-test
    VERSION 0.1.0
    LANGUAGES C)

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
    # UMock only adds one extra file to the tester automatically
    target_link_libraries(mylib_test PRIVATE cmocka mylib)
    
    # Tell UMock to create the mocks for this tester
    umock_this(SUTS mylib TESTER mylib_test)

    enable_testing()
    add_test(NAME mylib_test COMMAND mylib_test)
endif()
