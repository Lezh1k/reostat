TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/avr/include
INCLUDEPATH += include

DEFINES += __AVR_ATtiny2313__

DISTFILES += \
    Makefile


SOURCES += src/main.c \
    src/lcd16x2.c

HEADERS += \
    include/lcd16x2.h \
    include/commons.h

