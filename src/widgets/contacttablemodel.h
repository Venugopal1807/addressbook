// contacttablemodel.h - bridges Contact data to QTableView

#ifndef CONTACTTABLEMODEL_H
#define CONTACTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "../models/contact.h"

// Custom model so we can display contacts in a QTableView.
// Columns: Name | Mobile | Email | Birthday
class ContactTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column {
        ColName = 0,
        ColMobile,
        ColEmail,
        ColBirthday,
        ColCount
    };

    explicit ContactTableModel(QObject *parent = nullptr);

    // QAbstractTableModel overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Bulk-replace all contacts (e.g., on initial load)
    void setContacts(const QList<Contact> &contacts);

    void addContact(const Contact &contact);
    void updateContact(int row, const Contact &contact);
    void removeContact(int row);
    Contact contactAt(int row) const;

private:
    QList<Contact> m_contacts;
};

#endif // CONTACTTABLEMODEL_H
