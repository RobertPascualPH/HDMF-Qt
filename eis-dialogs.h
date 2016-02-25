/* ******************************************************************
 * eis-dialogs.h
 *
 * Declaration for the various dialogs we might care to use
 * with the application.
 *
 * Written by: Robert Pascual, 2016
 *
 * *****************************************************************/
#include "eis-common.h"

#ifndef EIS_DIALOGS_HEADER
#define EIS_DIALOGS_HEADER

class csv_Dialog : public QDialog
{
    Q_OBJECT


    public:
        csv_Dialog();    // Constructor
                        // Is a destructor required?
        void dump_To_HDMF_CSV(QString); // Create CRF copy as CSV File.
};

#endif

