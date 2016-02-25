/* ******************************************************************
 * emptabfunctions.c++
 *
 * Functions which operate on the QTableWidget EmployeeTable.
 *
 * Robert Pascual 2016
 *
 * Revision History:
 *
 * $Id$
 *
 * $Log$
 *
 * ******************************************************************/
#include "eis-common.h"


void EIS_Object::Clear_Employee_Table(void)
// Clear the QTableWidget EmployeeTable.
{
    int rowCount     = EIS_Employee_Table->rowCount();
    int columnCount  = EIS_Employee_Table->columnCount();

    for (int k = 0; k < rowCount; ++k) {
        for (int j = 0; j < columnCount; ++j) {
	    QTableWidgetItem * fItem = EIS_Employee_Table->item(k,j);
	    if (fItem != 0) {
                EIS_Employee_Table->removeCellWidget(k,j);
                delete fItem;
            }
        }
    }  
}


void EIS_Object::read_File_To_Table(QString rfile)
{
     QFile eTable(qPrintable(rfile));
     int row = 0;
     int sizeRead;
     char inbuffer[2048];

     if (eTable.open(QIODevice::ReadOnly | QIODevice::Text)) {
     this->Clear_Employee_Table();
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


void EIS_Object::dump_To_HDMF_CSV(QString filepath)
// Dump information contained in the EIS Object to a CSV file
// with the contents of the CSV file satisfying the requirements of the
// HDMF only.
//
{
    QFile outf(qPrintable(filepath));
    char lineBuffer[2048];

    QString firmName    = EIS_Firm_Name->text();
    QString firmHDMFNum = EIS_HDMF_Number->text();
    QString firmAddress = EIS_Firm_Address->text();
    QString dateCovered = EIS_Coverage_Date->text();

    if (outf.open(QIODevice::WriteOnly | QIODevice::Text)) {
       sprintf(lineBuffer, "%s\n%s\n%s\n", 
           qPrintable(firmName),
           qPrintable(firmHDMFNum),
           qPrintable(firmAddress));
       outf.write(lineBuffer, strlen(lineBuffer));
    }

    // The following lines were copy pasted from create-crf.c++ and
    // modified. The function Create_CRF() from create-crf.c++
    // is well debugged. I hope I do not introduce new bugs
    // this modification.
    //
    int numRows    = EIS_Employee_Table -> rowCount();

    QTableWidgetItem *fItem;
    QString qstr;
    QByteArray tempStr;
    QString lastName, firstName, middleName;
    QString SSSIdent, HDMFIdent;
    double EEContrib, ERContrib;
    double EEContrib_Sum, ERContrib_Sum;
    double salaryAmount;


    EEContrib_Sum = ERContrib_Sum = 0;

    for (int row = 0; row < numRows; row++) {
        fItem = EIS_Employee_Table->item(row,0);   // Location of last name
    
        // Check if row has valid data by looking at the last name.
        //
        if (fItem != 0x00 && strlen(qPrintable(fItem->text())) != 0) { 

            lastName   = fItem->text();
            firstName  = (EIS_Employee_Table->item(row,1))->text();
            middleName = (EIS_Employee_Table->item(row,2))->text();
            HDMFIdent  = (EIS_Employee_Table->item(row,5))->text();

            if (strlen(qPrintable(HDMFIdent)) == 0) {
                SSSIdent   = (EIS_Employee_Table->item(row,3))->text();
                HDMFIdent  = SSSIdent;
            }

            // Read in the Monthly Compensation.
            //
            qstr  = (EIS_Employee_Table->item(row,6))->text();
            qstr = qstr.remove(",");
            salaryAmount = strtod((qstr.toLatin1()).data(), NULL);

            // Read in the EE Contribution
            //
            qstr  = (EIS_Employee_Table->item(row,12))->text();
            qstr = qstr.remove(",");
            EEContrib = strtod((qstr.toLatin1()).data(), NULL);
            EEContrib_Sum += EEContrib;

            // Read in the ER Contribution
            //
            qstr  = (EIS_Employee_Table->item(row,13))->text();
            qstr = qstr.remove(",");
            ERContrib = strtod((qstr.toLatin1()).data(), NULL);
            ERContrib_Sum += ERContrib;

            // Now print. Note use of commas as separators.
            // We want the CSV file to actually be a Comma Separated Values
            // file. Whoever thought of using commas as a separator in CVS
            // files is an idiot.

            sprintf(lineBuffer,
                "%s|%s, %s %s|%s|%10.2f|%10.2f|%10.2f|%10.2f\n",
                qPrintable(HDMFIdent),
                qPrintable(lastName.toUpper()),
                qPrintable(firstName),
                qPrintable(middleName),
                qPrintable(dateCovered),
                salaryAmount,
                EEContrib, ERContrib, EEContrib + ERContrib);

            outf.write(lineBuffer, strlen(lineBuffer));
        } // end if
     } // end for (row)

     // Now print sum
     sprintf(lineBuffer, "||||||%10.2f\n", ERContrib_Sum + EEContrib_Sum);
     outf.write(lineBuffer, strlen(lineBuffer));

     outf.close();      
}

