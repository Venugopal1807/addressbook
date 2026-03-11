/**
 * @file tst_contact.cpp
 * @brief Unit tests for the Contact data model.
 *
 * Tests construction, getters/setters, equality operators,
 * and validation delegation.
 */

#include <QtTest/QtTest>
#include "../src/models/contact.h"

class TestContact : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Tests default construction produces an empty, invalid contact.
     */
    void testDefaultConstructor()
    {
        Contact contact;
        QCOMPARE(contact.id(), -1);
        QVERIFY(contact.name().isEmpty());
        QVERIFY(contact.mobile().isEmpty());
        QVERIFY(contact.email().isEmpty());
        QVERIFY(!contact.birthday().isValid());
    }

    /**
     * @brief Tests parameterized construction with all fields.
     */
    void testParameterizedConstructor()
    {
        Contact contact(1, "John Doe", "+91 98765 43210",
                         "john@example.com", QDate(1990, 5, 15));

        QCOMPARE(contact.id(), 1);
        QCOMPARE(contact.name(), QString("John Doe"));
        QCOMPARE(contact.mobile(), QString("+91 98765 43210"));
        QCOMPARE(contact.email(), QString("john@example.com"));
        QCOMPARE(contact.birthday(), QDate(1990, 5, 15));
    }

    /**
     * @brief Tests setter methods.
     */
    void testSetters()
    {
        Contact contact;

        contact.setId(42);
        QCOMPARE(contact.id(), 42);

        contact.setName("Jane Smith");
        QCOMPARE(contact.name(), QString("Jane Smith"));

        contact.setMobile("1234567890");
        QCOMPARE(contact.mobile(), QString("1234567890"));

        contact.setEmail("jane@test.com");
        QCOMPARE(contact.email(), QString("jane@test.com"));

        contact.setBirthday(QDate(1985, 12, 25));
        QCOMPARE(contact.birthday(), QDate(1985, 12, 25));
    }

    /**
     * @brief Tests equality operator (compares fields, ignores id).
     */
    void testEquality()
    {
        Contact a(1, "John", "1234567", "j@e.com", QDate(1990, 1, 1));
        Contact b(2, "John", "1234567", "j@e.com", QDate(1990, 1, 1));

        // Same data, different ids — should be equal
        QVERIFY(a == b);
        QVERIFY(!(a != b));
    }

    /**
     * @brief Tests inequality when fields differ.
     */
    void testInequality()
    {
        Contact a(1, "John", "1234567", "j@e.com", QDate(1990, 1, 1));
        Contact b(1, "Jane", "1234567", "j@e.com", QDate(1990, 1, 1));

        QVERIFY(a != b);
    }

    /**
     * @brief Tests isValid() with a fully valid contact.
     */
    void testIsValidWithValidContact()
    {
        Contact contact(1, "John Doe", "+91 9876543210",
                         "john@example.com", QDate(1990, 5, 15));
        QVERIFY(contact.isValid());
    }

    /**
     * @brief Tests isValid() with an empty (invalid) contact.
     */
    void testIsValidWithEmptyContact()
    {
        Contact contact;
        QVERIFY(!contact.isValid());
    }

    /**
     * @brief Tests isValid() with an invalid email.
     */
    void testIsValidWithBadEmail()
    {
        Contact contact(1, "John Doe", "1234567890",
                         "not-an-email", QDate(1990, 5, 15));
        QVERIFY(!contact.isValid());
    }
};

QTEST_APPLESS_MAIN(TestContact)
#include "tst_contact.moc"
