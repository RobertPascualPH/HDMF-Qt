/* ********************************************************************
 * eis-dialogs.c++
 *
 * Implementation of the various EIS Dialogs.
 *
 * Written by: 
 *     Robert Pascual
 *     rtonypascual@gmail.com
 *     2016
 *
 * *******************************************************************/
#include "eis-common.h"
#include "eis-dialogs.h"


csv_Dialog::csv_Dialog(void)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonBox = new QHBoxLayout();

    QLabel *message 
        = new QLabel("<h2>Do you want to create a CSV\n"
                     "copy of this CRF Report?</h2>");

    QPushButton *NoButton = new QPushButton("No");
    QObject::connect(NoButton, SIGNAL(clicked()), this, SLOT(reject()));

    QPushButton *YesButton = new QPushButton("Yes");
    QObject::connect(YesButton, SIGNAL(clicked()), this, SLOT(accept()));

    buttonBox->addStretch();
    buttonBox->addWidget(NoButton);
    buttonBox->addWidget(YesButton);

    mainLayout->addWidget(message);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonBox);
}

