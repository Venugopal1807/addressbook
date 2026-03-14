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
    return ColCount;
}

QVariant ContactTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_contacts.count())
        return {};

    const Contact &c = m_contacts.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case ColName:     return c.name();
        case ColMobile:   return c.mobile();
        case ColEmail:    return c.email();
        case ColBirthday: return c.birthday().toString("dd MMM yyyy");
        }
    }

    if (role == Qt::TextAlignmentRole && index.column() == ColBirthday)
        return QVariant(Qt::AlignCenter);

    if (role == Qt::TextAlignmentRole)
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);

    // expose raw QDate for proper sorting via proxy model
    if (role == Qt::UserRole && index.column() == ColBirthday)
        return c.birthday();

    return {};
}

QVariant ContactTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};

    switch (section) {
    case ColName:     return tr("Name");
    case ColMobile:   return tr("Mobile");
    case ColEmail:    return tr("Email");
    case ColBirthday: return tr("Birthday");
    }
    return {};
}

void ContactTableModel::setContacts(const QList<Contact> &contacts)
{
    beginResetModel();
    m_contacts = contacts;
    endResetModel();
}

void ContactTableModel::addContact(const Contact &contact)
{
    int row = m_contacts.count();
    beginInsertRows(QModelIndex(), row, row);
    m_contacts.append(contact);
    endInsertRows();
}

void ContactTableModel::updateContact(int row, const Contact &contact)
{
    if (row < 0 || row >= m_contacts.count())
        return;
    m_contacts[row] = contact;
    emit dataChanged(index(row, 0), index(row, ColCount - 1));
}

void ContactTableModel::removeContact(int row)
{
    if (row < 0 || row >= m_contacts.count())
        return;
    beginRemoveRows(QModelIndex(), row, row);
    m_contacts.removeAt(row);
    endRemoveRows();
}

Contact ContactTableModel::contactAt(int row) const
{
    if (row < 0 || row >= m_contacts.count())
        return Contact();
    return m_contacts.at(row);
}
