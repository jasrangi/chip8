/* 
* AUTHOR: Jasjeet Rangi
* License: MIT
*/

#include "./chip8.h"

#include <cstdlib> /* rand(), srand() */
#include <ctime>
#include <fstream>
#include <iostream>

Chip8::Chip8(void) : update_screen(0), sound_ctrl(0), pc(0x200), ir(0), sp(0),
timer_delay(0), timer_sound(0), opcode(0) {
  std::srand(std::time(0));

  /* initialize display */
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      display[y][x] = 0;
    }
  }

  /* initialize registers, stack, and key states */
  for (int i = 0; i < 16; i++) {
    V[i] = 0;
    stack[i] = 0;
    keypad[i] = 0;
  }

  /* initialize memory */
  for (int i = 0; i < 4096; i++) {
    mem[i] = 0;
  }

  /* load the fontset */
  for (int i = 0; i < 80; i++) {
    mem[i] = font[i];
  }
}

Chip8::~Chip8(void) {}

/* initialize Chip8 CPU data members without recreating class instance */
void Chip8::init_cpu(void) {
  std::srand(std::time(0));
  update_screen = 0;
  sound_ctrl = 0;
  pc = 0x200;
  ir = 0;
  sp = 0;
  timer_delay = 0;
  timer_sound = 0;
  opcode = 0;

  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      display[y][x] = 0;
    }
  }

  for (int i = 0; i < 16; i++) {
    V[i] = 0;
    stack[i] = 0;
    keypad[i] = 0;
  }

  for (int i = 0; i < 4096; i++) {
    mem[i] = 0;
  }

  for (int i = 0; i < 80; ++i) {
    mem[i] = font[i];
  }
}

/* returns false if there is an error, true if program loaded successfully */
bool Chip8::load_prog(std::string file_name) {
  std::ifstream prog(file_name, std::ios::in | std::ios::binary |
  std::ios::ate);

  if (!(prog.is_open())) {
    std::cerr << "ERROR: " << file_name << " NOT FOUND" << std::endl;
    return false;
  }

  std::streampos prog_size = prog.tellg();
  if (prog_size > 0xFFF - 0x200) {
    std::cerr << "ERROR: " << file_name << " FILESIZE TOO LARGE" << std::endl;
    prog.close();
    return false;
  }

  char* buffer = new char[prog_size];
  prog.seekg(0, std::ios::beg);
  prog.read(buffer, prog_size);
  prog.close();

  /* load the program into memory */
  for (int i = 0; i < prog_size; i++) {
    mem[i + 0x200] = (unsigned char)buffer[i];
  }
  delete[] buffer;
  return true;
}

/* run a single clock cycle of the CPU */
void Chip8::run_cycle(void) {
  /* fetch 2 bytes and combine into opcode */
  opcode = mem[pc] << 8 | mem[pc + 1];

  /* decode and execute opcode */
  switch (opcode & 0xF000) {
    case 0x0000: {
      switch (opcode & 0x000F) {
        case 0x0000: {
          for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 64; x++) {
              display[y][x] = 0;
            }
          }
          update_screen = true;
          pc += 2;
          break;
        }

        case 0x000E: {
          sp--;
          pc = stack[sp];
          pc += 2;
          break;
        }

        default: {
          std::cerr << "ERROR: opcode " << opcode << "of type 0x0 not valid"
          << std::endl;
        }
      }
      break;
    }

    case 0x1000: {
      pc = opcode & 0x0FFF;
      break;
    }

    case 0x2000: {
      stack[sp] = pc;
      sp++;
      pc = opcode & 0x0FFF;
      break;
    }

    case 0x3000: {
      if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;
    }

    case 0x4000: {
      if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;
    }

    case 0x5000: {
      if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;
    }

    case 0x6000: {
      V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
      pc += 2;
      break;
    }

    case 0x7000: {
      V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
      pc += 2;
      break;
    }

    case 0x8000: {
      switch (opcode & 0x000F) {
        case 0x0000: {
          V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
          pc += 2;
          break;
        }

        case 0x0001: {
          V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
          pc += 2;
          break;
        }

        case 0x0002: {
          V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
          pc += 2;
          break;
        }

        case 0x0003: {
          V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
          pc += 2;
          break;
        }

        case 0x0004: {
          /* detect overflow */
          if (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] <
          V[(opcode & 0x0F00) >> 8]) {
            V[0xF] = 1;
          } else {
            V[0xF] = 0;
          }
          V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
          pc += 2;
          break;
        }

        case 0x0005: {
          /* detect bit borrow */
          if (V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4]) {
            V[0xF] = 0;
          } else {
            V[0xF] = 1;
          }
          V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
          pc += 2;
          break;
        }

        case 0x0006: {
          V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x01;
          V[(opcode & 0x0F00) >> 8] >>= 1;
          pc += 2;
          break;
        }

        case 0x0007: {
            if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) {
            V[0xF] = 0;
          } else {
            V[0xF] = 1;
          }
          V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] -
          V[(opcode & 0x0F00) >> 8];
          pc += 2;
          break;
        }

        case 0x000E: {
          V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0x80) >> 7;
          V[(opcode & 0x0F00) >> 8] <<= 1;
          pc += 2;
          break;
        }

        default: {
          std::cerr << "ERROR: opcode " << opcode << "of type 0x8 not valid"
          << std::endl;
        }
      }
      break;
    }

    case 0x9000: {
      if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;
    }

    case 0xA000: {
      ir = opcode & 0x0FFF;
      pc += 2;
      break;
    }

    case 0xB000: {
      pc = V[0] + (opcode & 0x0FFF);
      break;
    }

    case 0xC000: {
      V[(opcode & 0x0F00) >> 8] = (std::rand() % 256) & (opcode & 0x0FF);
      pc += 2;
      break;
    }

    case 0xD000: {
      unsigned char Vx = V[(opcode & 0x0F00) >> 8];
      unsigned char Vy = V[(opcode & 0x00F0) >> 4];
      unsigned char height = opcode & 0x000F;
      unsigned char pixel_byte;
      unsigned char wrap_offset_y, wrap_offset_x = 0;
      V[0xF] = 0;
      /* y corresponds to row, x to col, displays pixel_byte per row */
      for (unsigned char ypos = 0; ypos < height; ypos++) {
        if (Vy + ypos > 31) {
          wrap_offset_y = 32;
        }
        pixel_byte = mem[ir + ypos];
        for (unsigned char xpos = 0; xpos < 8; xpos++) {
          if (Vx + xpos > 63) {
            wrap_offset_x = 64;
          }
          /* extract current pixel bit from pixel_byte */
          if ((pixel_byte & (0x80 >> xpos)) != 0) {
            if (display[Vy + ypos - wrap_offset_y][Vx + xpos - wrap_offset_x]
            == 1) {
              V[0xF] = 1;
            }
            display[Vy + ypos][Vx + xpos] ^= 1;
          }
        }
      }
      update_screen = 1;
      pc += 2;
      break;
    }

    case 0xE000: {
      switch (opcode & 0x00FF) {
        case 0x009E: {
          if (keypad[V[(opcode & 0x0F00) >> 8]] != 0) {
            pc += 4;
          } else {
            pc += 2;
          }
          break;
        }

        case 0x00A1: {
          if (keypad[V[(opcode & 0x0F00) >> 8]] == 0) {
            pc += 4;
          } else {
            pc += 2;
          }
          break;
        }

        default: {
          std::cerr << "ERROR: opcode " << opcode << "of type 0xEX not valid"
          << std::endl;
        }
      }
      break;
    }

    case 0xF000: {
      switch (opcode & 0x00FF) {
        case 0x0007: {
          V[(opcode & 0x0F00) >> 8] = timer_delay;
          pc += 2;
          break;
        }

        case 0x000A: {
          bool is_pressed = 0;
          /* check if key is in a pressed down state */
          for (int key = 0; key < 16; key++) {
            if (keypad[key] == 1) {
              V[(opcode & 0x0F00) >> 8] = key;
              is_pressed = 1;
            }
          }
          if (is_pressed) {
            pc += 2;
          } else {
            return;
          }
          break;
        }

        case 0x0015: {
          timer_delay = V[(opcode & 0x0F00) >> 8];
          pc += 2;
          break;
        }

        case 0x0018: {
          timer_sound = V[(opcode & 0x0F00) >> 8];
          pc += 2;
          break;
        }

        case 0x001E: {
          ir += (unsigned short)V[(opcode & 0x0F00) >> 8];
          pc += 2;
          break;
        }

        case 0x0029: {
          /* location of sprite in fontset array */
          ir = V[(opcode & 0x0F00) >> 8] * 5;
          pc += 2;
          break;
        }

        case 0x0033: {
          if (V[(opcode & 0x0F00) >> 8] >= 100) {
            mem[ir] = V[(opcode & 0x0F00) >> 8] / 100;
            mem[ir + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
            mem[ir + 2] = V[(opcode & 0x0F00) >> 8] % 10;
          } else if (V[(opcode & 0x0F00) >> 8] >= 10) {
            mem[ir] = 0;
            mem[ir + 1] = V[(opcode & 0x0F00) >> 8] / 10;
            mem[ir + 2] = V[(opcode & 0x0F00) >> 8] % 10;
          } else {
            mem[ir] = 0;
            mem[ir + 1] = 0;
            mem[ir + 2] = V[(opcode & 0x0F00) >> 8];
          }
          pc += 2;
          break;
        }

        case 0x0055: {
          for (unsigned short i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
            mem[ir + i] = V[i];
          }
          pc += 2;
          break;
        }

        case 0x0065: {
          for (unsigned short i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
            V[i] = mem[ir + i];
          }
          pc += 2;
          break;
        }

        default: {
          std::cerr << "ERROR: opcode " << opcode << "of type 0xFX not valid"
          << std::endl;
        }
      }
      break;
    }

    default:
      std::cerr << "ERROR: opcode " << opcode << " not valid AT ALL"
      << std::endl;
  }

  if (timer_delay) {
    timer_delay--;
  }
  if (timer_sound) {
    sound_ctrl = 1;
    timer_sound--;
  }
}
