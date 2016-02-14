/* ***********************************************************************
 * create-crf.c++
 *
 * This contains the implementation of the function
 * EIS_Object::Create_CRF().
 *
 * Written by: Robert Pascual
 *
 * Revision History:
 *
 * $Id$
 *
 * $Log$
 *
 * ***********************************************************************/
#include "eis-common.h"

////////////////////////////////////////////////////////////////////////////
//
// A. The format of the input fields for this version of Create_CRF()
//    should be the following:
//     0. ID Number
//     1. Last Name
//     2. First Name
//     3. Middle Name
//     4. SSS Number
//     5. PHilhealth Number
//     6. HDMF Number
//     7. Gross Monthly Salary
//     8. SSS Employee Contribution Amount
//     9. SSS Employer Contribution Amount
//    10. EC Contribution (ER Only)
//    11. PhilHealth EE Contribution
//    12. PhilHealth ER Contribution
//    13. HDMF EE Contribution
//    14. HDMF ER Contribution
//
////////////////////////////////////////////////////////////////////////////



void EIS_Object::Create_CRF(void)
{
     char lineBuffer[16384];
     QFile outputFile("temp.tex");
     QString firmName    = EIS_Firm_Name->text(); // Does this make a copy?
     QString firmHDMFNum = EIS_HDMF_Number->text();
     QString firmAddress = EIS_Firm_Address->text();
     QString dateCovered = EIS_Coverage_Date->text();

     outputFile.open(QIODevice::WriteOnly | QIODevice::Text);


     /////////////////////////////////////////////////////
     //                                                 //
     // Write out the header for the TeX file.          //
     //                                                 //
     /////////////////////////////////////////////////////

     sprintf(lineBuffer, "%s", 
         "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
	 "%% This document is designed to be printed on legal sized paper in landscape mode.\n"
	 "%% Instructions for printing.\n"
	 "%%    $ latex temp\n"
	 "%%    $ xdvi -paper legalr temp\n"
	 "%%\n"
	 "%% Notes: \n"
	 "%%     1. The \\layout command produces a layout of the document.\n"
	 "%%\n"
	 "%%\n"
	 "%%\n"
	 "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
	 "\\documentclass[10pt,legalpaper,landscape]{article}\n"
	 "\\usepackage{palatino}\n"
	 "\\usepackage{layout}\n"
	 "\\setlength{\\textheight}{7.75in}\n"
	 "\\setlength{\\textwidth}{11in}\n"
	 "\\setlength{\\voffset}{-1.2in}\n"
	 "\\setlength{\\hoffset}{-3.5in}\n"
	 "\\begin{document}\n"
	 "\\pagestyle{empty}\n"
	 "%% \\layout\n"
	 "%% \\vfill\\eject\n"
	 "\\centerline{\\vtop{%%\n"
	 "    \\hsize = 4 in\n"
	 "    \\leftline{\\bf\\LARGE MEMBERS CONTRIBUTION}\n"
	 "    \\vspace{10pt}\n"
	 "    \\leftline{\\bf\\LARGE REMITTANCE FORM (MCRF)}\n"
	 "}\n"
	 "\\hskip 1 in\n");
     outputFile.write(lineBuffer, strlen(lineBuffer));


     // The following code will print the HDMF Number of the
     // Employer.
    
     sprintf(lineBuffer, 
	 "\\vtop{%%%%\n"
	 "    \\hsize = 4 in\n"
	 "    \\leftline{\\bf Pag-IBIG EMPLOYER'S ID NUMBER}\n"
	 "    \\vspace{8pt}\n"
	 "    \\leftline{\\bf\\LARGE %s}\n"
	 "}}\n\n", qPrintable(firmHDMFNum));
     outputFile.write(lineBuffer, strlen(lineBuffer));


     // Prepare the LaTeX table. Print prologue commands for the LaTeX tabular
     // environment.
     //
     sprintf(lineBuffer, "%s",
	 "\\vspace {0.15 in}\n"
	 "\n"
	 "\n"
	 "\\renewcommand{\\arraystretch}{1.5}\n"
	 "\n"
	 "\\begin{center}\n"
	 "\\begin{tabular}[t]{|c|c|c|c|c|c|c|}\n"
	 "\\hline\n");
     outputFile.write(lineBuffer, strlen(lineBuffer));


     // Print the name and address of the employer.
     //
     sprintf(lineBuffer, 
	 "\\multicolumn{7}{|l|}{Employer Name: \\bf\\large %s}\\\\\n"
	 "\\hline\n"
	 "\\multicolumn{7}{|l|}{Employer Addr: \\bf %s}\\\\\n"
	 "\\hline\n",
         qPrintable(firmName.toUpper()), qPrintable(firmAddress));
     outputFile.write(lineBuffer, strlen(lineBuffer));


     // Continue with the printing
     //
     sprintf(lineBuffer, "%s",
	 "\\bf HDMF Num & \\bf Name of Members \n"
	 "      & \\parbox{0.75in}{\\strut\\centering \\bf Period\\\\ Covered\\strut}\n"
	 "	& \\parbox{1in}{\\strut\\centering \\bf Monthly\\\\Compensation\\strut}\n"
         "      & \\bf EE & \\bf ER & \\bf Total\\\\\n"
	 "\\hline\n");
     outputFile.write(lineBuffer, strlen(lineBuffer));


    //////////////////////////////////////////////////////////////////
    // Now we print the employee information.                       //
    //////////////////////////////////////////////////////////////////
    int numRows    = EIS_Employee_Table -> rowCount();
    // int numColumns = EIS_Employee_Table -> columnCount();
 
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

            printf("Now processing the name: %s %s %s\n",
                 qPrintable(lastName), qPrintable(firstName), qPrintable(middleName));

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

            sprintf(lineBuffer,
                "%s  & \\textbf{%s}, %s %s  & %s  & %10.2f  & %10.2f  & %10.2f & %10.2f\\\\\n",
                qPrintable(HDMFIdent),
                qPrintable(lastName),
                qPrintable(firstName),
                qPrintable(middleName),
                qPrintable(dateCovered),
                salaryAmount,
                EEContrib, ERContrib, EEContrib + ERContrib);

            outputFile.write(lineBuffer, strlen(lineBuffer));
        }
     }

     sprintf(lineBuffer, 
         "\\hline\n\\multicolumn{4}{|l|}{\\bf GRAND TOTAL} & & & \\textbf{%10.2f}\\\\\n",
         ERContrib_Sum + EEContrib_Sum);
     outputFile.write(lineBuffer, strlen(lineBuffer));


     // Set the epilogue of the LaTeX table
     //
     sprintf(lineBuffer, "%s", 
	"\\hline\n"
	"\n"
	"\\end{tabular}\n"
	"\\end{center}\n"
	"\n"
	"\\vfill\n"
	"\n"
	"\\centerline{\\bf EMPLOYER CERTIFICATION}\n"
	"\n"
	"\\vspace {0.2 in}\n"
	"\n"
	"I hereby certify under pain of perjury that\n"
	"the information given and the statements made herein are true and correct to the\n"
	"best of my knowledge and belief. I further certify that my signature appearing\n"
	"herein is genuine and authentic.\n"
	"\n"
	"\\vspace {0.3 in}\n"
	"\n"
	"\\centerline{%%\n"
	"    \\vtop{%%\n"
	"	\\hsize = 3 in\n"
	"	\\centerline{\\underbar{\\hskip 1.5in}}\n"
	"	\\centerline{Authorized Representative}\n"
	"    }\n"
	"    \\vtop{%%\n"
	"	\\hsize = 3 in\n"
	"	\\centerline{\\underbar{\\hskip 1.5in}}\n"
	"	\\centerline{Designation}\n"
	"    }\n"
	"    \\vtop{%%\n"
	"	\\hsize = 3 in\n"
	"	\\centerline{\\underbar{\\hskip 1.5 in}}\n"
	"	\\centerline{Date}\n"
	"    }\n"
	"}\n"
	"\n"
	"\n"
	"\\end{document}\n");

     outputFile.write(lineBuffer, strlen(lineBuffer));

     outputFile.close();


}

