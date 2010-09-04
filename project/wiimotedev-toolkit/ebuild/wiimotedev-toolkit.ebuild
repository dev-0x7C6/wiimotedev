# Copyright 1999-2010 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI="2"
inherit cmake-utils

DESCRIPTION="Wiimotedev toolkit utility"
HOMEPAGE="http://gitorious.org/wiimotedev"
SRC_URI="http://devstrikeu.pl/packages/${P}.tar.bz2"

LICENSE="LGPL-2"
SLOT="0"
KEYWORDS="amd64 x86"
IUSE=""

DEPEND="x11-libs/qt-gui:4
	x11-libs/qt-dbus:4
	dev-util/wiimotedev-daemon"

RDEPEND="${DEPEND}"
