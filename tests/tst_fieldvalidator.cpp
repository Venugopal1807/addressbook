#include <QtTest>
#include "../src/validators/fieldvalidator.h"

class TestFieldValidator : public QObject
{
    Q_OBJECT

private slots:
    // name validation
    void validNames();
    void nameEmptyRejects();
    void nameTooShort();
    void nameWithNumbers();

    // mobile validation
    void validMobiles();
    void mobileTooFewDigits();
    void mobileWithLetters();

    // email validation
    void validEmails();
    void emailMissingAt();
    void emailNoDotInDomain();

    // birthday validation
    void validBirthday();
    void futureDateRejects();
    void tooOldRejects();
};

// --- Name ---

void TestFieldValidator::validNames()
{
    QVERIFY(FieldValidator::validateName("Alice").isValid);
    QVERIFY(FieldValidator::validateName("Mary Jane").isValid);
    QVERIFY(FieldValidator::validateName("O'Brien").isValid);
    QVERIFY(FieldValidator::validateName("Lee-Ann").isValid);
}

void TestFieldValidator::nameEmptyRejects()
{
    QVERIFY(!FieldValidator::validateName("").isValid);
    QVERIFY(!FieldValidator::validateName("   ").isValid);
}

void TestFieldValidator::nameTooShort()
{
    QVERIFY(!FieldValidator::validateName("A").isValid);
}

void TestFieldValidator::nameWithNumbers()
{
    QVERIFY(!FieldValidator::validateName("User123").isValid);
}

// --- Mobile ---

void TestFieldValidator::validMobiles()
{
    QVERIFY(FieldValidator::validateMobile("+1234567890").isValid);
    QVERIFY(FieldValidator::validateMobile("98765 43210").isValid);
    QVERIFY(FieldValidator::validateMobile("+91-98765-43210").isValid);
}

void TestFieldValidator::mobileTooFewDigits()
{
    QVERIFY(!FieldValidator::validateMobile("12345").isValid);
}

void TestFieldValidator::mobileWithLetters()
{
    QVERIFY(!FieldValidator::validateMobile("abc1234567").isValid);
}

// --- Email ---

void TestFieldValidator::validEmails()
{
    QVERIFY(FieldValidator::validateEmail("user@example.com").isValid);
    QVERIFY(FieldValidator::validateEmail("first.last@company.co.in").isValid);
}

void TestFieldValidator::emailMissingAt()
{
    QVERIFY(!FieldValidator::validateEmail("userexample.com").isValid);
}

void TestFieldValidator::emailNoDotInDomain()
{
    QVERIFY(!FieldValidator::validateEmail("user@localhost").isValid);
}

// --- Birthday ---

void TestFieldValidator::validBirthday()
{
    QVERIFY(FieldValidator::validateBirthday(QDate(1995, 6, 15)).isValid);
    QVERIFY(FieldValidator::validateBirthday(QDate::currentDate()).isValid);
}

void TestFieldValidator::futureDateRejects()
{
    QVERIFY(!FieldValidator::validateBirthday(QDate::currentDate().addDays(1)).isValid);
}

void TestFieldValidator::tooOldRejects()
{
    QVERIFY(!FieldValidator::validateBirthday(QDate(1899, 12, 31)).isValid);
}

QTEST_APPLESS_MAIN(TestFieldValidator)
#include "tst_fieldvalidator.moc"
