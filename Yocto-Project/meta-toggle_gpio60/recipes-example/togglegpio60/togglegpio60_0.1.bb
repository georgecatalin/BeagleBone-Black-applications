DESCRIPTION = "This is a program to toggle GPIO number 60 on/off at an interval of 1s"
PRIORITY = "optional"
SECTION = "examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI = "file://togglegpio60.c"
S = "${WORKDIR}"
do_compile() {
${CC} ${CFLAGS} ${LDFLAGS} togglegpio60.c -o togglegpio60
}
do_install() {
install -d ${D}${bindir}
install -m 0755 togglegpio60 ${D}${bindir}
}

inherit systemd
SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE_${PN} = "togglegpio60.service"

SRC_URI_append = " file://togglegpio60.service "
FILES_${PN} += "${systemd_unitdir}/system/togglegpio60.service"

do_install_append() {
  install -d ${D}/${systemd_unitdir}/system
  install -m 064 ${WORKDIR}/togglegpio60.service ${D}/${systemd_unitdir}/system
}
