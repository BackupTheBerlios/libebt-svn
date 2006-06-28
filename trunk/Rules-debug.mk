# Straight native compile...
CROSSDEV=
CC:=$(CROSSDEV)gcc
CXX:=$(CROSSDEV)g++
AR:=$(CROSSDEV)ar
RANLIB:=$(CROSSDEV)ranlib

TARGET_INCLUDES=
coma:=,
TARGET_LDFLAGS=$(addprefix -Wl$(coma)-rpath$(coma),$(ADON_LIBDIRS))

TOP_CXXFLAGS+=-g3
TOP_CFLAGS+=-g3
TOP_CPPFLAGS+=-DDEBUG
