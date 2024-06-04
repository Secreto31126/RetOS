#ifndef PINT_H
#define PINT_H

#include <interruptions.h>

#include <lib.h>
#include <proc.h>
#include <exec.h>
#include <mman.h>
#include <audio.h>
#include <video.h>
#include <ticks.h>
#include <stdin.h>
#include <stdout.h>
#include <stderr.h>
#include <stdkey.h>
#include <files.h>
#include <stdint.h>
#include <header.h>
#include <images.h>
#include <console.h>
#include <semaphore.h>
#include <localization.h>

typedef void (*InterruptionHandler)();
void noop();

#endif
