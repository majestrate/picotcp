/*********************************************************************
   PicoTCP. Copyright (c) 2012-2017 Altran Intelligent Systems. Some rights reserved.
   See COPYING, LICENSE.GPLv2 and LICENSE.GPLv3 for usage.

   Authors: Michiel Kustermans
 *********************************************************************/

#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "pico_device.h"
#include "pico_dev_ipc.h"
#include "pico_stack.h"

struct pico_device_pipe {
    struct pico_device dev;
    int read_fd;
    int write_fd;
};

#define IPC_MTU 2048

static int pico_pipe_send(struct pico_device *dev, void *buf, int len)
{
    struct pico_device_pipe *pipe = (struct pico_device_pipe *) dev;
    return (int)write(pipe->write_fd, buf, (uint32_t)len);
}

static int pico_pipe_poll(struct pico_device *dev, int loop_score)
{
    struct pico_device_pipe *pipe = (struct pico_device_pipe *) dev;
    struct pollfd pfd;
    unsigned char buf[IPC_MTU];
    int len;
    pfd.fd = pipe->read_fd;
    pfd.events = POLLIN;
    do  {
        if (poll(&pfd, 1, 0) <= 0)
            return loop_score;

        len = (int)read(pipe->read_fd, buf, IPC_MTU);
        if (len > 0) {
          loop_score--;
          pico_stack_recv(dev, buf, (uint32_t)len);
        }

    } while(loop_score > 0);
    return 0;
}

/* Public interface: create/destroy. */

void pico_pipe_destroy(struct pico_device *dev)
{
    struct pico_device_pipe *pipe = (struct pico_device_pipe *) dev;
    if(pipe->read_fd > 0) {
        close(pipe->read_fd);
    }
    if(pipe->write_fd > 0) {
        close(pipe->write_fd);
    }
}

struct pico_device *pico_pipe_create(int fds[2], const char * name, const uint8_t *mac)
{
    struct pico_device_pipe *pipe = PICO_ZALLOC(sizeof(struct pico_device_pipe));

    if (!pipe)
        return NULL;

    pipe->read_fd = fds[0];
    pipe->write_fd = fds[1];
    pipe->dev.mtu = IPC_MTU;

    if( 0 != pico_device_init((struct pico_device *)pipe, name, mac)) {
        dbg("pipe init failed.\n");
        pico_pipe_destroy((struct pico_device *)pipe);
        return NULL;
    }

    pipe->dev.overhead = 0;

    pipe->dev.send = pico_pipe_send;
    pipe->dev.poll = pico_pipe_poll;
    pipe->dev.destroy = pico_pipe_destroy;
    dbg("Device %s created.\n", pipe->dev.name);
    return (struct pico_device *)pipe;
}
