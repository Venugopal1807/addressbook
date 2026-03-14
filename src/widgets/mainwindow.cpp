#include "mainwindow.h"
#include "contactdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Addressbook");
    setMinimumSize(800, 500);

    m_db = new DatabaseManager();
    m_db->init("addressbook.db");

    setupUi();
    setupActions();
    setupMenuBar();
    setupToolBar();

    loadContacts();
}

MainWindow::~MainWindow()
{
    delete m_db;
}

void MainWindow::setupUi()
{
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);
    layout->setSpacing(8);
    layout->setContentsMargins(12, 8, 12, 8);

    // search bar at the top
    auto *searchLayout = new QHBoxLayout();
    auto *searchLabel = new QLabel("Search:", this);
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Type to filter contacts...");
    m_searchEdit->setClearButtonEnabled(true);
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_searchEdit);
    layout->addLayout(searchLayout);

    // set up model + proxy for filtering/sorting
    m_model = new ContactTableModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterKeyColumn(-1); // search across all columns
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    m_tableView = new QTableView(this);
    m_tableView->setModel(m_proxyModel);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setSortingEnabled(true);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView->verticalHeader()->hide();
    m_tableView->sortByColumn(0, Qt::AscendingOrder);
    layout->addWidget(m_tableView);

    // status bar
    m_statusLabel = new QLabel(this);
    statusBar()->addPermanentWidget(m_statusLabel);

    setCentralWidget(central);

    connect(m_searchEdit, &QLineEdit::textChanged,
            this, &MainWindow::onSearchChanged);
    connect(m_tableView, &QTableView::doubleClicked,
            this, &MainWindow::onEditContact);
}

void MainWindow::setupActions()
{
    m_addAction = new QAction("&Add Contact", this);
    m_addAction->setShortcut(QKeySequence("Ctrl+N"));
    m_addAction->setStatusTip("Add a new contact");
    connect(m_addAction, &QAction::triggered, this, &MainWindow::onAddContact);

    m_editAction = new QAction("&Edit Contact", this);
    m_editAction->setShortcut(QKeySequence("Ctrl+E"));
    m_editAction->setStatusTip("Edit selected contact");
    connect(m_editAction, &QAction::triggered, this, &MainWindow::onEditContact);

    m_deleteAction = new QAction("&Delete Contact", this);
    m_deleteAction->setShortcut(QKeySequence::Delete);
    m_deleteAction->setStatusTip("Delete selected contact");
    connect(m_deleteAction, &QAction::triggered, this, &MainWindow::onDeleteContact);
}

void MainWindow::setupMenuBar()
{
    auto *contactsMenu = menuBar()->addMenu("&Contacts");
    contactsMenu->addAction(m_addAction);
    contactsMenu->addAction(m_editAction);
    contactsMenu->addSeparator();
    contactsMenu->addAction(m_deleteAction);
}

void MainWindow::setupToolBar()
{
    auto *toolbar = addToolBar("Main");
    toolbar->setMovable(false);
    toolbar->addAction(m_addAction);
    toolbar->addAction(m_editAction);
    toolbar->addAction(m_deleteAction);
}

void MainWindow::loadContacts()
{
    m_model->setContacts(m_db->getAllContacts());
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    int total = m_model->rowCount();
    int shown = m_proxyModel->rowCount();

    if (total == shown)
        m_statusLabel->setText(QString("%1 contacts").arg(total));
    else
        m_statusLabel->setText(QString("Showing %1 of %2 contacts").arg(shown).arg(total));
}

void MainWindow::onAddContact()
{
    ContactDialog dlg(this);
    dlg.setWindowTitle("Add Contact");

    if (dlg.exec() == QDialog::Accepted) {
        Contact c = dlg.getContact();
        int newId = m_db->addContact(c);
        if (newId >= 0) {
            c.setId(newId);
            m_model->addContact(c);
            updateStatusBar();
            statusBar()->showMessage("Contact added.", 3000);
        } else {
            QMessageBox::warning(this, "Error", "Could not save the contact.");
        }
    }
}

void MainWindow::onEditContact()
{
    auto selection = m_tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::information(this, "No Selection", "Please select a contact to edit.");
        return;
    }

    int row = sourceRow(selection.first());
    Contact existing = m_model->contactAt(row);

    ContactDialog dlg(this);
    dlg.setContact(existing);

    if (dlg.exec() == QDialog::Accepted) {
        Contact updated = dlg.getContact();
        if (m_db->updateContact(updated)) {
            m_model->updateContact(row, updated);
            updateStatusBar();
            statusBar()->showMessage("Contact updated.", 3000);
        } else {
            QMessageBox::warning(this, "Error", "Could not update the contact.");
        }
    }
}

void MainWindow::onDeleteContact()
{
    auto selection = m_tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::information(this, "No Selection", "Please select a contact to delete.");
        return;
    }

    int row = sourceRow(selection.first());
    Contact c = m_model->contactAt(row);

    auto result = QMessageBox::question(
        this, "Confirm Delete",
        QString("Are you sure you want to delete \"%1\"?").arg(c.name()),
        QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        if (m_db->deleteContact(c.id())) {
            m_model->removeContact(row);
            updateStatusBar();
            statusBar()->showMessage("Contact deleted.", 3000);
        } else {
            QMessageBox::warning(this, "Error", "Could not delete the contact.");
        }
    }
}

void MainWindow::onSearchChanged(const QString &text)
{
    m_proxyModel->setFilterFixedString(text);
    updateStatusBar();
}

int MainWindow::sourceRow(const QModelIndex &proxyIndex) const
{
    return m_proxyModel->mapToSource(proxyIndex).row();
}
