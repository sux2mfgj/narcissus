
TARGET := narcissus

all: $(TARGET)

$(TARGET):
	mkdir -p build
	cd build; cmake ..
	cd build; make

test: $(TARGET)
	cd build; ctest -VV
  
run: $(TARGET)
	./build/narcissus

clean:
	rm -rf build
