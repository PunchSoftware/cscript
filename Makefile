TARGET := libcscript.so
CC := gcc
LEX := flex
YACC := bison
RM := rm -f

# -MMD and -MP track header changes
CFLAGS := -Wall -Wextra -g -MMD -MP -fPIC
CPPFLAGS := -I .
LDFLAGS := -shared
MAKEFLAGS += --no-print-directory
YACCFLAGS := -Wcounterexamples

LIBS := -lreis -lfl -lm

SRCS := src/cscript.c
TESTS := tests/
OBJS := src/cscript.o
LEX_SRCS := src/cscript.l
LEX_GEN := src/cscript.lex.c
LEX_GEN_H := cscript_lexer.h
LEX_OBJ := src/cscript.lex.o
YACC_SRCS := src/cscript.y
YACC_GEN := src/cscript.yacc.c
YACC_GEN_H := cscript_parser.h
YACC_OBJ := src/cscript.yacc.o

all: $(TARGET)

$(TARGET): $(OBJS) $(LEX_OBJ) $(YACC_OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LEX_OBJ) $(YACC_OBJ) $(LIBS) 
	$(info CREATED SHARED LIBRARY $(TARGET))

%.o: %.c $(LEX_GEN) $(YACC_GEN)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	$(info CREATED $@)

$(LEX_OBJ): $(LEX_GEN)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	$(info COMPILED LEXER $@)

$(LEX_GEN): $(LEX_SRCS)
	$(LEX) -o $@ $<
	$(info GENERATED LEX FILE $@)

$(YACC_OBJ): $(YACC_GEN)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	$(info COMPILED YACC $@)

$(YACC_GEN): $(YACC_SRCS)
	$(YACC) $(YACCFLAGS) -o $@ $<
	$(info GENERATED YACC FILE $@)

test:
	$(CC) $(TESTS) -o "test-runner"

clean:
	$(RM) $(OBJS) $(LEX_OBJ) $(LEX_GEN) $(LEX_GEN_H) $(YACC_OBJ) $(YACC_GEN) $(YACC_GEN_H) *.d src/*.d 
	$(RM) -r dist/

fclean:
	$(RM) $(TARGET)
re:

	$(MAKE) fclean
	$(MAKE) all

.PHONY: clean fclean re test
.SILENT:
