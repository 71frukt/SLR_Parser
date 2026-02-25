cmake_minimum_required(VERSION 3.21)
project(slr_parser LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

###===========================================FLAGS=================================================

option(SANITIZE "Enable Sanitizers" OFF)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_library(project_warnings INTERFACE)
    target_compile_options(project_warnings INTERFACE
        -Wall -Wextra -Werror -Wpedantic
        $<$<CONFIG:DEBUG>:-Og -g>
        $<$<CONFIG:RELEASE>:-O3 -DNDEBUG>
    )
endif()

if(SANITIZE AND NOT MSVC)
    add_library(project_sanitizers INTERFACE)
    target_compile_options(project_sanitizers INTERFACE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -fno-optimize-sibling-calls
    )
    target_link_options(project_sanitizers INTERFACE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )
endif()


###===========================================FLEX==================================================
find_package(FLEX REQUIRED)
FLEX_TARGET(MyScanner ${CMAKE_CURRENT_SOURCE_DIR}/source/src/lexer/lexer.l 
                      ${CMAKE_CURRENT_SOURCE_DIR}/source/src/lexer/lexer.cpp)

###===========================================SUBDIRECTORY==========================================

add_executable(${PROJECT_NAME}
    source/main.cpp

    source/src/lexer/lexer.cpp
    source/src/lexer/crazy_lexer.cpp

    source/src/syntaxer/parser.cpp
    source/src/syntaxer/grammar.cpp
    source/src/syntaxer/grammar_worker.cpp
    source/src/syntaxer/dfa_builder.cpp

    ${FLEX_MyScanner_OUTPUTS}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
)


###===========================================LIB_LINK==============================================

target_link_libraries(${PROJECT_NAME} PRIVATE project_warnings)

if(SANITIZE AND NOT MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE project_sanitizers)
endif()

# target_link_libraries(${PROJECT_NAME} PRIVATE
# )