# Copyright 1999-2010 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI="2"
inherit cmake-utils

DESCRIPTION="Wiimotedev daemon service"
HOMEPAGE="http://code.google.com/p/wiimotedev"
SRC_URI="http://wiimotedev.googlecode.com/files/${P}.tar.bz2"

LICENSE="LGPL-2"
SLOT="0"
KEYWORDS="~amd64 ~x86"
IUSE="+daemon +register client examples toolkit qwiimotedev"

COMMON_DEPEND="
	x11-libs/qt-core:4
	examples? ( x11-libs/qt-gui:4 )
        toolkit? ( x11-libs/qt-gui:4 )
	qwiimotedev? ( x11-libs/qt-gui:4 )
        x11-libs/qt-dbus:4
	net-wireless/bluez:0"

RDEPEND="${COMMON_DEPEND}"
DEPEND="${COMMON_DEPEND}"

src_configure() {
    local mycmakeargs=(
	  $(cmake-utils_use daemon BUILD_DAEMON)
	  $(cmake-utils_use client BUILD_CLIENT)
	  $(cmake-utils_use client BUILD_UINPUT)
	  $(cmake-utils_use client BUILD_REGISTER)
	  $(cmake-utils_use client BUILD_TOOLKIT)
	  $(cmake-utils_use client BUILD_EXAMPLES)
	  $(cmake-utils_use client BUILD_QWIIMOTEDEV)
	)

    cmake-utils_src_configure
}