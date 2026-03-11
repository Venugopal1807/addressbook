######################################################################
# Addressbook Application - Root qmake Project
# Build: qmake && make
######################################################################

TEMPLATE = subdirs
SUBDIRS = src tests

# Ensure src is built before tests
tests.depends = src
