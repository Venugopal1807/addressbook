#include <QApplication>
#include "widgets/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Addressbook");
    app.setOrganizationName("Addressbook");

    // global stylesheet for a cleaner look
    app.setStyleSheet(R"(
        QMainWindow {
            background: #f5f6fa;
        }

        QTableView {
            background: white;
            border: 1px solid #dcdde1;
            border-radius: 6px;
            gridline-color: #ecf0f1;
            selection-background-color: #3498db;
            selection-color: white;
            font-size: 13px;
        }
        QTableView::item {
            padding: 6px 8px;
        }
        QTableView::item:alternate {
            background: #f8f9fa;
        }

        QHeaderView::section {
            background-color: #2c3e50;
            color: white;
            padding: 8px 12px;
            border: none;
            font-weight: bold;
            font-size: 13px;
        }

        QLineEdit, QDateEdit {
            padding: 8px 12px;
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            background: white;
            font-size: 13px;
        }
        QLineEdit:focus, QDateEdit:focus {
            border-color: #3498db;
        }

        QPushButton {
            padding: 8px 20px;
            border: none;
            border-radius: 4px;
            font-size: 13px;
            font-weight: bold;
            background: #3498db;
            color: white;
        }
        QPushButton:hover {
            background: #2980b9;
        }
        QPushButton:pressed {
            background: #2471a3;
        }
        QPushButton:disabled {
            background: #bdc3c7;
            color: #7f8c8d;
        }
        QPushButton#cancelButton {
            background: #95a5a6;
        }
        QPushButton#cancelButton:hover {
            background: #7f8c8d;
        }

        QGroupBox {
            font-weight: bold;
            border: 1px solid #dcdde1;
            border-radius: 6px;
            margin-top: 8px;
            padding-top: 16px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 6px;
        }

        QLabel#errorLabel {
            color: #e74c3c;
            font-size: 11px;
            padding-left: 4px;
        }

        QToolBar {
            background: #2c3e50;
            spacing: 4px;
            padding: 4px;
            border: none;
        }
        QToolBar QToolButton {
            color: white;
            padding: 6px 12px;
            border-radius: 3px;
        }
        QToolBar QToolButton:hover {
            background: #34495e;
        }

        QMenuBar {
            background: #2c3e50;
            color: white;
        }
        QMenuBar::item:selected {
            background: #34495e;
        }
        QMenu {
            background: white;
            border: 1px solid #dcdde1;
        }
        QMenu::item:selected {
            background: #3498db;
            color: white;
        }

        QStatusBar {
            background: #ecf0f1;
            color: #2c3e50;
            font-size: 12px;
        }
    )");

    MainWindow w;
    w.show();

    return app.exec();
}
