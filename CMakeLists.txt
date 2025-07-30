cmake_minimum_required(VERSION 3.21)
project(cscript VERSION 1.0 LANGUAGES C )

set(TARGET cscript)

set(CMAKE_C_FLAGS "-Wall -Wextra -g -MMD -MP -fPIC")
set(CMAKE_SHARED_LINKER_FLAGS "")

include_directories(.)

set(SRC_DIR "src")
set(PARSER_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(FILES_TRANSPORT
		"${SRC_DIR}/typechecker.h"
		"${SRC_DIR}/typechecker.c"
		"${SRC_DIR}/interpreter.h"
		"${SRC_DIR}/interpreter.c"
		"${SRC_DIR}/cscript.h"
		"${SRC_DIR}/cscript_util.h"
		"${SRC_DIR}/cscript_util.c"
		"${SRC_DIR}/cscript_internals.h"
		"${SRC_DIR}/cscript_types.h"
)

# Copy AST package
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ast"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "../${SRC_DIR}/ast" "${CMAKE_CURRENT_BINARY_DIR}/ast"
    DEPENDS "${SRC_DIR}/ast"
)

foreach(file ${FILES_TRANSPORT})
    get_filename_component(fname ${file} NAME)
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${fname}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "../${file}" "${CMAKE_CURRENT_BINARY_DIR}/${fname}"
        DEPENDS "${file}"
    )
    list(APPEND COPIED_FILES "${CMAKE_CURRENT_BINARY_DIR}/${fname}")
endforeach()

add_custom_target(copy_sources ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ast ${COPIED_FILES})

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set(LIBS reis fl m)

set(LEXER_OUT "${PARSER_DIR}/cscript_lexer.c")
set(PARSER_OUT "${PARSER_DIR}/cscript_parser.c")

flex_target(LEXER 
		"${SRC_DIR}/cscript.l" 
		${LEXER_OUT} 
    DEFINES_FILE "${PARSER_DIR}/cscript_lexer.h")
bison_target(PARSER "${SRC_DIR}/cscript.y" "${PARSER_OUT}"
             DEFINES_FILE "${PARSER_DIR}/cscript_parser.h")
						 #REPORT_FILE ${CMAKE_BINARY_DIR}/bison_report.txt
						 #COMPILE_FLAGS -Wcounterexamples)
add_flex_bison_dependency(LEXER PARSER)

add_library(${TARGET} SHARED 
		"${SRC_DIR}/cscript.c"
		"${SRC_DIR}/cscript_util.c"
		"${SRC_DIR}/ast/ast.c"
		"${SRC_DIR}/typechecker.c"
		"${SRC_DIR}/interpreter.c"
		"${LEXER_OUT}"
		"${PARSER_OUT}")

include_directories(${SRC_DIR})

target_include_directories(${TARGET} PRIVATE ${PARSER_DIR})
target_link_libraries(${TARGET} ${LIBS})

# Testing binary
add_executable(tester tests/tester.c)
target_link_libraries(tester ${TARGET} ${LIBS})
target_include_directories(tester PRIVATE ${CMAKE_BINARY_DIR})
#add_executable(Arithmetic tests/+arithmetic.c)

enable_testing()

add_test(
		NAME tester
		COMMAND tester 
)
