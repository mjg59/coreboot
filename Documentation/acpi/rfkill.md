# Coreboot platform rfkill control

## Introduction

Many devices support platform-level control over integrated radio devices
such as wifi or bluetooth cards. However, the ACPI specification provides
no generic mechanism for operating systems to interact with these platform
interfaces. This has led to a proliferation of vendor-specific control
mechanisms. Coreboot ports to existing platforms tend to mimic those
interfaces and make use of existing drivers, but this doesn't solve the
problem for devices where coreboot is the primary firmware implementation.

This specification defines an ACPI device that exposes an interface for
controlling any radios defined by the platform. It is intended to allow for
generic OS drivers to be written which will then work on any coreboot
platform that implements this specification.

## Device specification

The coreboot rfkill device must be declared with a _HID of "BOOT0001".
It must implement the following ACPI methods:

### DEVS

This method provides a list of the supported radios whose state can be
controlledvia this device. It takes no arguments and returns a single
integer. The integer is a bitmask of radio types supported by the device,
defined as follows:

| Bit | Radio type    |
|-----|---------------|
|  0  | Wifi          |
|  1  | Bluetooth     |
|  2  | Ultra-Wideband|
|  3  | WiMax         |
|  4  | WWAN          |
|  5  | GPS           |
|  6  | FM            |
|  7  | NFC           |

### SSTA

This defines the mutable ("soft") state of all radios in the system. This
method takes no arguments and returns a single integer. The integer is a
bitmask representing radio state using the same values as defined in
DEVS. If a bit is 1 then the radio is enabled. If a bit is 0 then the radio
is not present. If a bit is not set in the value returned by the DEVS method
then the meaning of the bit returned by SSTA is undefined. The state
returned by SSTA can be overridden by the CNTL method.

### HSTA

This defines the immutable ("hard") state of all radios in the system. This
method takes no arguments and returns a single integer. The integer is a
bitmask representing radio state using the same values as defined in
DEVS. If a bit is 1 then the radio is enabled. If a bit is 0 then the radio
is disabled. If a bit is not set in the value returned by the DEVS method
then the meaning of the bit returned by HSTA is undefined. The state
returned by HSTA cannot be overridden by the CNTL method.

### CNTL

This method takes a single integer as an argument and returns nothing. The
argument is a bitmask representing radio state using the same values as
defined in DEVS. If a bit is 1 then the platform should enable the
corresponding radio. If a bit is 0 then the platform should disable the
corresponding radio. This method only alters the soft radio state, not the
hard radio state.

### Notifications

The platform will send a Notify with a value of 0x80 if the state is updated
via any mechanism other than an OS request. The OS should then re-evaluate
the state by calling SSTA and HSTA.

## Implementation design

Two types of blocking are defined - "soft" and "hard". Soft state can be
overridden on request by the OS, for example in response to a hotkey
press. Hard state cannot be overridden by the OS, and should correspond to
cases where some external interface has requested that radios be disabled
(for example, a physical switch on the side of the device). Hard and soft
state should be tracked independently - for instance, if a radio is
currently enabled and then disabled via a physical switch being moved, the
state returned by SSTA should still indicate that the radio is enabled while
the state returned by HSTA should indicate that the radio is disabled. If
the OS then requests that the radio be disabled via the CNTL method, SSTA
and HSTA should now both indicate that the radio is disabled. If the
physical switch is now switched back, HSTA should indicate that the radio is
enabled but SSTA should still indicate that the radio is disabled. Only when
the OS requests that the radio be enabled via CNTL should the radio be
re-enabled.

If the platform alters the radio state (in response to any form of policy
decision or in response to a physical switch), it should update the soft
block state (if the OS can override this decision) or the hard block state
(if the OS cannot override this decision) and then call Notify on the device
with an argument of 0x80.

If the platform provides more than one radio of a given type, it should
implement an additional rfkill device. Each radio should only be represented
in a single device.

If an rfkill device should be explicitly tied to a radio device, then the
radio should be exposed in the ACPI device heirarchy and an rfkill device
that controls only that radio implemented as a child of that device.
