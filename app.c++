/* ****************************************************************
 * app.c++
 *
 * Main compilation unit for the HDMF-Qt application.
 *
 * Revision History:
 *
 * $Id: app.c++,v 1.4 2016/01/29 16:27:11 robert Exp $
 *
 * $Log: app.c++,v $
 * Revision 1.4  2016/01/29 16:27:11  robert
 * No comment.
 *
 * Revision 1.3  2016/01/23 04:35:49  robert
 * No comment.
 *
 * Revision 1.2  2016/01/15 16:13:15  robert
 * Added another capability.
 *
 * Revision 1.1  2016/01/13 11:26:16  robert
 * Initial import into CVS.
 *
 *
 * ****************************************************************/
#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include "eis-common.h"

int main(int argc, char *argv[])
{
    QApplication mainApp(argc, argv);
    EIS_Object eisobject;

    // Create the application and assign it a layout.

    QWidget *parent = new QWidget();
    QGridLayout *mainLayout = new QGridLayout(parent);

    // Set the applications current file name.
    eisobject.setCurrentFileName((const char *) DEFAULT_EMPLOYEE_FILE);

    // Lay down the Document Title.

    QLabel *titleLabel = new QLabel("<h1>HDMF Members Contribution Remittance Form</h1>");
    mainLayout->addWidget(titleLabel, 1,1,1,4);
   
 
    // Lay down the Company Information panel on the layout.

    int lineStart = FIRM_INFORMATION_LINE;

    QLabel *companyInfoLabel = new QLabel("<h2>Company Information</h2>");
    mainLayout->addWidget(companyInfoLabel, lineStart, 1,1,2);

    QLabel *hdmfNumLabel = new QLabel("HDMF Employer Number");
    QLineEdit *hdmfNumName = new QLineEdit("02-0078300-4");
    mainLayout->addWidget(hdmfNumLabel, lineStart+1,1);
    mainLayout->addWidget(hdmfNumName, lineStart+1,2);

    QLabel *firmLabel = new QLabel("Name of Firm");
    QLineEdit *firmName = new QLineEdit("Makabulos Memorial High School");
    mainLayout->addWidget(firmLabel, lineStart+2, 1);
    mainLayout->addWidget(firmName,  lineStart+2, 2);

    QLabel *addressLabel = new QLabel("Address");
    QLineEdit *addressName = new QLineEdit("Rizal St., Brgy San Fernando, Victoria, Tarlac, 2313");
    mainLayout->addWidget(addressLabel, lineStart+3, 1);
    mainLayout->addWidget(addressName,  lineStart+3, 2);

    QLabel *dateLabel = new QLabel("Date of Coverage");
    QLineEdit *dateName = new QLineEdit("Month Year");
    mainLayout->addWidget(dateLabel, lineStart+4, 1);
    mainLayout->addWidget(dateName,  lineStart+4, 2);

    QPushButton *saveInfoButton = new QPushButton("Save Firm Info");
    mainLayout->addWidget(saveInfoButton, lineStart, 6);
    QObject::connect(saveInfoButton, SIGNAL(clicked()), &eisobject, 
        SLOT(save_Company_Info()));
    
    eisobject.set_Employee_Info();

    // Lay down the Table view for the employees and populate it with
    // employee data. The employee data is read from the file
    // DEFAULT_EMPLOYEE_FILE. This manifest constant is defined in the
    // the header file common.h

    lineStart = EMPLOYEE_TABLE_LINE;

    QTableWidget *employeeTable = new QTableWidget(20,15);
    mainLayout->addWidget(employeeTable, lineStart+1,1,1,6);

    int empNum = eisobject.init_employee_table(employeeTable);    // This function initializes the employee table.

    // Now for the various buttons which allow us to change the
    // contents of the table, save the table, quit the application,
    // and so on.

    QLabel *employeeLabel = new QLabel("<h2>Employee Information</h2>");
    mainLayout->addWidget(employeeLabel, lineStart, 1,1,2);

    QPushButton *readFileButton = new QPushButton("Read table");
    mainLayout->addWidget(readFileButton, lineStart,4);

    QPushButton *saveAsButton = new QPushButton("Save Table as ...");
    mainLayout->addWidget(saveAsButton, lineStart,5);

    QPushButton *saveButton = new QPushButton("Save Table");
    mainLayout->addWidget(saveButton, lineStart,6);
    QObject::connect(saveButton, SIGNAL(clicked()), &eisobject, 
        SLOT(save_table(QString)));

    QPushButton *cancelButton = new QPushButton("Cancel");
    mainLayout->addWidget(cancelButton, lineStart+3,5);
    QObject::connect(cancelButton, SIGNAL(clicked()), &mainApp, SLOT(quit()));

    QPushButton *createButton = new QPushButton("Create Report");
    mainLayout->addWidget(createButton, lineStart+3,6);
    QObject::connect(createButton, SIGNAL(clicked()), &eisobject, SLOT(create_crf()));

    // Now done with the lay down of widgets. Show the application and
    // pass control over to the application event handler.

    parent->show();
    return mainApp.exec();
}

