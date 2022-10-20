FROM tools

RUN git clone --recursive https://github.com/VP-Vibes/Pulpissimo-VP/

# get required conan packages for Pulpissimo-VP
WORKDIR /Pulpissimo-VP/
RUN mkdir build
WORKDIR /Pulpissimo-VP/build/
RUN cmake -DCMAKE_BUILD_TYPE=Release ..
WORKDIR /

# cross-compiler for Pulpissimo SoC
# gcc
#RUN git clone --recursive https://github.com/pulp-platform/pulp-riscv-gnu-toolchain
