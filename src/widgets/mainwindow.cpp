/**
 * @file mainwindow.cpp
 * @brief Implementation of the main application window.
 */

#include "mainwindow.h"
#include "contactdialog.h"

#include <QApplication>
#include <QHeaderView>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QVBoxLayout>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_tableModel(new ContactTableModel(this))
    , m_proxyModel(new QSortFilterProxyModel(this))
{
    setWindowTitle(QStringLiteral("Addressbook"));
    setMinimumSize(800, 500);
    resize(900, 600);

    // Initialize database
    const QString dbPath = QApplication::applicationDirPath()
                         + QStringLiteral("/addressbook.db");
    if (!m_databaseManager.init(dbPath)) {
        QMessageBox::critical(this, QStringLiteral("Database Error"),
            QStringLiteral("Failed to open the database. The application "
                           "may not function correctly."));
    }

    // Configure proxy model for sorting and filtering
    m_proxyModel->setSourceModel(m_tableModel);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterKeyColumn(-1);  // Search all columns
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    setupCentralWidget();
    createMenuBar();
    createToolBar();
    createStatusBar();

    loadContacts();
}

MainWindow::~MainWindow() = default;

// ---------------------------------------------------------------------------
// UI Setup
// ---------------------------------------------------------------------------

void MainWindow::setupCentralWidget()
{
    auto *centralWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(centralWidget);
    layout->setSpacing(8);
    layout->setContentsMargins(12, 12, 12, 4);

    // Search bar
    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText(
        QStringLiteral("\xF0\x9F\x94\x8D  Search contacts by name, email, or phone..."));
    m_searchBar->setClearButtonEnabled(true);
    m_searchBar->setObjectName(QStringLiteral("searchBar"));
    connect(m_searchBar, &QLineEdit::textChanged,
            this, &MainWindow::onSearchTextChanged);
    layout->addWidget(m_searchBar);

    // Table view
    m_tableView = new QTableView(this);
    m_tableView->setModel(m_proxyModel);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSortingEnabled(true);
    m_tableView->sortByColumn(0, Qt::AscendingOrder);
    m_tableView->setShowGrid(false);
    m_tableView->setWordWrap(false);
    m_tableView->verticalHeader()->setVisible(false);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Column sizing
    auto *header = m_tableView->horizontalHeader();
    header->setStretchLastSection(true);
    header->setSectionResizeMode(0, QHeaderView::Stretch);       // Name
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents); // Mobile
    header->setSectionResizeMode(2, QHeaderView::Stretch);       // Email
    header->setMinimumSectionSize(120);
    header->setDefaultSectionSize(180);

    // Row height
    m_tableView->verticalHeader()->setDefaultSectionSize(36);

    connect(m_tableView, &QTableView::doubleClicked,
            this, &MainWindow::onTableDoubleClicked);

    layout->addWidget(m_tableView);
    setCentralWidget(centralWidget);
}

void MainWindow::createMenuBar()
{
    // --- File menu ---
    QMenu *fileMenu = menuBar()->addMenu(QStringLiteral("&File"));
    fileMenu->addAction(QStringLiteral("E&xit"), this,
                        &QWidget::close, QKeySequence::Quit);

    // --- Contact menu ---
    QMenu *contactMenu = menuBar()->addMenu(QStringLiteral("&Contact"));

    m_addAction = contactMenu->addAction(
        QStringLiteral("&Add Contact"),
        this, &MainWindow::onAddContact,
        QKeySequence(QStringLiteral("Ctrl+N")));

    m_editAction = contactMenu->addAction(
        QStringLiteral("&Edit Contact"),
        this, &MainWindow::onEditContact,
        QKeySequence(QStringLiteral("Ctrl+E")));

    m_deleteAction = contactMenu->addAction(
        QStringLiteral("&Delete Contact"),
        this, &MainWindow::onDeleteContact,
        QKeySequence::Delete);

    // --- Help menu ---
    QMenu *helpMenu = menuBar()->addMenu(QStringLiteral("&Help"));
    helpMenu->addAction(QStringLiteral("&About"), this,
                        &MainWindow::onAbout);
}

void MainWindow::createToolBar()
{
    QToolBar *toolBar = addToolBar(QStringLiteral("Main Toolbar"));
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(20, 20));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolBar->addAction(m_addAction);
    toolBar->addAction(m_editAction);
    toolBar->addAction(m_deleteAction);
}

void MainWindow::createStatusBar()
{
    m_statusLabel = new QLabel(this);
    statusBar()->addWidget(m_statusLabel);
    updateStatusBar();
}

// ---------------------------------------------------------------------------
// Slots
// ---------------------------------------------------------------------------

void MainWindow::onAddContact()
{
    ContactDialog dialog(this);
    dialog.setWindowTitle(QStringLiteral("Add Contact"));

    if (dialog.exec() == QDialog::Accepted) {
        Contact contact = dialog.getContact();
        int newId = m_databaseManager.addContact(contact);
        if (newId >= 0) {
            contact.setId(newId);
            m_tableModel->addContact(contact);
            updateStatusBar();
        } else {
            QMessageBox::warning(this, QStringLiteral("Error"),
                QStringLiteral("Failed to save the contact."));
        }
    }
}

void MainWindow::onEditContact()
{
    const int sourceRow = selectedSourceRow();
    if (sourceRow < 0) {
        QMessageBox::information(this, QStringLiteral("No Selection"),
            QStringLiteral("Please select a contact to edit."));
        return;
    }

    Contact existing = m_tableModel->contactAt(sourceRow);

    ContactDialog dialog(this);
    dialog.setContact(existing);

    if (dialog.exec() == QDialog::Accepted) {
        Contact updated = dialog.getContact();
        if (m_databaseManager.updateContact(updated)) {
            m_tableModel->updateContact(sourceRow, updated);
        } else {
            QMessageBox::warning(this, QStringLiteral("Error"),
                QStringLiteral("Failed to update the contact."));
        }
    }
}

void MainWindow::onDeleteContact()
{
    const int sourceRow = selectedSourceRow();
    if (sourceRow < 0) {
        QMessageBox::information(this, QStringLiteral("No Selection"),
            QStringLiteral("Please select a contact to delete."));
        return;
    }

    Contact contact = m_tableModel->contactAt(sourceRow);

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        QStringLiteral("Delete Contact"),
        QStringLiteral("Are you sure you want to delete \"%1\"?")
            .arg(contact.name()),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_databaseManager.deleteContact(contact.id())) {
            m_tableModel->removeContact(sourceRow);
            updateStatusBar();
        } else {
            QMessageBox::warning(this, QStringLiteral("Error"),
                QStringLiteral("Failed to delete the contact."));
        }
    }
}

void MainWindow::onTableDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    onEditContact();
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    m_proxyModel->setFilterFixedString(text);
    updateStatusBar();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, QStringLiteral("About Addressbook"),
        QStringLiteral(
            "<h3>Addressbook v1.0</h3>"
            "<p>A modern contact management application built with Qt.</p>"
            "<p>Features:</p>"
            "<ul>"
            "<li>Add, edit, and delete contacts</li>"
            "<li>Search and filter</li>"
            "<li>Field validation</li>"
            "<li>SQLite persistence</li>"
            "</ul>"
            "<p>&copy; 2026</p>"));
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

void MainWindow::loadContacts()
{
    QList<Contact> contacts = m_databaseManager.getAllContacts();
    m_tableModel->setContacts(contacts);
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    const int total = m_tableModel->rowCount();
    const int visible = m_proxyModel->rowCount();

    if (total == visible) {
        m_statusLabel->setText(
            QStringLiteral("%1 contact(s)").arg(total));
    } else {
        m_statusLabel->setText(
            QStringLiteral("Showing %1 of %2 contact(s)")
                .arg(visible).arg(total));
    }
}

int MainWindow::selectedSourceRow() const
{
    const QModelIndexList selected =
        m_tableView->selectionModel()->selectedRows();

    if (selected.isEmpty()) {
        return -1;
    }

    // Map proxy index back to source model index
    const QModelIndex proxyIndex = selected.first();
    const QModelIndex sourceIndex = m_proxyModel->mapToSource(proxyIndex);
    return sourceIndex.row();
}
