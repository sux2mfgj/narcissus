
TARGET := narcissus

BIN_FILE := ./object_code/03/bootload/kzload.bin

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

clean:
	rm -rf build
