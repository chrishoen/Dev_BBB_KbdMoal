// SPDX-License-Identifier: GPL-2.0
/*
 * Hidraw Userspace Example
 *
 * Copyright (c) 2010 Alan Ott <alan@signal11.us>
 * Copyright (c) 2010 Signal 11 Software
 *
 * The code may be used by anyone for any purpose,
 * and can serve as a starting point for developing
 * applications using hidraw.
 */

/* Linux */
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

/*
 * Ugly hack to work around failing compilation on systems that don't
 * yet populate new version of hidraw.h to userspace.
 */
#ifndef HIDIOCSFEATURE
#warning Please have your distro update the userspace kernel headers
#define HIDIOCSFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x06, len)
#define HIDIOCGFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x07, len)
#endif

/* Unix */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

const char *bus_str(int bus);

int main(int argc, char** argv)
{
	int fd;
	struct input_event ev;
	int res;
	const char* device = "/dev/input/event1";

	fd = open(device, O_RDONLY);

	if (fd < 0) {
		perror("Unable to open device");
		return 1;
	}

	memset(&ev, 0x0, sizeof(ev));

	int count = 0;
	while (1)
	{
		printf("start read******************************************* %d\n", count++);

		read(fd, &ev, sizeof(ev));
		if (res < 0)
		{
			perror("read");
			return 1;
		}

		if (ev.type == 1 && ev.value == 1) {
			printf("Key: %i State: %i\n", ev.code, ev.value);
		}
	}

	close(fd);
	return 0;
}

