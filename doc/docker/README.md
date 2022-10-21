

This doc stuff is probably only relevant for dev purposes

# useful command
```
$ docker run --mount "type=bind,source=$(pwd)/../../Pulpissimo-VP/,target=/Pulpissimo-VP/" pulpissimo
```
# contents of pulpissimo-dep image
`/root/.conan/` : conan packages needed for Pulpissimo-VP
`/opt/riscv/` : pulp compiler used for cross compiling firmware
`/custom-pulp-sdk/` : pre-built runtime for firmware

This results in about ~2.56 GB image
- ~500 MB for Ubuntu20 + build tools
- ~1.2 GB for conan packages
- ~900 MB for pulp compiler

This container is then meant as a base with all tools ready:
- develop firmware
- develop VP
- run simulations
