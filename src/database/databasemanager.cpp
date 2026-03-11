/**
 * @file databasemanager.cpp
 * @brief Implementation of SQLite database operations for contacts.
 */

#include "databasemanager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>
#include <QDebug>

DatabaseManager::DatabaseManager()
    : m_connectionName(QUuid::createUuid().toString())
{
}

DatabaseManager::~DatabaseManager()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool DatabaseManager::init(const QString &dbPath)
{
    m_database = QSqlDatabase::addDatabase(
        QStringLiteral("QSQLITE"), m_connectionName);
    m_database.setDatabaseName(dbPath);

    if (!m_database.open()) {
        qWarning() << "DatabaseManager: Failed to open database:"
                    << m_database.lastError().text();
        return false;
    }

    // Enable WAL mode for better concurrent access
    QSqlQuery walQuery(m_database);
    walQuery.exec(QStringLiteral("PRAGMA journal_mode=WAL"));

    // Enable foreign keys
    QSqlQuery fkQuery(m_database);
    fkQuery.exec(QStringLiteral("PRAGMA foreign_keys=ON"));

    return createTable();
}

bool DatabaseManager::createTable()
{
    QSqlQuery query(m_database);
    const QString sql = QStringLiteral(
        "CREATE TABLE IF NOT EXISTS contacts ("
        "    id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    name     TEXT    NOT NULL,"
        "    mobile   TEXT    NOT NULL,"
        "    email    TEXT    NOT NULL,"
        "    birthday TEXT    NOT NULL"
        ")");

    if (!query.exec(sql)) {
        qWarning() << "DatabaseManager: Failed to create table:"
                    << query.lastError().text();
        return false;
    }

    return true;
}

int DatabaseManager::addContact(const Contact &contact)
{
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "INSERT INTO contacts (name, mobile, email, birthday) "
        "VALUES (:name, :mobile, :email, :birthday)"));

    query.bindValue(QStringLiteral(":name"), contact.name());
    query.bindValue(QStringLiteral(":mobile"), contact.mobile());
    query.bindValue(QStringLiteral(":email"), contact.email());
    query.bindValue(QStringLiteral(":birthday"),
                    contact.birthday().toString(Qt::ISODate));

    if (!query.exec()) {
        qWarning() << "DatabaseManager: Failed to add contact:"
                    << query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool DatabaseManager::updateContact(const Contact &contact)
{
    if (contact.id() < 0) {
        qWarning() << "DatabaseManager: Cannot update contact with invalid id.";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "UPDATE contacts SET "
        "    name     = :name,"
        "    mobile   = :mobile,"
        "    email    = :email,"
        "    birthday = :birthday "
        "WHERE id = :id"));

    query.bindValue(QStringLiteral(":name"), contact.name());
    query.bindValue(QStringLiteral(":mobile"), contact.mobile());
    query.bindValue(QStringLiteral(":email"), contact.email());
    query.bindValue(QStringLiteral(":birthday"),
                    contact.birthday().toString(Qt::ISODate));
    query.bindValue(QStringLiteral(":id"), contact.id());

    if (!query.exec()) {
        qWarning() << "DatabaseManager: Failed to update contact:"
                    << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DatabaseManager::deleteContact(int id)
{
    if (id < 0) {
        qWarning() << "DatabaseManager: Cannot delete contact with invalid id.";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare(QStringLiteral("DELETE FROM contacts WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        qWarning() << "DatabaseManager: Failed to delete contact:"
                    << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

QList<Contact> DatabaseManager::getAllContacts() const
{
    QList<Contact> contacts;

    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT id, name, mobile, email, birthday "
        "FROM contacts ORDER BY name COLLATE NOCASE ASC"));

    if (!query.exec()) {
        qWarning() << "DatabaseManager: Failed to retrieve contacts:"
                    << query.lastError().text();
        return contacts;
    }

    while (query.next()) {
        Contact contact(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            QDate::fromString(query.value(4).toString(), Qt::ISODate));
        contacts.append(contact);
    }

    return contacts;
}

Contact DatabaseManager::getContactById(int id) const
{
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT id, name, mobile, email, birthday "
        "FROM contacts WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec() || !query.next()) {
        qWarning() << "DatabaseManager: Contact not found with id:" << id;
        return Contact();
    }

    return Contact(
        query.value(0).toInt(),
        query.value(1).toString(),
        query.value(2).toString(),
        query.value(3).toString(),
        QDate::fromString(query.value(4).toString(), Qt::ISODate));
}

bool DatabaseManager::isOpen() const
{
    return m_database.isOpen();
}
