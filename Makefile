
TARGET := narcissus

BIN_FILE := ./object_code/04/bootload/kzload.bin

all: $(TARGET)

$(TARGET):
	mkdir -p build
	cd build; cmake ..
	cd build; make

test: $(TARGET)
	cd build; ctest -VV

debug: $(TARGET)
	./build/debug $(BIN_FILE)
  
run: $(TARGET)
	./build/narcissus $(BIN_FILE)

serial: $(TARGET)
	socat -d -d pty,raw,echo=0 "exec:./build/narcissus ./object_code/04/bootload/kzload.bin,pty,raw,echo=0"
#      socat -d -d "exec:./build/narcissus ./object_code/04/bootload/kzload.bin,pty,raw,echo=0" pty,raw,echo=0 


clean:
	rm -rf build
