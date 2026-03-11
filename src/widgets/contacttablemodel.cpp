/**
 * @file contacttablemodel.cpp
 * @brief Implementation of the ContactTableModel for QTableView display.
 */

#include "contacttablemodel.h"

ContactTableModel::ContactTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int ContactTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_contacts.count();
}

int ContactTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant ContactTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_contacts.count()) {
        return QVariant();
    }

    const Contact &contact = m_contacts.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case ColumnName:
            return contact.name();
        case ColumnMobile:
            return contact.mobile();
        case ColumnEmail:
            return contact.email();
        case ColumnBirthday:
            return contact.birthday().toString(QStringLiteral("dd MMM yyyy"));
        default:
            return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case ColumnBirthday:
            return QVariant(Qt::AlignCenter);
        default:
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }

    // Store raw data for sorting by actual date
    if (role == Qt::UserRole && index.column() == ColumnBirthday) {
        return contact.birthday();
    }

    return QVariant();
}

QVariant ContactTableModel::headerData(int section,
                                        Qt::Orientation orientation,
                                        int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QVariant();
    }

    switch (section) {
    case ColumnName:     return QStringLiteral("Name");
    case ColumnMobile:   return QStringLiteral("Mobile");
    case ColumnEmail:    return QStringLiteral("Email");
    case ColumnBirthday: return QStringLiteral("Birthday");
    default: return QVariant();
    }
}

void ContactTableModel::setContacts(const QList<Contact> &contacts)
{
    beginResetModel();
    m_contacts = contacts;
    endResetModel();
}

void ContactTableModel::addContact(const Contact &contact)
{
    const int row = m_contacts.count();
    beginInsertRows(QModelIndex(), row, row);
    m_contacts.append(contact);
    endInsertRows();
}

void ContactTableModel::updateContact(int row, const Contact &contact)
{
    if (row < 0 || row >= m_contacts.count()) {
        return;
    }

    m_contacts[row] = contact;
    emit dataChanged(index(row, 0), index(row, ColumnCount - 1));
}

void ContactTableModel::removeContact(int row)
{
    if (row < 0 || row >= m_contacts.count()) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_contacts.removeAt(row);
    endRemoveRows();
}

Contact ContactTableModel::contactAt(int row) const
{
    if (row < 0 || row >= m_contacts.count()) {
        return Contact();
    }
    return m_contacts.at(row);
}
