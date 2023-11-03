#ifndef PINT_H
#define PINT_H

#include <interruptions.h>

#include <lib.h>
#include <audio.h>
#include <video.h>
#include <ticks.h>
#include <stdin.h>
#include <stderr.h>
#include <stdkey.h>
#include <stdint.h>
#include <header.h>
#include <memory.h>
#include <console.h>
#include <localization.h>

typedef void (*InterruptionHandler)();
void noop();

#endif
