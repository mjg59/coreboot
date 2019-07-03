#include <stdint.h>

#ifndef _APPLE_SMC_H_

#ifdef CONFIG_DRIVERS_EMULATED_APPLE_SMC
int apple_smc_io_trap(int addr, int val, bool write);
#else
static inline int apple_smc_io_trap(int addr, int val, bool write) {
	return 0;
}
#endif

#endif /* _APPLE_SMC_H_ */
