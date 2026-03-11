######################################################################
# Addressbook Application - Unit Tests
# Build: cd tests && qmake && make
# Run:   ./tst_contact && ./tst_fieldvalidator && ./tst_databasemanager
######################################################################

QT       += core sql testlib
QT       -= gui
CONFIG   += c++17 testcase console
CONFIG   -= app_bundle

TEMPLATE  = app

# Compiler warnings
QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic

DESTDIR     = $$PWD/../build/tests
OBJECTS_DIR = $$PWD/../build/tests/obj
MOC_DIR     = $$PWD/../build/tests/moc

# ---- Shared source files under test ----
INCLUDEPATH += $$PWD/../src

HEADERS += \
    ../src/models/contact.h \
    ../src/database/databasemanager.h \
    ../src/validators/fieldvalidator.h

SOURCES += \
    ../src/models/contact.cpp \
    ../src/database/databasemanager.cpp \
    ../src/validators/fieldvalidator.cpp

# ---- Choose which test to build ----
# By default, builds all tests as separate targets.
# Use: qmake "TEST=tst_contact" to build a specific test.

isEmpty(TEST) {
    # Multi-target build helper: build all tests individually
    TEMPLATE = subdirs
    SUBDIRS  = tst_contact tst_fieldvalidator tst_databasemanager

    tst_contact.file         = tst_contact.pro
    tst_fieldvalidator.file  = tst_fieldvalidator.pro
    tst_databasemanager.file = tst_databasemanager.pro
} else {
    SOURCES += $${TEST}.cpp
    TARGET   = $$TEST
}
