/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2018 Jonathan Neuschäfer
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

#ifndef ARCH_RISCV_INCLUDE_ARCH_BOOT_H
#define ARCH_RISCV_INCLUDE_ARCH_BOOT_H

#include <stdint.h>

extern const void *rom_fdt;
void run_payload(
		void (*fn)(uintptr_t arg0, uintptr_t arg1),
			uintptr_t arg0,
			uintptr_t arg1);

#endif
