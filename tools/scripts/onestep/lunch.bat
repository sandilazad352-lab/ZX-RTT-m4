@echo off
rem SPDX-License-Identifier: GPL-2.0+
rem

if not "%1" == "" scons --apply-def=%1 -C %SDK_PRJ_TOP_DIR%
