# Custom PULP SDK build process

## tl;dr

```
make all
```

## Notes
- Location of C-compiler can be set by defining environment variable `PULP_CC` e.g. 

    export PULP_CC=/afs/wsi/es/tools/pulp/pulp-riscv-gnu-toolchain/bin/riscv32-unknown-elf-gcc

  By default the correct path is set for `freia`.
  
- The deafult ISA is rv32imc.
- Pulp extensions can be switched on/off as described in `shared_flags.mk`.

- Need specific pulp-riscv compiler..compile and install `pulp-riscv-gnu-toolchain` in `pulp-platform` on github
