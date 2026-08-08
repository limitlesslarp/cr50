# -*- makefile -*-
# Copyright 2013 The ChromiumOS Authors
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
# Board specific files build
#

CHIP:=citadel

board-y=board.o

# Build and link with tpm2
EXTLIB := $(realpath ./tpm2$(BRANCH_EXT))
CFLAGS += -I$(EXTLIB)