/* SPDX-License-Identifier: GPL-2.0-or-later */

Device (EC)
{
	Name (_HID, EisaId ("PNP0C09"))
	Name (_UID, 0)

	Name (_GPE, 0x4F)  // _GPE: General Purpose Events
	Name (_CRS, ResourceTemplate () {
		IO (Decode16, 0x62, 0x62, 1, 1)
		IO (Decode16, 0x66, 0x66, 1, 1)
	})

	Name (BTN, 0)

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

	/* Fn+F2 */
	Method (_Q06)
	{
		Store (0x1002, Btn)
		Notify(HKEY, 0x80)
	}

	/* Fn+F3 */
	Method (_Q07)
	{
		Store (0x1003, Btn)
		Notify(HKEY, 0x80)
	}

	/* Fn+F5 */
	Method (_Q08)
	{
		Store (0x1005, Btn)
		Notify(HKEY, 0x80)
	}

	/* Fn+F7 */
	Method (_Q09)
	{
		Store (0x1007, Btn)
		Notify(HKEY, 0x80)
	}

	/* Fn+F8 */
	Method (_Q0A)
	{
		Store (0x1008, Btn)
		Notify(HKEY, 0x80)
	}

	/* Fn+F9 */
	Method (_Q0B)
	{
		Store (0x1009, Btn)
		Notify(HKEY, 0x80)
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

	/* Fn+F12 */
	Method (_Q15)
	{
		Store (0x100c, Btn)
		Notify(HKEY, 0x80)
	}

	/* Thinkvantage */
	Method (_Q16)
	{
		Store (0x1018, Btn)
		Notify(HKEY, 0x80)
	}

	Device (HKEY)
	{
		Name (_HID, "LEN0068")
		Method (_STA)
		{
			Return (0x0F)
		}

		/* MASK */
		Name (DHKN, 0x080C)

		/* Effective Mask */
		Name (EMSK, 0)

		/* Device enabled. */
		Name (EN, 0)

		Method (MHKP, 0, NotSerialized)
		{
			Store (BTN, Local0)
			Store (Zero, BTN)
			Return (Local0)
		}		

		/* Enable/disable all events.  */
		Method (MHKC, 1, NotSerialized)
		{
			If (Arg0) {
				Store (DHKN, EMSK)
			}
			Else
			{
				Store (Zero, EMSK)
			}
			Store (Arg0, EN)
		}

		Method (DHKC, 0, NotSerialized) {
		       Return (EN)
		}

		/* Enable/disable event.  */
		Method (MHKM, 2, NotSerialized) {
			If (LLessEqual (Arg0, 0x20)) {
				ShiftLeft (One, Subtract (Arg0, 1), Local0)
				If (Arg1)
				{
					Or (DHKN, Local0, DHKN)
				}
				Else
				{
					And (DHKN, Not (Local0), DHKN)
				}
				If (EN)
				{
					Store (DHKN, EMSK)
				}
			}
		}

		/* Mask hotkey all. */
		Method (MHKA, 0, NotSerialized)
		{
			Return (0x07FFFFFF)
		}

		/* Version */
		Method (MHKV, 0, NotSerialized)
		{
			Return (0x0100)
		}
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

	#include "battery.asl"
}
