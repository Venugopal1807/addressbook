/**
 * @file tst_fieldvalidator.cpp
 * @brief Unit tests for the FieldValidator class.
 *
 * Exercises all validation rules with both valid and invalid inputs
 * for name, mobile, email, and birthday fields.
 */

#include <QtTest/QtTest>
#include "../src/validators/fieldvalidator.h"

class TestFieldValidator : public QObject
{
    Q_OBJECT

private slots:
    // ---- Name validation ----

    void testNameValid()
    {
        QVERIFY(FieldValidator::validateName("John Doe").isValid);
        QVERIFY(FieldValidator::validateName("Anne-Marie O'Brien").isValid);
        QVERIFY(FieldValidator::validateName("Li").isValid);
    }

    void testNameEmpty()
    {
        auto result = FieldValidator::validateName("");
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("required"));
    }

    void testNameTooShort()
    {
        auto result = FieldValidator::validateName("A");
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("2 characters"));
    }

    void testNameWithNumbers()
    {
        auto result = FieldValidator::validateName("John123");
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("letters"));
    }

    void testNameTooLong()
    {
        QString longName(101, 'A');
        auto result = FieldValidator::validateName(longName);
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("100"));
    }

    // ---- Mobile validation ----

    void testMobileValid()
    {
        QVERIFY(FieldValidator::validateMobile("+91 98765 43210").isValid);
        QVERIFY(FieldValidator::validateMobile("1234567890").isValid);
        QVERIFY(FieldValidator::validateMobile("+1-555-123-4567").isValid);
        QVERIFY(FieldValidator::validateMobile("9876543").isValid);
    }

    void testMobileEmpty()
    {
        auto result = FieldValidator::validateMobile("");
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("required"));
    }

    void testMobileTooFewDigits()
    {
        auto result = FieldValidator::validateMobile("123456");
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("7 digits"));
    }

    void testMobileTooManyDigits()
    {
        auto result = FieldValidator::validateMobile("1234567890123456");
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("15 digits"));
    }

    void testMobileWithLetters()
    {
        auto result = FieldValidator::validateMobile("12345abc");
        QVERIFY(!result.isValid);
    }

    // ---- Email validation ----

    void testEmailValid()
    {
        QVERIFY(FieldValidator::validateEmail("user@example.com").isValid);
        QVERIFY(FieldValidator::validateEmail("first.last@domain.co.uk").isValid);
        QVERIFY(FieldValidator::validateEmail("user+tag@gmail.com").isValid);
    }

    void testEmailEmpty()
    {
        auto result = FieldValidator::validateEmail("");
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("required"));
    }

    void testEmailNoAtSign()
    {
        auto result = FieldValidator::validateEmail("userexample.com");
        QVERIFY(!result.isValid);
    }

    void testEmailNoDomain()
    {
        auto result = FieldValidator::validateEmail("user@");
        QVERIFY(!result.isValid);
    }

    void testEmailNoDot()
    {
        auto result = FieldValidator::validateEmail("user@localhost");
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("dot"));
    }

    // ---- Birthday validation ----

    void testBirthdayValid()
    {
        QVERIFY(FieldValidator::validateBirthday(QDate(1990, 5, 15)).isValid);
        QVERIFY(FieldValidator::validateBirthday(QDate(1900, 1, 1)).isValid);
        QVERIFY(FieldValidator::validateBirthday(QDate::currentDate()).isValid);
    }

    void testBirthdayInvalid()
    {
        auto result = FieldValidator::validateBirthday(QDate());
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("valid date"));
    }

    void testBirthdayInFuture()
    {
        auto result = FieldValidator::validateBirthday(
            QDate::currentDate().addDays(1));
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("future"));
    }

    void testBirthdayTooOld()
    {
        auto result = FieldValidator::validateBirthday(QDate(1899, 12, 31));
        QVERIFY(!result.isValid);
        QVERIFY(result.errorMessage.contains("1900"));
    }
};

QTEST_APPLESS_MAIN(TestFieldValidator)
#include "tst_fieldvalidator.moc"
