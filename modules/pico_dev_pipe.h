/*********************************************************************
   PicoTCP. Copyright (c) 2012-2017 Altran Intelligent Systems. Some rights reserved.
   See COPYING, LICENSE.GPLv2 and LICENSE.GPLv3 for usage.

 *********************************************************************/
#ifndef INCLUDE_PICO_PIPE
#define INCLUDE_PICO_PIPE
#include "pico_config.h"
#include "pico_device.h"

void pico_pipe_destroy(struct pico_device *ipc);
struct pico_device *pico_pipe_create(int fds[2], const char * name, const uint8_t *mac);

#endif

