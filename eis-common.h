/* *************************************************************************************
 * eis-common.h
 *
 * This is the primary header file for the Employee Information System (EIS) Project.
 * It contains all declarations and constants which are used by EIS.
 *
 * Revision History:
 *
 * $Id: eis-common.h,v 1.3 2016/01/29 16:27:11 robert Exp $
 *
 * $Log: eis-common.h,v $
 * Revision 1.3  2016/01/29 16:27:11  robert
 * No comment.
 *
 * Revision 1.2  2016/01/23 04:35:49  robert
 * No comment.
 *
 * Revision 1.1  2016/01/15 16:10:46  robert
 * Initial import into CVS.
 *
 *
 * ************************************************************************************/
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QStringList>
#include <QString>
#include <QByteArray>
#include <QGridLayout>
#include <QLineEdit>
#include <QLayoutItem>
#include <QFile>
#include <QObject>
#include <QGridLayout>
#include <stdio.h>
#include <RString.h>

// The following indicates where the Employee Table is Saved.
#define  DEFAULT_EMPLOYEE_FILE     "sample.csv"

// Company information is saved and retrieved from the following file.
#define  DEFAULT_INFO_FILE         "firm-info.csv"

// In the main dialog, information about the firm is laid out starting
// at the following row.
#define  FIRM_INFORMATION_LINE     3

// The employee table is laid out starting at the following row. 
#define  EMPLOYEE_TABLE_LINE       8


class EIS_Object : public QObject
{
    Q_OBJECT

    public slots:
        void create_crf(void);
        void save_table(QString);
        void save_Company_Info(void);

    public:
        void set_Current_File_Name(char *);
        void set_Employee_Table(QTableWidget *table);
        void set_Firm_Name(QLineEdit *efirmname);
        void set_HDMF_Number(QLineEdit *ehdmfnum);
        void set_Firm_Address(QLineEdit *efirmaddr);

        QString       get_Current_File_Name(void);
        QTableWidget *get_Employee_Table(void);
        QLineEdit    *get_Firm_Name(void);
        QLineEdit    *get_HDMF_Number(void);
        QLineEdit    *get_Firm_Address(void);

        int  printTableAsTabular(void);
        int  init_employee_table(QTableWidget *employeeTable);
        void set_Employee_Info(void);

    private:
        char         *EIS_csv_filename;
        QTableWidget *EIS_employee_Table;
        int           EIS_num_Employees;
        QLineEdit    *EIS_Firm_Name;
        QLineEdit    *EIS_HDMF_Number;
        QLineEdit    *EIS_Firm_Address;
};


