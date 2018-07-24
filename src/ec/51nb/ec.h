/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2018 Google
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

#ifndef _EC_51NB_H
#define _EC_51NB_H

#define SETUP_COMMAND 0x4e
#define SETUP_DATA 0x4f
#define LDN_SEL 0x07
#define LDN_ENABLE 0x30

void ec_51nb_enable(void);

#endif /* _EC_51NB_H */
