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
    if (m_database.isOpen())
        m_database.close();
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool DatabaseManager::init(const QString &dbPath)
{
    m_database = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
    m_database.setDatabaseName(dbPath);

    if (!m_database.open()) {
        qWarning() << "Failed to open database:" << m_database.lastError().text();
        return false;
    }

    // WAL mode gives better performance for concurrent reads
    QSqlQuery(QStringLiteral("PRAGMA journal_mode=WAL"), m_database);
    QSqlQuery(QStringLiteral("PRAGMA foreign_keys=ON"), m_database);

    return createTable();
}

bool DatabaseManager::createTable()
{
    QSqlQuery q(m_database);
    bool ok = q.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS contacts ("
        "    id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    name     TEXT NOT NULL,"
        "    mobile   TEXT NOT NULL,"
        "    email    TEXT NOT NULL,"
        "    birthday TEXT NOT NULL"
        ")"));

    if (!ok)
        qWarning() << "Failed to create contacts table:" << q.lastError().text();
    return ok;
}

int DatabaseManager::addContact(const Contact &contact)
{
    QSqlQuery q(m_database);
    q.prepare(QStringLiteral(
        "INSERT INTO contacts (name, mobile, email, birthday) "
        "VALUES (:name, :mobile, :email, :birthday)"));
    q.bindValue(QStringLiteral(":name"), contact.name());
    q.bindValue(QStringLiteral(":mobile"), contact.mobile());
    q.bindValue(QStringLiteral(":email"), contact.email());
    q.bindValue(QStringLiteral(":birthday"), contact.birthday().toString(Qt::ISODate));

    if (!q.exec()) {
        qWarning() << "addContact failed:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

bool DatabaseManager::updateContact(const Contact &contact)
{
    if (contact.id() < 0) {
        qWarning() << "Can't update contact with invalid id";
        return false;
    }

    QSqlQuery q(m_database);
    q.prepare(QStringLiteral(
        "UPDATE contacts SET name=:name, mobile=:mobile, "
        "email=:email, birthday=:birthday WHERE id=:id"));
    q.bindValue(QStringLiteral(":name"), contact.name());
    q.bindValue(QStringLiteral(":mobile"), contact.mobile());
    q.bindValue(QStringLiteral(":email"), contact.email());
    q.bindValue(QStringLiteral(":birthday"), contact.birthday().toString(Qt::ISODate));
    q.bindValue(QStringLiteral(":id"), contact.id());

    if (!q.exec()) {
        qWarning() << "updateContact failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool DatabaseManager::deleteContact(int id)
{
    if (id < 0) {
        qWarning() << "Can't delete contact with invalid id";
        return false;
    }

    QSqlQuery q(m_database);
    q.prepare(QStringLiteral("DELETE FROM contacts WHERE id = :id"));
    q.bindValue(QStringLiteral(":id"), id);

    if (!q.exec()) {
        qWarning() << "deleteContact failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

QList<Contact> DatabaseManager::getAllContacts() const
{
    QList<Contact> results;
    QSqlQuery q(m_database);
    q.prepare(QStringLiteral(
        "SELECT id, name, mobile, email, birthday FROM contacts "
        "ORDER BY name COLLATE NOCASE ASC"));

    if (!q.exec()) {
        qWarning() << "getAllContacts failed:" << q.lastError().text();
        return results;
    }

    while (q.next()) {
        results.append(Contact(
            q.value(0).toInt(),
            q.value(1).toString(),
            q.value(2).toString(),
            q.value(3).toString(),
            QDate::fromString(q.value(4).toString(), Qt::ISODate)));
    }
    return results;
}

Contact DatabaseManager::getContactById(int id) const
{
    QSqlQuery q(m_database);
    q.prepare(QStringLiteral(
        "SELECT id, name, mobile, email, birthday FROM contacts WHERE id = :id"));
    q.bindValue(QStringLiteral(":id"), id);

    if (!q.exec() || !q.next()) {
        qWarning() << "Contact not found:" << id;
        return Contact();
    }

    return Contact(
        q.value(0).toInt(),
        q.value(1).toString(),
        q.value(2).toString(),
        q.value(3).toString(),
        QDate::fromString(q.value(4).toString(), Qt::ISODate));
}

bool DatabaseManager::isOpen() const
{
    return m_database.isOpen();
}
