TARGET := narcissus

BIN_FILE := ./osbook_03/08/bootload/kzload.bin
ELF_FILE := ./kzload.elf
GDB := h8300-elf-gdb

all: $(TARGET)

$(TARGET):
	mkdir -p build
	cd build; cmake ..
	cd build; make

test: $(TARGET)
	cd build; ctest -VV
  
run: $(TARGET)
	./build/narcissus -i $(BIN_FILE)

gdb: $(TARGET)
	./build/narcissus -i $(BIN_FILE) -d 

serial: $(TARGET)
	socat -d -d pty,raw,echo=0 "exec:./build/narcissus -i $(BIN_FILE) -d,pty,raw,echo=0"

clean:
	rm -rf build
