/*
	simduino.c

	Copyright 2008, 2009 Michel Pollet <buserror@gmail.com>

 	This file is part of simavr.

	simavr is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	simavr is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "sim_arduino.h"


// avr special flash initalization
// here: open and map a file to enable a persistent storage for the flash memory
void avr_special_init( avr_t * avr, void * data)
{
	struct avr_flash *flash_data = (struct avr_flash *)data;

	printf("%s\n", __func__);
	// open the file
	flash_data->avr_flash_fd = open(flash_data->avr_flash_path,
									O_RDWR|O_CREAT, 0644);
	if (flash_data->avr_flash_fd < 0) {
		perror(flash_data->avr_flash_path);
		exit(1);
	}
	// resize and map the file the file
	(void)ftruncate(flash_data->avr_flash_fd, avr->flashend + 1);
	ssize_t r = read(flash_data->avr_flash_fd, avr->flash, avr->flashend + 1);
	if (r != avr->flashend + 1) {
		fprintf(stderr, "unable to load flash memory\n");
		perror(flash_data->avr_flash_path);
		exit(1);
	}
}

// avr special flash deinitalization
// here: cleanup the persistent storage
void avr_special_deinit( avr_t* avr, void * data)
{
	struct avr_flash *flash_data = (struct avr_flash *)data;

	printf("%s\n", __func__);
	lseek(flash_data->avr_flash_fd, SEEK_SET, 0);
	ssize_t r = write(flash_data->avr_flash_fd, avr->flash, avr->flashend + 1);
	if (r != avr->flashend + 1) {
		fprintf(stderr, "unable to load flash memory\n");
		perror(flash_data->avr_flash_path);
	}
	close(flash_data->avr_flash_fd);
	//uart_pty_stop(&uart_pty);
}
