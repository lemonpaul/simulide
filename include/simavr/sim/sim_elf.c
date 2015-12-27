/*
	sim_elf.c

	Loads a .elf file, extract the code, the data, the eeprom and
	the "mcu" specification section, also load usable code symbols
	to be able to print meaningful trace information.

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

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <libelf.h>
//#include <gelf.h>

#include "sim_elf.h"
#include "sim_vcd_file.h"
#include "avr_eeprom.h"
#include "avr_ioport.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

void avr_load_firmware(avr_t * avr, elf_firmware_t * firmware)
{
	if (firmware->frequency)
		avr->frequency = firmware->frequency;
	if (firmware->vcc)
		avr->vcc = firmware->vcc;
	if (firmware->avcc)
		avr->avcc = firmware->avcc;
	if (firmware->aref)
		avr->aref = firmware->aref;
#if CONFIG_SIMAVR_TRACE && ELF_SYMBOLS
	int scount = firmware->flashsize >> 1;
	avr->trace_data->codeline = malloc(scount * sizeof(avr_symbol_t*));
	memset(avr->trace_data->codeline, 0, scount * sizeof(avr_symbol_t*));

	for (int i = 0; i < firmware->symbolcount; i++)
		if (firmware->symbol[i]->addr < firmware->flashsize)	// code address
			avr->trace_data->codeline[firmware->symbol[i]->addr >> 1] =
				firmware->symbol[i];
	// "spread" the pointers for known symbols forward
	avr_symbol_t * last = NULL;
	for (int i = 0; i < scount; i++) {
		if (!avr->trace_data->codeline[i])
			avr->trace_data->codeline[i] = last;
		else
			last = avr->trace_data->codeline[i];
	}
#endif

	avr_loadcode(avr, firmware->flash, firmware->flashsize, firmware->flashbase);
	avr->codeend = firmware->flashsize + firmware->flashbase - firmware->datasize;
	if (firmware->eeprom && firmware->eesize) {
		avr_eeprom_desc_t d = { .ee = firmware->eeprom, .offset = 0, .size = firmware->eesize };
		avr_ioctl(avr, AVR_IOCTL_EEPROM_SET, &d);
	}
	// load the default pull up/down values for ports
	for (int i = 0; i < 8; i++)
		if (firmware->external_state[i].port == 0)
			break;
		else {
			avr_ioport_external_t e = {
				.name = firmware->external_state[i].port,
				.mask = firmware->external_state[i].mask,
				.value = firmware->external_state[i].value,
			};
			avr_ioctl(avr, AVR_IOCTL_IOPORT_SET_EXTERNAL(e.name), &e);
		}
	avr_set_command_register(avr, firmware->command_register_addr);
	avr_set_console_register(avr, firmware->console_register_addr);

	// rest is initialization of the VCD file

	if (firmware->tracecount == 0)
		return;
	avr->vcd = malloc(sizeof(*avr->vcd));
	memset(avr->vcd, 0, sizeof(*avr->vcd));
	avr_vcd_init(avr,
		firmware->tracename[0] ? firmware->tracename: "gtkwave_trace.vcd",
		avr->vcd,
		firmware->traceperiod >= 1000 ? firmware->traceperiod : 1000);

	AVR_LOG(avr, LOG_TRACE, "Creating VCD trace file '%s'\n", avr->vcd->filename);
	for (int ti = 0; ti < firmware->tracecount; ti++) {
		if (firmware->trace[ti].mask == 0xff || firmware->trace[ti].mask == 0) {
			// easy one
			avr_irq_t * all = avr_iomem_getirq(avr,
					firmware->trace[ti].addr,
					firmware->trace[ti].name,
					AVR_IOMEM_IRQ_ALL);
			if (!all) {
				AVR_LOG(avr, LOG_ERROR, "ELF: %s: unable to attach trace to address %04x\n",
					__FUNCTION__, firmware->trace[ti].addr);
			} else {
				avr_vcd_add_signal(avr->vcd, all, 8, firmware->trace[ti].name);
			}
		} else {
			int count = 0;
			for (int bi = 0; bi < 8; bi++)
				if (firmware->trace[ti].mask & (1 << bi))
					count++;
			for (int bi = 0; bi < 8; bi++)
				if (firmware->trace[ti].mask & (1 << bi)) {
					avr_irq_t * bit = avr_iomem_getirq(avr,
							firmware->trace[ti].addr,
							firmware->trace[ti].name,
							bi);
					if (!bit) {
						AVR_LOG(avr, LOG_ERROR, "ELF: %s: unable to attach trace to address %04x\n",
							__FUNCTION__, firmware->trace[ti].addr);
						break;
					}

					if (count == 1) {
						avr_vcd_add_signal(avr->vcd, bit, 1, firmware->trace[ti].name);
						break;
					}
					char comp[128];
					sprintf(comp, "%s.%d", firmware->trace[ti].name, bi);
					avr_vcd_add_signal(avr->vcd, bit, 1, firmware->trace[ti].name);
				}
		}
	}
	// if the firmware has specified a command register, do NOT start the trace here
	// the firmware probably knows best when to start/stop it
	if (!firmware->command_register_addr)
		avr_vcd_start(avr->vcd);
}
