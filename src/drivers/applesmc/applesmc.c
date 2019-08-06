/*
 *  Apple SMC controller
 *
 *  Copyright (c) 2007 Alexander Graf
 *
 *  Authors: Alexander Graf <agraf@suse.de>
 *           Susanne Graf <suse@csgraf.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * *****************************************************************
 *
 * In all Intel-based Apple hardware there is an SMC chip to control the
 * backlight, fans and several other generic device parameters. It also
 * contains the magic keys used to dongle Mac OS X to the device.
 *
 * This driver was mostly created by looking at the Linux AppleSMC driver
 * implementation and does not support IRQ.
 *
 */

#include <stdint.h>
#include <string.h>
#include <console/console.h>
#include <device/applesmc.h>

#define APPLESMC_DEFAULT_IOBASE        0x300

enum {
	APPLESMC_DATA_PORT               = 0x00,
	APPLESMC_CMD_PORT                = 0x04,
	APPLESMC_ERR_PORT                = 0x1e,
	APPLESMC_NUM_PORTS               = 0x20,
};

enum {
	APPLESMC_READ_CMD                = 0x10,
	APPLESMC_WRITE_CMD               = 0x11,
	APPLESMC_GET_KEY_BY_INDEX_CMD    = 0x12,
	APPLESMC_GET_KEY_TYPE_CMD        = 0x13,
};

enum {
	APPLESMC_ST_CMD_DONE             = 0x00,
	APPLESMC_ST_DATA_READY           = 0x01,
	APPLESMC_ST_BUSY                 = 0x02,
	APPLESMC_ST_ACK                  = 0x04,
	APPLESMC_ST_NEW_CMD              = 0x08,
};

enum {
	APPLESMC_ST_1E_CMD_INTRUPTED     = 0x80,
	APPLESMC_ST_1E_STILL_BAD_CMD     = 0x81,
	APPLESMC_ST_1E_BAD_CMD           = 0x82,
	APPLESMC_ST_1E_NOEXIST           = 0x84,
	APPLESMC_ST_1E_WRITEONLY         = 0x85,
	APPLESMC_ST_1E_READONLY          = 0x86,
	APPLESMC_ST_1E_BAD_INDEX         = 0xb8,
};

#ifdef DEBUG_SMC
#define smc_debug(...) printk(BIOS_DEBUG, "AppleSMC: " __VA_ARGS__)
#else
#define smc_debug(...) do { } while (0)
#endif

static char osk[64] = CONFIG_DRIVERS_APPLE_SMC_OSK;

struct AppleSMCData {
	uint8_t len;
	char key[4];
	char data[32];
};

#define NUM_KEYS 6
static struct AppleSMCData keys[NUM_KEYS];

static uint8_t cmd;
static uint8_t status;
static uint8_t status_1e;
static uint8_t last_ret;
static char key[4];
static uint8_t read_pos;
static uint8_t data_len;
static uint8_t data_pos;
static uint8_t data[255];

static bool applesmc_init;

static void applesmc_io_cmd_write(uint16_t addr, uint64_t val)
{
	status = status & 0x0f;

	smc_debug("CMD received: 0x%02x\n", (uint8_t)val);
	switch (val) {
	case APPLESMC_READ_CMD:
		/* did last command run through OK? */
		if (status == APPLESMC_ST_CMD_DONE ||
		    status == APPLESMC_ST_NEW_CMD) {
			cmd = val;
			status = APPLESMC_ST_NEW_CMD | APPLESMC_ST_ACK;
		} else {
			smc_debug("ERROR: previous command interrupted!\n");
			status = APPLESMC_ST_NEW_CMD;
			status_1e = APPLESMC_ST_1E_CMD_INTRUPTED;
		}
		break;
	default:
		smc_debug("UNEXPECTED CMD 0x%02x\n", (uint8_t)val);
		status = APPLESMC_ST_NEW_CMD;
		status_1e = APPLESMC_ST_1E_BAD_CMD;
	}
	read_pos = 0;
	data_pos = 0;
}

static struct AppleSMCData *applesmc_find_key(char *keyname)
{
	int i;

	for (i = 0; i < NUM_KEYS; i++) {
		smc_debug("Comparing against %c%c%c%c\n", keys[i].key[0], keys[i].key[1], keys[i].key[2], keys[i].key[3]);
		if (!memcmp(keys[i].key, keyname, 4)) {
			smc_debug("Found\n");
			return &keys[i];
		}
	}

	return NULL;
}

static void applesmc_io_data_write(uint16_t addr, uint64_t val)
{
	struct AppleSMCData *d;

	smc_debug("DATA received: 0x%02x\n", (uint8_t)val);
	switch (cmd) {
	case APPLESMC_READ_CMD:
		if ((status & 0x0f) == APPLESMC_ST_CMD_DONE) {
			break;
		}
		if (read_pos < 4) {
			key[read_pos] = val;
			status = APPLESMC_ST_ACK;
		} else if (read_pos == 4) {
			smc_debug("READ CMD: reading '%c%c%c%c'\n",
				  key[0], key[1], key[2], key[3]);
			d = applesmc_find_key(key);
			if (d != NULL) {
				smc_debug("Returning %d bytes\n", d->len);
				memcpy(data, d->data, d->len);
				data_len = d->len;
				data_pos = 0;
				status = APPLESMC_ST_ACK |
					APPLESMC_ST_DATA_READY;
				/* clear on valid key */
				status_1e = APPLESMC_ST_CMD_DONE;
			} else {
				smc_debug("READ_CMD: key '%c%c%c%c' not found!\n",
					  key[0], key[1], key[2], key[3]);
				status = APPLESMC_ST_CMD_DONE;
				status_1e = APPLESMC_ST_1E_NOEXIST;
			}
		}
		read_pos++;
		break;
	default:
		status = APPLESMC_ST_CMD_DONE;
		status_1e = APPLESMC_ST_1E_STILL_BAD_CMD;
	}
}

static void applesmc_io_err_write(uint16_t addr, uint64_t val)
{
	smc_debug("ERR_CODE received: 0x%02x, ignoring!\n", (uint8_t)val);
	/* NOTE: writing to the error port not supported! */
}

static uint64_t applesmc_io_data_read(uint16_t addr)
{
	switch (cmd) {
	case APPLESMC_READ_CMD:
		if (!(status & APPLESMC_ST_DATA_READY)) {
			break;
		}
		if (data_pos < data_len) {
			last_ret = data[data_pos];
			smc_debug("READ '%c%c%c%c'[%d] = %02x\n",
				  key[0], key[1], key[2], key[3],
				  data_pos, last_ret);
			data_pos++;
			if (data_pos == data_len) {
				status = APPLESMC_ST_CMD_DONE;
				smc_debug("READ '%c%c%c%c' Len=%d complete!\n",
					  key[0], key[1], key[2], key[3],
					  data_len);
			} else {
				status = APPLESMC_ST_ACK |
					APPLESMC_ST_DATA_READY;
			}
		}
		break;
	default:
		status = APPLESMC_ST_CMD_DONE;
		status_1e = APPLESMC_ST_1E_STILL_BAD_CMD;
	}
	smc_debug("DATA sent: 0x%02x\n", last_ret);

	return last_ret;
}

static uint64_t applesmc_io_cmd_read(uint16_t addr)
{
	smc_debug("CMD sent: 0x%02x\n", status);
	return status;
}

static uint64_t applesmc_io_err_read(uint16_t addr)
{
	/* NOTE: read does not clear the 1e status */
	smc_debug("ERR_CODE sent: 0x%02x\n", status_1e);
	return status_1e;
}

static void applesmc_add_key(const char *keyname, int len, const char *keydata)
{
	int i;

	if (len > 32) {
		smc_debug("Key data %s too long - %d\n", keyname, len);
		return;
	}

	for (i=0; i < NUM_KEYS; i++) {
		if (keys[i].key[0] != 0)
			continue;
		keys[i].len = len;
		memcpy(keys[i].key, keyname, 4);
		memcpy(keys[i].data, keydata, len);
		return;
	}

	smc_debug("Too many keys, unable to add %s\n", keyname);
}

static void apple_smc_init(void)
{
	status = 0x00;
	status_1e = 0x00;
	last_ret = 0x00;

	memset(keys, 0, sizeof(keys));
	applesmc_add_key("REV ", 6, "\x01\x13\x0f\x00\x00\x03");
	applesmc_add_key("OSK0", 32, osk);
	applesmc_add_key("OSK1", 32, osk + 32);
	applesmc_add_key("NATJ", 1, "\0");
	applesmc_add_key("MSSP", 1, "\0");
	applesmc_add_key("MSSD", 1, "\0x3");
}

int apple_smc_io_trap(int addr, int val, bool write)
{
	if (addr < APPLESMC_DEFAULT_IOBASE ||
	    addr >= (APPLESMC_DEFAULT_IOBASE + APPLESMC_NUM_PORTS)) {
		return 0;
	}

	if (!applesmc_init) {
		apple_smc_init();
		applesmc_init = true;
	}

	switch(addr - APPLESMC_DEFAULT_IOBASE) {
	case APPLESMC_DATA_PORT:
		if (write) {
			applesmc_io_data_write(addr, val);
			return 0;
		}
		return applesmc_io_data_read(addr);
	case APPLESMC_CMD_PORT:
		if (write) {
			applesmc_io_cmd_write(addr, val);
			return 0;
		}
		return applesmc_io_cmd_read(addr);
	case APPLESMC_ERR_PORT:
		if (write) {
			applesmc_io_err_write(addr, val);
			return 0;
		}
		return applesmc_io_err_read(addr);
	default:
		return 0;
	}
}
