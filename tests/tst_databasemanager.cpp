#include <QtTest>
#include <QTemporaryFile>
#include "../src/database/databasemanager.h"

class TestDatabaseManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void addAndRetrieve();
    void updateExisting();
    void deleteById();
    void getAllSorted();
    void emptyDbReturnsNothing();
    void deleteNonexistent();
    void duplicateNamesAllowed();

private:
    DatabaseManager *m_db;
    QString m_tmpPath;
};

void TestDatabaseManager::initTestCase()
{
    // use a temp file so tests don't mess with real data
    QTemporaryFile tmp;
    tmp.setAutoRemove(false);
    tmp.open();
    m_tmpPath = tmp.fileName();
    tmp.close();

    m_db = new DatabaseManager();
    QVERIFY(m_db->init(m_tmpPath));
    QVERIFY(m_db->isOpen());
}

void TestDatabaseManager::cleanupTestCase()
{
    delete m_db;
    QFile::remove(m_tmpPath);
}

void TestDatabaseManager::addAndRetrieve()
{
    Contact c(-1, "Test User", "+1234567890", "test@mail.com", QDate(1990, 5, 10));
    int id = m_db->addContact(c);
    QVERIFY(id > 0);

    Contact fetched = m_db->getContactById(id);
    QCOMPARE(fetched.name(), QString("Test User"));
    QCOMPARE(fetched.email(), QString("test@mail.com"));
}

void TestDatabaseManager::updateExisting()
{
    Contact c(-1, "Before Update", "9999999999", "before@test.com", QDate(2000, 1, 1));
    int id = m_db->addContact(c);

    Contact updated(id, "After Update", "9999999999", "after@test.com", QDate(2000, 1, 1));
    QVERIFY(m_db->updateContact(updated));

    Contact fetched = m_db->getContactById(id);
    QCOMPARE(fetched.name(), QString("After Update"));
    QCOMPARE(fetched.email(), QString("after@test.com"));
}

void TestDatabaseManager::deleteById()
{
    Contact c(-1, "To Delete", "1111111111", "del@test.com", QDate(1985, 7, 20));
    int id = m_db->addContact(c);
    QVERIFY(id > 0);

    QVERIFY(m_db->deleteContact(id));

    // should be gone now
    Contact fetched = m_db->getContactById(id);
    QCOMPARE(fetched.id(), -1);
}

void TestDatabaseManager::getAllSorted()
{
    // clear everything first
    auto all = m_db->getAllContacts();
    for (const auto &c : all)
        m_db->deleteContact(c.id());

    m_db->addContact(Contact(-1, "Charlie", "1111111111", "c@t.com", QDate(2000, 1, 1)));
    m_db->addContact(Contact(-1, "Alice",   "2222222222", "a@t.com", QDate(2000, 1, 1)));
    m_db->addContact(Contact(-1, "Bob",     "3333333333", "b@t.com", QDate(2000, 1, 1)));

    auto sorted = m_db->getAllContacts();
    QCOMPARE(sorted.size(), 3);
    QCOMPARE(sorted[0].name(), QString("Alice"));
    QCOMPARE(sorted[1].name(), QString("Bob"));
    QCOMPARE(sorted[2].name(), QString("Charlie"));
}

void TestDatabaseManager::emptyDbReturnsNothing()
{
    // clear everything
    auto all = m_db->getAllContacts();
    for (const auto &c : all)
        m_db->deleteContact(c.id());

    QVERIFY(m_db->getAllContacts().isEmpty());
}

void TestDatabaseManager::deleteNonexistent()
{
    QVERIFY(!m_db->deleteContact(99999));
}

void TestDatabaseManager::duplicateNamesAllowed()
{
    int id1 = m_db->addContact(Contact(-1, "Same Name", "1234567890", "a@b.com", QDate(2000, 1, 1)));
    int id2 = m_db->addContact(Contact(-1, "Same Name", "0987654321", "c@d.com", QDate(2001, 2, 2)));
    QVERIFY(id1 > 0);
    QVERIFY(id2 > 0);
    QVERIFY(id1 != id2);
}

QTEST_APPLESS_MAIN(TestDatabaseManager)
#include "tst_databasemanager.moc"
