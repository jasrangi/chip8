/* 
* AUTHOR: Jasjeet Rangi
* License: MIT
*/

#ifndef _CHIP8_H_
#define _CHIP8_H_

#include <string>

class Chip8 {
 public:
    Chip8(void);
    ~Chip8(void);

 public:
    /* functions */
    void init_cpu(void);
    bool load_prog(std::string fileName);
    void run_cycle(void);

    /* data needed publicly */
    bool keypad[16];
    bool display[32][64];
    bool update_screen;
    bool sound_ctrl;

 private:
    /* internal data */
    unsigned char mem[4096];
    unsigned char V[16];
    unsigned short pc;
    unsigned short ir;
    unsigned short sp;
    unsigned short stack[16];
    unsigned char timer_delay;
    unsigned char timer_sound;

    unsigned short opcode;

    unsigned char font[80] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
      0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
      0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
      0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
      0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
      0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
      0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
      0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
      0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
      0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
      0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
      0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
      0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
      0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
      0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
      0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
    };
};

#endif
