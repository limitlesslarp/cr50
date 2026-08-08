/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_HW_REGDEFS_H
#define __CROS_EC_CHIP_CITADEL_HW_REGDEFS_H

#define GC_INTERRUPTS_COUNT                      218

#define GC_PMU_BASE_ADDR                         0x40000000u
#define GC_PMU_BOOT_FLAGS_OFFSET                 0x000u
#define GC_PMU_RESET_REQUEST_OFFSET              0x00cu
#define GC_PMU_REBOOT_PARAM_OFFSET               0x04cu
#define GC_PMU_PERICLKSET_OFFSET                 0x060u
#define GC_PMU_SCRATCH_WR_EN_OFFSET              0x138u
#define GC_PMU_RETRY_COUNTER_OFFSET              0x13cu
#define GC_PMU_STRAP_FUSE_OFFSET                 0x1fff8u
#define GC_PMU_LONG_LIFE_SCRATCH1_OFFSET         0x1fffcu

#define GC_RNG_BASE_ADDR                         0x40030000u
#define GC_RNG_GLITCH_REFRESH_OFFSET             0x018u

#define GC_PINMUX_BASE_ADDR                      0x40040000u
#define GC_PINMUX_UART_TX_SEL_OFFSET             0x028u
#define GC_PINMUX_UART_CTL_OFFSET                0x034u
#define GC_PINMUX_SPI_CS_SEL_OFFSET              0x084u
#define GC_PINMUX_SPI_RX_SEL_OFFSET              0x130u
#define GC_PINMUX_UART_RX_SEL_OFFSET             0x268u
#define GC_PINMUX_OUTPUT_ENABLE_OFFSET           0x300u
#define GC_PINMUX_SPI_DIO0_CTL_OFFSET            0x0e8u
#define GC_PINMUX_SPI_DIO1_CTL_OFFSET            0x0ecu
#define GC_PINMUX_SPI_DIO2_CTL_OFFSET            0x0f0u
#define GC_PINMUX_SPI_DIO3_CTL_OFFSET            0x0f4u

#define GC_TIMER_BASE_ADDR                       0x40470000u
#define GC_TIMER_CTRL_OFFSET                     0x040u
#define GC_TIMER_REBOOT_DELAY_OFFSET             0x048u

#define GC_FUSE_BASE_ADDR                        0x40480000u
#define GC_FUSE_WORD0_OFFSET                     0x038u
#define GC_FUSE_FW_DEFINED_BROM_ERR_RESPONSE_OFFSET 0x0f0u
#define GC_FUSE_FW_DEFINED_BROM_CONFIG1_OFFSET   0x0f8u
#define GC_FUSE_C2_PVT_MARKER_ADDR               0x40480104u
#define GC_FUSE_RBOX_KEY_COMBO0_VAL_ADDR         0x40490038u
#define GC_FUSE_RBOX_POL_KEY1_IN_ADDR            0x40490048u

#define GC_UART_BASE_ADDR                        0x40520000u
#define GC_UART_RDATA_OFFSET                     0x000u
#define GC_UART_WDATA_OFFSET                     0x004u
#define GC_UART_NCO_OFFSET                       0x008u
#define GC_UART_CTRL_OFFSET                      0x00cu
#define GC_UART_STATE_OFFSET                     0x014u

#define GC_SPI_FLAG_BASE_ADDR                    0x40550000u
#define GC_SPI_FLAG_STATUS_OFFSET                0x000u

#define GC_GLOBALSEC_BASE_ADDR                   0x40100000u
#define GC_GLOBALSEC_HASH_REGION0_CTRL_OFFSET    0x040u
#define GC_GLOBALSEC_HASH_REGION1_CTRL_OFFSET    0x044u
#define GC_GLOBALSEC_HASH_REGION2_CTRL_OFFSET    0x048u
#define GC_GLOBALSEC_ROM_LOCK_OFFSET             0x060u
#define GC_GLOBALSEC_CPU0_D_REGION0_SIZE_OFFSET  0x134u
#define GC_GLOBALSEC_FLASH_REGION0_CTRL_OFFSET   0x154u
#define GC_GLOBALSEC_FLASH_REGION7_CTRL_OFFSET   0x16cu
#define GC_GLOBALSEC_HASH_REGION0_BASE_OFFSET    0x180u
#define GC_GLOBALSEC_HASH_REGION0_SIZE_OFFSET    0x184u
#define GC_GLOBALSEC_HASH_REGION1_BASE_OFFSET    0x188u
#define GC_GLOBALSEC_HASH_REGION1_SIZE_OFFSET    0x18cu
#define GC_GLOBALSEC_HASH_REGION2_BASE_OFFSET    0x190u
#define GC_GLOBALSEC_HASH_REGION2_SIZE_OFFSET    0x194u
#define GC_GLOBALSEC_LAUNCH_REGION_BASE_OFFSET   0x278u
#define GC_GLOBALSEC_LAUNCH_REGION_SIZE_OFFSET   0x27cu
#define GC_GLOBALSEC_CPU0_I_STAGING_REGION6_BASE_ADDR_OFFSET 0x2bcu
#define GC_GLOBALSEC_SOFTWARE_LVL_OFFSET         0x2dcu
#define GC_GLOBALSEC_RX_REGION_BASE_OFFSET       0x300u
#define GC_GLOBALSEC_RX_REGION_SIZE_OFFSET       0x304u
#define GC_GLOBALSEC_CPU0_S_PERMISSION_OFFSET    0x350u
#define GC_GLOBALSEC_CPU1_S_PERMISSION_OFFSET    0x358u

#define GC_RSA_BASE_ADDR                         0x40101000u
#define GC_RSA_STATUS_OFFSET                     0x000u
#define GC_RSA_HASH0_OFFSET                      0x004u
#define GC_RSA_HASH_END_OFFSET                   0x024u

#define GC_ALERT_BASE_ADDR                       0x40104000u
#define GC_ALERT_FW_TRIGGER_OFFSET               0x000u
#define GC_ALERT_AES_WIPE_SECRETS_OFFSET         0x040u
#define GC_ALERT_AES_WIPE_COMMIT_OFFSET          0x08cu
#define GC_ALERT_FLASH_RCV_WIPE_OFFSET           0x098u
#define GC_ALERT_INFO0_OFFSET                    0x0b4u
#define GC_ALERT_INFO1_OFFSET                    0x0bcu
#define GC_ALERT_INFO2_OFFSET                    0x0c4u
#define GC_ALERT_INFO3_OFFSET                    0x0ccu
#define GC_ALERT_INFO4_OFFSET                    0x0d4u
#define GC_ALERT_INFO5_OFFSET                    0x0dcu
#define GC_ALERT_REBOOT_LEVEL_OFFSET             0x114u
#define GC_ALERT_EXPECTATION_OFFSET              0x178u

#define GC_CRYPTO_BASE_ADDR                      0x40220000u
#define GC_CRYPTO_WIPE_LAUNCH0_OFFSET            0x024u
#define GC_CRYPTO_WIPE_SECRETS_OFFSET            0x05cu

#define GC_KEYMGR_BASE_ADDR                      0x40230000u
#define GC_KEYMGR_WIPE_SECRETS_OFFSET            0x0b8u
#define GC_KEYMGR_WIPE_LAUNCH2_OFFSET            0x0d0u
#define GC_KEYMGR_MODE_OFFSET                    0x108u
#define GC_KEYMGR_LAUNCH_CTRL_OFFSET             0x110u
#define GC_KEYMGR_LAUNCH_KEY0_OFFSET             0x3100u
#define GC_KEYMGR_LAUNCH_IV_OFFSET               0x3118u
#define GC_KEYMGR_LAUNCH_DST_OFFSET              0x311cu
#define GC_KEYMGR_LAUNCH_WORDS_OFFSET            0x3120u
#define GC_KEYMGR_LAUNCH_FLAGS_OFFSET            0x3124u
#define GC_KEYMGR_WIPE_LAUNCH3_OFFSET            0x3420u

#define GC_LADDER_BASE_ADDR                      0x40240000u
#define GC_LADDER_WIPE_LAUNCH1_OFFSET            0x004u

#define GC_SPI_BASE_ADDR                         0x40320000u
#define GC_SPI_CTRL_OFFSET                       0x000u
#define GC_SPI_BOOT_STATUS_OFFSET                0x004u
#define GC_SPI_TRIGGER_OFFSET                    0x008u
#define GC_SPI_TX_READ_PTR_OFFSET                0x010u
#define GC_SPI_TX_WRITE_PTR_OFFSET               0x014u
#define GC_SPI_RX_READ_PTR_OFFSET                0x020u
#define GC_SPI_RX_WRITE_PTR_OFFSET               0x024u
#define GC_SPI_FIFO_STATE_OFFSET                 0x030u
#define GC_SPI_FIFO_RESET_OFFSET                 0x044u
#define GC_SPI_TX_FIFO_BASE_ADDR                 0x40321000u
#define GC_SPI_RX_FIFO_BASE_ADDR                 0x40321800u

#define GC_TRNG_BASE_ADDR                        0x40410000u
#define GC_TRNG_OUTPUT_TIME_COUNTER_OFFSET       0x024u

#define GC_M3_BASE_ADDR                          0xe0000000u
#define GC_M3_DWT_CYCCNT_OFFSET                  0x1004u
#define GC_M3_VTOR_OFFSET                        0xed08u
#define GC_M3_DWT_CYCCNT_ADDR                    GC_M3_BASE_ADDR + GC_M3_DWT_CYCCNT_OFFSET
#define GC_M3_VTOR_ADDR                          GC_M3_BASE_ADDR + GC_M3_VTOR_OFFSET

#endif /* __CROS_EC_CHIP_CITADEL_HW_REGDEFS_H */
