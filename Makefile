
all:
	mkdir -p build
	cd build; cmake ..
	cd build; make


run:
	./build/narcissus
