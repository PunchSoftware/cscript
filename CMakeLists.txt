cmake_minimum_required(VERSION 3.21)
project(cscript VERSION 1.0 LANGUAGES C )

set(TARGET cscript)

set(CMAKE_C_FLAGS "-Wall -Wextra -g -MMD -MP -fPIC")
set(CMAKE_SHARED_LINKER_FLAGS "")

include_directories(.)

set(SRC_DIR "src")
set(PARSER_DIR "${CMAKE_CURRENT_BINARY_DIR}")

file(COPY "${SRC_DIR}/ast" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

file(COPY "${SRC_DIR}/typechecker.h" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC_DIR}/typechecker.c" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC_DIR}/interpreter.h" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC_DIR}/interpreter.c" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC_DIR}/cscript.h" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC_DIR}/cscript_internals.h" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC_DIR}/cscript_types.h" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")


find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set(LIBS reis fl m)

set(LEXER_OUT "${PARSER_DIR}/cscript_lexer.c")
set(PARSER_OUT "${PARSER_DIR}/cscript_parser.c")

flex_target(LEXER 
		"${SRC_DIR}/cscript.l" 
		${LEXER_OUT} 
    DEFINES_FILE "${PARSER_DIR}/cscript_lexer.h")
bison_target(PARSER 
            "${SRC_DIR}/cscript.y"
			"${PARSER_OUT}"
            DEFINES_FILE "${PARSER_DIR}/cscript_parser.h" 
            REPORT_FILE ${CMAKE_BINARY_DIR}/bison_report.txt
			OPTIONS -Wcounterexamples)
add_flex_bison_dependency(LEXER PARSER)

add_library(${TARGET} SHARED 
		"${SRC_DIR}/cscript.c"
		"${SRC_DIR}/ast/ast.c"
		"${SRC_DIR}/typechecker.c"
		"${SRC_DIR}/interpreter.c"
		"${LEXER_OUT}"
		"${PARSER_OUT}")

target_include_directories(${TARGET} PRIVATE ${PARSER_DIR})
target_link_libraries(${TARGET} ${LIBS})

enable_testing()

add_executable(Arithmetic tests/+arithmetic.c)
target_link_libraries(Arithmetic ${TARGET} ${LIBS})
target_include_directories(Arithmetic PRIVATE ${CMAKE_BINARY_DIR})


add_test(NAME Arithmetic COMMAND Arithmetic)


set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
    "cscript.h;cscript_internals.h")
