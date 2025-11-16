SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://mocksensor.c;beginline=1;endline=131;md5=e3a4452b9315a870d13b82f3f8476357"

SRC_URI = "file://mocksensor.c \
           file://Makefile \
          "
UNPACKDIR = "${S}"

inherit module

EXTRA_OEMAKE:append:task-install = " -C ${STAGING_KERNEL_DIR} M=${S}"