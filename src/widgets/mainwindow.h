// mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QAction>

#include "../database/databasemanager.h"
#include "contacttablemodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddContact();
    void onEditContact();
    void onDeleteContact();
    void onSearchChanged(const QString &text);

private:
    void setupUi();
    void setupActions();
    void setupMenuBar();
    void setupToolBar();
    void loadContacts();
    void updateStatusBar();

    // maps a proxy model row back to the source model row
    int sourceRow(const QModelIndex &proxyIndex) const;

    DatabaseManager    *m_db;
    ContactTableModel  *m_model;
    QSortFilterProxyModel *m_proxyModel;
    QTableView         *m_tableView;
    QLineEdit          *m_searchEdit;
    QLabel             *m_statusLabel;

    QAction *m_addAction;
    QAction *m_editAction;
    QAction *m_deleteAction;
};

#endif // MAINWINDOW_H
