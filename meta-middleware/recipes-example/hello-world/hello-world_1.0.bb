SUMMARY = "A simple CMake-based Hello World application"
DESCRIPTION = "This is a simple Hello World application built with CMake"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "curl"
RDEPENDS_${PN} += "libcurl"

SRC_URI = "file://hello.cpp \
           file://CMakeLists.txt \
          "

UNPACKDIR = "${S}"

inherit cmake
