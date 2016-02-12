/* ******************************************************************
 * emptabfunctions.c++
 *
 * Functions which operate on the QTableWidget EmployeeTable.
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



