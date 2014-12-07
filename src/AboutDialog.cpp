

#include "AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent, const char *name)
  : QDialog(parent, name) {

  // Create a font for smaller text in the dialog.
  //   QFont courier("Courier", 10);

  // Set the caption:
  setCaption(tr("About AlarmME"));

  // Set the icon.
  setIcon( QPixmap::fromMimeSource("luu.png") );

  // Set the about strings.
  setAboutStrings();

  // Create the push buttons.
  closeButton_ = new QPushButton(QString(tr("&Close")), this);
  showLicenseButton_ = new QPushButton(QString(tr("&Show License")), this);

  // Create the labels.
  aboutPicLabel_         = new QLabel(this);
  aboutInfoLabel_        = new QLabel(this);
  aboutLicenseLabel_     = new QLabel(this);
  aboutDescriptionLabel_ = new QLabel(this);

  // Set this label as rich text.
  aboutDescriptionLabel_->setTextFormat(Qt::RichText);

  // Set the picture label.
  aboutPicLabel_->setPixmap( QPixmap::fromMimeSource("frog.png") );

  // Set the text for the other labels.
  aboutInfoLabel_       ->setText(aboutInfoStr_);
  aboutLicenseLabel_    ->setText(aboutLicenseStr_);
  aboutDescriptionLabel_->setText(aboutDescriptionStr_);

  // Set the font text for the labels.
  //   aboutInfoLabel_       ->setFont(courier);
  //   aboutLicenseLabel_    ->setFont(courier);
  //   aboutDescriptionLabel_->setFont(courier);

  // Set up the layout of the dialog.  

  // This is how the layouts are set up.  There are two layouts within
  // the main layout.  They are the top layout and the bottom layout.
  // The top layout contains most of About stuff.  The bottom layout
  // contains the CLOSE button.  The top layout contains a left layout
  // and a right layout.  The left layout contains just icon label.
  // The right layout actually contains the text and raw meat of the
  // About info.  There a layout embedded in the right layout, it is
  // the licenseLayout, which is used so we can add a button to view
  // the license inside the right layout.

  // This layout is a horizontal layout that contains the license
  // label and a button to display the license.
  QHBoxLayout *licenseLayout = new QHBoxLayout;
  licenseLayout->addWidget(aboutLicenseLabel_);
  licenseLayout->addWidget(showLicenseButton_);

  QVBoxLayout *leftLayout = new QVBoxLayout;
  leftLayout->addSpacing(20);
  leftLayout->addWidget(aboutPicLabel_);
  leftLayout->addStretch(1);
  
  QVBoxLayout *rightLayout = new QVBoxLayout;
  rightLayout->addSpacing(20);  
  rightLayout->addWidget(aboutDescriptionLabel_);
  rightLayout->addSpacing(30);
  rightLayout->addWidget(aboutInfoLabel_);
  rightLayout->addSpacing(30);
  rightLayout->addLayout(licenseLayout);
  rightLayout->addSpacing(30);

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addLayout(leftLayout);
  topLayout->addSpacing(20);
  topLayout->addLayout(rightLayout);
  
  QHBoxLayout *bottomLayout = new QHBoxLayout;
  bottomLayout->addWidget(closeButton_);
  
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(topLayout);
  mainLayout->addLayout(bottomLayout);


  // Connect the signals to slots for the buttons.
  connect(closeButton_, SIGNAL(clicked()),
	  this, SLOT(closeButtonClicked()));
  connect(showLicenseButton_, SIGNAL(clicked()),
	  this, SLOT(showLicenseButtonClicked()));
  
}

void AboutDialog::showLicenseButtonClicked() {
  LicenseDialog licenseDialog(this);
  licenseDialog.exec();
}

void AboutDialog::closeButtonClicked() {
  hide();
}


// Helper function for the constructor to make it clearer to read what
// the constructor is doing.
void AboutDialog::setAboutStrings() {

  // About:  License.  (As Normal Text).
  aboutLicenseStr_ = tr("License: GNU General Public License (GPL).");


  // About:  General info about usage.  (As Normal Text).
  aboutInfoStr_ = QString("AlarmME") + tr(" version ") 
    + QString(VERSION) + tr(", Copyright (C) ") + QString(YEAR)
    + QString(" Ryan Luu.\n") 
    + QString("AlarmME") + tr(" comes with ABSOLUTELY NO WARRANTY.\n")
    + tr("This is free software, and you are welcome \n")
    + tr("to redistribute it under certain conditions.\n")
    + tr("Please click on the 'View License' button below \n")
    + tr("to view license.");


  // About:  Application description.  (as Rich Text)
  aboutDescriptionStr_ = 
    tr("AlarmME stands for Alarm Made Easy.  It is a<br />")
    + tr("clock alarm which wakes you up by opening an audio <br />")
    + tr("or video file.  It is designed so you can (very) quickly<br />")
    + tr("reset the alarm for additional snooze sleep time.  The<br />")
    + tr("sourceforge website for this project is: <br />")
    + QString("<a href=\"http://sourceforge.net/projects/alarmme/\">")
    + QString("http://sourceforge.net/projects/alarmme/ </a><br />");
}


//////////////////////////////////////////////////////////////////////////////
// End of file
//////////////////////////////////////////////////////////////////////////////
