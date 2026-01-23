# CST320 Lab 3 - Recursive Descent Calc

CXX      := g++
CXXFLAGS := -Wall -Wextra -g -O0 -std=c++11

TARGET   := calc

# Prefer flex if available; otherwise fall back to scanner.cpp.
FLEX_BIN := $(shell command -v flex 2>/dev/null)

LEX_SRC  := calc.l
LEX_C    := calclex.c
LEX_O    := calclex.o

CORE_OBJS := main.o parse.o utils.o

ifeq ($(FLEX_BIN),)
  # No flex available
  OBJS := $(CORE_OBJS) scanner.o
else
  OBJS := $(CORE_OBJS) $(LEX_O)
endif

.PHONY: all clean test

all: $(TARGET) lang

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Keep the starter test scripts working (they call ./lang)
lang: $(TARGET)
	ln -sf $(TARGET) lang

$(LEX_C): $(LEX_SRC) lex.h
	flex -o $(LEX_C) $(LEX_SRC)

$(LEX_O): $(LEX_C) lex.h
	$(CXX) $(CXXFLAGS) -c $(LEX_C) -o $(LEX_O)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) lang *.o $(LEX_C) out

test: all
	bash test/regress
