#include "stdin.h"

static uint8_t on_your_left = 0;
static uint64_t reader = 0;
static uint64_t writer = 0;
static sem_t sem;

#define INC(x) ((x) = ((x) + 1) % BUF_SIZE)

int init_stdin() { return sem_init(&sem, 1, 0); }

static char getc() {
  char c = buffer[reader];
  INC(reader);
  on_your_left = 0;
  return c;
}

static void putc(uint8_t c) {
  buffer[writer] = c;
  INC(writer);

  if (on_your_left) {
    INC(reader);
  } else if (reader == writer) {
    on_your_left = 1;
  }
}

uint16_t read_stdin(uint8_t *buf, uint16_t len) {
  __label__ read;

  if (!len) {
    return 0;
  }

  int i = 0;
read:
  while (i < len) {
    if (stdin_empty()) {
      break;
    }

    buf[i++] = getc();
  }

  if (!i) {
    sem_wait(&sem);
    goto read;
  }

  return i;
}

uint16_t write_stdin(uint8_t *buf, uint16_t len) {
  if (!len) {
    return 0;
  }

  int i = 0;
  while (i < len) {
    if (buf[i]) {
      putc(buf[i++]);
    }
  }

  int value;
  if (!sem_getvalue(&sem, &value) && !value) {
    sem_post(&sem);
  }

  return i;
}

bool stdin_empty() { return reader == writer && !on_your_left; }

void flush_stdin() {
  reader = writer;
  on_your_left = 0;
}
