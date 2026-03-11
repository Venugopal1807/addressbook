# Architecture — Addressbook Application

## Overview

The Addressbook application follows a **layered MVC (Model-View-Controller)** architecture with strict separation of concerns. Each layer has a single responsibility and communicates through well-defined interfaces.

---

## Layer Diagram

```
┌─────────────────────────────────────────────────────┐
│                    UI Layer (View)                   │
│  ┌──────────────┐  ┌───────────────┐                │
│  │  MainWindow   │  │ ContactDialog │                │
│  │  - Table view │  │ - Form fields │                │
│  │  - Toolbar    │  │ - Validation  │                │
│  │  - Search bar │  │   feedback    │                │
│  └──────┬───────┘  └───────┬───────┘                │
│         │                  │                         │
│  ┌──────┴──────────────────┴───────┐                │
│  │      ContactTableModel          │                │
│  │  (QAbstractTableModel bridge)   │                │
│  └──────────────┬──────────────────┘                │
├─────────────────┼───────────────────────────────────┤
│         Validation Layer            │                │
│  ┌──────────────┴──────────────────┐                │
│  │        FieldValidator           │                │
│  │  - validateName()               │                │
│  │  - validateMobile()             │                │
│  │  - validateEmail()              │                │
│  │  - validateBirthday()           │                │
│  └─────────────────────────────────┘                │
├─────────────────────────────────────────────────────┤
│              Data Layer (Model)                      │
│  ┌─────────────────┐  ┌────────────────────┐        │
│  │    Contact       │  │  DatabaseManager   │        │
│  │  - id            │  │  - init()          │        │
│  │  - name          │  │  - addContact()    │        │
│  │  - mobile        │  │  - updateContact() │        │
│  │  - email         │  │  - deleteContact() │        │
│  │  - birthday      │  │  - getAllContacts() │        │
│  └─────────────────┘  └────────┬───────────┘        │
├─────────────────────────────────┼───────────────────┤
│              Storage Layer      │                    │
│                        ┌────────┴───────────┐        │
│                        │   SQLite Database   │        │
│                        │   (addressbook.db)  │        │
│                        └────────────────────┘        │
├─────────────────────────────────────────────────────┤
│           Schema Management (Python)                 │
│  ┌─────────────────┐  ┌────────────────────┐        │
│  │ SQLAlchemy Model │  │  Alembic Migrations │       │
│  │ (schema/models)  │  │  (migrations/)      │       │
│  └─────────────────┘  └────────────────────┘        │
└─────────────────────────────────────────────────────┘
```

---

## Component Descriptions

### 1. UI Layer (`src/widgets/`)

| Component | Responsibility |
|-----------|---------------|
| `MainWindow` | Primary window: table view, toolbar, menu bar, search, status bar |
| `ContactDialog` | Modal form for creating/editing contacts with validation feedback |
| `ContactTableModel` | QAbstractTableModel bridging Contact data to QTableView |

**Key design**: The `QSortFilterProxyModel` sits between `ContactTableModel` and `QTableView`, providing sorting and search filtering without modifying source data.

### 2. Validation Layer (`src/validators/`)

| Component | Responsibility |
|-----------|---------------|
| `FieldValidator` | Static validation methods for each contact field type |
| `ValidationResult` | Struct carrying `isValid` flag and error message |

**Key design**: Validation is decoupled from both the model and the UI. Both `Contact::isValid()` and `ContactDialog` delegate to `FieldValidator`, ensuring consistent rules everywhere.

### 3. Data Layer (`src/models/`, `src/database/`)

| Component | Responsibility |
|-----------|---------------|
| `Contact` | Value-type data class representing a single addressbook entry |
| `DatabaseManager` | Encapsulates SQLite connection and all CRUD operations |

**Key design**: `DatabaseManager` uses `QSqlQuery` with bound parameters to prevent SQL injection. Each instance manages its own unique connection name for thread safety.

### 4. Schema Management (`schema/`, `migrations/`)

| Component | Responsibility |
|-----------|---------------|
| `schema/models.py` | SQLAlchemy ORM model (source of truth for schema definition) |
| `schema/manage_schema.py` | CLI wrapper around Alembic migration commands |
| `migrations/` | Alembic environment with versioned migration scripts |

**Key design**: The schema is defined in Python and managed via Alembic. This allows version-controlled, reviewable schema changes independent of the C++ code. The C++ `DatabaseManager` also creates the table on first run as a fallback.

---

## Data Flow

### Adding a Contact

```
User fills form → ContactDialog validates via FieldValidator
    → User clicks Save → ContactDialog::accept()
        → MainWindow::onAddContact() receives Contact
            → DatabaseManager::addContact() persists to SQLite
                → ContactTableModel::addContact() updates the view
                    → Status bar updates count
```

### Searching Contacts

```
User types in search bar → QLineEdit::textChanged signal
    → QSortFilterProxyModel::setFilterFixedString()
        → Table view automatically shows only matching rows
            → Status bar shows "Showing X of Y contacts"
```

### Schema Migration

```
Developer modifies schema/models.py
    → Runs: python manage_schema.py migrate -m "description"
        → Alembic auto-generates migration script in versions/
            → Developer reviews the script
                → Runs: python manage_schema.py upgrade
                    → Database schema updated
```

---

## Naming Conventions

| Type | Convention | Example |
|------|-----------|---------|
| Files | kebab-case | `field-validator.h` (exception: Qt convention uses lowercase) |
| Classes | PascalCase | `DatabaseManager`, `ContactDialog` |
| Functions | camelCase | `addContact()`, `validateEmail()` |
| Member variables | m_camelCase | `m_database`, `m_tableModel` |
| Constants | UPPER_SNAKE | `ColumnCount` |

---

## Build System

The project uses **qmake** as specified by the requirements:

- `addressbook.pro` — Root project (SUBDIRS template)
- `src/src.pro` — Application project (app template)
- `tests/tests.pro` — Test project (SUBDIRS template)
- `tests/tst_*.pro` — Individual test projects

All output goes to `build/` to keep the source tree clean.

---

## Security Considerations

- **SQL Injection Prevention**: All database queries use `QSqlQuery::bindValue()` with named parameters
- **Input Validation**: All user inputs are validated before persistence
- **No Hardcoded Secrets**: Database path is relative to the application binary

---

## Future Improvements

- [ ] Contact photo/avatar support
- [ ] Export contacts to CSV/vCard
- [ ] Import from CSV/vCard
- [ ] Contact groups/categories
- [ ] Dark mode theme toggle
- [ ] Multi-language (i18n) support via Qt Linguist
