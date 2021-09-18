CXXFLAGS = -Wall -Werror -MMD -std=c++17
SDL2FLAGS = -lSDL2
execs = chip8emulator
objs = chip8.o chip8emu.o

all: $(execs)

deps = $(patsubst %.o,%.d,$(objs))
-include $(deps)

chip8emulator: $(objs)
	$(CXX) $(CXXFLAGS) $(SDL2FLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(execs) $(objs) $(deps)
