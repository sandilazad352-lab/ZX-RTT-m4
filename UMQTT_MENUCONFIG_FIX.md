# uMQTT menuconfig issue status

## What happened
Running `scons --menuconfig` failed with:

```
can't open file ".../tools/env/packages/packages/iot/umqtt/Kconfig"
```

The package index `tools/env/packages/packages/iot/Kconfig` includes `umqtt/Kconfig`, but this project state was missing a local Kconfig entry needed by the local package tree.

## What is fixed now
A local Kconfig file was added for the uMQTT package:

- `packages/third-party/umqtt/Kconfig`

This allows Kconfig parsing to complete and enables uMQTT options to appear in menuconfig.

## Verification
Command executed:

```powershell
scons --menuconfig
```

Result: menuconfig opens normally (no missing `umqtt/Kconfig` error).

## Current relevant files
- `tools/env/packages/packages/iot/umqtt/Kconfig` (exists)
- `packages/third-party/umqtt/Kconfig` (added)
- `tools/env/packages/packages/iot/umqtt/package.json` (exists)

## Notes
- The newly added local Kconfig is a minimal functional definition (`PKG_USING_UMQTT`, path, example option).
- If you need full advanced uMQTT tuning options mirrored locally, we can expand `packages/third-party/umqtt/Kconfig` to match all options in the tools/env package Kconfig.
