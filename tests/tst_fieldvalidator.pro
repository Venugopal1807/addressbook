######################################################################
# Test: Field Validator
######################################################################
QT       += core testlib
QT       -= gui
CONFIG   += c++17 testcase console
CONFIG   -= app_bundle
TEMPLATE  = app
TARGET    = tst_fieldvalidator

DESTDIR     = $$PWD/../build/tests
OBJECTS_DIR = $$PWD/../build/tests/obj
MOC_DIR     = $$PWD/../build/tests/moc

INCLUDEPATH += $$PWD/../src

HEADERS += \
    ../src/validators/fieldvalidator.h

SOURCES += \
    ../src/validators/fieldvalidator.cpp \
    tst_fieldvalidator.cpp
