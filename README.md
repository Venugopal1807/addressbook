# Addressbook

[![Build & Test](https://github.com/Venugopal1807/addressbook/actions/workflows/build.yml/badge.svg)](https://github.com/Venugopal1807/addressbook/actions/workflows/build.yml)

A modern, production-grade contact management application built with **Qt6/C++** and **SQLite**, featuring real-time field validation, search & filtering, and database schema management via **SQLAlchemy + Alembic**.

![Qt](https://img.shields.io/badge/Qt-6-41CD52?logo=qt&logoColor=white)
![C++](https://img.shields.io/badge/C++-17-00599C?logo=cplusplus&logoColor=white)
![SQLite](https://img.shields.io/badge/SQLite-3-003B57?logo=sqlite&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-blue)

---

## Features

### Core
- **Contact management** — Add, edit, and delete contacts with Name, Mobile, Email, and Birthday fields
- **Persistent storage** — SQLite database with automatic schema creation
- **Contact list** — Sortable table view displaying all stored contacts on launch

### Enhanced
- **Real-time search** — Filter contacts across all fields as you type
- **Column sorting** — Click any column header to sort ascending/descending
- **Field validation** — Type-aware validation with visual feedback:
  - **Name**: Letters, spaces, hyphens, apostrophes (2–100 chars)
  - **Mobile**: International format support, 7–15 digits
  - **Email**: RFC-compliant format validation
  - **Birthday**: Calendar date picker, no future dates
- **Keyboard shortcuts** — `Ctrl+N` (add), `Ctrl+E` (edit), `Delete` (remove)
- **Modern UI** — Clean, professional stylesheet with hover effects and alternating row colors

### Bonus Features
- ✅ **Unit test suite** — Qt Test framework with 20+ test cases
- ✅ **Field validation** — Regex-based validation for all data types
- ✅ **SQLite database** — Persistent storage with parameterized queries
- ✅ **SQLAlchemy + Alembic** — Python-based schema migration management
- ✅ **GitHub Actions CI** — Automated build & test on Ubuntu/Debian
- ✅ **Comprehensive documentation** — Architecture docs, inline comments, this README

---

## Screenshots

The application features a clean, modern interface:

| Main Window | Add/Edit Dialog |
|---|---|
| Sortable contact table with search bar | Form with real-time validation feedback |

---

## Prerequisites

### For Building the Application (Debian/Ubuntu)

```bash
sudo apt-get update
sudo apt-get install -y qt6-base-dev qt6-base-dev-tools libqt6sql6-sqlite qmake6 g++ make
```

### For Schema Management (Optional)

```bash
python3 -m pip install -r schema/requirements.txt
```

---

## Build & Run

### Build from Command Line

```bash
# Clone the repository
git clone https://github.com/Venugopal1807/addressbook.git
cd addressbook

# Build the application
cd src
qmake6 src.pro
make -j$(nproc)

# Run the application
cd ../build
./addressbook
```

### Using the Convenience Makefile

```bash
make build     # Build the application
make run       # Build and run
make test      # Build and run all unit tests
make clean     # Clean build artifacts
make all       # Build everything (app + tests)
```

---

## Project Structure

```
addressbook/
├── .github/workflows/
│   └── build.yml                 # CI/CD pipeline
├── src/
│   ├── main.cpp                  # Entry point + stylesheet
│   ├── models/
│   │   ├── contact.h/cpp         # Contact data model
│   ├── database/
│   │   ├── databasemanager.h/cpp # SQLite CRUD operations
│   ├── validators/
│   │   ├── fieldvalidator.h/cpp  # Input validation rules
│   ├── widgets/
│   │   ├── mainwindow.h/cpp      # Main application window
│   │   ├── contactdialog.h/cpp   # Add/Edit dialog
│   │   ├── contacttablemodel.h/cpp # Table model (MVC)
│   └── src.pro                   # qmake project file
├── tests/
│   ├── tst_contact.cpp           # Contact model tests
│   ├── tst_fieldvalidator.cpp    # Validation tests
│   ├── tst_databasemanager.cpp   # Database CRUD tests
│   └── *.pro                     # Test project files
├── schema/
│   ├── models.py                 # SQLAlchemy ORM model
│   ├── manage_schema.py          # Schema management CLI
│   └── requirements.txt          # Python dependencies
├── migrations/
│   ├── alembic.ini               # Alembic configuration
│   ├── env.py                    # Migration environment
│   └── versions/
│       └── 001_initial_schema.py # Initial migration
├── addressbook.pro               # Root qmake project
├── Makefile                      # Convenience build targets
├── ARCHITECTURE.md               # Architecture documentation
├── README.md                     # This file
└── LICENSE                       # MIT License
```

---

## Testing

### Run All Unit Tests

```bash
# Build tests
cd tests
qmake6 tests.pro
make -j$(nproc)

# Run individual test suites
cd ../build/tests
./tst_contact           # Contact model tests
./tst_fieldvalidator    # Validation rule tests
./tst_databasemanager   # Database CRUD tests
```

### Test Coverage

| Suite | Tests | Coverage |
|-------|-------|----------|
| `tst_contact` | 7 | Construction, getters, setters, equality, validation |
| `tst_fieldvalidator` | 16 | Valid/invalid inputs for all 4 field types |
| `tst_databasemanager` | 8 | Full CRUD cycle, edge cases, empty DB |

---

## Schema Management (SQLAlchemy + Alembic)

The database schema is defined using SQLAlchemy ORM models and managed via Alembic migrations. This enables version-controlled schema changes independently of the C++ application code.

### Setup

```bash
cd schema
pip install -r requirements.txt
```

### Commands

```bash
python manage_schema.py init       # Initialize database with current schema
python manage_schema.py migrate    # Auto-generate migration from model changes
python manage_schema.py upgrade    # Apply pending migrations
python manage_schema.py downgrade  # Rollback last migration
python manage_schema.py current    # Show current revision
python manage_schema.py history    # Show migration history
```

### Workflow for Schema Changes

1. Modify the SQLAlchemy model in `schema/models.py`
2. Generate a migration: `python manage_schema.py migrate -m "add phone_type column"`
3. Review the generated migration in `migrations/versions/`
4. Apply: `python manage_schema.py upgrade`
5. Update the corresponding C++ model and database manager

---

## CI/CD

The GitHub Actions pipeline (`.github/workflows/build.yml`) runs on every push and pull request to `main`:

1. **Install** — Qt6 development packages on Ubuntu
2. **Build** — Compile the application via `qmake6 && make`
3. **Test** — Run all unit test suites
4. **Schema** — Verify Alembic migrations apply cleanly

---

## Architecture

See [ARCHITECTURE.md](ARCHITECTURE.md) for a detailed overview of the layered architecture, design decisions, and data flow.

**Summary**: The application follows an **MVC (Model-View-Controller)** pattern:

- **Model Layer** — `Contact` data class + `DatabaseManager` (SQLite persistence)
- **View Layer** — `MainWindow` + `ContactDialog` (Qt Widgets)
- **Controller Logic** — `ContactTableModel` bridges the model and view layers
- **Validation Layer** — `FieldValidator` provides reusable, testable validation rules

---

## Design Decisions

| Decision | Rationale |
|----------|-----------|
| **QAbstractTableModel** | Clean MVC separation; avoids coupling data to the view |
| **QSortFilterProxyModel** | Enables sorting and search without modifying source data |
| **Parameterized queries** | Prevents SQL injection attacks |
| **WAL journal mode** | Better concurrent read/write performance for SQLite |
| **Separate validation class** | Reusable across UI and tests; single source of truth |
| **SQLAlchemy + Alembic** | Industry-standard schema migrations; version-controlled DB changes |
| **In-memory SQLite for tests** | Fast, isolated test execution; no file cleanup needed |

---

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.
