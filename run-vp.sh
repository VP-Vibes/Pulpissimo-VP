#!/bin/sh

export RISCV=/opt/riscv/
export PULP_CC=$RISCV/bin/riscv32-unknown-elf-gcc
export PULP_SDK_HOME=/Pulpissimo-VP/fw/custom-pulp-sdk-master/

# # if you want to compile vp
# mkdir -p build/
# cp conan.cmake build/.
# cd build/
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cd /Pulpissimo-VP/
#
#
# # if you want to compile SDK
# cd fw/custom-pulp-sdk-master/
# make clean
# make all
# cd /Pulpissimo-VP/


# if you want to compile fw
cd fw/i2s/test/
make clean
mkdir -p build/obj/
make
/opt/riscv/bin/riscv32-unknown-elf-objdump -d build/i2s > build/i2s.dump
cd /Pulpissimo-VP/






# if build and run vp simulation
FW_TO_RUN=/Pulpissimo-VP/fw/i2s/test/build/i2s

cd build/
make -j4 tgc-vp
src/tgc-vp -f ${FW_TO_RUN}
