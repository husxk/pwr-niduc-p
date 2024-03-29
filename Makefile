all: ninja build/

build/:
	mkdir build/
	cmake -S . -B build/ -GNinja

ninja: build/
	ninja -C build/

clean:
	rm -rf build/

.PHONY: all ninja clean
