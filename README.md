# Cr50
Cr50 firmware, based on Chrome EC.

>[!IMPORTANT]
>now featuring new citadel RO decomp! available at [chip/citadel](chip/citadel)
>
![Shocked](/shocked.png)

## Compilation
To compile, ensure you have `arm-none-eabi-gcc` installed first.

Now, run `BOARD=cr50 make` and you should have a final image at `build/cr50/ec.bin`

Please note that this image is not signed with a valid SignedHeader & CANNOT be ran on a production cr50. For proper signing, you need to have `cr50-codesigner` installed.
