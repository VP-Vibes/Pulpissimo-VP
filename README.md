# Pulpissimo-VP
This repo is meant to provide a VP solution for the Pulpissimo chip to quickly develop &amp; test SW.
Initially this is forked from TGC-VP project in VP-Vibes.

## Quick start

* you need to have a C++14 capable compiler, make, python, and cmake installed

* install conan.io (see also http://docs.conan.io/en/latest/installation.html):
  
  ```

  pip3 install --user conan

  ```
  
  It is advised to use conan version 1.5x or newer. In case of an older version please run
  
  ```sh

  pip3 install --user --upgrade conan

  ``` 
  
  Installing conan for the first time you need to create a profile:
  
  ```
  
  conan profile create default --detect
  
  ```
  
* checkout source from git

  ```sh

  git clone --recursive https://github.com/VP-Vibes/Pulpissimo-VP

  ``` 

* start an out-of-source build:
  
  ```

  cd Pulpissimo-VP/
  
  # for Pulpissimo-VP simulation specific changes to `dbt-rise-tgc` submodule, we have to apply a patch
  cd tgc-iss/dbt-rise-tgc/src/sysc/
  patch < /Pulpissimo-VP/core_complex.patch
  cd ../../../../

  # now we can build VP
  cmake -B build/ -S . -DCMAKE_BUILD_TYPE=Release
  cmake --build build/ --target tgc-vp -- -j4

  ```
  
* run the VP with pre-built firmware

  ```

  ctest

  ```

  or

  ```

  build/src/tgc-vp -f ../fw/hello-world/prebuilt/hello.elf 

  ```
  
To rebuild the firmware you need to install a RISC-V toolchain like https://github.com/riscv/riscv-tools.

# Windows/Visual Studio build
TODO

# TODO
- [x] uDMA configuration regs 
- [x] clean up TGC-VP stuff (models, target-SW)
- [x] SPI-M model in UDMA subsystem
- [x] I2S model in UDMA subsystem
- [x] report issue with standalone RDL generator that includes duplicated headers (i2c_channel.h for UDMA)
      default values? read/write masks?
      trailing #endif?
- [x] modification in generated source -> RDL files
- [x] merge to main/master
- [x] core-complex patch (i. break in wfi_evt ii. redirect udma::interrupt peripheral mapped calls)



- [ ] use VPVibes::vpvper as submodule
- [ ] rest of SoC peripherals and support
- [ ] testing via FreeRTOS apps

- [ ] is there something useful to merge from Trax-VP (private minres space)?
