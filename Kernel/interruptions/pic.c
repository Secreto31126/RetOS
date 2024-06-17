#include "interruptions.h"

#define PRESSED_OR_RELEASED(x) ((x)&0x7F)
#define IS_RELEASE(x) ((x)&0x80)
#define RELEASED(x) ((x) | 0x80)

#define BIN(x) ((x) ? 1 : 0)
#define NEGATE(x) (1 - (x))

static void tick_handler();
static void keyboard_handler();

static void (*pic_handlers[2])(void) = {
    tick_handler,
    keyboard_handler,
};

void pic_manager(uint8_t interrupt) {
  if (interrupt < 2) {
    pic_handlers[interrupt]();
  }
}

static void tick_handler() { update_tick(); }

static void keyboard_handler() {
  static int caps = 0;
  static int shift = 0;
  static int altgr = 0;
  static int event = 0;

  // Read the scancode from the keyboard controller
  uint8_t scancode = input_byte(0x60);

  // ncPrintHex(scancode);
  // ncTab();

  stdkey_keyevent(scancode);

  // If esc key
  if (scancode == 1) {
    // Cause a opcode exception
    // char code[] = "👀👀👀";
    // ((void (*)(void))code)();
    // int i = 1 / 0;
    toggle_language();
    return;
  }

  // LShift or RShift pressed
  if (scancode == 0x2A || scancode == 0x36) {
    shift++;
    return;
  }

  // LShift or RShift released
  if (scancode == RELEASED(0x2A) || scancode == RELEASED(0x36)) {
    shift--;
    return;
  }

  // CapsLock pressed
  if (scancode == 0x3A) {
    caps = BIN(!caps);
    return;
  }

  // CapsLock released
  if (scancode == RELEASED(0x3A)) {
    return;
  }

  // Alt or AltGr pressed or released
  if (PRESSED_OR_RELEASED(scancode) == 0x38) {
    if (event) {
      altgr = BIN(!altgr);
      event = 0;
      return;
    } else {
      if (!IS_RELEASE(scancode))
        return;

      // strlen + 1
      uint16_t i = 0;
      while (dump_reg_string[i++])
        ;

      write_stderr(dump_reg_string, i);
      return;
    }
  }

  if (scancode == 0xE0) {
    event = 1;
    return;
  } else if (event) {
    event = 0;
    return;
  }

  if (IS_RELEASE(scancode))
    return;

  char modifier = (shift ? NEGATE(caps) : caps) + altgr * 2;

  uint16_t letter = get_scancode_utf16(scancode, modifier);
  if (letter)
    write_stdin((uint8_t *)&letter, 1);
}
