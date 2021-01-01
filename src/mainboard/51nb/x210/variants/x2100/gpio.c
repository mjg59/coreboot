/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <soc/ramstage.h>
#include "gpio.h"

void mainboard_silicon_init_params(FSP_S_CONFIG *params)
{
        /* Configure pads prior to SiliconInit() in case there's any
         * dependencies during hardware initialization. */
        gpio_configure_pads(gpio_table, ARRAY_SIZE(gpio_table));
}
