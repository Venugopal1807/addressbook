/**
 * @file contactdialog.h
 * @brief Dialog for adding and editing contacts.
 *
 * Provides a form with real-time field validation, visual feedback
 * (red borders on invalid fields), and OK/Cancel action buttons.
 */

#ifndef CONTACTDIALOG_H
#define CONTACTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QLabel>
#include <QPushButton>

#include "../models/contact.h"

/**
 * @class ContactDialog
 * @brief Modal dialog for creating or editing a contact.
 *
 * Supports two modes:
 * - **Add mode**: empty form for new contact entry
 * - **Edit mode**: pre-filled form with existing contact data
 *
 * Real-time validation highlights invalid fields and disables
 * the OK button until all fields pass validation.
 */
class ContactDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the dialog.
     * @param parent Parent widget.
     */
    explicit ContactDialog(QWidget *parent = nullptr);

    /**
     * @brief Sets the dialog to edit mode with pre-filled data.
     * @param contact The contact to edit.
     */
    void setContact(const Contact &contact);

    /**
     * @brief Returns the contact data from the form fields.
     * @return A Contact populated with the current form values.
     */
    Contact getContact() const;

private slots:
    /** @brief Validates all fields and updates UI feedback. */
    void validateFields();

private:
    /** @brief Sets up the dialog layout and widgets. */
    void setupUi();

    /** @brief Connects signals to validation slots. */
    void connectSignals();

    /**
     * @brief Applies visual validation feedback to a field.
     * @param field  The input field widget.
     * @param label  The error label to show/hide.
     * @param valid  Whether the field is valid.
     * @param error  Error message to display if invalid.
     */
    void setFieldValidation(QWidget *field, QLabel *label,
                            bool valid, const QString &error);

    // Form fields
    QLineEdit   *m_nameEdit;        ///< Name input
    QLineEdit   *m_mobileEdit;      ///< Mobile number input
    QLineEdit   *m_emailEdit;       ///< Email input
    QDateEdit   *m_birthdayEdit;    ///< Birthday date picker

    // Validation error labels
    QLabel      *m_nameError;       ///< Name validation error
    QLabel      *m_mobileError;     ///< Mobile validation error
    QLabel      *m_emailError;      ///< Email validation error
    QLabel      *m_birthdayError;   ///< Birthday validation error

    // Action buttons
    QPushButton *m_okButton;        ///< OK / Save button
    QPushButton *m_cancelButton;    ///< Cancel button

    int m_contactId;                ///< Preserved contact id in edit mode
};

#endif // CONTACTDIALOG_H
