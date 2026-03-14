#include "contactdialog.h"
#include "../validators/fieldvalidator.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCalendarWidget>

ContactDialog::ContactDialog(QWidget *parent)
    : QDialog(parent), m_contactId(-1)
{
    setupUi();
    connectSignals();
    validateFields();
}

void ContactDialog::setupUi()
{
    setWindowTitle("Contact");
    setMinimumWidth(450);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    // group box for the form fields
    auto *formGroup = new QGroupBox("Contact Details");
    auto *form = new QFormLayout(formGroup);
    form->setSpacing(8);
    form->setContentsMargins(16, 20, 16, 16);

    // Name
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("e.g. John Doe");
    m_nameEdit->setMaxLength(100);
    m_nameError = new QLabel(this);
    m_nameError->setObjectName("errorLabel");
    auto *nameBox = new QVBoxLayout();
    nameBox->setSpacing(2);
    nameBox->addWidget(m_nameEdit);
    nameBox->addWidget(m_nameError);
    form->addRow("Name *", nameBox);

    // Mobile
    m_mobileEdit = new QLineEdit(this);
    m_mobileEdit->setPlaceholderText("e.g. +91 98765 43210");
    m_mobileEdit->setMaxLength(20);
    m_mobileError = new QLabel(this);
    m_mobileError->setObjectName("errorLabel");
    auto *mobileBox = new QVBoxLayout();
    mobileBox->setSpacing(2);
    mobileBox->addWidget(m_mobileEdit);
    mobileBox->addWidget(m_mobileError);
    form->addRow("Mobile *", mobileBox);

    // Email
    m_emailEdit = new QLineEdit(this);
    m_emailEdit->setPlaceholderText("e.g. john@example.com");
    m_emailEdit->setMaxLength(254);
    m_emailError = new QLabel(this);
    m_emailError->setObjectName("errorLabel");
    auto *emailBox = new QVBoxLayout();
    emailBox->setSpacing(2);
    emailBox->addWidget(m_emailEdit);
    emailBox->addWidget(m_emailError);
    form->addRow("Email *", emailBox);

    // Birthday
    m_birthdayEdit = new QDateEdit(this);
    m_birthdayEdit->setDisplayFormat("dd MMM yyyy");
    m_birthdayEdit->setCalendarPopup(true);
    m_birthdayEdit->setDate(QDate(2000, 1, 1));
    m_birthdayEdit->setMaximumDate(QDate::currentDate());
    m_birthdayEdit->setMinimumDate(QDate(1900, 1, 1));
    if (auto *cal = m_birthdayEdit->calendarWidget()) {
        cal->setGridVisible(true);
        cal->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    }
    m_birthdayError = new QLabel(this);
    m_birthdayError->setObjectName("errorLabel");
    auto *bdayBox = new QVBoxLayout();
    bdayBox->setSpacing(2);
    bdayBox->addWidget(m_birthdayEdit);
    bdayBox->addWidget(m_birthdayError);
    form->addRow("Birthday *", bdayBox);

    mainLayout->addWidget(formGroup);

    // buttons
    auto *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);
    btnLayout->addStretch();

    m_cancelButton = new QPushButton("Cancel", this);
    m_cancelButton->setObjectName("cancelButton");
    btnLayout->addWidget(m_cancelButton);

    m_okButton = new QPushButton("Save", this);
    m_okButton->setObjectName("saveButton");
    m_okButton->setDefault(true);
    btnLayout->addWidget(m_okButton);

    mainLayout->addLayout(btnLayout);
}

void ContactDialog::connectSignals()
{
    connect(m_nameEdit, &QLineEdit::textChanged, this, &ContactDialog::validateFields);
    connect(m_mobileEdit, &QLineEdit::textChanged, this, &ContactDialog::validateFields);
    connect(m_emailEdit, &QLineEdit::textChanged, this, &ContactDialog::validateFields);
    connect(m_birthdayEdit, &QDateEdit::dateChanged, this, &ContactDialog::validateFields);
    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ContactDialog::setContact(const Contact &contact)
{
    m_contactId = contact.id();
    m_nameEdit->setText(contact.name());
    m_mobileEdit->setText(contact.mobile());
    m_emailEdit->setText(contact.email());
    m_birthdayEdit->setDate(contact.birthday());
    setWindowTitle("Edit Contact");
}

Contact ContactDialog::getContact() const
{
    return Contact(m_contactId,
                   m_nameEdit->text().trimmed(),
                   m_mobileEdit->text().trimmed(),
                   m_emailEdit->text().trimmed(),
                   m_birthdayEdit->date());
}

void ContactDialog::validateFields()
{
    auto nr = FieldValidator::validateName(m_nameEdit->text());
    auto mr = FieldValidator::validateMobile(m_mobileEdit->text());
    auto er = FieldValidator::validateEmail(m_emailEdit->text());
    auto br = FieldValidator::validateBirthday(m_birthdayEdit->date());

    setFieldValidation(m_nameEdit, m_nameError, nr.isValid, nr.errorMessage);
    setFieldValidation(m_mobileEdit, m_mobileError, mr.isValid, mr.errorMessage);
    setFieldValidation(m_emailEdit, m_emailError, er.isValid, er.errorMessage);
    setFieldValidation(m_birthdayEdit, m_birthdayError, br.isValid, br.errorMessage);

    m_okButton->setEnabled(nr.isValid && mr.isValid && er.isValid && br.isValid);
}

void ContactDialog::setFieldValidation(QWidget *field, QLabel *label,
                                       bool valid, const QString &error)
{
    if (valid) {
        field->setStyleSheet("");
        label->clear();
        label->hide();
    } else {
        field->setStyleSheet("border: 1px solid #e74c3c; border-radius: 4px;");
        label->setText(error);
        label->show();
    }
}
