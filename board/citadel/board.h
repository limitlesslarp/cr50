/* Copyright 2013 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Emulator board configuration */

#ifndef __CROS_EC_BOARD_H
#define __CROS_EC_BOARD_H

/*
 * The default watchdog timeout is 1.6 seconds, but there are some legitimate
 * flash-intensive TPM operations that actually take close to that long to
 * complete. Make sure we don't trigger the watchdog accidentally if the timing
 * is just a little off.
 */
#undef CONFIG_WATCHDOG_PERIOD_MS
#define CONFIG_WATCHDOG_PERIOD_MS 5000

/* TODO(b/462195667): reenable once Cr50 has more space */
#undef CONFIG_CMD_GPIOCFG
/* Features that we don't want */
#undef CONFIG_CMD_LID_ANGLE
#undef CONFIG_CMD_POWERINDEBUG
#undef CONFIG_DMA_DEFAULT_HANDLERS
#undef CONFIG_FMAP
#undef CONFIG_HIBERNATE
#undef CONFIG_LID_SWITCH
#undef CONFIG_CMD_RW
#undef CONFIG_CMD_SYSINFO
#undef CONFIG_CMD_SYSJUMP
#undef CONFIG_CMD_SYSLOCK
#undef CONFIG_CMD_TIMERINFO
#undef CONFIG_CONSOLE_HISTORY

#define CONFIG_CUSTOMIZED_RO
#define CONFIG_FW_INCLUDE_RO /* literally don't undef this, it's how loader is built. */

#ifndef CR50_DEV
/* Disable stuff that should only be in debug builds */
#undef CONFIG_CMD_CRASH
#undef CONFIG_CMD_MD
#undef CONFIG_CMD_SLEEPMASK_SET
#undef CONFIG_CMD_WAITMS
#undef CONFIG_FLASH
#endif

/* Optional features */
#undef CONFIG_EXTPOWER_GPIO
#undef CONFIG_FMAP
#undef CONFIG_POWER_BUTTON
#undef CONFIG_WATCHDOG
#undef CONFIG_SWITCH
#undef CONFIG_INDUCTIVE_CHARGING
#undef CONFIG_HOSTCMD
#undef CONFIG_HOSTCMD_EVENTS
#undef CONFIG_CMD_GPIO
#undef CONFIG_CMD_GPIOCFG
#undef CONFIG_CONSOLE
#undef CONFIG_CMD_CONSOLE
#undef CONFIG_FLASH_LOG
#undef CONFIG_FLASH
#undef CONFIG_CMD_FLASH
#undef CONFIG_WP_ACTIVE_HIGH

#undef CONFIG_LIBCRYPTOC

/* Standard-current Rp */
#define PD_SRC_VNC           PD_SRC_DEF_VNC_MV
#define PD_SRC_RD_THRESHOLD  PD_SRC_DEF_RD_THRESH_MV

/* delay necessary for the voltage transition on the power supply */
#define PD_POWER_SUPPLY_TURN_ON_DELAY  20000 /* us */
#define PD_POWER_SUPPLY_TURN_OFF_DELAY 20000 /* us */

/* Define typical operating power and max power */
#define PD_OPERATING_POWER_MW 15000
#define PD_MAX_POWER_MW       60000
#define PD_MAX_CURRENT_MA     3000
#define PD_MAX_VOLTAGE_MV     20000

#define PD_MIN_CURRENT_MA     500
#define PD_MIN_POWER_MW       7500

/* Configuration for fake Fingerprint Sensor */
#define CONFIG_SPI_CONTROLLER
#define CONFIG_SPI_FP_PORT    1 /* SPI1: third controller config */

#define CONFIG_RNG

#undef CONFIG_FLASH
#undef CONFIG_FLASH_PSTATE
/* We're using TOP_A for partition 0, TOP_B for partition 1 */
#undef CONFIG_FLASH_NVMEM
/* Offset to start of NvMem area from base of flash */
#define CONFIG_FLASH_NVMEM_OFFSET_A (CFG_TOP_A_OFF)
#define CONFIG_FLASH_NVMEM_OFFSET_B (CFG_TOP_B_OFF)
/* Address of start of Nvmem area */
#define CONFIG_FLASH_NVMEM_BASE_A                                              \
	(CONFIG_PROGRAM_MEMORY_BASE + CONFIG_FLASH_NVMEM_OFFSET_A)
#define CONFIG_FLASH_NVMEM_BASE_B                                              \
	(CONFIG_PROGRAM_MEMORY_BASE + CONFIG_FLASH_NVMEM_OFFSET_B)
#define CONFIG_FLASH_NEW_NVMEM_BASE_A                                          \
	(CONFIG_FLASH_NVMEM_BASE_A + CONFIG_FLASH_BANK_SIZE)
#define CONFIG_FLASH_NEW_NVMEM_BASE_B                                          \
	(CONFIG_FLASH_NVMEM_BASE_B + CONFIG_FLASH_BANK_SIZE)

/* Size partition in NvMem */
#define NVMEM_PARTITION_SIZE (CFG_TOP_SIZE)
#define NEW_NVMEM_PARTITION_SIZE (NVMEM_PARTITION_SIZE - CONFIG_FLASH_BANK_SIZE)
#define NEW_NVMEM_TOTAL_PAGES                                                  \
	(2 * NEW_NVMEM_PARTITION_SIZE / CONFIG_FLASH_BANK_SIZE)
/* Size in bytes of NvMem area */
#define CONFIG_FLASH_LOG
#define CONFIG_FLASH_NVMEM_SIZE (NVMEM_PARTITION_SIZE * NVMEM_NUM_PARTITIONS)
/* Enable <key, value> variable support. */
#define CONFIG_FLASH_NVMEM_VARS
#define NVMEM_CR50_SIZE 272
#define CONFIG_FLASH_NVMEM_VARS_USER_SIZE NVMEM_CR50_SIZE

#define CONFIG_CRC8

#ifndef __ASSEMBLER__
#include "common.h"
#include "gpio_signal.h"

enum temp_sensor_id {
	TEMP_SENSOR_CPU = 0,
	TEMP_SENSOR_BOARD,
	TEMP_SENSOR_CASE,
	TEMP_SENSOR_BATTERY,

	TEMP_SENSOR_COUNT
};

enum adc_channel {
	ADC_CH_CHARGER_CURRENT,
	ADC_AC_ADAPTER_ID_VOLTAGE,

	ADC_CH_COUNT
};

/* Fake test charge suppliers */
enum {
	CHARGE_SUPPLIER_TEST1,
	CHARGE_SUPPLIER_TEST2,
	CHARGE_SUPPLIER_TEST3,
	CHARGE_SUPPLIER_TEST4,
	CHARGE_SUPPLIER_TEST5,
	CHARGE_SUPPLIER_TEST6,
	CHARGE_SUPPLIER_TEST7,
	CHARGE_SUPPLIER_TEST8,
	CHARGE_SUPPLIER_TEST9,
	CHARGE_SUPPLIER_TEST10,
	CHARGE_SUPPLIER_TEST_COUNT
};

/* Nv Memory users */
enum nvmem_users {
	NVMEM_TPM = 0,
	NVMEM_CR50,
	NVMEM_NUM_USERS
};

/* UART indexes (use define rather than enum to expand them) */
enum {
	UART_DEFAULT = 0,
	UART_CR50 = 0,
	UART_AP = 1,
	UART_EC = 2,
	UART_COUNT,

	UART_NULL = 0xff,
};

/* Mock functions for EC-CR50 communication test */
int board_has_ec_cr50_comm_support(void);
void board_reboot_ec_deferred(int usec_delay);
void ccd_update_state(void);
#endif /* !__ASSEMBLER__ */

#endif /* __CROS_EC_BOARD_H */
