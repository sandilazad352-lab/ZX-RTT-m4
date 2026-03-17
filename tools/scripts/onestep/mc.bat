@echo off
rem SPDX-License-Identifier: GPL-2.0+
rem

if NOT exist %SDK_PRJ_TOP_DIR%\.config (
	echo Not lunch project yet
	goto mb_exit
)

call scons -c -C %SDK_PRJ_TOP_DIR% -j 8
call scons -C %SDK_PRJ_TOP_DIR% -j 8
