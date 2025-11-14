FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " file://resolvconf.head file://resolvconf-update "

do_install:append() {
    install -d ${D}${sysconfdir}/resolvconf/resolv.conf.d
    install -d ${D}${sysconfdir}/init.d/
    install -m 0644 ${UNPACKDIR}/resolvconf.head ${D}${sysconfdir}/resolvconf/resolv.conf.d/head
    install -m 0755 ${UNPACKDIR}/resolvconf-update ${D}${sysconfdir}/init.d/
}
inherit update-rc.d

INITSCRIPT_PACKAGES = "${PN}"
INITSCRIPT_NAME = "resolvconf-update"
INITSCRIPT_PARAMS = "defaults 95"