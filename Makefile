
TARGET := narcissus

all: $(TARGET)

$(TARGET):
	mkdir -p build
	cd build; cmake ..
	cd build; make

test: $(TARGET)
	cd build; ctest -VV
  
run: $(TARGET)
	./build/narcissus ./object_code/02/bootload/kzload.bin

clean:
	rm -rf build
