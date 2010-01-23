# -------------------------------------------------
# Project created by QtCreator 2009-11-10T20:39:05
# -------------------------------------------------
QT += dbus
QT -= gui
LIBS += -lcwiid
TARGET = wiimotedev
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
UI_DIR = cache
MOC_DIR = cache
RCC_DIR = cache
OBJECTS_DIR = cache
SOURCES += src/main.cpp \
    src/wiimoteconnection.cpp \
    src/connectionmanager.cpp
HEADERS += src/wiimoteconnection.h \
    src/connectionmanager.h
unix { 
    INSTALL_PREFIX = /usr/sbin/
    CONFIG_PREFIX = /etc/wiimotedev/
    INITD_PREFIX = /etc/init.d/
    DBUS_PREFIX = /etc/dbus-1/system.d/
    target.path = $${INSTALL_PREFIX}
    config.path = $${CONFIG_PREFIX}
    config.files = default/scancode.ini \
        default/wiimotedev.conf
    initd.path = $${INITD_PREFIX}
    initd.files = initd/wiimotedev
    dbus.path = $${DBUS_PREFIX}
    dbus.files = dbus-1/org.wiimotedev.conf
    INSTALLS = target \
        config \
        initd \
        dbus
}
