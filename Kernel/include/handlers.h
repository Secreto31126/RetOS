#ifndef HND_H
#define HND_H

/**
 * @brief Handles the zero division exception.
 * @note dump_regs() and dump_regs_include_rip() are called,
 * so dump_reg_string is setted.
 */
extern void zero_division_exception_handler(void);
/**
 * @brief Handles the invalid opcode exception.
 * @note dump_regs() and dump_regs_include_rip() are called,
 * so dump_reg_string is setted.
 */
extern void invalid_opcode_exception_handler(void);
extern void sus_exception_handler(void);
extern void tick_handler(void);
extern void keyboard_handler(void);
extern void cascade_pic(void);
extern void even_ports_handler(void);
extern void odd_ports_handler(void);
extern void usb_handler(void);
extern void syscall_handler(void);

#endif
