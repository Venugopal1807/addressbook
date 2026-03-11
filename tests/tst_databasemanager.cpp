/**
 * @file tst_databasemanager.cpp
 * @brief Unit tests for the DatabaseManager class.
 *
 * Tests CRUD operations using an in-memory SQLite database to ensure
 * isolation between test runs and fast execution.
 */

#include <QtTest/QtTest>
#include "../src/database/databasemanager.h"

class TestDatabaseManager : public QObject
{
    Q_OBJECT

private:
    DatabaseManager *m_db;

private slots:
    /**
     * @brief Per-test setup: create a fresh in-memory database.
     */
    void init()
    {
        m_db = new DatabaseManager();
        QVERIFY(m_db->init(":memory:"));
        QVERIFY(m_db->isOpen());
    }

    /**
     * @brief Per-test cleanup.
     */
    void cleanup()
    {
        delete m_db;
        m_db = nullptr;
    }

    /**
     * @brief Tests adding a contact and retrieving it.
     */
    void testAddAndGetContact()
    {
        Contact contact(-1, "John Doe", "+91 9876543210",
                         "john@example.com", QDate(1990, 5, 15));

        int id = m_db->addContact(contact);
        QVERIFY(id > 0);

        Contact retrieved = m_db->getContactById(id);
        QCOMPARE(retrieved.id(), id);
        QCOMPARE(retrieved.name(), QString("John Doe"));
        QCOMPARE(retrieved.mobile(), QString("+91 9876543210"));
        QCOMPARE(retrieved.email(), QString("john@example.com"));
        QCOMPARE(retrieved.birthday(), QDate(1990, 5, 15));
    }

    /**
     * @brief Tests retrieving all contacts.
     */
    void testGetAllContacts()
    {
        m_db->addContact(Contact(-1, "Charlie", "1111111",
                                  "c@e.com", QDate(1990, 1, 1)));
        m_db->addContact(Contact(-1, "Alice", "2222222",
                                  "a@e.com", QDate(1991, 2, 2)));
        m_db->addContact(Contact(-1, "Bob", "3333333",
                                  "b@e.com", QDate(1992, 3, 3)));

        QList<Contact> contacts = m_db->getAllContacts();
        QCOMPARE(contacts.count(), 3);

        // Should be sorted by name (case-insensitive)
        QCOMPARE(contacts[0].name(), QString("Alice"));
        QCOMPARE(contacts[1].name(), QString("Bob"));
        QCOMPARE(contacts[2].name(), QString("Charlie"));
    }

    /**
     * @brief Tests updating a contact.
     */
    void testUpdateContact()
    {
        Contact contact(-1, "John Doe", "1234567",
                         "j@e.com", QDate(1990, 1, 1));
        int id = m_db->addContact(contact);

        Contact updated(id, "Jane Doe", "7654321",
                         "jane@e.com", QDate(1985, 6, 15));
        QVERIFY(m_db->updateContact(updated));

        Contact retrieved = m_db->getContactById(id);
        QCOMPARE(retrieved.name(), QString("Jane Doe"));
        QCOMPARE(retrieved.mobile(), QString("7654321"));
        QCOMPARE(retrieved.email(), QString("jane@e.com"));
        QCOMPARE(retrieved.birthday(), QDate(1985, 6, 15));
    }

    /**
     * @brief Tests deleting a contact.
     */
    void testDeleteContact()
    {
        Contact contact(-1, "John Doe", "1234567",
                         "j@e.com", QDate(1990, 1, 1));
        int id = m_db->addContact(contact);
        QVERIFY(id > 0);

        QVERIFY(m_db->deleteContact(id));

        Contact retrieved = m_db->getContactById(id);
        QCOMPARE(retrieved.id(), -1);  // Not found
    }

    /**
     * @brief Tests delete with invalid id returns false.
     */
    void testDeleteInvalidId()
    {
        QVERIFY(!m_db->deleteContact(-1));
        QVERIFY(!m_db->deleteContact(9999));
    }

    /**
     * @brief Tests update with invalid id returns false.
     */
    void testUpdateInvalidId()
    {
        Contact contact(-1, "John", "1234567", "j@e.com", QDate(1990, 1, 1));
        QVERIFY(!m_db->updateContact(contact));
    }

    /**
     * @brief Tests that an empty database returns an empty list.
     */
    void testGetAllContactsEmpty()
    {
        QList<Contact> contacts = m_db->getAllContacts();
        QVERIFY(contacts.isEmpty());
    }

    /**
     * @brief Tests retrieving a non-existent contact.
     */
    void testGetContactByIdNotFound()
    {
        Contact contact = m_db->getContactById(9999);
        QCOMPARE(contact.id(), -1);
    }
};

QTEST_APPLESS_MAIN(TestDatabaseManager)
#include "tst_databasemanager.moc"
