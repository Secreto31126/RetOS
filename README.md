# RetOS

A preemptive, POSIX-flavoured multitasking kernel for x86-64, written from scratch in C and
NASM assembly. No libc, no host OS, no runtime — it boots from its own disk image, brings up
its own scheduler, memory manager and drivers, and runs a userland of twelve programs on top
of a 34-call system interface.

**GPL-3.0 · C / x86-64 assembly · 21.4k lines · 34 syscalls**

RetOS was built at [ITBA](https://www.itba.edu.ar/) across two term-long systems projects
(October 2023 – June 2024). See [Authorship](#authorship) for who wrote what.

---

## At a glance

|                 |                                                                                        |
| --------------- | -------------------------------------------------------------------------------------- |
| **Target**      | x86-64 long mode, single core, flat identity-mapped physical memory                      |
| **Kernel**      | ~7,100 lines, freestanding (`-ffreestanding -nostdlib -mno-red-zone -mno-sse`)           |
| **Syscalls**    | 34, via `int 0x80`                                                                       |
| **Scheduling**  | 40 priority bands, weighted lottery + round-robin, preemption at 18.2 Hz                 |
| **Concurrency** | `fork` / `execv` / `waitpid`, pipes, POSIX named semaphores, `pselect`                   |
| **Memory**      | two interchangeable heap allocators, selected at compile time                            |
| **Graphics**    | 1024×768×24bpp VBE linear framebuffer with alpha blending                                |
| **Userland**    | 12 flat binaries: pipeline shell, `grep`/`cat`/`wc`/`less`, dining philosophers, tests    |
| **Image**       | 6 MiB BMFS disk, Pure64 bootloader, boots under QEMU                                     |

## Quick start

Everything builds inside a container, so the only host requirements are Docker and QEMU:

```sh
git clone https://github.com/Secreto31126/RetOS.git
cd RetOS
./run.sh build          # build the image and boot it in QEMU
```

`./run.sh build buddy` builds the same image against the buddy allocator instead of the
default first-fit one. See [Building and running](#building-and-running) for the full matrix.

---

## Design highlights

Four pieces are worth reading the source for. Each is described as *the constraint, the
design, and what it costs* — with file references so the claims can be checked.

### 1. Proportional-share priority scheduling

`Kernel/proc/scheduler.c:200-370`

The scheduler keeps **40 priority bands** (`PRIO_MIN -20` … `PRIO_MAX 19`), each a circular
list of runnable processes. Band *i* carries weight `w_i = i + 1`, so weights run 1…40 and sum
to `W = 820` (precomputed — a context switch should not pay for a summation).

Each scheduling round draws a uniform index in `[0, W)` from a 16-bit linear-feedback shift
register and walks the bands downward, subtracting weights until the index is exhausted. Band
*i* is therefore selected with probability `w_i / W`. The selected band is then **sticky for
exactly `size` switches**, so every process in it takes one quantum before the next draw.

That combination buys three properties.

**Per-process CPU share is independent of crowding.** A round goes to band *i* with
probability `w_i/W` and yields `n_i` quanta, so band *i* receives

```
       w_i · n_i                                             w_i
   ───────────────────  of the CPU, and each process in it  ───────────────
     Σ_j  w_j · n_j                                          Σ_j  w_j · n_j
```

A process's share is set by *its own* priority weight and does not depend on `n_i`. Spawning
ten more processes at the same priority does not dilute the ones already there — the band
simply runs longer when it wins. This is precisely what a conventional per-process lottery or
a flat round-robin does not give you.

**No process starves.** Every weight is at least 1, so every non-empty band retains non-zero
selection probability, `PRIO_MIN` included. There is no aging machinery because none is needed.

**Switch overhead does not grow with the runnable count.** Band selection is `O(40)`, amortised
over the `n_i` switches the band stays sticky for, and picking the next process inside a band
is `O(1)` — one pointer step around a circular list.

Two honest caveats. `robin_remove` is `O(n)` in the band's length, but it sits off the hot
path: it runs only when a process dies or changes priority. And when the drawn band turns out
to be empty, selection falls back to a deterministic downward scan, which biases slightly
toward higher bands in sparsely populated configurations.

> **Note on direction.** Numeric priority here is **inverted relative to POSIX nice**: a
> *higher* number means a *larger* weight and therefore more CPU. The comment in
> `Kernel/include/proc.h` claiming "lower is higher priority" is stale and contradicts the
> weight table.

### 2. Context switching without virtual memory

`Kernel/proc/scheduler.c:14-73`, `Kernel/asm/proc.asm:70-102`

Pure64 leaves the machine with a 2 MiB-page identity-mapped address space, and the kernel never
touches `CR3`, installs no `#PF` handler, and builds no per-process page tables. Every process
shares one flat address space.

That creates a specific problem for `fork`. A child needs its own stack, but it has to keep
executing at exactly the addresses its parent was running at — every saved pointer in the
copied frame refers to them.

RetOS resolves this by **physically swapping stack contents at context-switch time**. Each PCB
carries two ranges: `stack`, the process's own 512 KiB allocation (`MIN_STACK_SIZE 0x80000`),
and `running_stack`, the range it must actually execute at. `swap_stacks` — hand-written
assembly, eight bytes per iteration with a byte-wise tail — exchanges them in place.

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

The detail that matters: **the copy only happens while `running_stack != stack`** — strictly
inside the window between `fork` and `execv`. Once a process owns the stack it runs on,
steady-state context switches copy nothing at all. The cost is bounded by `running_stack_size`
and completely predictable, which is the property you actually want when there is no MMU to
make isolation free.

The design's consequences show up elsewhere in the kernel, and are worth naming rather than
hiding:

- `pselect` must copy its file-descriptor arrays onto the kernel heap before blocking, because
  a userland pointer is meaningless while another process's stack is loaded
  (`Kernel/proc/blocks.c:176-208`).
- `fork` is an eager, full-stack `memcpy` with **no copy-on-write** — as the source puts it,
  *"We aren't Linux, we must copy the stack at creation time"* (`Kernel/proc/proc.c:152`).
- `sbrk` is consequently a stub; a process's heap cannot grow into a neighbour's address range.

The scheduler reuses the two ranges for a cheap stack-overflow check, warning when a restored
`rsp` falls outside `[running_stack, running_stack + running_stack_size]`.

### 3. Two allocators behind one interface

`Kernel/mman/`, `Global/stdmem/buddy_system.c`

Both allocators implement the same four entry points — `malloc`, `free`,
`init_memory_manager`, `memory_state` — and are selected at compile time with `-DBUDDY_SYSTEM`
(`./run.sh build buddy`). The `#ifdef` split lives in `mman/malloc.c`, `mman/free.c` and
`mman/mman.c`, so the two implementations never see each other.

|           | First-fit (default)                                       | Buddy (`BUDDY=1`)                                   |
| --------- | --------------------------------------------------------- | --------------------------------------------------- |
| Structure | run-length map, one `uint32_t` per 4 heap bytes            | implicit binary tree, 2 bits per node               |
| Heap      | 128 MiB at `0x18000000`                                    | 64 MiB at `0x18000000`, 1 MiB bitmap                |
| `malloc`  | `O(n)` linear scan; run length doubles as a skip pointer   | `O(log n)` depth-first descent, 256 B minimum block |
| `free`    | `O(1)`; whole runs are stored, so no coalescing is needed  | `O(log n)`, collapsing SPLIT→EMPTY upward           |
| Trade-off | no metadata overhead, external fragmentation               | bounded internal fragmentation, predictable timing  |

The buddy implementation is the more interesting of the two. It packs four node states
(`EMPTY`, `SPLIT`, `FULL`, `ALLOCATED`) into two bits at indices `2i` and `2i+1`, navigating
with `GET_LEFT` / `GET_RIGHT` / `GET_PARENT` / `GET_BROTHER` macros. A 64 MiB heap is tracked
by a 1 MiB bitmap with **no per-allocation headers at all** — `free` takes only an address and
recovers the block by walking the tree down to it, and state propagates back up so a freed
block merges with its sibling automatically.

A second, independently parameterised copy of the same allocator (`BLOCK 64`, a 129 KiB arena
anchored at `sbrk(0)`) serves as the per-process userland heap, keeping kernel and userland
allocation fully separate.

### 4. A `pselect`-driven pipeline shell

`Userland/SampleCodeModule/shell/` (~1,600 lines)

Included because it is the clearest demonstration that the kernel primitives actually compose.
The shell registers **24 built-in commands**; `handleCommand` splits a line on `|`,
NUL-terminating each stage in place, and threads a handle from stage to stage. `pipeAndExec`
creates the pipe, `fork`s, `dup2`s the ends into position and `execv`s the stage.

The foreground loop then blocks in a **single `pselect`** over the current module's output,
stdin, the raw scancode stream, and every backgrounded job — which is what makes `ps | grep …`,
a trailing `&`, and Ctrl-C / Ctrl-D chord handling work without a polling loop anywhere.

Two details worth singling out:

- **`ps` is implemented as a pipe.** Kernel-side it creates a pipe, writes the formatted process
  table into the write end, closes it, and returns the *read* file descriptor
  (`Kernel/proc/proc.c:410`). It composes with `grep` with no special-casing at all.
- **`less` reads from two pipes.** Upstream data arrives on stdin while keystrokes arrive on a
  separate pipe at fd 4, because there is no tty layer to multiplex the two.

The shell also documents where RetOS deliberately diverges from POSIX: closing stdin here makes
reads return 0 immediately rather than invoking undefined behaviour, which is what lets a
pipeline stage with no upstream terminate cleanly.

---

## Kernel tour

### System call interface

`int 0x80`, with the call number in `rax` and arguments in the System V registers
(`rdi`, `rsi`, `rdx`). Dispatch is a bounds-checked jump table
(`Kernel/interruptions/syscalls.c:85`); handlers additionally receive the call number and the
saved context pointer, which is how `fork` reaches the interrupt frame.

| Group           | Calls                                                                                                            |
| --------------- | ---------------------------------------------------------------------------------------------------------------- |
| **I/O**         | `read` `write` `pipe` `close` `dup2` `pselect` `flush`                                                             |
| **Memory**      | `malloc` `free` `sbrk` `memory_state`                                                                              |
| **Process**     | `fork` `execv` `_exit` `kill` `waitpid` `getpid` `sched_yield` `sleep` `usleep` `getpriority` `setpriority` `ps`    |
| **Semaphores**  | `sem_open` `sem_close` `sem_unlink` `sem_post` `sem_wait`                                                           |
| **Devices**     | `draw` `get_screen_size` `beep` `get_time` `get_tick` `get_dump`                                                    |

`fork` is worth a note: the syscall stub pushes a zero into the child's saved-`rax` slot before
dispatching, so the child returns 0 out of the same interrupt frame the parent returns a PID
from.

### Processes

Processes live in a static table indexed directly by PID (`MAX_PROCESSES 50`), so
`get_process(pid)` is pointer arithmetic. States are stored as ASCII characters (`R`, `W`, `B`,
`Z`, `D`) so `ps` can print them without a lookup. Children hang off intrusive
`next_child` / `next_brother` lists.

Lifecycle is conventional POSIX in shape: orphans are reparented to init, exit status is held
in a zombie until reaped, and each process carries two semaphores — one signalling "this child
exited", one counting "some child exited" — so `waitpid(-1, …)` and `waitpid(pid, …)` both
block properly. `init` runs the canonical `while (1) waitpid(-1, NULL, 0)` reaper loop.
Killing PID 1 powers the machine off.

### IPC

**Pipes** are 32 KiB buffers with reference-counted ends. A pipe descriptor is encoded as
`(id * 2 + end) | 0x8000`, so the low bit distinguishes the read end from the write end and the
high bit distinguishes pipes from the built-in streams. Closing the last write end wakes every
blocked reader once, which is how EOF propagates.

**Semaphores** are POSIX-named and refcounted (`MAX_SEMS 300`). The top bit of the counter word
doubles as a spinlock, acquired with a test-and-set built on `xchg`; the remaining 31 bits hold
the value. Each semaphore owns its own blocked-process list rather than sharing a global queue,
so a wake touches only the waiters that care.

### Blocking

Blocking is condition-callback based rather than queue based: a blocked process stores a
predicate plus up to five words of state, and every timer tick re-evaluates the predicates and
promotes the satisfied processes back to ready. Sleep, `pselect`, semaphore waits and `SIGSTOP`
are all expressed as predicates over that one mechanism.

### I/O streams

There is no filesystem. Descriptors 0–3 are four 64 KiB ring buffers — stdin, stdout, stderr,
and **stdkey** — dispatched through function-pointer tables; anything else is a pipe.

`stdkey` is the unusual one: it carries raw scancodes and re-emits the *entire currently-held
key set* on every key event. That is what lets the shell detect Ctrl-C/Ctrl-D/Ctrl-Z chords and
lets snake support three simultaneous players, with no kernel-side line discipline at all. Line
editing and echo are done in userland.

### Drivers

- **Video** — 24-bit VBE linear framebuffer at 1024×768, with alpha blending and a fast path
  that packs eight pixels into `uint64_t` batches for full-screen fills. Font rendering and
  double buffering live in userland.
- **Keyboard** — PS/2, with shift/caps/AltGr state tracking and three switchable layouts
  (ES_AR, FR_FR, EN_US), cycled with Escape.
- **Timer** — PIT at its default 18.2 Hz, which sets the scheduling quantum at ~55 ms.
- **Audio** — PC speaker via PIT channel 2, square-wave mode.
- **Console** — a VGA text-mode console used during boot and for panics, mirrored to serial
  COM1 so the boot log lands in the terminal.

Exceptions are handled for divide-by-zero and invalid opcode: the register file is dumped to
stderr (retrievable from userland, or on demand with the Alt key) and a full-screen bitmap is
drawn before the machine halts.

### Boot chain

```
BMFS MBR ──► Pure64 ──► kernel @ 0x100000 ──► fork() ──► init (PID 1) ──► shell
             long mode      subsystem init,     via         reaper
             identity map   idle process        int 0x80
```

The kernel bootstraps userland by issuing a `fork` syscall *against itself*: the parent becomes
the idle process, and the child `execv`s `init`. Modules are flat binaries copied to fixed load
addresses — there is no ELF parsing and no relocation.

---

## Userland

Twelve modules are packed into the image by `Toolchain/ModulePacker` and copied to fixed
addresses at boot.

| Module    | What it does                                                                        |
| --------- | ------------------------------------------------------------------------------------ |
| `module`  | The shell and desktop: module selector, command shell, snake                          |
| `init`    | PID 1 — forks the shell, then reaps orphans forever                                   |
| `cat`     | Copies stdin to stdout                                                                |
| `wc`      | Counts lines                                                                          |
| `grep`    | Echoes lines matching a substring                                                     |
| `filter`  | Strips vowels from the stream                                                         |
| `less`    | Pager — data on stdin, keystrokes on fd 4                                             |
| `loop`    | Prints its PID on an interval                                                         |
| `sing`    | PC-speaker jukebox, six tunes                                                         |
| `phylos`  | Dining philosophers                                                                   |
| `tests`   | Scheduler, memory and synchronisation stress tests                                    |
| `data`    | Non-executable data blob                                                              |

Two of these exist to exercise the kernel rather than decorate it:

**`phylos`** runs one process per philosopher plus a dedicated printer process, coordinating
through the named-semaphore API. Deadlock is avoided by the classic asymmetry — odd-indexed
philosophers take the left fork first, even-indexed take the right. Philosophers can be added
and removed at runtime, guarded so that the affected neighbour is holding no forks when the
table is resized.

**`tests`** dispatches four suites: `testmm` (randomised allocate / write / verify / free
against either heap), `testsync` (racing increments with and without a semaphore, with a
deliberate yield between load and store), `testprio` (priority bands under load, exercising
renice, block and unblock), and `testprocesses` (random kill/block/unblock across 32 processes).

### Shell commands

```
help        snake       set-theme   set-size    set-letter-color   set-highlight-color
clear       dump        sing        echo        cat                wc
filter      loop        grep        less        kill               block
unblock     phylos      ps          nice        mem                tests
```

Commands that spawn a process (`cat`, `wc`, `grep`, `filter`, `less`, `loop`, `sing`, `phylos`,
`tests`, `ps`) compose with `|` and accept a trailing `&` to run in the background; the rest are
handled in-shell. `snake` supports up to three simultaneous players (`wasd`, `ijkl`, and `v`/`b`)
with themes `windows`, `mario`, `creation` and `idyllic`. `help [command]` lists everything.

The boot-time module selector also offers a clock, a register dump, an exception test harness,
and shutdown.

---

## Building and running

Requirements on the host: **Docker** and **QEMU**. The build container carries gcc, nasm and
`qemu-img`.

```sh
./run.sh build [naive|buddy]     # build the image, then boot it
./run.sh debug [naive|buddy]     # build with symbols, print the QEMU/GDB invocation
./run.sh pvs                     # PVS-Studio static analysis, SARIF report to Image/
./run.sh docs                    # generate Doxygen HTML into docs/
./run.sh format                  # clang-format the tree
```

Append `wsl` as a third argument to stage the image into a WSL path instead of booting
directly.

Underneath, the flags are plain make variables — `make all BUDDY=1 DEBUG=1` works if you would
rather drive the build yourself. `BUDDY=1` selects the buddy allocator; `DEBUG=1` adds debug
info and additionally emits ELF copies of every binary for GDB.

To boot the image by hand:

```sh
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -serial stdio \
    -machine pcspk-audiodev=snd0 -audiodev none,id=snd0
```

> The PC-speaker flags above are the modern spelling. `run.sh` still passes `-soundhw pcspk`,
> which **QEMU removed in 6.0** — on a current QEMU, either patch that line or drop the sound
> flags entirely.

For source-level debugging, build with `./run.sh debug`, then start QEMU with `-s -S` (gdbstub
on `:1234`, halted at reset) and attach GDB to the `.elf` files in `Image/`.

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
Toolchain/      ModulePacker — concatenates kernel + modules into one payload
Image/          image assembly (BMFS format, qcow2/vmdk conversion)
Bootloader/     vendored: Pure64 + BMFS
```

## Authorship

RetOS was written by four people across two ITBA course projects.

**Lucas Lonegro** — lead author of the scheduler and memory subsystems: 348 of 370 lines of
`scheduler.c`, 228 of 263 of `mman.c`, all 225 of the buddy allocator, and 358 of 468 of
`proc.c`. Also the shell and its pipeline engine (~96%) and `phylos`. Around 53% of kernel lines
overall.

**Tomás Raiti** — co-author of the kernel and primary author of the video driver, the pipe layer
and the semaphore implementation. Around 47% of kernel lines.

**Tobias Noceda** — contributions to `phylos`, the piano and snake modules, and early
semaphore work. **Peter Seggi** — repository and review contributions.

Not our work: `Bootloader/` vendors **Pure64** (Return Infinity, Ian Seyler) and **BMFS**,
modified only to enable VESA. The repository was seeded from the **x64BareBones** ITBA course
template by **Rodrigo Rearden**, with **Augusto Nizzo McIntosh**.

## Known limitations

RetOS is a purpose-built kernel, it has several limitations for practical applications:

- **No virtual memory.** One flat identity-mapped address space; isolation between processes is
  by convention, not by hardware.
- **No privilege separation.** "Userland" runs at CS `0x8` — ring 0. There is no user/kernel
  boundary the CPU enforces.
- **No copy-on-write.** `fork` eagerly copies the whole stack.
- **No filesystem.** Programs are flat binaries at fixed load addresses, baked into the image;
  adding one means editing four files.
- **Single core.** Pure64 brings up the other cores and RetOS parks them.
- Caps of 50 processes, 100 pipes, 300 semaphores, 10 descriptors per process.
- Three signals (`SIGKILL`, `SIGSTOP`, `SIGCONT`), no handlers and no masks.
- No CI; static analysis and formatting are run manually via `run.sh`.

## Licence

RetOS is released under the **GPL-3.0** (`LICENSE`). The inherited x64BareBones portion remains
under its original **3-clause BSD** licence (`License.txt`). Pure64 and BMFS carry their own
licences under `Bootloader/`.
