/**
 * @file main.cpp
 * @brief Application entry point for the Addressbook application.
 *
 * Initializes the QApplication, applies the custom stylesheet for a modern
 * look and feel, and launches the main window.
 */

#include <QApplication>
#include <QFont>

#include "widgets/mainwindow.h"

/**
 * @brief Returns the application-wide stylesheet (QSS).
 *
 * Defines a modern, clean visual theme with:
 * - Light gray background with white cards
 * - Blue accent color for primary actions
 * - Alternating row colors in the table
 * - Styled scrollbars, buttons, and input fields
 */
static QString applicationStyleSheet()
{
    return QStringLiteral(R"(
        /* ---- Global ---- */
        QMainWindow {
            background-color: #f5f6fa;
        }

        /* ---- Menu Bar ---- */
        QMenuBar {
            background-color: #ffffff;
            border-bottom: 1px solid #e0e0e0;
            padding: 2px 8px;
            font-size: 13px;
        }
        QMenuBar::item:selected {
            background-color: #e3f2fd;
            border-radius: 4px;
        }
        QMenu {
            background-color: #ffffff;
            border: 1px solid #e0e0e0;
            border-radius: 6px;
            padding: 4px;
        }
        QMenu::item {
            padding: 6px 28px 6px 12px;
            border-radius: 4px;
        }
        QMenu::item:selected {
            background-color: #e3f2fd;
            color: #1565c0;
        }

        /* ---- Toolbar ---- */
        QToolBar {
            background-color: #ffffff;
            border-bottom: 1px solid #e0e0e0;
            padding: 4px 8px;
            spacing: 6px;
        }
        QToolBar QToolButton {
            background-color: transparent;
            border: 1px solid transparent;
            border-radius: 6px;
            padding: 6px 14px;
            font-size: 13px;
            font-weight: 500;
            color: #333333;
        }
        QToolBar QToolButton:hover {
            background-color: #e3f2fd;
            border-color: #90caf9;
            color: #1565c0;
        }
        QToolBar QToolButton:pressed {
            background-color: #bbdefb;
        }

        /* ---- Search Bar ---- */
        QLineEdit#searchBar {
            border: 2px solid #e0e0e0;
            border-radius: 8px;
            padding: 10px 16px;
            font-size: 14px;
            background-color: #ffffff;
            selection-background-color: #90caf9;
        }
        QLineEdit#searchBar:focus {
            border-color: #42a5f5;
        }

        /* ---- Table View ---- */
        QTableView {
            background-color: #ffffff;
            alternate-background-color: #f8f9fc;
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            gridline-color: transparent;
            selection-background-color: #e3f2fd;
            selection-color: #1565c0;
            font-size: 13px;
            outline: none;
        }
        QTableView::item {
            padding: 4px 12px;
            border-bottom: 1px solid #f0f0f0;
        }
        QTableView::item:selected {
            background-color: #e3f2fd;
            color: #1565c0;
        }
        QHeaderView::section {
            background-color: #fafbfd;
            color: #666666;
            font-weight: 600;
            font-size: 12px;
            text-transform: uppercase;
            border: none;
            border-bottom: 2px solid #e0e0e0;
            padding: 8px 12px;
        }

        /* ---- Scrollbar ---- */
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0;
        }
        QScrollBar::handle:vertical {
            background: #c0c0c0;
            min-height: 30px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background: #a0a0a0;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }

        /* ---- Status Bar ---- */
        QStatusBar {
            background-color: #ffffff;
            border-top: 1px solid #e0e0e0;
            font-size: 12px;
            color: #888888;
            padding: 4px 12px;
        }

        /* ---- Contact Dialog ---- */
        QDialog {
            background-color: #f5f6fa;
        }
        QGroupBox {
            font-weight: 600;
            font-size: 14px;
            color: #333333;
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            margin-top: 12px;
            padding-top: 20px;
            background-color: #ffffff;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 4px 12px;
            color: #1565c0;
        }
        QLineEdit, QDateEdit {
            border: 2px solid #e0e0e0;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 13px;
            background-color: #ffffff;
            selection-background-color: #90caf9;
        }
        QLineEdit:focus, QDateEdit:focus {
            border-color: #42a5f5;
        }
        QLabel#errorLabel {
            color: #e74c3c;
            font-size: 11px;
            padding-left: 4px;
        }

        /* ---- Buttons ---- */
        QPushButton#saveButton {
            background-color: #1976d2;
            color: #ffffff;
            border: none;
            border-radius: 6px;
            padding: 10px 28px;
            font-size: 14px;
            font-weight: 600;
        }
        QPushButton#saveButton:hover {
            background-color: #1565c0;
        }
        QPushButton#saveButton:pressed {
            background-color: #0d47a1;
        }
        QPushButton#saveButton:disabled {
            background-color: #b0bec5;
            color: #eceff1;
        }
        QPushButton#cancelButton {
            background-color: transparent;
            color: #666666;
            border: 1px solid #e0e0e0;
            border-radius: 6px;
            padding: 10px 28px;
            font-size: 14px;
        }
        QPushButton#cancelButton:hover {
            background-color: #f5f5f5;
            border-color: #bdbdbd;
        }

        /* ---- Message Box ---- */
        QMessageBox {
            background-color: #ffffff;
        }
        QMessageBox QPushButton {
            padding: 6px 20px;
            border-radius: 4px;
            font-size: 13px;
        }
    )");
}

/**
 * @brief Application entry point.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Application metadata
    QApplication::setApplicationName(QStringLiteral("Addressbook"));
    QApplication::setApplicationVersion(QStringLiteral("1.0.0"));
    QApplication::setOrganizationName(QStringLiteral("Addressbook"));

    // Set a clean, modern font
    QFont appFont(QStringLiteral("Segoe UI"));
    appFont.setPointSize(10);
    appFont.setStyleStrategy(QFont::PreferAntialias);
    app.setFont(appFont);

    // Apply stylesheet
    app.setStyleSheet(applicationStyleSheet());

    // Launch main window
    MainWindow window;
    window.show();

    return app.exec();
}
