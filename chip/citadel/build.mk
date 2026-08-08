# -*- makefile -*-
# Copyright 2026 The ChromiumOS Authors
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

CORE:=cortex-m
CFLAGS_CPU+=-march=armv7-m -mcpu=cortex-m3

chip-y+=flash.o
chip-y+=gpio.o
chip-y+=system.o
chip-y+=clock.o
chip-y+=uart.o

ifneq ($(CONFIG_CUSTOMIZED_RO),)
CPPFLAGS_RO += -I$(realpath cryptoc/include)
custom-ro_objs-y += chip/$(CHIP)/loader/key_ladder.o
custom-ro_objs-y += chip/$(CHIP)/loader/debug_printf.o
custom-ro_objs-y += chip/$(CHIP)/loader/hw_sha256.o
custom-ro_objs-y += chip/$(CHIP)/loader/launch.o
custom-ro_objs-y += chip/$(CHIP)/loader/main.o
custom-ro_objs-y += chip/$(CHIP)/loader/regtable.o
custom-ro_objs-y += chip/$(CHIP)/loader/rescue.o
custom-ro_objs-y += chip/$(CHIP)/loader/rom_flash.o
custom-ro_objs-y += chip/$(CHIP)/loader/ro_uart.o
custom-ro_objs-y += chip/$(CHIP)/loader/setup.o
custom-ro_objs-y += chip/$(CHIP)/loader/spicert.o
custom-ro_objs-y += chip/$(CHIP)/loader/spiflash.o
custom-ro_objs-y += chip/$(CHIP)/loader/system.o
custom-ro_objs-y += chip/$(CHIP)/loader/vectors.o
custom-ro_objs-y += chip/$(CHIP)/loader/verify.o
custom-ro_objs-y += common/util.o
dirs-y += chip/$(CHIP)/loader
endif

test-list-y=

ifneq ($(BRANCH),)
CITADEL_BRANCH_MANIFEST := chip/$(CHIP)/signing/manifest.$(BRANCH).json
ifneq ($(wildcard $(CITADEL_BRANCH_MANIFEST)),)
MANIFEST := $(CITADEL_BRANCH_MANIFEST)
else
MANIFEST := chip/$(CHIP)/signing/manifest.TOT.json
endif
else
MANIFEST := chip/$(CHIP)/signing/manifest.TOT.json
endif

RO_MANIFEST := chip/$(CHIP)/signing/manifest.RO.json
CITADEL_FUSES := chip/$(CHIP)/signing/fuses.xml
CR50_RO_KEY ?= a1_dev.pem

ifeq ($(CHIP_MK_INCLUDED_ONCE),)

CHIP_MK_INCLUDED_ONCE := 1

CODESIGNER_PATH := $(abspath util/signer)
SIGNER := $(firstword $(wildcard ../../build/bin/$(shell uname -m)/cr50-codesigner \
		/usr/bin/cr50-codesigner \
		/usr/local/bin/cr50-codesigner \
		$(CODESIGNER_PATH)/codesigner))
cmd_elf_to_signed = $(SIGNER) --key=chip/$(CHIP)/signing/$(3) --b \
	--input=$< --format=bin --output=$@.signed $(SIGNER_EXTRAS) && \
	/usr/bin/printf '\xfe\xff\xff\xff' | \
	dd of=$@.signed bs=1 count=4 conv=notrunc status=none && \
	mv $@.signed $@

SIGNER_MANIFEST := $(shell mktemp /tmp/citadel.signer.XXXXXX)
COPY_MANIFEST := $(shell /bin/cp $(MANIFEST) $(SIGNER_MANIFEST))

RO_SIGNER_EXTRAS += -j $(RO_MANIFEST) -x $(CITADEL_FUSES)
RW_SIGNER_EXTRAS += -j $(SIGNER_MANIFEST) -x $(CITADEL_FUSES)

ifeq ($(H1_DEVIDS),)
CR50_RW_KEY = loader-testkey-A.pem
else
$(error Citadel node-locked signing keys are not configured)
endif

HEX_NAME := $(shell printf "$(BOARD)" | /usr/bin/awk -F_ ' \
	 {if (NF == 2) \
	     { printf($$1" "toupper($$2)) } \
	   else \
	     { printf($$0) } \
	  }' | hexdump -ve '1/1 "%.2x"')
HEX_LEN  := $(shell printf $(HEX_NAME) | wc -c)
$(shell sed -i "s/tag\": \"0\{$(HEX_LEN)\}/tag\": \"$(HEX_NAME)/" \
       ${SIGNER_MANIFEST})

else

$(out)/RO/ec.RO.flat: $(RO_MANIFEST) $(CITADEL_FUSES)
$(out)/RO/ec.RO.flat: SIGNER_EXTRAS = $(RO_SIGNER_EXTRAS)
$(out)/RW/ec.RW.flat $(out)/RW/ec.RW_B.flat: $(MANIFEST) $(CITADEL_FUSES)
$(out)/RW/ec.RW_B.flat: $(out)/RW/ec.RW.flat
$(out)/RW/ec.RW.flat $(out)/RW/ec.RW_B.flat: SIGNER_EXTRAS = $(RW_SIGNER_EXTRAS)

endif
