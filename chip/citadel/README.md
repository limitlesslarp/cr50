# based on citadel image `brick.r0.0.2.w0.0.3_0.0.7547-632a0c58b.bin.prod`

>[!IMPORTANT]
>For more Citadel images, please check out https://github.com/gsc-archive/gsc-archive !

# compilation
ensure this folder is at `chip/citadel` in your cr50 clone.

now, ensure you have all the deps.

now, run `BOARD=[your board, e.g: cr50] CHIP=citadel make` and you should have a compiled citadel loader!

<!--
Hi! If you are reading this, I thought I'd let you know that this
decompilation was assisted with AI. It was not 100% written with AI,
but, AI was used to assist with stuff like function names, register
names, and variable names based on context clues from the code around
it. The actual code itself was written by me based on the actual
disassembly of the RO firmware.

If you find something that is inaccurate or doesn't match what you
know, please feel free to make a Pull Request!

Thanks!
-->