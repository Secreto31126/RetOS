#ifndef HND_H
#define HND_H

extern void zero_division_exception_handler(void);
extern void invalid_opcode_exception_handler(void);
extern void tick_handler(void);
extern void keyboard_handler(void);
extern void cascade_pic(void);
extern void even_ports_handler(void);
extern void odd_ports_handler(void);
extern void syscall_handler(void);

#endif
