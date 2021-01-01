# SPDX-License-Identifier: GPL-2.0-or-later

ramstage-y += $(VARIANT_DIR)/gpio.c
ramstage-$(CONFIG_SOC_INTEL_COMMON_BLOCK_HDA_VERB) += $(VARIANT_DIR)/hda_verb.c
ramstage-$(CONFIG_MAINBOARD_USE_LIBGFXINIT) += gma-mainboard.ads
