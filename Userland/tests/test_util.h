#include <stdint.h>
#include <string.h>
#include <sys.h>

#define puts(str) write(1, (str), strlen(str))

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(char *str);
void bussy_wait(uint64_t n);
void endless_loop();
void endless_loop_print(uint64_t wait);
