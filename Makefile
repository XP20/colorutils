CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic
BUILD = build
TARGET = $(BUILD)/colorutils
PREFIX = /usr/local/bin

.PHONY: all install uninstall clean
all: hex2rgb rgb2hex

$(TARGET): main.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $@ main.c

hex2rgb: $(TARGET)
	ln -sf $(TARGET) $@

rgb2hex: $(TARGET)
	ln -sf $(TARGET) $@

install: $(TARGET)
	install -d "$(PREFIX)"
	install -m 755 "$(TARGET)" "$(PREFIX)/colorutils"
	ln -sf colorutils "$(PREFIX)/hex2rgb"
	ln -sf colorutils "$(PREFIX)/rgb2hex"

uninstall:
	rm -f "$(PREFIX)/colorutils" "$(PREFIX)/hex2rgb" "$(PREFIX)/rgb2hex"
	
clean:
	rm -rf $(BUILD) hex2rgb rgb2hex
