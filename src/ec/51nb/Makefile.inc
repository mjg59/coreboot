##
## This file is part of the coreboot project.
##
## Copyright (C) 2017 Iru Cai
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##

ifeq ($(CONFIG_EC_51NB),y)
51NB_EC_INSERT:=$(top)/util/51nb/51nb_ec_insert
INTERMEDIATE+=51nb_ec_insert

51nb_ec_insert: $(obj)/coreboot.pre
ifeq ($(CONFIG_51NB_EC_FIRMWARE),y)
	printf "    Building 51nb_ec_insert.\n"
	$(MAKE) -C util/51nb
	printf "       Inserting EC firmware blobs.\n"
	$(51NB_EC_INSERT) $(obj)/coreboot.pre \
		$(CONFIG_51NB_EC_FW) $(CONFIG_51NB_EC_FW_OFFSET)
endif

PHONY+=51nb_ec_insert

build_complete::
ifeq ($(CONFIG_51NB_EC_FIRMWARE),)
	printf "\n** WARNING **\n"
	printf "You haven't added the firmware blobs for 51NB EC.\n"
	printf "You may be unable to power on your laptop without these blobs.\n"
	printf "Please select the following option to add them:\n\n"
	printf "  Chipset --->\n"
	printf "    [*] Add firmware images for 51NB EC\n\n"
endif

romstage-y += early_init.c

endif
