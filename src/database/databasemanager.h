// databasemanager.h - handles all SQLite operations for contacts

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QList>
#include <QString>
#include <QSqlDatabase>

#include "../models/contact.h"

// Wraps QSqlDatabase and provides CRUD methods for contacts.
// Uses parameterized queries to avoid SQL injection.
// Each instance gets its own connection name so you can
// safely use multiple instances (e.g., in tests).
class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    // Open (or create) the database at the given path.
    // Creates the contacts table if it doesn't exist yet.
    bool init(const QString &dbPath);

    // Standard CRUD - addContact returns the new row id, or -1 on failure
    int addContact(const Contact &contact);
    bool updateContact(const Contact &contact);
    bool deleteContact(int id);

    // Fetch contacts sorted by name
    QList<Contact> getAllContacts() const;
    Contact getContactById(int id) const;

    bool isOpen() const;

private:
    bool createTable();

    QSqlDatabase m_database;
    QString m_connectionName;
};

#endif // DATABASEMANAGER_H
