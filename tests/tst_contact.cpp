#include <QtTest>
#include "../src/models/contact.h"

class TestContact : public QObject
{
    Q_OBJECT

private slots:
    void defaultConstructor();
    void parameterizedConstructor();
    void settersWork();
    void equalityIgnoresId();
    void validContactPasses();
    void emptyFieldsFail();
    void invalidEmailFails();
};

void TestContact::defaultConstructor()
{
    Contact c;
    QCOMPARE(c.id(), -1);
    QVERIFY(c.name().isEmpty());
    QVERIFY(c.mobile().isEmpty());
    QVERIFY(c.email().isEmpty());
    QVERIFY(!c.birthday().isValid());
}

void TestContact::parameterizedConstructor()
{
    QDate bday(1995, 6, 15);
    Contact c(42, "Alice", "+1234567890", "alice@mail.com", bday);

    QCOMPARE(c.id(), 42);
    QCOMPARE(c.name(), QString("Alice"));
    QCOMPARE(c.mobile(), QString("+1234567890"));
    QCOMPARE(c.email(), QString("alice@mail.com"));
    QCOMPARE(c.birthday(), bday);
}

void TestContact::settersWork()
{
    Contact c;
    c.setId(7);
    c.setName("Bob");
    c.setMobile("9876543210");
    c.setEmail("bob@test.com");
    c.setBirthday(QDate(1990, 3, 1));

    QCOMPARE(c.id(), 7);
    QCOMPARE(c.name(), QString("Bob"));
    QCOMPARE(c.email(), QString("bob@test.com"));
}

void TestContact::equalityIgnoresId()
{
    Contact a(1, "Same", "1234567890", "same@mail.com", QDate(2000, 1, 1));
    Contact b(2, "Same", "1234567890", "same@mail.com", QDate(2000, 1, 1));
    QVERIFY(a == b); // ids differ but data matches
}

void TestContact::validContactPasses()
{
    Contact c(1, "Valid Person", "+91 99887 76655", "valid@example.com", QDate(1990, 5, 20));
    QVERIFY(c.isValid());
}

void TestContact::emptyFieldsFail()
{
    Contact c; // everything empty
    QVERIFY(!c.isValid());
}

void TestContact::invalidEmailFails()
{
    Contact c(1, "Test User", "1234567890", "not-an-email", QDate(1990, 1, 1));
    QVERIFY(!c.isValid());
}

QTEST_APPLESS_MAIN(TestContact)
#include "tst_contact.moc"
