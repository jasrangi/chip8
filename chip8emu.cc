/* 
* AUTHOR: Jasjeet Rangi
* License: MIT
*/

#include "./chip8.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <chrono>

void update_renderer(const Chip8& chip8, SDL_Renderer* renderer,
const int width, const int height, const int scale);
void process_input(Chip8* chip8, SDL_Event& key_event, bool& end);

int main(int argc, char** argv) {
  int clock_speed;
  int window_scale;
  if (argc < 2) {
    std::cerr << "USAGE: ./chip8emu <ROM> <Clock Speed (Hz)> <Window Scale>" 
    << std::endl;
    return 1;
  } else if (argc == 2) {
    clock_speed = 1000;
    window_scale = 15;
  } else if (argc == 3) {
    clock_speed = std::stoi(argv[2]);
    window_scale = 15;
  } else {
    clock_speed = std::stoi(argv[2]);
    window_scale = std::stoi(argv[3]);
  }
  

  Chip8 chip8;
  SDL_Event key_event;
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool end = false;
  const int width = window_scale * 64;
  const int height = window_scale * 32;
  const int clock_interval_microsecond = 1000000 / clock_speed;

  if (!(chip8.load_prog(std::string(argv[1])))) {
    return 1;
  }

  /* setup graphics and input */
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Init(SDL_INIT_EVENTS);
  SDL_Init(SDL_INIT_AUDIO);
  window = SDL_CreateWindow("chip8vm", SDL_WINDOWPOS_UNDEFINED,
  SDL_WINDOWPOS_UNDEFINED, width, height, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  /* set up timers for clock speed control */
  std::chrono::time_point<std::chrono::system_clock,
  std::chrono::duration<long, std::ratio<1, 1000000000>>> cycle_start;

  std::chrono::time_point<std::chrono::system_clock,
  std::chrono::duration<long, std::ratio<1, 1000000000>>> cycle_stop;

  std::chrono::duration<long, std::ratio<1, 1000000>> cycle_duration;

  std::chrono::time_point<std::chrono::system_clock,
  std::chrono::duration<long, std::ratio<1, 1000000000>>> slowdown_start;

  std::chrono::time_point<std::chrono::system_clock,
  std::chrono::duration<long, std::ratio<1, 1000000000>>> slowdown_stop;

  std::chrono::duration<long, std::ratio<1, 1000000>> slowdown_difference;

  while (!end) {
    cycle_start = std::chrono::high_resolution_clock::now();
    chip8.run_cycle();

    if (chip8.sound_ctrl) {
      /* play a sound here */
      chip8.sound_ctrl = 0;
    }

    if (chip8.update_screen) {
      update_renderer(chip8, renderer, width, height, window_scale);
      SDL_RenderPresent(renderer);
      chip8.update_screen = 0;
    }
    cycle_stop = std::chrono::high_resolution_clock::now();
    cycle_duration = std::chrono::duration_cast<std::chrono::microseconds>(
    cycle_stop - cycle_start);

    if (cycle_duration.count() < clock_interval_microsecond) {
      auto target = clock_interval_microsecond - cycle_duration.count();
      slowdown_start = std::chrono::high_resolution_clock::now();
      do {
        slowdown_stop = std::chrono::high_resolution_clock::now();
        slowdown_difference =
        std::chrono::duration_cast<std::chrono::microseconds>(slowdown_stop -
        slowdown_start);
      } while (slowdown_difference.count() < target);
    }

    process_input(&chip8, key_event, end);
  }

  /* SDL cleanup */
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

void update_renderer(const Chip8& chip8, SDL_Renderer* renderer,
const int width, const int height, const int scale) {
  for (int y = 0; y < height / scale; y++) {
    for (int x = 0; x < width / scale; x++) {
      if (chip8.display[y][x]) {
        /* draw scaled white dot */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = y * scale; i < y * scale + scale; i++) {
          SDL_RenderDrawLine(renderer, x * scale, i, x * scale + scale - 1, i);
        }
      } else {
        /* draw scaled black dot */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = y * scale; i < y * scale + scale; i++) {
          SDL_RenderDrawLine(renderer, x * scale, i, x * scale + scale - 1, i);
        }
      }
    }
  }
}

void process_input(Chip8* chip8, SDL_Event& key_event, bool& end) {
  while (SDL_PollEvent(&key_event)) {
    switch (key_event.type) {
      case SDL_KEYDOWN: {
        switch (key_event.key.keysym.sym) {
          case SDLK_1: {
            chip8->keypad[0x1] = 1;
            break;
          }
          case SDLK_2: {
            chip8->keypad[0x2] = 1;
            break;
          }
          case SDLK_3: {
            chip8->keypad[0x3] = 1;
            break;
          }
          case SDLK_4: {
            chip8->keypad[0xC] = 1;
            break;
          }

          case SDLK_q: {
            chip8->keypad[0x4] = 1;
            break;
          }
          case SDLK_w: {
            chip8->keypad[0x5] = 1;
            break;
          }
          case SDLK_e: {
            chip8->keypad[0x6] = 1;
            break;
          }
          case SDLK_r: {
            chip8->keypad[0xD] = 1;
            break;
          }

          case SDLK_a: {
            chip8->keypad[0x7] = 1;
            break;
          }
          case SDLK_s: {
            chip8->keypad[0x8] = 1;
            break;
          }
          case SDLK_d: {
            chip8->keypad[0x9] = 1;
            break;
          }
          case SDLK_f: {
            chip8->keypad[0xE] = 1;
            break;
          }

          case SDLK_z: {
            chip8->keypad[0xA] = 1;
            break;
          }
          case SDLK_x: {
            chip8->keypad[0x0] = 1;
            break;
          }
          case SDLK_c: {
            chip8->keypad[0xB] = 1;
            break;
          }
          case SDLK_v: {
            chip8->keypad[0xF] = 1;
            break;
          }

          default: {
            std::cerr << "invalid key DOWN" << std::endl;
          }
        }
        break;
      }

      case SDL_KEYUP: {
        switch (key_event.key.keysym.sym) {
          case SDLK_1: {
            chip8->keypad[0x1] = 0;
            break;
          }
          case SDLK_2: {
            chip8->keypad[0x2] = 0;
            break;
          }
          case SDLK_3: {
            chip8->keypad[0x3] = 0;
            break;
          }
          case SDLK_4: {
            chip8->keypad[0xC] = 0;
            break;
          }

          case SDLK_q: {
            chip8->keypad[0x4] = 0;
            break;
          }
          case SDLK_w: {
            chip8->keypad[0x5] = 0;
            break;
          }
          case SDLK_e: {
            chip8->keypad[0x6] = 0;
            break;
          }
          case SDLK_r: {
            chip8->keypad[0xD] = 0;
            break;
          }

          case SDLK_a: {
            chip8->keypad[0x7] = 0;
            break;
          }
          case SDLK_s: {
            chip8->keypad[0x8] = 0;
            break;
          }
          case SDLK_d: {
            chip8->keypad[0x9] = 0;
            break;
          }
          case SDLK_f: {
            chip8->keypad[0xE] = 0;
            break;
          }

          case SDLK_z: {
            chip8->keypad[0xA] = 0;
            break;
          }
          case SDLK_x: {
            chip8->keypad[0x0] = 0;
            break;
          }
          case SDLK_c: {
            chip8->keypad[0xB] = 0;
            break;
          }
          case SDLK_v: {
            chip8->keypad[0xF] = 0;
            break;
          }

          default: {
            std::cerr << "invalid key UP" << std::endl;
          }
        }
        break;
      }

      case SDL_QUIT: {
        end = true;
        break;
      }
    }
  }
}
