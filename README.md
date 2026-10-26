# RetOS

A preemptive multitasking kernel for x86-64, written in C and NASM assembly. It boots from its
own disk image and runs a small userland (a shell with pipes, some Unix-style tools, dining
philosophers and test programs) on top of 34 system calls. It doesn't use libc or any host OS
code.

GPL-3.0 · C / x86-64 assembly · 21.4k lines · 34 syscalls

We wrote RetOS at [ITBA](https://www.itba.edu.ar/) for two systems courses, between October
2023 and June 2024. The bootloader and the starting template aren't ours; see
[Authorship](#authorship).

## Overview

|             |                                                                               |
| ----------- | ----------------------------------------------------------------------------- |
| Target      | x86-64 long mode, single core, flat identity-mapped physical memory           |
| Kernel      | ~7,100 lines, freestanding (`-ffreestanding -nostdlib -mno-red-zone -mno-sse`) |
| Syscalls    | 34, via `int 0x80`                                                            |
| Scheduling  | 40 priority bands, weighted lottery + round-robin, preemption at 18.2 Hz      |
| Concurrency | `fork` / `execv` / `waitpid`, pipes, POSIX named semaphores, `pselect`        |
| Memory      | two heap allocators, selected at compile time                                 |
| Graphics    | 1024×768×24bpp VBE linear framebuffer with alpha blending                     |
| Userland    | 12 flat binaries: shell, `grep`/`cat`/`wc`/`less`, dining philosophers, tests |
| Image       | 6 MiB BMFS disk, Pure64 bootloader, runs under QEMU                           |

## Quick start

The build runs inside a Docker container, so you only need Docker and QEMU on the host:

```sh
git clone https://github.com/Secreto31126/RetOS.git
cd RetOS
./run.sh build          # build the image and boot it in QEMU
```

`./run.sh build buddy` builds the same image with the buddy allocator instead of the default
first-fit one. [Building and running](#building-and-running) lists the other options.

## Design notes

### Scheduler

`Kernel/proc/scheduler.c:211-375`

Runnable processes are kept in 40 priority bands (`PRIO_MIN` = -20 to `PRIO_MAX` = 19), each
one a circular list. Counting bands from 0 at `PRIO_MIN`, band *i* has weight `w_i = i + 1`, so
weights go from 1 to 40.

At the start of each round the scheduler adds up the weights of the non-empty bands (`S`),
draws a random number in `[0, S)`, and walks the non-empty bands from the top, subtracting
weights until the number goes negative. Band *i* is therefore picked with probability
`w_i / S`. The chosen band then runs for `n_i` quanta in a row, where `n_i` is the number of
processes in it, so each of them gets one quantum before the next draw.

Over time, each process in band *i* gets this fraction of the CPU:

```
      w_i
 ─────────────
  Σ_j w_j · n_j
```

Any two processes split the CPU in the ratio of their weights, however the other processes are
distributed, and everyone's share goes down as more processes become runnable. This is the
same expected split you get from lottery scheduling with `w_i` tickets per process (Waldspurger
& Weihl, 1994). The difference is that we draw once per round instead of once per quantum, and
processes in the same band always take turns.

Some consequences:

- No runnable process starves, since every weight is at least 1. A process at `PRIO_MIN` wins
  on average at least one round out of every `S` (at most 820).
- Picking a band takes two passes over the 40 bands (sum, then walk), and that cost is spread
  over the `n_i` quanta the band then runs. Picking the next process inside a band is one
  pointer step.
- `robin_remove` is O(n) in the band's length. It only runs when a process dies, blocks or
  changes priority.
- The random numbers come from a 16-bit LFSR. Long-run frequencies are fine (modulo bias under
  2%), but consecutive draws are correlated, because each state is the previous one shifted by
  one bit.

Priorities go the opposite way from POSIX nice: a higher number means more weight and more
CPU. The comment in `Kernel/include/proc.h` that says "lower is higher priority" is out of
date.

### fork without virtual memory

`Kernel/proc/scheduler.c:14-73`, `Kernel/asm/proc.asm:70-102`

Pure64 sets up an identity-mapped address space with 2 MiB pages, and RetOS leaves it as is. It
doesn't touch `CR3`, has no page-fault handler and no per-process page tables, so all
processes share one flat address space.

That makes `fork` awkward. The child needs its own stack, but it has to keep running at the
same addresses as its parent, because every pointer in the copied stack frame points into the
parent's stack.

We handle this by swapping stack contents during context switches. Each PCB has two ranges:
`stack`, the 512 KiB the process owns (`MIN_STACK_SIZE 0x80000`), and `running_stack`, the
range it actually runs at. `swap_stacks` (assembly, 8 bytes per iteration plus a byte-wise
tail) exchanges the two in place.

```
fork()   child allocates its own stack, but keeps executing at the parent's
         addresses - it is "living in its parent's house"

                    stack (owned)      running_stack (executing at)
         parent     0xA ---------------> 0xA        equal   -> no swap
         child      0xB - - - - - - - -> 0xA        differ  -> swap each switch

switch   swap_stacks() writes the child's live bytes back to 0xB and
         restores the parent's bytes at 0xA, so each sees its own memory

execv()  move_away_from_parents_house() repoints the child at its own range
         child      0xB ---------------> 0xB        equal   -> never swaps again
```

The copy only happens while `running_stack != stack`, which is between `fork` and `execv`.
After `execv` the process runs on its own stack and context switches don't copy anything. The
cost of a switch is bounded by `running_stack_size`.

This shows up in other parts of the kernel:

- `pselect` copies its file-descriptor arrays to the kernel heap before blocking, because a
  pointer into a user stack isn't valid while another process's stack is swapped in
  (`Kernel/proc/blocks.c:176-208`).
- `fork` copies the whole stack up front; there's no copy-on-write. From the comment in
  `Kernel/proc/proc.c:152`: "We aren't Linux, we must copy the stack at creation time".
- `sbrk` is a stub, since a process's heap can't grow into a neighbour's range.

The scheduler also uses the two ranges as a cheap stack-overflow check: it prints a warning if
a saved `rsp` is outside `[running_stack, running_stack + running_stack_size]`.

### Memory allocators

`Kernel/mman/`, `Global/stdmem/buddy_system.c`

Both allocators implement `malloc`, `free`, `init_memory_manager` and `memory_state`. You pick
one at compile time with `-DBUDDY_SYSTEM` (`./run.sh build buddy`); the `#ifdef`s are confined
to `mman/malloc.c`, `mman/free.c` and `mman/mman.c`.

|           | First-fit (default)                                      | Buddy (`BUDDY=1`)                                   |
| --------- | -------------------------------------------------------- | --------------------------------------------------- |
| Structure | run-length map, one `uint32_t` per 4 heap bytes          | implicit binary tree, 2 bits per node               |
| Heap      | 128 MiB at `0x18000000`                                  | 64 MiB at `0x18000000`, 1 MiB bitmap                |
| `malloc`  | O(n) linear scan; run length doubles as a skip pointer   | O(log n) depth-first descent, 256 B minimum block   |
| `free`    | O(1); whole runs are stored, so no coalescing is needed  | O(log n), collapsing SPLIT→EMPTY upward             |
| Trade-off | no metadata overhead, external fragmentation             | bounded internal fragmentation, predictable timing  |

The buddy allocator stores four node states (`EMPTY`, `SPLIT`, `FULL`, `ALLOCATED`) in two bits
per node, at bit indices `2i` and `2i+1`, and moves around the tree with the `GET_LEFT`,
`GET_RIGHT`, `GET_PARENT` and `GET_BROTHER` macros. The 64 MiB heap is tracked by a 1 MiB
bitmap and blocks have no headers: `free` receives just an address, walks down the tree to find
the block, and updates the state on the way back up, which merges freed siblings.

Userland heaps use a second copy of the same allocator with different parameters (`BLOCK 64`, a
129 KiB arena starting at `sbrk(0)`), so kernel and user allocations are kept apart.

### Shell

`Userland/SampleCodeModule/shell/` (~1,600 lines)

The shell has 24 built-in commands. `handleCommand` splits the input line on `|`
(NUL-terminating each stage in place) and passes a handle from one stage to the next.
`pipeAndExec` creates the pipe, forks, `dup2`s the ends into place and `execv`s the stage.

While a foreground job runs, the shell blocks in a single `pselect` over the job's output,
stdin, the raw scancode stream and all background jobs. That's how `ps | grep …`, a trailing
`&`, and Ctrl-C / Ctrl-D work without polling.

A few details:

- `ps` returns a pipe. The kernel creates a pipe, writes the formatted process table into it,
  closes the write end and returns the read end (`Kernel/proc/proc.c:410`), so `ps | grep`
  needs no special handling.
- `less` reads from two pipes: data on stdin and keystrokes on fd 4, since there's no tty
  layer to combine them.
- Unlike POSIX, reading from a closed stdin returns 0 right away. A pipeline stage with nothing
  upstream relies on this to exit.

## Kernel internals

### System calls

Syscalls go through `int 0x80`, with the call number in `rax` and arguments in `rdi`, `rsi` and
`rdx` (System V order). Dispatch is a bounds-checked jump table
(`Kernel/interruptions/syscalls.c:85`). Handlers also receive the call number and a pointer to
the saved context, which `fork` needs to get at the interrupt frame.

| Group      | Calls                                                                                                         |
| ---------- | ------------------------------------------------------------------------------------------------------------- |
| I/O        | `read` `write` `pipe` `close` `dup2` `pselect` `flush`                                                          |
| Memory     | `malloc` `free` `sbrk` `memory_state`                                                                           |
| Process    | `fork` `execv` `_exit` `kill` `waitpid` `getpid` `sched_yield` `sleep` `usleep` `getpriority` `setpriority` `ps` |
| Semaphores | `sem_open` `sem_close` `sem_unlink` `sem_post` `sem_wait`                                                        |
| Devices    | `draw` `get_screen_size` `beep` `get_time` `get_tick` `get_dump`                                                 |

For `fork`, the syscall stub pushes a zero into the child's saved `rax` slot before
dispatching, so the child gets 0 back from the same interrupt frame in which the parent gets
the child's PID.

### Processes

The process table is a static array indexed by PID (`MAX_PROCESSES 50`). States are stored as
the characters `R`, `W`, `B`, `Z` and `D` so `ps` can print them directly. Children are kept in
intrusive `next_child` / `next_brother` lists.

Orphans are reparented to init, and exited processes stay as zombies until they're reaped. Each
process has two semaphores, one for "this child exited" and one counting "some child exited",
which `waitpid(pid, …)` and `waitpid(-1, …)` block on. `init` just loops on
`waitpid(-1, NULL, 0)`. Killing PID 1 shuts the machine down.

### IPC

Pipes are 32 KiB buffers with reference-counted ends. A pipe descriptor is
`(id * 2 + end) | 0x8000`: the low bit separates the read end from the write end, and bit 15
marks it as a pipe rather than one of the built-in streams. When the last write end is closed,
every blocked reader is woken once, which is how they see EOF.

Semaphores are named (POSIX-style) and reference counted, up to `MAX_SEMS 300`. The top bit of
the counter is used as a spinlock, taken with an `xchg`-based test-and-set, and the other 31
bits hold the value. Each semaphore has its own list of blocked processes, so `sem_post` only
looks at its own waiters.

### Blocking

A blocked process stores a predicate and up to five words of state. On every timer tick the
kernel re-evaluates the predicates and moves the processes whose condition holds back to the
ready list. Sleep, `pselect`, semaphore waits and `SIGSTOP` all work this way.

### I/O streams

There's no filesystem. File descriptors 0–3 are four 64 KiB ring buffers (stdin, stdout,
stderr and stdkey), accessed through function-pointer tables. Any other descriptor is a pipe.

stdkey carries raw scancodes, and on every key event it re-sends the whole set of keys
currently held down. The shell uses it to detect Ctrl-C, Ctrl-D and Ctrl-Z, and snake uses it to
let three players share one keyboard. The kernel has no line discipline; line editing and echo
are done in userland.

### Drivers

- Video: 24-bit VBE linear framebuffer at 1024×768, with alpha blending. Full-screen fills
  write eight pixels at a time as `uint64_t`s. Font rendering and double buffering are done in
  userland.
- Keyboard: PS/2, with shift/caps/AltGr state and three layouts (ES_AR, FR_FR, EN_US), switched
  with Escape.
- Timer: PIT at its default 18.2 Hz, so the scheduling quantum is about 55 ms.
- Audio: PC speaker via PIT channel 2 in square-wave mode.
- Console: VGA text mode, used during boot and for panics, and mirrored to serial COM1 so the
  boot log shows up in the terminal.

The kernel handles divide-by-zero and invalid-opcode exceptions. It dumps the registers to
stderr (which userland can read, or you can trigger with the Alt key), draws a full-screen
bitmap and halts.

### Boot

```
BMFS MBR ──► Pure64 ──► kernel @ 0x100000 ──► fork() ──► init (PID 1) ──► shell
             long mode      subsystem init,     via         reaper
             identity map   idle process        int 0x80
```

The kernel starts userland by calling the `fork` syscall on itself: the parent becomes the idle
process and the child `execv`s `init`. Modules are flat binaries copied to fixed addresses;
there's no ELF loader and no relocation.

## Userland

`Toolchain/ModulePacker` packs twelve modules into the image, and they're copied to fixed
addresses at boot.

| Module   | What it does                                                 |
| -------- | ------------------------------------------------------------ |
| `module` | The shell and desktop: module selector, command shell, snake |
| `init`   | PID 1: forks the shell, then reaps orphans forever           |
| `cat`    | Copies stdin to stdout                                       |
| `wc`     | Counts lines                                                 |
| `grep`   | Prints lines matching a substring                            |
| `filter` | Removes vowels from the stream                               |
| `less`   | Pager; data on stdin, keystrokes on fd 4                     |
| `loop`   | Prints its PID on an interval                                |
| `sing`   | PC-speaker jukebox, six tunes                                |
| `phylos` | Dining philosophers                                          |
| `tests`  | Scheduler, memory and synchronisation stress tests           |
| `data`   | Non-executable data blob                                     |

`phylos` runs each philosopher as a separate process, plus one process for printing, all
synchronised with named semaphores. Odd-numbered philosophers pick up their left fork first and
even-numbered ones their right, which prevents deadlock. You can add and remove philosophers
while it runs; a neighbour is only changed when it isn't holding any forks.

`tests` has four suites: `testmm` (random allocate / write / verify / free against either
heap), `testsync` (racing increments with and without a semaphore, with a yield between the
load and the store), `testprio` (priority changes, block and unblock under load) and
`testprocesses` (random kill/block/unblock across 32 processes).

### Shell commands

```
help        snake       set-theme   set-size    set-letter-color   set-highlight-color
clear       dump        sing        echo        cat                wc
filter      loop        grep        less        kill               block
unblock     phylos      ps          nice        mem                tests
```

Commands that start a process (`cat`, `wc`, `grep`, `filter`, `less`, `loop`, `sing`,
`phylos`, `tests`, `ps`) can be combined with `|` and run in the background with a trailing
`&`; the rest run inside the shell. `snake` supports up to three players (`wasd`, `ijkl`, and
`v`/`b`) and has the themes `windows`, `mario`, `creation` and `idyllic`. `help [command]`
lists everything.

The module selector shown at boot also has a clock, a register dump, an exception test and
shutdown.

## Building and running

You need Docker and QEMU on the host. The build container has gcc, nasm and `qemu-img`.

```sh
./run.sh build [naive|buddy]     # build the image, then boot it
./run.sh debug [naive|buddy]     # build with symbols, print the QEMU/GDB invocation
./run.sh pvs                     # PVS-Studio static analysis, SARIF report to Image/
./run.sh docs                    # generate Doxygen HTML into docs/
./run.sh format                  # clang-format the tree
```

Add `wsl` as a third argument to copy the image to a WSL path instead of booting it.

`run.sh` just passes make variables, so you can also run `make all BUDDY=1 DEBUG=1` yourself.
`BUDDY=1` selects the buddy allocator, and `DEBUG=1` adds debug info and also produces ELF
copies of every binary for GDB.

To boot the image by hand:

```sh
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -serial stdio \
    -machine pcspk-audiodev=snd0 -audiodev none,id=snd0
```

`run.sh` still uses `-soundhw pcspk`, which was removed in QEMU 6.0. On a newer QEMU, replace it
with the flags above or remove the sound flags.

To debug, build with `./run.sh debug`, start QEMU with `-s -S` (GDB server on `:1234`, stopped
at reset) and attach GDB to the `.elf` files in `Image/`.

## Repository layout

```
Kernel/         scheduler, memory managers, syscalls, IPC, drivers
  proc/           process table, scheduler, blocking, signals
  mman/           kernel heap: first-fit and buddy behind one interface
  interruptions/  IDT, PIC, syscall dispatch, exception handling
  pipes/  semaphore/  files/  std{in,out,err,key}/    IPC and I/O
  video/  console/  audio/  localization/  ticks/     drivers
Global/         freestanding libc subset shared by kernel and userland
Userland/       the 12 modules, including the shell under SampleCodeModule/
Toolchain/      ModulePacker: concatenates kernel + modules into one payload
Image/          image assembly (BMFS format, qcow2/vmdk conversion)
Bootloader/     vendored Pure64 + BMFS
```

## Authorship

RetOS was written by four people over two ITBA course projects.

- Lucas Lonegro: most of the scheduler and memory subsystems (348 of 370 lines of
  `scheduler.c`, 228 of 263 of `mman.c`, all 225 of the buddy allocator, 358 of 468 of
  `proc.c`), the shell and its pipeline code (~96%), and `phylos`. About 53% of the kernel
  overall.
- Tomás Raiti: co-author of the kernel and main author of the video driver, pipes and
  semaphores. About 47% of the kernel.
- Tobias Noceda: parts of `phylos`, the piano and snake modules, and early semaphore work.
- Peter Seggi: repository maintenance and code review.

Not written by us: `Bootloader/` contains Pure64 (Return Infinity, Ian Seyler) and BMFS, which
we only modified to enable VESA. The repository started from ITBA's x64BareBones course
template by Rodrigo Rearden and Augusto Nizzo McIntosh.

## Known limitations

RetOS was built for coursework, not for real use, and it has a number of limitations:

- No virtual memory. There is one flat identity-mapped address space, so processes are only
  isolated by convention.
- No privilege separation. Userland runs at CS `0x8`, which is ring 0, so the CPU doesn't
  enforce a user/kernel boundary.
- No copy-on-write. `fork` copies the whole stack.
- No filesystem. Programs are flat binaries at fixed addresses built into the image, and adding
  one means editing four files.
- Single core. Pure64 starts the other cores and RetOS parks them.
- At most 50 processes, 100 pipes, 300 semaphores and 10 descriptors per process.
- Only three signals (`SIGKILL`, `SIGSTOP`, `SIGCONT`), with no handlers or masks.
- No CI. Static analysis and formatting are run by hand through `run.sh`.

## Licence

RetOS is released under the GPL-3.0 (`LICENSE`). The parts that come from x64BareBones keep
their original 3-clause BSD licence (`License.txt`). Pure64 and BMFS have their own licences
under `Bootloader/`.
