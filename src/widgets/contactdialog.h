// contactdialog.h - form dialog for adding / editing a contact

#ifndef CONTACTDIALOG_H
#define CONTACTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QLabel>
#include <QPushButton>
#include "../models/contact.h"

// Modal dialog that shows a form with Name, Mobile, Email, Birthday.
// Has real-time validation - fields get a red border when invalid,
// and the Save button stays disabled until everything passes.
class ContactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContactDialog(QWidget *parent = nullptr);

    // Pre-fill the form for editing an existing contact
    void setContact(const Contact &contact);

    // Build a Contact from the current field values
    Contact getContact() const;

private slots:
    void validateFields();

private:
    void setupUi();
    void connectSignals();
    void setFieldValidation(QWidget *field, QLabel *label,
                            bool valid, const QString &error);

    QLineEdit   *m_nameEdit;
    QLineEdit   *m_mobileEdit;
    QLineEdit   *m_emailEdit;
    QDateEdit   *m_birthdayEdit;

    QLabel      *m_nameError;
    QLabel      *m_mobileError;
    QLabel      *m_emailError;
    QLabel      *m_birthdayError;

    QPushButton *m_okButton;
    QPushButton *m_cancelButton;

    int m_contactId;
};

#endif // CONTACTDIALOG_H
