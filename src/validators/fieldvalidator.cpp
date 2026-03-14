#include "fieldvalidator.h"
#include <QRegularExpression>

ValidationResult FieldValidator::validateName(const QString &name)
{
    const QString trimmed = name.trimmed();

    if (trimmed.isEmpty())
        return {false, QStringLiteral("Name is required.")};

    if (trimmed.length() < 2)
        return {false, QStringLiteral("Name must be at least 2 characters.")};

    if (trimmed.length() > 100)
        return {false, QStringLiteral("Name must not exceed 100 characters.")};

    // Unicode-aware: allows letters, spaces, hyphens, apostrophes
    static const QRegularExpression re(QStringLiteral("^[\\p{L} '\\-]+$"));
    if (!re.match(trimmed).hasMatch())
        return {false, QStringLiteral("Name may only contain letters, spaces, hyphens, and apostrophes.")};

    return {true, {}};
}

ValidationResult FieldValidator::validateMobile(const QString &mobile)
{
    const QString trimmed = mobile.trimmed();

    if (trimmed.isEmpty())
        return {false, QStringLiteral("Mobile number is required.")};

    // optional + prefix, then digits/spaces/hyphens
    static const QRegularExpression re(QStringLiteral("^\\+?[\\d \\-]+$"));
    if (!re.match(trimmed).hasMatch())
        return {false, QStringLiteral("Mobile number may only contain digits, spaces, hyphens, and an optional leading '+'.")};

    // count actual digits
    int digits = 0;
    for (const QChar &ch : trimmed)
        if (ch.isDigit()) ++digits;

    if (digits < 7)
        return {false, QStringLiteral("Mobile number must contain at least 7 digits.")};
    if (digits > 15)
        return {false, QStringLiteral("Mobile number must not exceed 15 digits.")};

    return {true, {}};
}

ValidationResult FieldValidator::validateEmail(const QString &email)
{
    const QString trimmed = email.trimmed();

    if (trimmed.isEmpty())
        return {false, QStringLiteral("Email address is required.")};

    // simplified RFC 5322ish check
    static const QRegularExpression re(
        QStringLiteral("^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@"
                        "[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?"
                        "(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$"));
    if (!re.match(trimmed).hasMatch())
        return {false, QStringLiteral("Please enter a valid email address.")};

    // also make sure there's at least one dot in the domain part
    const int at = trimmed.indexOf(QLatin1Char('@'));
    if (!trimmed.mid(at + 1).contains(QLatin1Char('.')))
        return {false, QStringLiteral("Email domain must contain at least one dot.")};

    return {true, {}};
}

ValidationResult FieldValidator::validateBirthday(const QDate &birthday)
{
    if (!birthday.isValid())
        return {false, QStringLiteral("Please enter a valid date.")};

    if (birthday > QDate::currentDate())
        return {false, QStringLiteral("Birthday cannot be in the future.")};

    if (birthday.year() < 1900)
        return {false, QStringLiteral("Birthday year must be 1900 or later.")};

    return {true, {}};
}
