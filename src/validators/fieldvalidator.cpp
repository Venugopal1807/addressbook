/**
 * @file fieldvalidator.cpp
 * @brief Implementation of field-level validation rules for contact data.
 */

#include "fieldvalidator.h"

#include <QRegularExpression>

ValidationResult FieldValidator::validateName(const QString &name)
{
    const QString trimmed = name.trimmed();

    if (trimmed.isEmpty()) {
        return {false, QStringLiteral("Name is required.")};
    }

    if (trimmed.length() < 2) {
        return {false, QStringLiteral("Name must be at least 2 characters.")};
    }

    if (trimmed.length() > 100) {
        return {false, QStringLiteral("Name must not exceed 100 characters.")};
    }

    // Allow letters (Unicode-aware), spaces, hyphens, apostrophes
    static const QRegularExpression nameRegex(
        QStringLiteral("^[\\p{L} '\\-]+$"));

    if (!nameRegex.match(trimmed).hasMatch()) {
        return {false, QStringLiteral("Name may only contain letters, spaces, "
                                      "hyphens, and apostrophes.")};
    }

    return {true, QString()};
}

ValidationResult FieldValidator::validateMobile(const QString &mobile)
{
    const QString trimmed = mobile.trimmed();

    if (trimmed.isEmpty()) {
        return {false, QStringLiteral("Mobile number is required.")};
    }

    // Allow optional leading +, then digits, spaces, and hyphens
    static const QRegularExpression mobileRegex(
        QStringLiteral("^\\+?[\\d \\-]+$"));

    if (!mobileRegex.match(trimmed).hasMatch()) {
        return {false, QStringLiteral("Mobile number may only contain digits, "
                                      "spaces, hyphens, and an optional leading '+'.")};
    }

    // Count actual digits (must be 7–15)
    int digitCount = 0;
    for (const QChar &ch : trimmed) {
        if (ch.isDigit()) {
            ++digitCount;
        }
    }

    if (digitCount < 7) {
        return {false, QStringLiteral("Mobile number must contain at least 7 digits.")};
    }

    if (digitCount > 15) {
        return {false, QStringLiteral("Mobile number must not exceed 15 digits.")};
    }

    return {true, QString()};
}

ValidationResult FieldValidator::validateEmail(const QString &email)
{
    const QString trimmed = email.trimmed();

    if (trimmed.isEmpty()) {
        return {false, QStringLiteral("Email address is required.")};
    }

    // RFC 5322 simplified email regex
    static const QRegularExpression emailRegex(
        QStringLiteral("^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@"
                        "[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?"
                        "(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$"));

    if (!emailRegex.match(trimmed).hasMatch()) {
        return {false, QStringLiteral("Please enter a valid email address.")};
    }

    // Must contain at least one dot after @
    const int atIndex = trimmed.indexOf(QLatin1Char('@'));
    const QString domain = trimmed.mid(atIndex + 1);
    if (!domain.contains(QLatin1Char('.'))) {
        return {false, QStringLiteral("Email domain must contain at least one dot.")};
    }

    return {true, QString()};
}

ValidationResult FieldValidator::validateBirthday(const QDate &birthday)
{
    if (!birthday.isValid()) {
        return {false, QStringLiteral("Please enter a valid date.")};
    }

    if (birthday > QDate::currentDate()) {
        return {false, QStringLiteral("Birthday cannot be in the future.")};
    }

    if (birthday.year() < 1900) {
        return {false, QStringLiteral("Birthday year must be 1900 or later.")};
    }

    return {true, QString()};
}
