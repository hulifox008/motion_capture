.PHONY: all

LDFLAGS=-lavformat -lavdevice

all: motion_capture

motion_capture: motion_capture.c
	gcc $^ $(LDFLAGS) -o $@
