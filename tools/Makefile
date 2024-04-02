ifeq ($(OS),Windows_NT)
    OS := Windows
else
    OS := unix
endif

# Define the name of your executable
EXECUTABLE := gen_dist_data

# Define command to run the executable based on the operating system
ifeq ($(OS),Windows)
    RUN_CMD := start $(EXECUTABLE).exe
else
    RUN_CMD := ./$(EXECUTABLE)
endif

all: plots

build/:
	mkdir build
	cmake -S . -B build/ -GNinja

ninja: build/
	ninja -C build/

dist_data: ninja
	cd build &&  ${RUN_CMD}

plots: dist_data
	python plots.py

clean:
	rm -rf build

.PHONY: all ninja clean plots dist_data