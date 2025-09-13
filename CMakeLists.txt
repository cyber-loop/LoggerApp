cmake_minimum_required(VERSION 3.16)
project(LoggerApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Gui Network QuickControls2)

add_executable(LoggerApp
    src/main.cpp
    src/loggermodel.h
    src/loggermodel.cpp
    src/proxyfiltermodel.h
    src/proxyfiltermodel.cpp
    qml/qml.qrc
)

target_link_libraries(LoggerApp PRIVATE Qt6::Core Qt6::Quick Qt6::Gui Qt6::Network Qt6::QuickControls2)

find_package(Doxygen)

if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )

    message(STATUS "Added \"make doc\" target to generate API-documentation")
else()
    message(STATUS "doxygen binary not found. Can't generate API-doc")
endif()

# ------------------------------------------------------------------------------
# Unit tests
# ------------------------------------------------------------------------------
option(BUILD_TESTING "Build unit tests" OFF)

if(BUILD_TESTING)
    find_package(Qt6 REQUIRED COMPONENTS Core Quick Gui Network QuickControls2 Test)
    enable_testing()

    # Test: LoggerModel
    add_executable(tst_loggermodel
        tests/tst_loggermodel.cpp
        src/loggermodel.cpp
    )
    target_include_directories(tst_loggermodel PRIVATE src)
    target_link_libraries(tst_loggermodel Qt6::Test Qt6::Core Qt6::Network)
    add_test(NAME tst_loggermodel COMMAND tst_loggermodel)
    set_tests_properties(tst_loggermodel PROPERTIES LABELS "LoggerApp")

    # Test: ProxyFilterModel
    add_executable(tst_proxyfiltermodel
        tests/tst_proxyfiltermodel.cpp
        src/loggermodel.cpp
        src/proxyfiltermodel.cpp
    )
    target_include_directories(tst_proxyfiltermodel PRIVATE src)
    target_link_libraries(tst_proxyfiltermodel Qt6::Test Qt6::Core Qt6::Network)
    add_test(NAME tst_proxyfiltermodel COMMAND tst_proxyfiltermodel)
    set_tests_properties(tst_proxyfiltermodel PROPERTIES LABELS "LoggerApp")

    # Test: Main application startup
    add_executable(tst_main
        tests/tst_main.cpp
        src/loggermodel.cpp
        src/proxyfiltermodel.cpp
    )
    target_include_directories(tst_main PRIVATE src)
    target_link_libraries(tst_main
        Qt6::Test
        Qt6::Core
        Qt6::Quick
        Qt6::Gui
        Qt6::Network
        Qt6::QuickControls2
    )
    add_test(NAME tst_main COMMAND tst_main)
    set_tests_properties(tst_main PROPERTIES LABELS "LoggerApp")
endif()
