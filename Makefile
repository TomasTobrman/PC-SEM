# ============================================================================ #
# Copyright (c) 2023 Tomáš Tobrman. All Rights Reserved.
# ==== Project Info ========================================================== #

PROJECT := othello
EXTENSION := .exe

# ==== Directories =========================================================== #

DIR_OBJ := obj
DIR_SRC := src
DIR_INC := include

# ==== Files ================================================================= #

TARGET := $(PROJECT)$(EXTENSION)
SOURCES := $(shell find $(DIR_SRC) -type f -name '*.c')
OBJECTS := $(patsubst $(DIR_SRC)/%.c, $(DIR_OBJ)/%.o, $(SOURCES))

# ==== Commands ============================================================== #

WARN := -Wall

CC := gcc
CFLAGS := -xc -std=c90 -I$(DIR_INC) -g

LD := gcc
LDFLAGS := 

# ==== PHONY ================================================================= #

default: mkdirs build

.PHONY: build
build: $(TARGET)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(DIR_OBJ)/*.o

.PHONY: mkdirs
mkdirs: $(DIR_OBJ)

# ==== Targets =============================================================== #

$(DIR_OBJ):
	mkdir -p $@

$(TARGET): $(OBJECTS)
	$(LD) $(WARN) $(LDFLAGS) -o $@ $^

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	$(CC) $(WARN) $(CFLAGS) -c -o $@ $^

# ============================================================================ #
