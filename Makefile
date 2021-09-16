CXXFLAGS = -Wall -Werror -std=c++17
SDL2FLAGS = -lSDL2
execs = chip8emu

all: chip8emu

chip8emu: chip8.cc chip8emu.cc
	$(CXX) $(CXXFLAGS) $(SDL2FLAGS) -o $@ $^

clean:
	rm -f $(execs)
