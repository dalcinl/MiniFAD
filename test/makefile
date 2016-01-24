MINIFAD_DIR = ..

STD = c++98
ifneq ($(STD),)
STDFLAGS = -std=$(STD) -pedantic
endif

OPT = 0
ifneq ($(OPT),)
OPTFLAGS = -O$(OPT)
endif

is_gcc   = $(if $(shell $(CXX) --version 2>/dev/null | grep "GCC\|g++"),1)
is_clang = $(if $(shell $(CXX) --version 2>/dev/null | grep "clang\|Apple LLVM"),1)
is_intel = $(if $(shell $(CXX) --version 2>/dev/null | grep "ICC"),1)
ifneq ($(is_gcc),)
WARNINGS += -Wall
WARNINGS += -Wextra
WARNINGS += -Wfloat-equal
#WARNINGS += -Wconversion
WARNINGS += -Wsign-conversion
#WARNINGS += -Wfloat-conversion
WARNINGS += -Wno-padded
WARNINGS += -Wno-psabi
endif
ifneq ($(is_clang),)
WARNINGS += -Weverything
WARNINGS += -Wno-padded
endif
ifneq ($(is_intel),)
WARNINGS += -Wall
WARNINGS += -Wcheck
WARNINGS += -Weffc++
WARNINGS += -wd2012
endif

DEFINES += $(foreach macro,$(DEFINE), -D$(macro))
DEFINES += $(foreach macro,$(DEFINE0),-D$(macro)=0)
DEFINES += $(foreach macro,$(DEFINE1),-D$(macro)=1)

LIBQUADMATH = -lquadmath

SOURCES = $(sort $(wildcard test_*.cpp))
TARGETS = $(SOURCES:%.cpp=%)
ALLTEST = $(TARGETS:%=%.run)

CPPFLAGS += -I$(MINIFAD_DIR)
CPPFLAGS += $(DEFINES)
CXXFLAGS += $(STDFLAGS)
CXXFLAGS += $(OPTFLAGS)
CXXFLAGS += $(WARNINGS)
ifneq ($(is_gcc)$(is_intel),)
test_fn_binary: LDLIBS += $(LIBQUADMATH)
test_fn_unary:  LDLIBS += $(LIBQUADMATH)
endif

.PHONY: default
default: all run clean

.PHONY: run
run: $(ALLTEST)

.PHONY: all
all: $(TARGETS)

.PHONY: %.run
%.run: %; $(VALGRIND) ./$<

.PHONY: clean
clean:; @$(RM) $(TARGETS)
