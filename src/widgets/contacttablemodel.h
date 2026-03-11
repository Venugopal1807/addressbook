/**
 * @file contacttablemodel.h
 * @brief Custom table model for displaying contacts in a QTableView.
 *
 * Implements QAbstractTableModel to provide a clean MVC separation
 * between contact data and the table view presentation.
 */

#ifndef CONTACTTABLEMODEL_H
#define CONTACTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "../models/contact.h"

/**
 * @class ContactTableModel
 * @brief Table model presenting Contact objects in four columns.
 *
 * Columns: Name | Mobile | Email | Birthday
 *
 * Supports:
 * - Dynamic data updates via setContacts() / addContact() / removeContact()
 * - Column sorting through QSortFilterProxyModel (external)
 * - Standard Qt model/view roles (Display, TextAlignment)
 */
class ContactTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    /** @brief Column index enumeration for readability. */
    enum Column {
        ColumnName     = 0,
        ColumnMobile   = 1,
        ColumnEmail    = 2,
        ColumnBirthday = 3,
        ColumnCount    = 4
    };

    /**
     * @brief Constructs the table model.
     * @param parent Optional parent object.
     */
    explicit ContactTableModel(QObject *parent = nullptr);

    // --- QAbstractTableModel interface ---
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // --- Custom methods ---

    /** @brief Replaces all model data with the given contact list. */
    void setContacts(const QList<Contact> &contacts);

    /** @brief Appends a single contact to the model. */
    void addContact(const Contact &contact);

    /** @brief Updates the contact at the given row. */
    void updateContact(int row, const Contact &contact);

    /** @brief Removes the contact at the given row. */
    void removeContact(int row);

    /**
     * @brief Returns the Contact at the given row.
     * @param row Row index (0-based).
     * @return The Contact, or an empty Contact if row is invalid.
     */
    Contact contactAt(int row) const;

private:
    QList<Contact> m_contacts;  ///< Internal contact storage
};

#endif // CONTACTTABLEMODEL_H
