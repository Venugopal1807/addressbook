/**
 * @file contact.cpp
 * @brief Implementation of the Contact data model.
 */

#include "contact.h"
#include "../validators/fieldvalidator.h"

Contact::Contact()
    : m_id(-1)
    , m_name()
    , m_mobile()
    , m_email()
    , m_birthday()
{
}

Contact::Contact(int id, const QString &name, const QString &mobile,
                 const QString &email, const QDate &birthday)
    : m_id(id)
    , m_name(name)
    , m_mobile(mobile)
    , m_email(email)
    , m_birthday(birthday)
{
}

// --- Getters ---

int Contact::id() const { return m_id; }
QString Contact::name() const { return m_name; }
QString Contact::mobile() const { return m_mobile; }
QString Contact::email() const { return m_email; }
QDate Contact::birthday() const { return m_birthday; }

// --- Setters ---

void Contact::setId(int id) { m_id = id; }
void Contact::setName(const QString &name) { m_name = name; }
void Contact::setMobile(const QString &mobile) { m_mobile = mobile; }
void Contact::setEmail(const QString &email) { m_email = email; }
void Contact::setBirthday(const QDate &birthday) { m_birthday = birthday; }

bool Contact::isValid() const
{
    return FieldValidator::validateName(m_name).isValid
        && FieldValidator::validateMobile(m_mobile).isValid
        && FieldValidator::validateEmail(m_email).isValid
        && FieldValidator::validateBirthday(m_birthday).isValid;
}

bool Contact::operator==(const Contact &other) const
{
    return m_name == other.m_name
        && m_mobile == other.m_mobile
        && m_email == other.m_email
        && m_birthday == other.m_birthday;
}

bool Contact::operator!=(const Contact &other) const
{
    return !(*this == other);
}
