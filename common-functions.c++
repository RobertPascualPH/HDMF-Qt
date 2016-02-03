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

void EIS_Object::setFirmName(QLineEdit *efirmname)
{
    EIS_FirmName = efirmname;
}


void EIS_Object::setHDMFNumber(QLineEdit *hdmfnum)
{
    EIS_HDMFNumber = ehdmfnum;
}


void EIS_Object::setFirmAddress(QLineEdit *efirmaddr)
{
    EIS_FirmAddress = efirmaddr;
}


void EIS_Object::save_Company_Info(void)
{

     QString firmName    = EIS_FirmName->text(); // Does this make a copy?
     QString firmHDMFNum = EIS_HDMFNumber->text();
     QString firmAddress = EIS_FirmAddress->text();

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
         

void EIS_Object::setCurrentFileName(char *cfile)
{
    current_csv_filename = cfile;
}


void EIS_Object::setCurrentEmployeeTable(QTableWidget *table)
{
    EIS_employeeTable = table;   
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

    for (row = 0; row < EIS_numEmployees; row++) {
        for (int col = 0; col < 15; col++) {
	    fItem = EIS_employeeTable->item(row,col);
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



