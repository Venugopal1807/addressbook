/**
 * @file contact.h
 * @brief Contact data model representing an addressbook entry.
 *
 * Encapsulates all fields for a single contact: name, mobile number,
 * email address, and birthday. Provides value semantics and validation
 * delegation to FieldValidator.
 */

#ifndef CONTACT_H
#define CONTACT_H

#include <QDate>
#include <QString>

/**
 * @class Contact
 * @brief Represents a single addressbook contact.
 *
 * This class provides a lightweight, value-type representation of a contact
 * with getter/setter access and equality comparison. The id field is set by
 * the database layer upon persistence.
 */
class Contact
{
public:
    /** @brief Constructs an empty contact with id = -1. */
    Contact();

    /**
     * @brief Constructs a fully-initialized contact.
     * @param id       Database primary key (-1 if not yet persisted).
     * @param name     Full name of the contact.
     * @param mobile   Mobile phone number.
     * @param email    Email address.
     * @param birthday Date of birth.
     */
    Contact(int id, const QString &name, const QString &mobile,
            const QString &email, const QDate &birthday);

    // --- Getters ---
    int id() const;
    QString name() const;
    QString mobile() const;
    QString email() const;
    QDate birthday() const;

    // --- Setters ---
    void setId(int id);
    void setName(const QString &name);
    void setMobile(const QString &mobile);
    void setEmail(const QString &email);
    void setBirthday(const QDate &birthday);

    /**
     * @brief Checks whether all fields pass validation.
     * @return true if all fields are valid, false otherwise.
     */
    bool isValid() const;

    /** @brief Equality comparison based on all fields (excluding id). */
    bool operator==(const Contact &other) const;
    bool operator!=(const Contact &other) const;

private:
    int m_id;          ///< Database primary key, -1 if unsaved
    QString m_name;    ///< Full name
    QString m_mobile;  ///< Mobile phone number
    QString m_email;   ///< Email address
    QDate m_birthday;  ///< Date of birth
};

#endif // CONTACT_H
