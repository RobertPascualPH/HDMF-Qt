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
#include <QObject>
#include <QTableWidget>
#include <QGridLayout>
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
        void setCurrentFileName(char *);
        void setCurrentEmployeeTable(QTableWidget *table);
        int  printTableAsTabular(void);
        int  init_employee_table(QTableWidget *employeeTable);
        void setLayout(QGridLayout *theLayout);
        void set_Employee_Info(void);

    private:
        char *current_csv_filename;
        QTableWidget *EIS_employeeTable;
        int EIS_numEmployees;
        QString EIS_FirmName;
        QString EIS_HDMFNumber;
        QString EIS_completeAddress;
        QGridLayout *theLayout;
};


