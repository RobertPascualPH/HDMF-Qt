/* ************************************************************
 * common-functions.c++
 *
 * Common functions used by our Government Forms Project.
 *
 * Revision History
 *
 * $Id: common-functions.c++,v 1.4 2016/01/29 16:27:11 robert Exp $
 *
 * $Log: common-functions.c++,v $
 * Revision 1.4  2016/01/29 16:27:11  robert
 * No comment.
 *
 * Revision 1.3  2016/01/23 04:35:49  robert
 * No comment.
 *
 * Revision 1.2  2016/01/15 16:12:48  robert
 * Added a function.
 *
 * Revision 1.1  2016/01/13 11:26:16  robert
 * Initial import into CVS.
 *
 *
 * ************************************************************/
#include "eis-common.h"


EIS_Object::EIS_Object()
// The constructor has assumed the duties of allocating
// the QWidgets and initializing their values.
{
    // Set the defaults.

    printf("Entering default constructor.");

    // QLineEdit *EIS_Firm_Name = new QLineEdit("Makabulos Memorial High School");
    // QLineEdit *EIS_HDMF_Number = new QLineEdit("00-0000000-0");
    // QLineEdit *EIS_Firm_Address = new QLineEdit("Right beside Harvard University");
    // QLineEdit *EIS_Coverage_Date = new QLineEdit("Month Year");

    // Now for the Employee Table
    //
    QTableWidget *EIS_Employee_Table
            = new QTableWidget(20, 15);  // ISSUE: I don't like hard-coded limits.
    QStringList *zHeaderStrings = new QStringList();

    // Set the headers for the QTableWidget

    zHeaderStrings -> append("Last Name");
    zHeaderStrings -> append("First Name");
    zHeaderStrings -> append("Middle Name");
    zHeaderStrings -> append("SSS Number");
    zHeaderStrings -> append("PH Num");
    zHeaderStrings -> append("HDMF Num");
    zHeaderStrings -> append("Salary");
    zHeaderStrings -> append("SSS EE\nContrib");
    zHeaderStrings -> append("SSS ER\nContrib");
    zHeaderStrings -> append("EC\nContrib");
    zHeaderStrings -> append("PH EE\nContrib");
    zHeaderStrings -> append("PH ER\nContrib");
    zHeaderStrings -> append("HDMF EE\nContrib");
    zHeaderStrings -> append("HDMF ER\nContrib");
    zHeaderStrings -> append("Remarks");

    EIS_Employee_Table-> setHorizontalHeaderLabels(*zHeaderStrings);   // Note that setHorizontalHeaderLabels() expects a reference to a QStringList.

    // Populate the QTableWidget with the contents of DEFAULT_EMPLOYEE_FILE

    FILE *eTable = fopen(DEFAULT_EMPLOYEE_FILE, "r");
    char *line = 0x00;    // line is a pointer to NULL.
    char *wordsptr[1024], wordsbuffer[2048];    // Words pointer and words buffer
    int retval, row = 0;
    size_t len = 0;

    if (eTable != NULL) {

        // We clear both wordsptr and wordsbuffer because the function
        // strsplit() does not clear them.

        for (int k = 0; k < 1024; ++k) {
            wordsptr[k] = 0x00;
        }

        for (int k = 0; k < 2048; ++k) {
            wordsbuffer[k] = 0x00;
        }

        row = 0;
        while ((retval = getline(&line, &len, eTable)) > 0) {
            retval = strsplit(line, wordsptr, wordsbuffer);

	    for (int column = 1; column < 15; ++column) {
	        QTableWidgetItem *newItem = new QTableWidgetItem(wordsptr[column]);
		EIS_Employee_Table->setItem(row, column-1, newItem);
	    }
            ++row;
        }
        free(line);
        fclose(eTable);
    }
    EIS_Num_Employees = row;

    printf("Exiting default constructor.");
}


QTableWidget * EIS_Object::get_Employee_Table(void)
{
    return EIS_Employee_Table;
}


QLineEdit * EIS_Object::get_Firm_Name(void)
{
    return EIS_Firm_Name;
}


QLineEdit * EIS_Object::get_HDMF_Number(void)
{
    return EIS_HDMF_Number;
}

void EIS_Object::set_HDMFN_Text_Field(QLineEdit *tl)
{
    EIS_HDMF_Number = tl;
}

QLineEdit * EIS_Object::get_Firm_Address(void)
{
    return EIS_Firm_Address;
}


QLineEdit * EIS_Object::get_Coverage_Date(void)
{
    return EIS_Coverage_Date;
}


void EIS_Object::save_Company_Info(void)
{

     QString firmName    = EIS_Firm_Name->text(); // Does this make a copy?
     QString firmHDMFNum = EIS_HDMF_Number->text();
     QString firmAddress = EIS_Firm_Address->text();

     QString info_text;
     info_text.sprintf("%s|%s|%s",
                        qPrintable(firmHDMFNum),
                        qPrintable(firmName),
                        qPrintable(firmAddress));

     QFile fout(DEFAULT_INFO_FILE);
     fout.open(QIODevice::WriteOnly | QIODevice::Text);
     fout.write(qPrintable(info_text));
     fout.close();
     printf("Done!\n");
     
}

#ifdef OLDVERSION
void EIS_Object::set_Employee_Info(void)
// Reads the company info from the DEFAULT_INFO_FILE
{
     QFile inFile(DEFAULT_INFO_FILE);
     char input_buffer[1024];
     char *field_pointer;
     char field_buffer[1024];

     // Open the Company Information File.

     if(inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
	 inFile.readLine(input_buffer, 1024);
	 int retval = strsplit(input_buffer, field_pointer, field_buffer);

	 int lineStart = FIRM_INFORMATION_LINE;
	
	 QLayoutItem *tlai = theLayout -> itemAtPosition(lineStart+1,2);
	 QLineEdit *editCtrl = (QLineEdit *) tlai -> widget();
         editCtrl.setText(tr(field_pointer[0]));

	 tlai =  theLayout->itemAtPosition(lineStart+2,2);
	 editCtrl = (QLineEdit *) tlai->widget();
         editCtrl.setText(tr(field_pointer[1]));

	 tlai = theLayout -> itemAtPosition(lineStart+3,2);
	 editCtrl = (QLineEdit *) tlai -> widget();
         editCtrl.setText(tr(field_pointer[1]));
     }
}
#endif

void EIS_Object::set_Current_File_Name(char *cfile)
{
    strcpy(EIS_Employee_CSV_File, cfile);
}


void EIS_Object::set_Employee_Table(QTableWidget *table)
{
    EIS_Employee_Table = table;   
}


void EIS_Object::create_crf(void)
// Creates the HDMF Contributions Report Form.
//
{
    printTableAsTabular();

}


void EIS_Object::save_table(QString cfname)
{
}


int EIS_Object::printTableAsTabular(void)
{
    QTableWidgetItem *fItem;
    QString qstr;
    QByteArray msgStr;
    int row;

    for (row = 0; row < EIS_Num_Employees; row++) {
        for (int col = 0; col < 15; col++) {
	    fItem = EIS_Employee_Table->item(row,col);
            if (fItem != 0x00) {
	        qstr = fItem->text();
	        msgStr = qstr.toLatin1();
	        printf("%s ", msgStr.data());
            }
        }
       printf("\n");  
    }

    return row;
}


#ifdef OLDVERSION

int EIS_Object::init_employee_table(QTableWidget *empTable)
// This function initializes employee table (empTable) by 
// a) setting the horizontal headers,
// b) loading the contents of DEFAULT_EMPLOYEE_FILE, and
// c) returning with the number of employees found.
// d) Setting EIS_employeeTable to empTable.
{
    QStringList *zHeaderStrings = new QStringList();
    EIS_employeeTable = empTable; // Set the current employee table

    // Set the headers for the QTableWidget

    zHeaderStrings -> append("Last Name");
    zHeaderStrings -> append("First Name");
    zHeaderStrings -> append("Middle Name");
    zHeaderStrings -> append("SSS Number");
    zHeaderStrings -> append("PH Num");
    zHeaderStrings -> append("HDMF Num");
    zHeaderStrings -> append("Salary");
    zHeaderStrings -> append("SSS EE\nContrib");
    zHeaderStrings -> append("SSS ER\nContrib");
    zHeaderStrings -> append("EC\nContrib");
    zHeaderStrings -> append("PH EE\nContrib");
    zHeaderStrings -> append("PH ER\nContrib");
    zHeaderStrings -> append("HDMF EE\nContrib");
    zHeaderStrings -> append("HDMF ER\nContrib");
    zHeaderStrings -> append("Remarks");

    EIS_employeeTable-> setHorizontalHeaderLabels(*zHeaderStrings);   // Note that setHorizontalHeaderLabels() expects a reference to a QStringList.

    // Populate the QTableWidget with the contents of DEFAULT_EMPLOYEE_FILE

    FILE *eTable = fopen(DEFAULT_EMPLOYEE_FILE, "r");
    char *line = 0x00;    // line is a pointer to NULL.
    char *wordsptr[1024], wordsbuffer[2048];    // Words pointer and words buffer
    int retval, row = 0;
    size_t len = 0;

    if (eTable != NULL) {

        // We clear both wordsptr and wordsbuffer because the function
        // strsplit() does not clear them.

        for (int k = 0; k < 1024; ++k) {
            wordsptr[k] = 0x00;
        }

        for (int k = 0; k < 2048; ++k) {
            wordsbuffer[k] = 0x00;
        }

        row = 0;
        while ((retval = getline(&line, &len, eTable)) > 0) {
            retval = strsplit(line, wordsptr, wordsbuffer);

	    for (int column = 1; column < 15; ++column) {
	        QTableWidgetItem *newItem = new QTableWidgetItem(wordsptr[column]);
		EIS_employeeTable->setItem(row, column-1, newItem);
	    }
            ++row;
        }
        free(line);
        fclose(eTable);
    }
    EIS_numEmployees = row;
    return EIS_numEmployees;
}

#endif


