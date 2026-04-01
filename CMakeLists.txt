cmake_minimum_required(VERSION 3.20)

project(IntentLang
    VERSION 0.1.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(INTENT_ENABLE_TESTS "Build tests" ON)

add_library(intent_core
    src/diagnostics/diagnostic.cpp
    src/lexer/lexer.cpp
    src/parser/parser.cpp)

target_include_directories(intent_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(intent
    src/main.cpp)

target_link_libraries(intent
    PRIVATE
        intent_core)

if(INTENT_ENABLE_TESTS)
    enable_testing()

    add_executable(intent_lexer_tests
        tests/lexer/lexer_tests.cpp)
    target_link_libraries(intent_lexer_tests PRIVATE intent_core)
    add_test(NAME LexerTests COMMAND intent_lexer_tests)
endif()

