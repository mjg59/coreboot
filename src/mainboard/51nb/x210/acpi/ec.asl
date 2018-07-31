/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2016 Google Inc.
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

Device (EC)
{
        Name (_HID, EisaId ("PNP0C09"))
        Name (_UID, 0)

        Name (_GPE, 0x4F)  // _GPE: General Purpose Events
        Name (_CRS, ResourceTemplate () {
                IO (Decode16, 0x62, 0x62, 1, 1)
                IO (Decode16, 0x66, 0x66, 1, 1)
        })

        OperationRegion (ERAM, EmbeddedControl, Zero, 0xFF)
        Field (ERAM, ByteAcc, Lock, Preserve)
        {
                Offset (0x50), 
                CTMP,   8, 
                CFAN,   8, 
                B1SS,   1, 
                BSTS,   2, 
                ACIN,   1, 
                Offset (0x53), 
                BKLG,   8, 
                TOUP,   1, 
                WIRE,   1, 
                BLTH,   1, 
                LIDC,   1, 
                APFG,   1, 
                WRST,   1, 
                BTST,   1, 
                ACEB,   1, 
                CAME,   1, 
                Offset (0x60), 
                DGCP,   16, 
                FLCP,   16, 
                DGVO,   16, 
                BDW,    16, 
                BDL,    16, 
                BPR,    16, 
                BRC,    16, 
                BPV,    16
        }

        Method (_REG, 2, NotSerialized)
        {
                /* Initialize AC power state */
                Store (ACIN, \PWRS)

                /* Initialize LID switch state */
                Store (LIDC, \LIDS)
        }

        /* KEY_BRIGHTNESSUP */
        Method (_Q04)
        {
                Notify(\_SB.PCI0.GFX0.LCD, 0x86)
        }

        /* KEY_BRIGHTNESSDOWN */
        Method (_Q05)
        {
                Notify(\_SB.PCI0.GFX0.LCD, 0x87)
        }

        /* Battery Information Event */
        Method (_Q0C)
        {
                Notify (BAT, 0x81)
        }

        /* AC event */
        Method (_Q0D)
        {
                Store (ACIN, \PWRS)
                Notify (AC, 0x80)
        }

        /* Lid event */
        Method (_Q0E)
        {
                Store (LIDC, \LIDS)
                Notify (LID0, 0x80)
        }

        /* Battery Information Event */
        Method (_Q13)
        {
                Notify (BAT, 0x81)
        }

        /* Battery Status Event */
        Method (_Q14)
        {
                Notify (BAT, 0x80)
        }

        Device (AC)
        {
                Name (_HID, "ACPI0003")
                Name (_PCL, Package () { \_SB })

                Method (_STA)
                {
                        Return (0x0F)
                }
                Method (_PSR)
                {
                        Return (\PWRS)
                }
        }

        Device (BAT)
        {
                Name (_HID, EisaId ("PNP0C0A"))
                Name (_UID, 1)
                Name (_PCL, Package () { \_SB })

                Method (_STA, 0, NotSerialized)  // _STA: Status
                {
                        If (B1SS)
                        {
                            Return (0x1F)
                        }
                        Else
                        {
                            Return (0x0F)
                        }
                }

                Name (PBIF, Package () {
                        0x00000001,  /* 0x00: Power Unit: mAH */ 
                        0xFFFFFFFF,  /* 0x01: Design Capacity */
                        0xFFFFFFFF,  /* 0x02: Last Full Charge Capacity */
                        0x00000001,  /* 0x03: Battery Technology: Rechargeable */
                        0xFFFFFFFF,  /* 0x04: Design Voltage */
                        0x00000000,  /* 0x05: Design Capacity of Warning */
                        0xFFFFFFFF,  /* 0x06: Design Capacity of Low */
                        0x00000001,  /* 0x07: Capacity Granularity 1 */
                        0x00000001,  /* 0x08: Capacity Granularity 2 */
                        "Y91",       /* 0x09: Model Number */
                        "",          /* 0x0a: Serial Number */
                        "LION",      /* 0x0b: Battery Type */
                        "CJOYIN"     /* 0x0c: OEM Information */
                })

                Method (_BIF, 0, Serialized)
                {
                        /* Design Capacity */
                        Store (DGCP, Index (PBIF, 1))

                        /* Last Full Charge Capacity */
                        Store (FLCP, Index (PBIF, 2))

                        /* Design Voltage */
                        Store (DGVO, Index (PBIF, 4))

                        /* Design Capacity of Warning */
                        Store (BDW, Index (PBIF, 5))

                        /* Design Capacity of Low */
                        Store (BDL, Index (PBIF, 6))

                        Return (PBIF)
                }

                Name (PBST, Package () {
                     0x00000000,  /* 0x00: Battery State */
                     0xFFFFFFFF,  /* 0x01: Battery Present Rate */
                     0xFFFFFFFF,  /* 0x02: Battery Remaining Capacity */
                     0xFFFFFFFF,  /* 0x03: Battery Present Voltage */
                })

                Method (_BST, 0, NotSerialized)  // _BST: Battery Status
                {
                        /*
                          * 0: BATTERY STATE
                          *
                          * bit 0 = discharging
                          * bit 1 = charging
                          * bit 2 = critical level
                          */
                        Store (BSTS, Index (PBST, 0))

                        /*
                          * 1: BATTERY PRESENT RATE
                          */
                        Store (BPR, Index (PBST, 1))

                        /*
                         * 2: BATTERY REMAINING CAPACITY
                         */
                        Store (BRC, Index (PBST, 2))

                        /*
                         * 3: BATTERY PRESENT VOLTAGE
                         */
                        Store (BPV, Index (PBST, 3))

                        Return (PBST)
                }
        }
}
