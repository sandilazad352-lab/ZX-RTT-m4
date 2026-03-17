@echo off
rem SPDX-License-Identifier: GPL-2.0+
rem

scons --menuconfig -C %SDK_PRJ_TOP_DIR% && scons --save-def -C %SDK_PRJ_TOP_DIR%
