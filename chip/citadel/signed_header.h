/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_SIGNED_HEADER_H
#define __CROS_EC_CHIP_CITADEL_SIGNED_HEADER_H

#include <stddef.h>
#include <stdint.h>

#define CITADEL_SIGNED_HEADER_MAGIC 0xfffffffeu
#define CITADEL_FUSE_IGNORE 0x3aabadacu
#define CITADEL_INFO_IGNORE 0xa5c35a3cu
#define CITADEL_FUSE_MAX 128
#define CITADEL_INFO_MAX 128

struct SignedHeader {
  uint32_t magic;
  uint32_t signature[96];
  uint32_t img_chk_;
  uint32_t tag[7];
  uint32_t keyid;
  uint32_t key[96];
  uint32_t image_size;
  uint32_t ro_base;
  uint32_t ro_max;
  uint32_t rx_base;
  uint32_t rx_max;
  uint32_t fusemap[CITADEL_FUSE_MAX / (8 * sizeof(uint32_t))];
  uint32_t infomap[CITADEL_INFO_MAX / (8 * sizeof(uint32_t))];
  uint32_t epoch_;
  uint32_t major_;
  uint32_t minor_;
  uint64_t timestamp_;
  uint32_t p4cl_;
  uint32_t applysec_;
  uint32_t config1_;
  uint32_t err_response_;
  uint32_t expect_response_;
  union {
    struct {
      uint32_t keyid;
      uint32_t r[8];
      uint32_t s[8];
    } ext_sig;
    struct {
      uint32_t FSH_SMW_SETTING_OPTION3;
      uint32_t FSH_SMW_SETTING_OPTION2;
      uint32_t FSH_SMW_SETTING_OPTIONA;
      uint32_t FSH_SMW_SETTING_OPTIONB;
      uint32_t FSH_SMW_SMP_WHV_OPTION1;
      uint32_t FSH_SMW_SMP_WHV_OPTION0;
      uint32_t FSH_SMW_SME_WHV_OPTION1;
      uint32_t FSH_SMW_SME_WHV_OPTION0;
    } fsh;
  } u;
  uint32_t _pad[5];
  struct {
    unsigned size : 12;
    unsigned offset : 20;
  } swap_mark;
  uint32_t rw_product_family_;
  uint32_t board_id_type;
  uint32_t board_id_type_mask;
  uint32_t board_id_flags;
  uint32_t dev_id0_;
  uint32_t dev_id1_;
  uint32_t fuses_chk_;
  uint32_t info_chk_;
};

_Static_assert(sizeof(struct SignedHeader) == 1024,
               "Citadel signed header must be 1024 bytes");
_Static_assert(offsetof(struct SignedHeader, image_size) == 0x328,
               "Citadel image_size offset drifted");
_Static_assert(offsetof(struct SignedHeader, info_chk_) == 0x3fc,
               "Citadel info_chk_ offset drifted");

#endif /* __CROS_EC_CHIP_CITADEL_SIGNED_HEADER_H */
