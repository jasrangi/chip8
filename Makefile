CXXFLAGS = -Wall -Werror -std=c++11
SDL2FLAGS = -lSDL2
SDL2FLAGS_NOBREW = -F /Library/Frameworks -framework SDL2
execs = chip8emu chip8emu_brew

all: chip8emu

chip8emu: chip8.cc chip8emu.cc
	$(CXX) $(CXXFLAGS) $(SDL2FLAGS) -o $@ $^

chip8emu_nobrew: chip8.cc chip8emu.cc
	$(CXX) $(CXXFLAGS) $(SDL2BREW) -o $@ $^
	
clean:
	rm -f $(execs)
