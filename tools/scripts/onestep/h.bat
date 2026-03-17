@echo off
rem SPDX-License-Identifier: GPL-2.0+
rem

echo.
echo ZX-RTT SDK OneStep commands:
echo   h             : Get this help.
echo   lunch [No.]   : Start with selected defconfig, .e.g. lunch 3
echo   me            : Config SDK with menuconfig
echo   m             : Build all and generate final image
echo   mc            : Clean ^& Build all and generate final image
echo   mb            : Build bootloader ^& app and generate final image
echo   c             : Clean all
echo   croot/cr      : cd to SDK root directory.
echo   cout/co       : cd to build output directory.
echo   cbuild/cb     : cd to build root directory.
echo   ctarget/ct    : cd to target board directory.
echo   list          : List all SDK defconfig.
echo   list_module   : List all enabled modules.
echo   i             : Get current project's information.
echo   buildall      : Build all the *defconfig in target/configs
echo   rebuildall    : Clean and build all the *defconfig in target/configs
echo.
