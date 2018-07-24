/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2019 Google LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _EC_51NB_NPCE985LA0DX_H
#define _EC_51NB_NPCE985LA0DX_H

#define LDN_SEL 0x07
#define LDN_ENABLE 0x30

struct ec_51nb_npce985la0dx_config {
	u8 setup_command;
	u8 setup_data;
};

#endif /* _EC_51NB_NPCE985LA0DX_H */
