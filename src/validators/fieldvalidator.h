// fieldvalidator.h - Input validation for contact fields

#ifndef FIELDVALIDATOR_H
#define FIELDVALIDATOR_H

#include <QDate>
#include <QString>

// Simple result struct so callers can show error messages in the UI
struct ValidationResult
{
    bool isValid;
    QString errorMessage;
};

// Static helper class for validating individual contact fields.
// Keeps the validation logic in one place so both the model and
// the dialog can use the same rules.
class FieldValidator
{
public:
    // Name: letters/spaces/hyphens/apostrophes, 2-100 chars
    static ValidationResult validateName(const QString &name);

    // Mobile: optional leading +, then digits/spaces/hyphens, 7-15 digit chars
    static ValidationResult validateMobile(const QString &mobile);

    // Email: basic RFC-style check (user@domain.tld)
    static ValidationResult validateEmail(const QString &email);

    // Birthday: must be valid date, not in future, year >= 1900
    static ValidationResult validateBirthday(const QDate &birthday);

private:
    FieldValidator() = delete; // all static, no instances
};

#endif // FIELDVALIDATOR_H
