######################################################################
# Test: Database Manager
######################################################################
QT       += core sql testlib
QT       -= gui
CONFIG   += c++17 testcase console
CONFIG   -= app_bundle
TEMPLATE  = app
TARGET    = tst_databasemanager

DESTDIR     = $$PWD/../build/tests
OBJECTS_DIR = $$PWD/../build/tests/obj
MOC_DIR     = $$PWD/../build/tests/moc

INCLUDEPATH += $$PWD/../src

HEADERS += \
    ../src/models/contact.h \
    ../src/database/databasemanager.h \
    ../src/validators/fieldvalidator.h

SOURCES += \
    ../src/models/contact.cpp \
    ../src/database/databasemanager.cpp \
    ../src/validators/fieldvalidator.cpp \
    tst_databasemanager.cpp
