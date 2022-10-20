FROM pulpissimo-dep

WORKDIR "/Pulpissimo-VP/build/"
CMD ["make", "-j", "tgc-vp"]
