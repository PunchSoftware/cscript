TARGET := +cscript.exe
CC := gcc
LEX := flex
YACC := bison
RM := rm -f

# -MMD and -MP track header changes
CFLAGS := -Wall -Wextra -g -MMD -MP 
CPPFLAGS := -I .
LDFLAGS :=
MAKEFLAGS += --no-print-directory
YACCFLAGS := -Wcounterexamples

LIBS := reis

SRCS := main.c
OBJS := main.o
LEX_SRCS := cscript.l
LEX_GEN := cscript.lex.c
LEX_GEN_H := cscript_lexer.h
LEX_OBJ := cscript.lex.o
YACC_SRCS := cscript.y
YACC_GEN := cscript.yacc.c
YACC_GEN_H := cscript_parser.h
YACC_OBJ := cscript.yacc.o

all: $(TARGET)

$(TARGET): $(OBJS) $(LEX_OBJ) $(YACC_OBJ)
	$(CC) -o $(TARGET) $(OBJS) $(LEX_OBJ) $(YACC_OBJ) -lfl -lm
	$(info CREATED $(TARGET))

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

clean:
	$(RM) $(OBJS) $(LEX_OBJ) $(LEX_GEN) $(LEX_GEN_H) $(YACC_OBJ) $(YACC_GEN) $(YACC_GEN_H) *.d

fclean:
	$(RM) $(TARGET)
re:

	$(MAKE) fclean
	$(MAKE) all

.PHONY: clean fclean re
.SILENT:
