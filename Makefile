# Addressbook Application — Convenience Makefile
# Wraps qmake/make commands for common build tasks.
#
# Usage:
#   make build   — Build the application
#   make run     — Build and run
#   make test    — Build and run all unit tests
#   make clean   — Remove all build artifacts
#   make all     — Build everything (app + tests)

.PHONY: all build run test clean build-app build-tests

# Default Qt qmake (override with: make QMAKE=qmake6)
QMAKE ?= qmake6
NPROC := $(shell nproc 2>/dev/null || echo 4)

all: build-app build-tests

build: build-app

build-app:
	@echo "=== Building Application ==="
	@mkdir -p build
	cd src && $(QMAKE) src.pro && $(MAKE) -j$(NPROC)
	@echo "Build complete: build/addressbook"

build-tests:
	@echo "=== Building Tests ==="
	@mkdir -p build/tests
	cd tests && $(QMAKE) tests.pro && $(MAKE) -j$(NPROC)
	@echo "Tests built: build/tests/"

run: build-app
	@echo "=== Running Addressbook ==="
	./build/addressbook

test: build-tests
	@echo "=== Running Unit Tests ==="
	@echo "--- Contact Model Tests ---"
	./build/tests/tst_contact
	@echo ""
	@echo "--- Field Validator Tests ---"
	./build/tests/tst_fieldvalidator
	@echo ""
	@echo "--- Database Manager Tests ---"
	./build/tests/tst_databasemanager
	@echo ""
	@echo "=== All Tests Passed ==="

clean:
	@echo "=== Cleaning ==="
	rm -rf build/
	cd src && $(MAKE) clean 2>/dev/null || true
	cd tests && $(MAKE) clean 2>/dev/null || true
	find . -name "Makefile" -not -path "./Makefile" -delete
	find . -name "*.o" -delete
	find . -name "moc_*" -delete
	@echo "Clean complete."
