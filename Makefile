.PHONY: all clean

CFLAGS=-g
LDFLAGS=-lavformat -lavdevice -lavcodec -lavutil

all: motion_capture

motion_capture: motion_capture.c
	gcc $^ $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -fr motion_capture
