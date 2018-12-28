/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2013 Google, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <cbmem.h>
#include <console/console.h>
#include <console/streams.h>
#include <console/uart.h>
#include <program_loading.h>
#include <soc/clock.h>
#include <soc/sdram.h>
#include <mcall.h>
#include <fdt.h>
#include <string.h>
#include <symbols.h>

extern char own_dtb;

static void update_dtb(void)
{
	uintptr_t dtb_maskrom = (uintptr_t) &own_dtb;
	uint32_t  dtb_size = fdt_size(dtb_maskrom);
	uintptr_t dtb_target = (uintptr_t)cbmem_add(CBMEM_ID_DEVICETREE, dtb_size);

#define DEQ(mon, x) ((cdate[0] == mon[0] && cdate[1] == mon[1] && cdate[2] == mon[2]) ? x : 0)
	const char *cdate = __DATE__;
	int month =
		DEQ("Jan", 1) | DEQ("Feb",  2) | DEQ("Mar",  3) | DEQ("Apr",  4) |
		DEQ("May", 5) | DEQ("Jun",  6) | DEQ("Jul",  7) | DEQ("Aug",  8) |
		DEQ("Sep", 9) | DEQ("Oct", 10) | DEQ("Nov", 11) | DEQ("Dec", 12);

	char date[11] = "YYYY-MM-DD";
	date[0] = cdate[7];
	date[1] = cdate[8];
	date[2] = cdate[9];
	date[3] = cdate[10];
	date[5] = '0' + (month/10);
	date[6] = '0' + (month%10);
	date[8] = cdate[4];
	date[9] = cdate[5];

	memcpy((void*)dtb_target, (void*)dtb_maskrom, dtb_size);
	fdt_reduce_mem(dtb_target, (uintptr_t)_dram + sdram_size_mb() * 1024 * 1024);
	fdt_set_prop(dtb_target, "sifive,fsbl", (uint8_t*)&date[0]);

	for (int i = 0; i < CONFIG_MAX_CPUS; i++)
		OTHER_HLS(i)->fdt = (void*)dtb_target;
}

extern void flash_init(void);

void main(void)
{
	console_init();

	/* TODO: Follow Section 6.3 (FSBL) of the FU540 manual */

	/*
	 * Flush console before changing clock/UART divisor to prevent garbage
	 * being printed.
	 */
	console_tx_flush();

	clock_init();

	// re-initialize UART
	if (IS_ENABLED(CONFIG_CONSOLE_SERIAL))
		uart_init(CONFIG_UART_FOR_CONSOLE);

	flash_init();
	//sdram_init();

	cbmem_initialize_empty();

	update_dtb();

	run_ramstage();
}
