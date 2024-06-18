#ifndef PINT_H
#define PINT_H

#include <interruptions.h>

#include <audio.h>
#include <console.h>
#include <files.h>
#include <header.h>
#include <images.h>
#include <lib.h>
#include <localization.h>
#include <mman.h>
#include <proc.h>
#include <sched.h>
#include <semaphores.h>
#include <signal.h>
#include <stderr.h>
#include <stdin.h>
#include <stdint.h>
#include <stdkey.h>
#include <stdout.h>
#include <sys/pselect.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <ticks.h>
#include <unistd.h>
#include <video.h>

typedef void (*InterruptionHandler)();
void noop();

#endif
