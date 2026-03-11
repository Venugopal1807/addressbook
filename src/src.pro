######################################################################
# Addressbook Application - Source Project
# Build: cd src && qmake && make
######################################################################

QT       += core gui widgets sql
CONFIG   += c++17

TARGET    = addressbook
TEMPLATE  = app

# Compiler warnings
QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic

# Output directory
DESTDIR   = $$PWD/../build
OBJECTS_DIR = $$PWD/../build/obj
MOC_DIR     = $$PWD/../build/moc

# ---- Headers ----
HEADERS += \
    models/contact.h \
    database/databasemanager.h \
    validators/fieldvalidator.h \
    widgets/contacttablemodel.h \
    widgets/contactdialog.h \
    widgets/mainwindow.h

# ---- Sources ----
SOURCES += \
    main.cpp \
    models/contact.cpp \
    database/databasemanager.cpp \
    validators/fieldvalidator.cpp \
    widgets/contacttablemodel.cpp \
    widgets/contactdialog.cpp \
    widgets/mainwindow.cpp
