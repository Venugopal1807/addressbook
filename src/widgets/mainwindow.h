/**
 * @file mainwindow.h
 * @brief Main application window with contact list, toolbar, and menus.
 *
 * Serves as the primary UI entry point. Displays contacts in a sortable,
 * filterable table view with Add, Edit, and Delete actions accessible
 * via toolbar buttons, menu bar, and keyboard shortcuts.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QLineEdit>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QAction>

#include "contacttablemodel.h"
#include "../database/databasemanager.h"

/**
 * @class MainWindow
 * @brief Primary application window managing the contact list.
 *
 * Features:
 * - Sortable table view with all contacts
 * - Search bar for real-time filtering by name
 * - Toolbar and menu bar with Add, Edit, Delete actions
 * - Status bar showing total contact count
 * - Keyboard shortcuts (Ctrl+N, Enter, Delete)
 * - Double-click to edit, confirmation before delete
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window.
     * @param parent Optional parent widget.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /** @brief Destructor. */
    ~MainWindow() override;

private slots:
    /** @brief Opens the Add Contact dialog. */
    void onAddContact();

    /** @brief Opens the Edit Contact dialog for the selected contact. */
    void onEditContact();

    /** @brief Deletes the selected contact after confirmation. */
    void onDeleteContact();

    /** @brief Handles double-click on a table row (triggers edit). */
    void onTableDoubleClicked(const QModelIndex &index);

    /** @brief Filters the table based on search text. */
    void onSearchTextChanged(const QString &text);

    /** @brief Shows the About dialog. */
    void onAbout();

private:
    /** @brief Creates the menu bar with File, Contact, and Help menus. */
    void createMenuBar();

    /** @brief Creates the toolbar with Add, Edit, Delete buttons. */
    void createToolBar();

    /** @brief Sets up the central widget layout. */
    void setupCentralWidget();

    /** @brief Creates the status bar. */
    void createStatusBar();

    /** @brief Loads all contacts from the database into the table model. */
    void loadContacts();

    /** @brief Updates the status bar contact count. */
    void updateStatusBar();

    /**
     * @brief Returns the selected row in the source model.
     * @return Source model row index, or -1 if nothing selected.
     */
    int selectedSourceRow() const;

    // Core components
    DatabaseManager     m_databaseManager;  ///< SQLite database interface
    ContactTableModel  *m_tableModel;       ///< Source data model
    QSortFilterProxyModel *m_proxyModel;    ///< Sorting and filtering proxy

    // UI widgets
    QTableView *m_tableView;    ///< Contact list table
    QLineEdit  *m_searchBar;    ///< Search input field
    QLabel     *m_statusLabel;  ///< Status bar label

    // Actions (shared between menu and toolbar)
    QAction *m_addAction;       ///< Add contact action
    QAction *m_editAction;      ///< Edit contact action
    QAction *m_deleteAction;    ///< Delete contact action
};

#endif // MAINWINDOW_H
