cmake_minimum_required(VERSION 2.6)

set(SINGLEAPP-SOURCES
    qtlocalpeer.cpp
    qtlockedfile.cpp
    qtlockedfile_unix.cpp
    qtsingleapplication.cpp
    qtsinglecoreapplication.cpp
)

set(SINGLEAPP-MOC-HEADERS
    qtlocalpeer.h
    qtsingleapplication.h
    qtsinglecoreapplication.h
)

QT4_WRAP_CPP(SINGLEAPP-SOURCES-MOC ${SINGLEAPP-MOC-HEADERS})

ADD_LIBRARY(qtsingleapplication STATIC
    ${SINGLEAPP-SOURCES}
    ${SINGLEAPP-SOURCES-MOC}
)
