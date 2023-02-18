## Makefile for Nightcrawler engine.

## ---------------------------------------------------------------------
## Set phony & default targets, and override the default suffix rules.
## ---------------------------------------------------------------------

.PHONY: all clean engine compiler
.SUFFIXES:

.DEFAULT_GOAL := all

## ---------------------------------------------------------------------
## Setup project details.
## ---------------------------------------------------------------------

TARGET_ENGINE   := nightcrawler
TARGET_COMPILER := ncpk
SOURCES         := src/
BUILD           := build/
OBJS            := $(patsubst $(SOURCES)%.c,$(BUILD)%.o,$(wildcard $(SOURCES)*.c))
OBJS_ENGINE     := $(patsubst $(SOURCES)$(TARGET_ENGINE)/%.c,$(BUILD)%.o,$(wildcard $(SOURCES)$(TARGET_ENGINE)/*.c))
OBJS_COMPILER   := $(patsubst $(SOURCES)$(TARGET_COMPILER)/%.c,$(BUILD)%.o,$(wildcard $(SOURCES)$(TARGET_COMPILER)/*.c))

## ---------------------------------------------------------------------
## Set flags for code generation.
## ---------------------------------------------------------------------

ifdef WINDOWS
ifdef WIN64
	GCCPREFIX        := x86_64-w64-mingw32-
else
	GCCPREFIX        := i686-w64-mingw32-
endif
	TARGET_ENGINE    := $(TARGET_ENGINE).exe
	TARGET_COMPILER  := $(TARGET_COMPILER).exe
endif

ifndef CLANG
	CC        := $(GCCPREFIX)gcc
	LD        := $(GCCPREFIX)gcc
	STRIP     := $(GCCPREFIX)strip
else
	CC        := clang
	LD        := clang
	STRIP     := strip
endif

CFLAGS   := -Wall -Os -fshort-enums -I$(SOURCES)
ifndef CLANG
	# -fno-printf-return-value unsupported on clang.
	CFLAGS += -fno-printf-return-value
endif

ifdef DEBUG
	CFLAGS += -D_DEBUG -g
endif

ifdef WINDOWS
	CFLAGS += -D_WINDOWS
ifdef WIN64
	CFLAGS += -D_WIN64
else
	CFLAGS += -D_WIN32
endif
endif

LDFLAGS  := $(CFLAGS)

## ---------------------------------------------------------------------
## Compilation rules.
## ---------------------------------------------------------------------

## Generate a 7zip release archive (requires p7zip).
%.7z: default.nst LICENSE README.md $(TARGET_ENGINE) $(TARGET_COMPILER)
	-@echo 'Compressing 7zip release archive ("$^"->"$@")'
	$(STRIP) -vs $(TARGET_ENGINE)
	$(STRIP) -vs $(TARGET_COMPILER)
	7z a $@ $^

## Generate a zip release archive (requires infozip).
%.zip: default.nst LICENSE README.md $(TARGET_ENGINE) $(TARGET_COMPILER)
	-@echo 'Compressing zip release archive ("$^"->"$@")'
	$(STRIP) -vs $(TARGET_ENGINE)
	$(STRIP) -vs $(TARGET_COMPILER)
	zip -v -9 $@ $^

## Compile all targets.
all: engine compiler

engine: $(TARGET_ENGINE)
	-@chmod +x $<

compiler: $(TARGET_COMPILER)
	-@chmod +x $<

## Build Engine.
$(TARGET_ENGINE): $(OBJS) $(OBJS_ENGINE)
	-@echo 'Linking objects for engine... ("$^"->"$@")'
	$(LD) $(LDFLAGS) $^ -o $@

## Build Compiler
$(TARGET_COMPILER): $(OBJS) $(OBJS_COMPILER)
	-@echo 'Linking objects for compiler... ("$^"->"$@")'
	$(LD) $(LDFLAGS) $^ -o $@

## Compile each C file.
$(OBJS): $(BUILD)%.o : $(SOURCES)%.c
	-@mkdir -p $(BUILD)
	-@echo 'Compiling object "$@"... ("$<"->"$@")'
	$(CC) $(CFLAGS) -c $< -o $@

## Compile Engine Objects.
$(OBJS_ENGINE): $(BUILD)%.o : $(SOURCES)$(TARGET_ENGINE)/%.c
	-@mkdir -p $(BUILD)
	-@echo 'Compiling engine object "$@"... ("$<"->"$@")'
	$(CC) $(CFLAGS) -c $< -o $@

## Compile Compiler Objects.
$(OBJS_COMPILER): $(BUILD)%.o : $(SOURCES)$(TARGET_COMPILER)/%.c
	-@mkdir -p $(BUILD)
	-@echo 'Compiling compiler object "$@"... ("$<"->"$@")'
	$(CC) $(CFLAGS) -c $< -o $@

## Remove $(BUILD) dir.
.IGNORE: clean
clean:
	@echo 'Cleaning up intermediary files...'
	@rm -rvf $(BUILD) $(TARGET_ENGINE) $(TARGET_COMPILER)
