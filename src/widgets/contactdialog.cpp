/**
 * @file contactdialog.cpp
 * @brief Implementation of the Add/Edit contact dialog.
 */

#include "contactdialog.h"
#include "../validators/fieldvalidator.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCalendarWidget>

ContactDialog::ContactDialog(QWidget *parent)
    : QDialog(parent)
    , m_contactId(-1)
{
    setupUi();
    connectSignals();
    validateFields();  // Initial validation state
}

void ContactDialog::setupUi()
{
    setWindowTitle(QStringLiteral("Contact"));
    setMinimumWidth(450);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    // --- Form group ---
    auto *formGroup = new QGroupBox(QStringLiteral("Contact Details"));
    auto *formLayout = new QFormLayout(formGroup);
    formLayout->setSpacing(8);
    formLayout->setContentsMargins(16, 20, 16, 16);

    // Name field
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText(QStringLiteral("e.g. John Doe"));
    m_nameEdit->setMaxLength(100);
    m_nameError = new QLabel(this);
    m_nameError->setObjectName(QStringLiteral("errorLabel"));
    auto *nameLayout = new QVBoxLayout();
    nameLayout->setSpacing(2);
    nameLayout->addWidget(m_nameEdit);
    nameLayout->addWidget(m_nameError);
    formLayout->addRow(QStringLiteral("Name *"), nameLayout);

    // Mobile field
    m_mobileEdit = new QLineEdit(this);
    m_mobileEdit->setPlaceholderText(QStringLiteral("e.g. +91 98765 43210"));
    m_mobileEdit->setMaxLength(20);
    m_mobileError = new QLabel(this);
    m_mobileError->setObjectName(QStringLiteral("errorLabel"));
    auto *mobileLayout = new QVBoxLayout();
    mobileLayout->setSpacing(2);
    mobileLayout->addWidget(m_mobileEdit);
    mobileLayout->addWidget(m_mobileError);
    formLayout->addRow(QStringLiteral("Mobile *"), mobileLayout);

    // Email field
    m_emailEdit = new QLineEdit(this);
    m_emailEdit->setPlaceholderText(QStringLiteral("e.g. john@example.com"));
    m_emailEdit->setMaxLength(254);
    m_emailError = new QLabel(this);
    m_emailError->setObjectName(QStringLiteral("errorLabel"));
    auto *emailLayout = new QVBoxLayout();
    emailLayout->setSpacing(2);
    emailLayout->addWidget(m_emailEdit);
    emailLayout->addWidget(m_emailError);
    formLayout->addRow(QStringLiteral("Email *"), emailLayout);

    // Birthday field
    m_birthdayEdit = new QDateEdit(this);
    m_birthdayEdit->setDisplayFormat(QStringLiteral("dd MMM yyyy"));
    m_birthdayEdit->setCalendarPopup(true);
    m_birthdayEdit->setDate(QDate(2000, 1, 1));
    m_birthdayEdit->setMaximumDate(QDate::currentDate());
    m_birthdayEdit->setMinimumDate(QDate(1900, 1, 1));

    // Style the calendar popup
    QCalendarWidget *calendar = m_birthdayEdit->calendarWidget();
    if (calendar) {
        calendar->setGridVisible(true);
        calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    }

    m_birthdayError = new QLabel(this);
    m_birthdayError->setObjectName(QStringLiteral("errorLabel"));
    auto *birthdayLayout = new QVBoxLayout();
    birthdayLayout->setSpacing(2);
    birthdayLayout->addWidget(m_birthdayEdit);
    birthdayLayout->addWidget(m_birthdayError);
    formLayout->addRow(QStringLiteral("Birthday *"), birthdayLayout);

    mainLayout->addWidget(formGroup);

    // --- Button row ---
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);
    buttonLayout->addStretch();

    m_cancelButton = new QPushButton(QStringLiteral("Cancel"), this);
    m_cancelButton->setObjectName(QStringLiteral("cancelButton"));
    buttonLayout->addWidget(m_cancelButton);

    m_okButton = new QPushButton(QStringLiteral("Save"), this);
    m_okButton->setObjectName(QStringLiteral("saveButton"));
    m_okButton->setDefault(true);
    buttonLayout->addWidget(m_okButton);

    mainLayout->addLayout(buttonLayout);
}

void ContactDialog::connectSignals()
{
    connect(m_nameEdit, &QLineEdit::textChanged,
            this, &ContactDialog::validateFields);
    connect(m_mobileEdit, &QLineEdit::textChanged,
            this, &ContactDialog::validateFields);
    connect(m_emailEdit, &QLineEdit::textChanged,
            this, &ContactDialog::validateFields);
    connect(m_birthdayEdit, &QDateEdit::dateChanged,
            this, &ContactDialog::validateFields);

    connect(m_okButton, &QPushButton::clicked,
            this, &QDialog::accept);
    connect(m_cancelButton, &QPushButton::clicked,
            this, &QDialog::reject);
}

void ContactDialog::setContact(const Contact &contact)
{
    m_contactId = contact.id();
    m_nameEdit->setText(contact.name());
    m_mobileEdit->setText(contact.mobile());
    m_emailEdit->setText(contact.email());
    m_birthdayEdit->setDate(contact.birthday());
    setWindowTitle(QStringLiteral("Edit Contact"));
}

Contact ContactDialog::getContact() const
{
    return Contact(
        m_contactId,
        m_nameEdit->text().trimmed(),
        m_mobileEdit->text().trimmed(),
        m_emailEdit->text().trimmed(),
        m_birthdayEdit->date());
}

void ContactDialog::validateFields()
{
    auto nameResult   = FieldValidator::validateName(m_nameEdit->text());
    auto mobileResult = FieldValidator::validateMobile(m_mobileEdit->text());
    auto emailResult  = FieldValidator::validateEmail(m_emailEdit->text());
    auto bdayResult   = FieldValidator::validateBirthday(m_birthdayEdit->date());

    setFieldValidation(m_nameEdit,   m_nameError,
                       nameResult.isValid, nameResult.errorMessage);
    setFieldValidation(m_mobileEdit, m_mobileError,
                       mobileResult.isValid, mobileResult.errorMessage);
    setFieldValidation(m_emailEdit,  m_emailError,
                       emailResult.isValid, emailResult.errorMessage);
    setFieldValidation(m_birthdayEdit, m_birthdayError,
                       bdayResult.isValid, bdayResult.errorMessage);

    const bool allValid = nameResult.isValid
                       && mobileResult.isValid
                       && emailResult.isValid
                       && bdayResult.isValid;

    m_okButton->setEnabled(allValid);
}

void ContactDialog::setFieldValidation(QWidget *field, QLabel *label,
                                       bool valid, const QString &error)
{
    if (valid) {
        field->setStyleSheet(QString());
        label->clear();
        label->hide();
    } else {
        field->setStyleSheet(
            QStringLiteral("border: 1px solid #e74c3c; border-radius: 4px;"));
        label->setText(error);
        label->show();
    }
}
