obj = 51nb_ec_insert
HOSTCC := $(if $(shell type gcc 2>/dev/null),gcc,cc)

all: $(obj)

%: %.c
	$(HOSTCC) -Wall -o $@ $<

clean:
	rm -f 51nb_ec_insert

.PHONY: all clean
