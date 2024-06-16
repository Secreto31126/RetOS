#ifndef PINT_H
#define PINT_H

#include <interruptions.h>

#include <lib.h>
#include <proc.h>
#include <mman.h>
#include <audio.h>
#include <video.h>
#include <ticks.h>
#include <stdin.h>
#include <sched.h>
#include <stdout.h>
#include <stderr.h>
#include <stdkey.h>
#include <files.h>
#include <stdint.h>
#include <header.h>
#include <images.h>
#include <unistd.h>
#include <signal.h>
#include <console.h>
#include <ethernet.h>
#include <sys/wait.h>
#include <semaphores.h>
#include <sys/pselect.h>
#include <sys/resource.h>
#include <localization.h>

typedef void (*InterruptionHandler)();
void noop();

#endif
