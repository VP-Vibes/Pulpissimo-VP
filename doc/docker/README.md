

This doc stuff is probably only relevant for dev purposes

# useful command
```
$ docker run --mount "type=bind,source=$(pwd)/../../Pulpissimo-VP/,target=/Pulpissimo-VP/" pulpissimo
```
# space estimate for images
600 MB Ubuntu20 + build tools
1.8 GB ^ + conan packages
~8 GB pulp compiler !!!
