LDFLAGS += -lpthread
simavr = ../../
IPATH += ${simavr}/simavr/sim

target := libsimavrparts

all: obj ${target}

include ${simavr}/Makefile.common

objects := $(patsubst %.c,${OBJ}/%.o, $(wildcard *.c))

# Unless the VHCI_USB env var is set, don't build this because it requires
# a supporting library to be installed. See examples/extra_board_usb/README
# for details on how to build it.
ifndef VHCI_USB
	objects := $(filter-out %vhci_usb.o, $(objects))
endif
# Unless the USE_GLUT env var is set, don't build this because it requires
# a supporting library to be installed.
ifndef USE_GLUT
	objects := $(filter-out %glut.o, $(objects))
endif
objects := $(filter-out %pty.o, $(objects))

#
# Static library
#
${OBJ}/${target}.a: ${objects}
${target}: ${OBJ}/${target}.a
#
# Shared library (Linux only)
#
ifeq (${shell uname}, Linux)
${target}: ${OBJ}/${target}.so
endif

clean: clean-${OBJ}
	rm -rf *.hex *.a *.axf *.vcd .*.swo .*.swp .*.swm .*.swn *.so *.o
