/**
 * @file databasemanager.h
 * @brief SQLite database manager for contact persistence.
 *
 * Provides a singleton-style interface for all database operations,
 * including CRUD methods for Contact objects. Uses Qt's SQL module
 * with parameterized queries for security.
 */

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QList>
#include <QString>
#include <QSqlDatabase>

#include "../models/contact.h"

/**
 * @class DatabaseManager
 * @brief Manages SQLite database connection and CRUD operations for contacts.
 *
 * This class handles:
 * - Database initialization and schema creation
 * - Adding, retrieving, updating, and deleting contacts
 * - Parameterized queries to prevent SQL injection
 * - Proper error logging via qWarning()
 *
 * Usage:
 * @code
 *   DatabaseManager db;
 *   if (db.init("addressbook.db")) {
 *       db.addContact(contact);
 *       QList<Contact> all = db.getAllContacts();
 *   }
 * @endcode
 */
class DatabaseManager
{
public:
    /**
     * @brief Constructs a DatabaseManager instance.
     *
     * The database is not opened until init() is called.
     */
    DatabaseManager();

    /**
     * @brief Destructor — closes the database connection.
     */
    ~DatabaseManager();

    /**
     * @brief Initializes the database connection and creates the schema.
     * @param dbPath Path to the SQLite database file.
     * @return true on success, false on failure.
     *
     * Creates the contacts table if it does not exist. If the database
     * file does not exist, it will be created automatically by SQLite.
     */
    bool init(const QString &dbPath);

    /**
     * @brief Adds a new contact to the database.
     * @param contact The contact to add (id is ignored; auto-generated).
     * @return The database-assigned id on success, -1 on failure.
     */
    int addContact(const Contact &contact);

    /**
     * @brief Updates an existing contact in the database.
     * @param contact The contact with updated fields (id must be valid).
     * @return true on success, false on failure.
     */
    bool updateContact(const Contact &contact);

    /**
     * @brief Deletes a contact by its id.
     * @param id The primary key of the contact to delete.
     * @return true on success, false on failure.
     */
    bool deleteContact(int id);

    /**
     * @brief Retrieves all contacts from the database.
     * @return A list of all stored Contact objects, sorted by name.
     */
    QList<Contact> getAllContacts() const;

    /**
     * @brief Retrieves a single contact by id.
     * @param id The primary key of the contact.
     * @return The Contact if found; an empty Contact (id = -1) otherwise.
     */
    Contact getContactById(int id) const;

    /**
     * @brief Checks whether the database connection is open.
     * @return true if connected, false otherwise.
     */
    bool isOpen() const;

private:
    /**
     * @brief Creates the contacts table if it does not exist.
     * @return true on success, false on failure.
     */
    bool createTable();

    QSqlDatabase m_database;       ///< Qt SQL database connection
    QString m_connectionName;      ///< Unique connection name
};

#endif // DATABASEMANAGER_H
