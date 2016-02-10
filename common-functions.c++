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

///////////////////////////////////////////////////////////////////
// CONSTRUCTORS
///////////////////////////////////////////////////////////////////

EIS_Object::EIS_Object()
// The constructor has assumed the duties of allocating
// the QWidgets and initializing their values.
{

    // printf("Entering default constructor.");

    strcpy(EIS_Employee_CSV_File, DEFAULT_EMPLOYEE_FILE);
    strcpy(EIS_Firm_CSV_File, DEFAULT_INFO_FILE);
    EIS_Employee_Table = 0x00;
    EIS_Num_Employees = 0;
    EIS_HDMF_Number = 0x00;
    EIS_Firm_Name = 0x00;
    EIS_Firm_Address = 0x00;
    EIS_Coverage_Date = 0x00;
    flags = 0;
    set_Firm_Info();

    // printf("Exiting default constructor.");
}





////////////////////////////////////////////////////////////////
// MUTATORS
////////////////////////////////////////////////////////////////

void EIS_Object::set_Current_File_Name(char *cfile)
{
    strcpy(EIS_Employee_CSV_File, cfile);
}


void EIS_Object::set_Firm_Info(void)
{
     QFile fin(tr(DEFAULT_INFO_FILE));
     char buffer[1024];

     if (fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
         flags = 1;     // Toggle the flag.
         fin.readLine(buffer, 1024);
         QString QStringBuffer(buffer);      // Hope this works.
         Firm_Info_String_List = QStringBuffer.split("|"); 
     }
      
     fin.close();
     // printf("Done!\n");
}

void EIS_Object::set_Employee_Table(QTableWidget *empTbl)
// This version uses the methods of the QString library.
//
{
    EIS_Employee_Table = empTbl;

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


    /////////////////////////////////////////////////////////////////////////
    //
    // Populate the QTableWidget with the contents of DEFAULT_EMPLOYEE_FILE
    //
    /////////////////////////////////////////////////////////////////////////

    QFile eTable(DEFAULT_EMPLOYEE_FILE);
    int row = 0;
    int sizeRead;
    char inbuffer[2048];

    if (eTable.open(QIODevice::ReadOnly | QIODevice::Text)) {
        row = 0;
        while ((sizeRead = eTable.readLine(inbuffer, 2048)) > 0) {
            QString readBuffer(inbuffer);    // Read one line from file.
            readBuffer.remove(QChar('\n'));
            QStringList fieldList = readBuffer.split("|");
            
	    for (int column = 1; column < fieldList.size(); ++column) {
	        QTableWidgetItem *newItem = new QTableWidgetItem(fieldList[column]);
		EIS_Employee_Table->setItem(row, column-1, newItem);
	    }
            ++row;
        }
        eTable.close();
    }
    EIS_Num_Employees = row;
}


void EIS_Object::set_HDMFN_Text_Field(QLineEdit *tl)
{
    EIS_HDMF_Number = tl;
    if (flags == 1) {
        EIS_HDMF_Number->setText(Firm_Info_String_List[0]);
    }
}


void EIS_Object::set_Firm_Name_Text_Field(QLineEdit *tl)
{
    EIS_Firm_Name = tl;
    if (flags == 1) {
        EIS_Firm_Name->setText(Firm_Info_String_List[1]);
    }
}



void EIS_Object::set_Firm_Address_Text_Field(QLineEdit *tl)
{
    EIS_Firm_Address = tl;
    if (flags == 1) {
        EIS_Firm_Address->setText(Firm_Info_String_List[2]);
    }
}



void EIS_Object::set_Coverage_Date_Text_Field(QLineEdit *tl)
{
    EIS_Coverage_Date = tl;
}




/////////////////////////////////////////////////////////////
// ACCESSORS
/////////////////////////////////////////////////////////////

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


QLineEdit * EIS_Object::get_Firm_Address(void)
{
    return EIS_Firm_Address;
}


QLineEdit * EIS_Object::get_Coverage_Date(void)
{
    return EIS_Coverage_Date;
}


void EIS_Object::save_Firm_Info(void)
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
}

void EIS_Object::Create_CRF(void)
// Creates the HDMF Contributions Report Form.
// This function is not yet functioning.
//
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
}


void EIS_Object::Save_Table(void)
{
    QTableWidgetItem *fItem;
    QString qstr;
    char *out_Buffer;
    QByteArray msgStr;
    int row;


    QFile fout(EIS_Employee_CSV_File);
    fout.open(QIODevice::WriteOnly | QIODevice::Text);

    out_Buffer = (char *) malloc(2048*sizeof(char));
    if (out_Buffer == NULL) return;

    out_Buffer[0] = 0x00;    // Initialize the output buffer.
                             // Believe me, this is needed because if
                             // There is nothing on the Employee Table then we
                             // want the output file to be empty too.
                        
    for (row = 0; ; row++) {
        out_Buffer[0] = 0x00;    // Make sure the string is empty.
        if ((fItem = EIS_Employee_Table->item(row,0)) == 0) {
            // If column 0 does not have an associated QTableWidget
            // then we're done.
             break;
        }
        else {
            // Check if the QTableWidgetItem in column 0 is not
            // empty.
            qstr = fItem->text();
            qstr.remove(QChar('\n'));
            msgStr = qstr.toLatin1();
            sprintf(out_Buffer, "%s", qPrintable(msgStr));
            if (strlen(out_Buffer) == 0) break;  // Break if empty string.

            // Ah, we are good to go.
            //
            sprintf(out_Buffer, "%d|", row+1);
            for (int col = 0; col < 15; col++) {
		fItem = EIS_Employee_Table->item(row,col);
		if (fItem != 0x00) {
		    qstr = fItem->text();
                    qstr.remove(QChar('\n'));
		    msgStr = qstr.toLatin1();
                    strcat(out_Buffer, qPrintable(qstr));
		    // strcat(out_Buffer, msgStr.data());
		}
                strcat(out_Buffer, "|");
            }
            strcat(out_Buffer, "\n");
            // printf("The contents of out_Buffer is %s with length %ld\n", 
            //       out_Buffer, strlen(out_Buffer));
	    fout.write(out_Buffer, strlen(out_Buffer));
       }
    }
    fout.close();
    free(out_Buffer);
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


