// contact.h - Contact data model for the addressbook

#ifndef CONTACT_H
#define CONTACT_H

#include <QDate>
#include <QString>

// Represents a single contact entry in the addressbook.
// Each contact has a name, mobile number, email, and birthday.
// The 'id' is assigned by the database; -1 means not yet saved.
class Contact
{
public:
    Contact();
    Contact(int id, const QString &name, const QString &mobile,
            const QString &email, const QDate &birthday);

    // Getters
    int id() const;
    QString name() const;
    QString mobile() const;
    QString email() const;
    QDate birthday() const;

    // Setters
    void setId(int id);
    void setName(const QString &name);
    void setMobile(const QString &mobile);
    void setEmail(const QString &email);
    void setBirthday(const QDate &birthday);

    // Runs all field validators, returns true only if everything checks out
    bool isValid() const;

    // Compare by data fields (ignores id since that's DB-specific)
    bool operator==(const Contact &other) const;
    bool operator!=(const Contact &other) const;

private:
    int m_id;
    QString m_name;
    QString m_mobile;
    QString m_email;
    QDate m_birthday;
};

#endif // CONTACT_H
