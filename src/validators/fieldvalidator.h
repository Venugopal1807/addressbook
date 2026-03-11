/**
 * @file fieldvalidator.h
 * @brief Field-level input validation for contact data.
 *
 * Provides static validation methods for each contact field, returning
 * structured results with error messages suitable for UI display.
 */

#ifndef FIELDVALIDATOR_H
#define FIELDVALIDATOR_H

#include <QDate>
#include <QString>

/**
 * @struct ValidationResult
 * @brief Encapsulates the result of a field validation check.
 */
struct ValidationResult
{
    bool isValid;          ///< Whether the field value passed validation
    QString errorMessage;  ///< Human-readable error message (empty if valid)
};

/**
 * @class FieldValidator
 * @brief Static utility class for validating contact fields.
 *
 * Each validation method enforces type-appropriate rules:
 * - Name: non-empty, letters and spaces only, 2–100 characters
 * - Mobile: digits with optional leading +, 7–15 characters
 * - Email: RFC-style format validation via regex
 * - Birthday: must be a valid date, not in the future
 */
class FieldValidator
{
public:
    /**
     * @brief Validates a contact name.
     * @param name The name string to validate.
     * @return ValidationResult indicating pass/fail with error message.
     *
     * Rules:
     * - Must not be empty
     * - Must contain only letters, spaces, hyphens, and apostrophes
     * - Must be between 2 and 100 characters
     */
    static ValidationResult validateName(const QString &name);

    /**
     * @brief Validates a mobile phone number.
     * @param mobile The phone number string to validate.
     * @return ValidationResult indicating pass/fail with error message.
     *
     * Rules:
     * - Must not be empty
     * - May start with an optional '+'
     * - Remaining characters must be digits, spaces, or hyphens
     * - Must contain between 7 and 15 digit characters
     */
    static ValidationResult validateMobile(const QString &mobile);

    /**
     * @brief Validates an email address.
     * @param email The email string to validate.
     * @return ValidationResult indicating pass/fail with error message.
     *
     * Rules:
     * - Must not be empty
     * - Must match standard email format (user@domain.tld)
     */
    static ValidationResult validateEmail(const QString &email);

    /**
     * @brief Validates a birthday date.
     * @param birthday The date to validate.
     * @return ValidationResult indicating pass/fail with error message.
     *
     * Rules:
     * - Must be a valid QDate
     * - Must not be in the future
     * - Must not be before year 1900
     */
    static ValidationResult validateBirthday(const QDate &birthday);

private:
    FieldValidator() = delete;  ///< Prevent instantiation (static-only class)
};

#endif // FIELDVALIDATOR_H
