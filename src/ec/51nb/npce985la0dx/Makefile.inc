##
## This file is part of the coreboot project.
##
## Copyright (C) 2017 Google LLC
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

ifeq ($(CONFIG_EC_51NB_NPCE985LA0DX),y)

files_added::
ifeq ($(CONFIG_EC_51NB_NPCE985LA0DX_FIRMWARE),y)
	$(CBFSTOOL) $(obj)/coreboot.rom write -r EC -f $(CONFIG_EC_51NB_NPCE985LA0DX_FW) --fill-upward
endif

build_complete::
ifeq ($(CONFIG_EC_51NB_NPCE985LA0DX_FIRMWARE),)
	printf "\n** WARNING **\n"
	printf "You haven't added the firmware blobs for 51NB EC.\n"
	printf "You may be unable to power on your laptop without these blobs.\n"
	printf "Please select the following option to add them:\n\n"
	printf "  Chipset --->\n"
	printf "    [*] Add firmware images for 51NB EC\n\n"
endif

ramstage-y += npce985la0dx.c

endif
