# Copyright 1999-2010 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI="2"
inherit cmake-utils

DESCRIPTION="Wiimotedev daemon service"
HOMEPAGE="http://gitorious.org/wiimotedev"
SRC_URI="http://devstrikeu.pl/packages/${P}.tar.bz2"

LICENSE="LGPL-2"
SLOT="0"
KEYWORDS="amd64 x86"
IUSE="+daemon +syslog"

DEPEND="x11-libs/qt-core:4
	x11-libs/qt-dbus:4
	net-wireless/bluez:0"

#make sure libcwiid is there too

RDEPEND="${DEPEND}"


cmake-utils_src_configure() 
{
    cmake CMakeLists.txt -DDISTRO=gentoo
}