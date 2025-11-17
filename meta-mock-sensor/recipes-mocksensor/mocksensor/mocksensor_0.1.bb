SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://mocksensor.c;beginline=1;endline=131;md5=53d8b4e639f9d5a4b123f0fd0592cb3f"

SRC_URI = "file://mocksensor.c \
           file://Makefile \
          "
UNPACKDIR = "${S}"

inherit module

EXTRA_OEMAKE:append:task-install = " -C ${STAGING_KERNEL_DIR} M=${S}"