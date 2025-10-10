cmake_minimum_required(VERSION 3.10)
project(FileScanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows
if(WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS SCANCORE_EXPORTS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Опция для включения тестов (по умолчанию выключена)
option(BUILD_TESTS "Build unit tests" OFF)

# Динамическая библиотека
add_library(ScanCore SHARED
    ScanCore.cpp
    FileScan.cpp
)

if(WIN32)
    target_link_libraries(ScanCore)
endif()

target_include_directories(ScanCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Scanner Scanner.cpp)

target_link_libraries(Scanner ScanCore)

# Unit тесты (только если включена опция BUILD_TESTS)
if(BUILD_TESTS)
    message(STATUS "Building unit tests")

    find_package(GTest REQUIRED)

    if(GTest_FOUND)
        message(STATUS "GTest found: ${GTEST_INCLUDE_DIRS}")

        add_executable(ScannerTests
            tests/main.cpp
            tests/FileScanTest.cpp
            tests/DirectoryScannerTest.cpp
            tests/MD5Test.cpp
        )

        target_link_libraries(ScannerTests
            ScanCore
            GTest::gtest
            GTest::gtest_main
        )

        include(GoogleTest)
        gtest_discover_tests(ScannerTests)

        message(STATUS "Unit tests enabled - use 'make test' or 'ctest' to run tests")
    else()
        message(WARNING "GTest not found, unit tests will not be built")
    endif()
else()
    message(STATUS "Unit tests disabled - use -DBUILD_TESTS=ON to enable")
endif()

install(TARGETS Scanner ScanCore
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES ScanCore.hpp DESTINATION include)