#!/bin/sh

export RISCV=/opt/riscv/
export PULP_CC=$RISCV/bin/riscv32-unknown-elf-gcc
export PULP_SDK_HOME=/Pulpissimo-VP/fw/custom-pulp-sdk-master/

# # if you want to compile vp
# mkdir -p build/
# cp conan.cmake build/.
# cd build/
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cd ../
#
#
# # if you want to compile SDK
# cd fw/custom-pulp-sdk-master/
# make clean
# make all
# cd ../../



# # if you want to compile fw
# cd fw/udma_spim_tests/
# make clean
# mkdir -p build/obj/
# make
# /opt/riscv/bin/riscv32-unknown-elf-objdump -d build/test > build/test.dump
# cd ../../






# if build and run vp simulation
FW_TO_RUN=../fw/udma_spim_tests/build/test

cd build/
make -j4 tgc-vp
src/tgc-vp -f ${FW_TO_RUN}
