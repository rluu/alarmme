//////////////////////////////////////////////////////////////////////////////
//
// FILE:   SetAlarmDialog.cpp
//
// DESCRIPTION:
//
// AUTHOR:  Ryan Luu
// 
//
//////////////////////////////////////////////////////////////////////////////
//
// UPDATE: well, now i have to add functionality for setting alarm media
// player's exe and the alarm media player's commandline flags.  i
// plan on doing that by resizing the dialog size whenever the person
// clicks on the show advanced optiosn button.  still trying to figure
// out how to do that...
//
// UPDATE 2/20/2005: I created the widgets and added the layouts
// and setup for exe filename and cmd-line options line edits.  I need
// to implement the browse button for it.  also, i need to implement
// the new constructor parameter that's the index for which alarm
// media player chosen by default when you open/start it up.  At
// startup the media player with this index will get
// 'clicked'/selected.  This variable needs to be stored when saved
// and loaded when loaded.  also, i need to create callbacks for the
// selection of each of the alarm media players.  The callbacks would
// update the line-edit fields for the media player filename and the
// media player commandline flags which are located directly to the
// right of it (these values would need to be hardcoded somewhere).
// Also the callbacks would need to disable the ability to edit these
// line-edits and disable the ability to click on the browse for it if
// the media player is not 'Custom'.  Also, I need to implement the
// 'Show advanced'/'Hide advanced' button eventually.  It will do this
// by resizing the dialog.  Default size is minimal size without the
// advanced settings.
//
// UPDATE:  2/26/2005:  I finished everything except for resizing the dialog.
// I still don't know how to get it to work.  The bottom of the 
// constructor has 
//
// NOTES: I use these precompiler declarations to determine the
// default application to use:
//
// #ifdef Q_OS_UNIX
// #elif Q_OS_WIN
// #endif
//
//////////////////////////////////////////////////////////////////////////////

#include <map>
using namespace std;
#include "SetAlarmDialog.h"

//////////////////////////////////////////////////////////////////////////////

// Precondition: The first 4 parameters to this function must be
// non-null and valid entries.

// Note: alarmMediaExe and alarmMediaExeOps only applies to the
// current custom media exe settings.
SetAlarmDialog::SetAlarmDialog(QTime *alarmTime,
			       unsigned int *alarmInterval, 
			       QString *alarmMessage,
			       QString *alarmFile,
			       QString *alarmMediaExe,
			       QString *alarmMediaExeOps,
			       unsigned int *alarmMediaExeIndex,
			       QWidget *parent, const char *name)
  : QDialog(parent, name), 
    saveDataFilename("alarmSave.dat"),
    showAdvancedString_(tr("Show &advanced")),
    hideAdvancedString_(tr("Hide &advanced"))
{

  // Let's make sure none of these pointers are NULL:
  if(alarmTime == 0 || alarmInterval == 0 || alarmMessage == 0 || 
     alarmFile == 0 || alarmMediaExe == 0 || alarmMediaExeOps == 0 ||
     alarmMediaExeIndex == 0) {
    QString errorMessage = tr("Pointers passed into constructor ");
    errorMessage += tr("'SetAlarmDialog()' are NULL.  ") + QString(__FILE__);

    QMessageBox::critical(this, QString(tr("Code Error")), errorMessage);
    return;
  }

  // Copy over the pointers for the data.
  alarmTime_ = alarmTime;
  alarmInterval_ = alarmInterval;
  alarmMessage_ = alarmMessage;
  alarmFile_ = alarmFile;
  alarmMediaExe_ = alarmMediaExe;
  alarmMediaExeOps_ = alarmMediaExeOps;
  alarmMediaExeIndex_ = alarmMediaExeIndex;

  // This is the value of the custom choice which is always saved.
  // This may or may not be == to alarmMediaExe_ or alarmMediaExeOps_.
  alarmMediaCustomExe_ = new QString;
  alarmMediaCustomExeOps_ = new QString;

  // Set the caption:
  setCaption(tr("Set Alarm Properties"));

  // Set the icon.
  setIcon( QPixmap::fromMimeSource("clockIcon.png") );

  // Create the stl maps for the hardcoded default values for different exe's.
  initializeMediaExeMaps();

  // Now create and initialize all the widgets.  Start with the little widgets.

  // These are the labels used in the dialog.
  alarmTimeLabel_ = new QLabel(tr("Alarm &Time: "), this);
  colonLabel_ = new QLabel(tr(":"), this);
  alarmIntervalLabel_ = new QLabel(tr("Re-alarm &Interval (sec): "), this);
  alarmMsgLabel_ = new QLabel(tr("Alarm &Message: "), this);
  alarmFileLabel_ = new QLabel(tr("Alarm Audio/Video &File: "), this);

  // Two spin boxes for the time.
  hourSpinBox_ = new QSpinBox(this);
  minuteSpinBox_ = new QSpinBox(this);
  // Set their ranges:
  hourSpinBox_->setMinValue(1);
  hourSpinBox_->setMaxValue(12);
  hourSpinBox_->setWrapping(true);
  minuteSpinBox_->setMinValue(0);
  minuteSpinBox_->setMaxValue(59);
  minuteSpinBox_->setWrapping(true);

  // And the radio buttons for the am/pm setting.
  ampmRadioButtons_ = 
    new QButtonGroup(1    /* strip */, Qt::Vertical /* Orientation */,
		     this /* parent */);
  ampmRadioButtons_->setRadioButtonExclusive(true);
  // Add the buttons to the button group.
  amTime_ = new QRadioButton(tr("AM"), ampmRadioButtons_);
  pmTime_ = new QRadioButton(tr("PM"), ampmRadioButtons_);

  // Create the spin box for the re-alarm interval.
  alarmIntervalSpinBox_ = new QSpinBox(this);
  alarmIntervalSpinBox_->setMinValue(0);
  alarmIntervalSpinBox_->setMaxValue(3600);  // Max of 1 hour should be enough.

  // Create the line edit fields.
  alarmMessageLineEdit_ = new QLineEdit(this);
  alarmFileLineEdit_ = new QLineEdit(this);
  
  // Create the buttons.
  okayButton_ = new QPushButton(tr("&Okay"), this);
  cancelButton_ = new QPushButton(tr("&Cancel"), this);
  browseAlarmFileButton_ = new QPushButton(tr("&Browse"), this);
  advancedButton_ = new QPushButton(tr("Show &advanced"), this);


  // Create the separator to separate between the normal settings and
  // the advanced settings.
  QFrame *horizontalSeparator = new QFrame(this);
  horizontalSeparator->setFrameStyle(QFrame::HLine | QFrame::Plain);

  ////////////////////////////////////////////////////////////////////////////
  // Now create objects for the advanced settings.  
  // This includes a button group and radio buttons for the media
  // player type to use for the alarm.
  
  mediaPlayerRadioButtons_ = 
    new QButtonGroup(1    /* strip */, Qt::Horizontal /* Orientation */,
		     tr("Alarm Media Player") /* title */, this /* parent */);
  mediaPlayerRadioButtons_->setRadioButtonExclusive(true);
  // Add the radio buttons to the button group.
  // Define a custom media player.
  customRadioButton_ = new QRadioButton(tr("Custom"), 
					mediaPlayerRadioButtons_);
  // These radio buttons define the windows-based media players.
  wmpRadioButton_ = new QRadioButton(tr("Windows Media Player"), 
				     mediaPlayerRadioButtons_);
  winampRadioButton_ = new QRadioButton(tr("Winamp"), 
					mediaPlayerRadioButtons_);
  // These radio buttons define the unix-based media players.
  xmmsRadioButton_ = new QRadioButton(tr("XMMS"), mediaPlayerRadioButtons_);
  xineRadioButton_ = new QRadioButton(tr("Xine"), mediaPlayerRadioButtons_);
  mplayerRadioButton_ = new QRadioButton(tr("MPlayer"), 
					 mediaPlayerRadioButtons_);
  
  // Create the labels.
  alarmMediaExeLabel_ = new QLabel(tr("Media Player full filename:  "), this);
  alarmMediaExeOpsLabel_ = new QLabel(tr("Media Player command-line options:"),
				      this);
  // Create the text-edit fields.
  alarmMediaExeLineEdit_ = new QLineEdit(this);
  alarmMediaExeOpsLineEdit_ = new QLineEdit(this);

  // Create a browse button.
  browseMediaExeButton_ = new QPushButton("Browse", this);

  ////////////////////////////////////////////////////////////////////////////

  // Now that everything is created, let's set the buddies of the labels.
  alarmTimeLabel_->setBuddy(hourSpinBox_);
  alarmIntervalLabel_->setBuddy(alarmIntervalSpinBox_);
  alarmMsgLabel_->setBuddy(alarmMessageLineEdit_);
  alarmFileLabel_->setBuddy(alarmFileLineEdit_);
    
  // The way we will do this is as follows.  For the main alarm
  // settings, there will be 3 main vertical layout boxes.  Left one
  // will have the labels, middle one will have the setting widgets,
  // and the right one will have the okay and cancel buttons.  For the
  // advanced alarm settings, there will be layout laying horizontally
  // below the 3 main vertical layout boxes above.  These 4 layouts
  // will be put into the 'main' layout.

  // Left layout.
  QVBoxLayout *leftLayout = new QVBoxLayout;
  leftLayout->addSpacing(10);
  leftLayout->addWidget(alarmTimeLabel_);
  leftLayout->addSpacing(12);
  leftLayout->addWidget(alarmIntervalLabel_);
  leftLayout->addWidget(alarmMsgLabel_);
  leftLayout->addWidget(alarmFileLabel_);
			
  // Top Middle layout.  (The time-setting stuff)
  QHBoxLayout *topMiddleLayout = new QHBoxLayout;
  topMiddleLayout->addWidget(hourSpinBox_);
  topMiddleLayout->addWidget(colonLabel_);
  topMiddleLayout->addWidget(minuteSpinBox_);
  topMiddleLayout->addWidget(ampmRadioButtons_);
  // Bottom Middle layout.  (Realarm Interval rate, alarm msg, alarm file).
  QVBoxLayout *bottomMiddleLayout = new QVBoxLayout;
  bottomMiddleLayout->addWidget(alarmIntervalSpinBox_);
  bottomMiddleLayout->addWidget(alarmMessageLineEdit_);
  bottomMiddleLayout->addWidget(alarmFileLineEdit_);
  
  // Middle layout.
  QVBoxLayout *middleLayout = new QVBoxLayout;
  //  middleLayout->setSpacing(6);
  middleLayout->addLayout(topMiddleLayout);
  middleLayout->addLayout(bottomMiddleLayout);

  // Right layout.
  QVBoxLayout *rightLayout = new QVBoxLayout;
  rightLayout->addWidget(okayButton_);
  rightLayout->addWidget(cancelButton_);
  rightLayout->addWidget(advancedButton_);
  //   rightLayout->addStretch(1);
  rightLayout->addWidget(browseAlarmFileButton_);

  // Assemble the layout for the non-advanced alarm settings.
  QHBoxLayout *nonAdvancedLayout = new QHBoxLayout;
  nonAdvancedLayout->setMargin(11);
  nonAdvancedLayout->setSpacing(6);
  nonAdvancedLayout->addLayout(leftLayout);
  nonAdvancedLayout->addLayout(middleLayout);
  nonAdvancedLayout->addLayout(rightLayout);


  // Advanced settings left layout.   (contains the radio buttons).
  QVBoxLayout *advancedLeftLayout = new QVBoxLayout;
  advancedLeftLayout->addWidget(mediaPlayerRadioButtons_);
  // Advanced settings middle layout. (contains the labels and linedit boxes.)
  QVBoxLayout *advancedMiddleLayout = new QVBoxLayout;
  advancedMiddleLayout->addSpacing(5);
  advancedMiddleLayout->addWidget(alarmMediaExeLabel_);
  advancedMiddleLayout->addWidget(alarmMediaExeLineEdit_);
  advancedMiddleLayout->addWidget(alarmMediaExeOpsLabel_);
  advancedMiddleLayout->addWidget(alarmMediaExeOpsLineEdit_);
  advancedMiddleLayout->addStretch(1);
  // Advanced settings right layout.  (contains the browse button).
  QVBoxLayout *advancedRightLayout = new QVBoxLayout;
  advancedRightLayout->addSpacing(12);
  advancedRightLayout->addWidget(browseMediaExeButton_);
  advancedRightLayout->addStretch(1);

  // Create the layout that will encompass the advanced alarm settings.
  QHBoxLayout *advancedLayout = new QHBoxLayout;
  advancedLayout->addLayout(advancedLeftLayout);
  advancedLayout->addLayout(advancedMiddleLayout);
  advancedLayout->addLayout(advancedRightLayout);

  // Main layout.  Contains the non-advanced alarm settings + the
  // advanced alarm settings.
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(nonAdvancedLayout);
  mainLayout->addSpacing(5);
  mainLayout->addWidget(horizontalSeparator);
  mainLayout->addSpacing(5);
  mainLayout->addLayout(advancedLayout);

  // At this point, everything should be set up in the SetAlarmDialog
  // as far as widget and layout positions.

  // Let's connect signals and slots.  (buttons)
  connect(okayButton_, SIGNAL(clicked()), this, SLOT(doneClicked()));
  connect(browseAlarmFileButton_, SIGNAL(clicked()), this, 
	  SLOT(browseAlarmFileClicked()));
  connect(browseMediaExeButton_, SIGNAL(clicked()), this, 
	  SLOT(browseMediaExeClicked()));
  connect(cancelButton_, SIGNAL(clicked()), this, SLOT(cancelClicked()));
  connect(advancedButton_, SIGNAL(clicked()), this, 
	  SLOT(showAdvancedClicked()));
  connect(minuteSpinBox_, SIGNAL(valueChanged(int)), 
	  this, SLOT(formatMinutes(int)));

  // Connect the radio buttons for the advanced settings.
  connect(customRadioButton_, SIGNAL(clicked()), 
	  this, SLOT(mediaPlayerRadioButtonClicked()));
  connect(wmpRadioButton_, SIGNAL(clicked()), 
	  this, SLOT(mediaPlayerRadioButtonClicked()));
  connect(winampRadioButton_, SIGNAL(clicked()), 
	  this, SLOT(mediaPlayerRadioButtonClicked()));
  connect(xmmsRadioButton_, SIGNAL(clicked()), 
	  this, SLOT(mediaPlayerRadioButtonClicked()));
  connect(xineRadioButton_, SIGNAL(clicked()), 
	  this, SLOT(mediaPlayerRadioButtonClicked()));
  connect(mplayerRadioButton_, SIGNAL(clicked()), 
	  this, SLOT(mediaPlayerRadioButtonClicked()));

  // Load the dialog widgets with the currently set variables.
  loadAlarmSettings();

  // Save some size information so we can easily resize the window for
  // advanced settings and non-advanced settings.

  // Store the advanced settings dialog size.
//   advancedSize_.setWidth(width());
//   advancedSize_.setHeight(height());
  advancedSize_ = mainLayout->minimumSize();
//   advancedSize_ = mainLayout->sizeHint();

  // Store the non-advanced settings dialog size..
  normalSize_ = nonAdvancedLayout->minimumSize();
//   normalSize_ = nonAdvancedLayout->sizeHint();


//   // DEBUG:  print the size dimensions.
//   char x[10];
//   char y[10];
//   sprintf(x, "%d", normalSize_.width());
//   sprintf(y, "%d", normalSize_.height());
//   QMessageBox::information(this,
//                         "normalSize:",
//                         QString("width  == ") + QString(x) + "\n" +
//                         QString("height == ") + QString(y) + "\n");
//   sprintf(x, "%d", advancedSize_.width());
//   sprintf(y, "%d", advancedSize_.height());
//   QMessageBox::information(this,
//                         "advancedSize:",
//                         QString("width  == ") + QString(x) + "\n" +
//                         QString("height == ") + QString(y) + "\n");


}


// Shows the dialog in the correct size.
void SetAlarmDialog::open() {
  advancedButton_->setText(showAdvancedString_);
  hide();
  show();
  setFixedSize(normalSize_);
}


void SetAlarmDialog::setAlarmTime(const QTime &newAlarmTime) {
  (*alarmTime_) = newAlarmTime;
}

int SetAlarmDialog::getAlarmInterval() {
  return (*alarmInterval_);
}
QTime SetAlarmDialog::getAlarmTime() {
  return (*alarmTime_);
}
QString SetAlarmDialog::getAlarmMessage() {
  return (*alarmMessage_);
}
QString SetAlarmDialog::getAlarmFile() {
  return (*alarmFile_);
}
QString SetAlarmDialog::getAlarmMediaExe() {
  return (*alarmMediaExe_);
}
QString SetAlarmDialog::getAlarmMediaExeOps() {
  return (*alarmMediaExeOps_);
}


void SetAlarmDialog::browseAlarmFileClicked() {
  QFileDialog* fd = new QFileDialog(this, "Alarm File Dialog", TRUE);
  fd->setMode(QFileDialog::ExistingFile);
  fd->addFilter("Audio Files (*.wav *.mp3 *.ogg)");

  // Get the file name:
  QString filename;
  if(fd->exec() == QDialog::Accepted) {
    // They selected a new file, so set the text with the new file name.
    filename = fd->selectedFile();
    alarmFileLineEdit_->setText(filename);
  }
}

void SetAlarmDialog::browseMediaExeClicked() {
  QFileDialog* fd = new QFileDialog(this, "Media Player Program File Dialog", 
				    TRUE);
  fd->setMode(QFileDialog::ExistingFile);

  // Get the file name:
  QString filename;
  if(fd->exec() == QDialog::Accepted) {
    // They selected a new file, so set the text with the new file name.
    filename = fd->selectedFile();
    alarmMediaExeLineEdit_->setText(filename);
  }
}


void SetAlarmDialog::doneClicked() {
  // Save the new settings and hide this dialog.
  saveAlarmSettings();
  hide();
}

void SetAlarmDialog::cancelClicked() {
  // Load the previous settings and hide this dialog.
  loadAlarmSettings();
  hide();
}



// TODO:  find a better way to refresh the window, instead of doing a hide() followed by a show().
void SetAlarmDialog::showAdvancedClicked() {
  // Check to see what text the button says, and adjust to whatever
  // according to that.

  if(advancedButton_->text() == showAdvancedString_) {
    advancedButton_->setText(hideAdvancedString_);
    hide();
    show();
    setFixedSize(advancedSize_);
  }
  else if(advancedButton_->text() == hideAdvancedString_) {
    advancedButton_->setText(showAdvancedString_);
    hide();
    show();
    setFixedSize(normalSize_);
  }
}


void SetAlarmDialog::formatMinutes(int mins) {
  if(mins < 10 && mins >= 0) {
    // Needs a prefixed 0.
    minuteSpinBox_->setPrefix("0");
  }
  else {
    minuteSpinBox_->setPrefix("");
  }
}


void SetAlarmDialog::mediaPlayerRadioButtonClicked() {
  // Should we enable the browse button and line edit fields?
  if(customRadioButton_->isChecked()) {
    // Enable them.
    alarmMediaExeLineEdit_->setReadOnly(false);
    alarmMediaExeOpsLineEdit_->setReadOnly(false);
    browseMediaExeButton_->show();
  }
  else {
    // Disable them.
    alarmMediaExeLineEdit_->setReadOnly(true);
    alarmMediaExeOpsLineEdit_->setReadOnly(true);
    browseMediaExeButton_->hide();
  }

  // Load the values into the line edit fields.
  if(customRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = CUSTOM;
    alarmMediaExeLineEdit_->setText(alarmMediaExeMap_[CUSTOM]);
    alarmMediaExeOpsLineEdit_->setText(alarmMediaExeOpsMap_[CUSTOM]);
  }
  else if(wmpRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = WINDOWS_MEDIA_PLAYER;
    alarmMediaExeLineEdit_->setText(alarmMediaExeMap_[WINDOWS_MEDIA_PLAYER]);
    alarmMediaExeOpsLineEdit_->
      setText(alarmMediaExeOpsMap_[WINDOWS_MEDIA_PLAYER]);
  }
  else if(winampRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = WINAMP;
    alarmMediaExeLineEdit_->setText(alarmMediaExeMap_[WINAMP]);
    alarmMediaExeOpsLineEdit_->setText(alarmMediaExeOpsMap_[WINAMP]);
  }
  else if(xmmsRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = XMMS;
    alarmMediaExeLineEdit_->setText(alarmMediaExeMap_[XMMS]);
    alarmMediaExeOpsLineEdit_->setText(alarmMediaExeOpsMap_[XMMS]);
  }
  else if(xineRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = XINE;
    alarmMediaExeLineEdit_->setText(alarmMediaExeMap_[XINE]);
    alarmMediaExeOpsLineEdit_->setText(alarmMediaExeOpsMap_[XINE]);
  }
  else if(mplayerRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = MPLAYER;
    alarmMediaExeLineEdit_->setText(alarmMediaExeMap_[MPLAYER]);
    alarmMediaExeOpsLineEdit_->setText(alarmMediaExeOpsMap_[MPLAYER]);
  }

  // Update the alarm variables.
  (*alarmMediaExe_) = alarmMediaExeLineEdit_->text();
  (*alarmMediaExeOps_) = alarmMediaExeOpsLineEdit_->text();

} // End of    void SetAlarmDialog::mediaPlayerRadioButtonClicked()



void SetAlarmDialog::loadDefaultValues() {
    (*alarmTime_) = QTime::currentTime();
    alarmTime_->setHMS(alarmTime_->hour(), 
		       alarmTime_->minute(), 0 /* seconds */);
    (*alarmInterval_) = 0;
    (*alarmMessage_) = "Wake up you sleepy bum!";
    (*alarmFile_) = "You-better-set-this-or-it-won't-work";
    (*alarmMediaCustomExe_) = "";
    (*alarmMediaCustomExeOps_) = "";

#ifdef Q_OS_UNIX
    (*alarmMediaExeIndex_) = XMMS;  // XMMS media player.
    xmmsRadioButton_->setChecked(true);
#else
#ifdef Q_OS_WIN
    (*alarmMediaExeIndex_) = WINDOWS_MEDIA_PLAYER;  // Windows Media Player.
    wmpRadioButton_->setChecked(true);
#else
    (*alarmMediaExeIndex_) = CUSTOM;  // CUSTOM media player.
    customRadioButton_->setChecked(true);
#endif
#endif

    // Run this function to simulate the radio button(right above)
    // actually being clicked.  This function updates the line-edit
    // fields and variables.
    mediaPlayerRadioButtonClicked();

} // End of   void SetAlarmDialog::loadDefaultValues()




void SetAlarmDialog::loadAlarmSettings() {
  // Load the variables from the file.
  if(!QFile::exists(saveDataFilename)) {
    // The data file doesn't exist yet.  Just use default values.
    QMessageBox::information(this, tr("You're new, aren't you?"),
			     tr("A saved data file could not be found.  "
				"Using defaults values..."));
    loadDefaultValues();
  }
  else {
    // Try to open the file.
    QFile file(saveDataFilename);
    if(!file.open(IO_ReadOnly)) {
      // Ahhh..  Problems opening the file.
      QMessageBox::warning(this, tr("Load Failed!"), 
			  tr("Could not open file ") + saveDataFilename + 
			  tr(" for reading.  Using default values..."));
      loadDefaultValues();
    }
    else {
      // Create a Q data stream for reading from.
      QDataStream in(&file);
      // Read the magic number to make sure it's a good file to read.
      unsigned int fileMagicNumber;
      in >> fileMagicNumber;
      if(fileMagicNumber != MagicNumber) {
	// Uh oh.  Something about this file is fishy.  Use defaults instead.
	QMessageBox::warning(this, tr("Load Failed!"),
			     tr("The saved data file ") + saveDataFilename +
			     tr(" is corrupt.  Using default values..."));
	loadDefaultValues();
      }
      else {
	// This is loadable data.
	in >> (*alarmTime_);
	in >> (*alarmInterval_);
	in >> (*alarmMessage_);
	in >> (*alarmFile_);
	in >> (*alarmMediaCustomExe_);    // Previous custom entries saved.
	in >> (*alarmMediaCustomExeOps_);
	in >> (*alarmMediaExe_);          // Previous media player exe saved.
	in >> (*alarmMediaExeOps_);
	in >> (*alarmMediaExeIndex_);  // Which radio button?  See the enum.
      }
    }
  }

  alarmMediaExeMap_[CUSTOM] = (*alarmMediaCustomExe_);
  alarmMediaExeOpsMap_[CUSTOM] = (*alarmMediaCustomExeOps_);

  // At this point, we should have the variables set to usable values.
  // Here we update the widgets with the variable values.  
  alarmIntervalSpinBox_->setValue(*alarmInterval_);
  alarmMessageLineEdit_->setText(*alarmMessage_);
  alarmFileLineEdit_->setText(*alarmFile_);
  
  // Select the radio button cooresponding to the index variable.
  // This will set and do the appropriate stuff for the
  // alarmMediaExe_, alarmMediaExeOps_ variables.
  switch (*alarmMediaExeIndex_) {
  case CUSTOM:
    customRadioButton_->setChecked(true);
    break;
  case WINDOWS_MEDIA_PLAYER:
    wmpRadioButton_->setChecked(true);
    break;
  case WINAMP:
    winampRadioButton_->setChecked(true);
    break;
  case XMMS:
    xmmsRadioButton_->setChecked(true);
    break;
  case XINE:
    xineRadioButton_->setChecked(true);
    break;
  case MPLAYER:
    mplayerRadioButton_->setChecked(true);
    break;
  default:
    // Shouldn't get here, but we'll just select 'custom' if it ever
    // does.
    customRadioButton_->setChecked(true);
  }
  // Run this function to simulate the radio button(right above)
  // actually being clicked.  This function updates the line-edit
  // fields and variables.
  mediaPlayerRadioButtonClicked();
  
  int hour = alarmTime_->hour();
  int minute = alarmTime_->minute();

  if( hour == 0 ) {
    // AM Time.  Zero hours would be alarm time 12:XX am.  Make sure
    // the spin box for hours does 12 and not 0.
    hour += 12;
    amOn_ = true;
    pmOn_ = false;
    amTime_->setChecked(true);
  }
  else if( (0 < hour) && (hour < 12) ) {
    // AM Time.
    amOn_ = true;
    pmOn_ = false;
    amTime_->setChecked(true);
  }
  else if( (12 <= hour) && (hour <= 23) ) {
    // PM Time.
    hour -= 12;
    amOn_ = false;
    pmOn_ = true;
    pmTime_->setChecked(true);
  }
  
  hourSpinBox_->setValue (hour);
  formatMinutes(minute);
  minuteSpinBox_->setValue(minute);
} // End of    void SetAlarmDialog::loadAlarmSettings()



void SetAlarmDialog::initializeMediaExeMaps() {
  // Note, the map settings for the CUSTOM media player type is set at
  // load time (load of the file).  

  // These are the full-path executable locations.
  alarmMediaExeMap_[WINDOWS_MEDIA_PLAYER] = 
    "C:\\Program Files\\Windows Media Player\\wmplayer.exe";
  alarmMediaExeMap_[WINAMP] = 
    "C:\\Program Files\\Winamp\\winamp.exe";
  alarmMediaExeMap_[XMMS] = 
    "/usr/bin/xmms";
  alarmMediaExeMap_[XINE] = 
    "/usr/bin/xine";
  alarmMediaExeMap_[MPLAYER] = 
    "/usr/bin/mplayer";

  // These are the flags to use to run the audio files with the 
  // media player executables.
  alarmMediaExeOpsMap_[WINDOWS_MEDIA_PLAYER] = 
    "/play";
  alarmMediaExeOpsMap_[WINAMP] = 
    "/NEW";
  alarmMediaExeOpsMap_[XMMS] = 
    "";
  alarmMediaExeOpsMap_[XINE] = 
    "-p";
  alarmMediaExeOpsMap_[MPLAYER] = 
    "";

  // Documentation on the command line options for the different players:
  // 
  // The XMMS readme, which contains info on the command line options.
  // (Must scroll down a lot, search for "Command Line Options").
  //   http://www.xmms.org/docs/readme.php
  // The XMMS man page:
  //   http://www.cs.vassar.edu/cgi-bin/man.cgi?xmms
  // The Xine manpage can be found here:
  //   http://linuxreviews.org/man/xine/
  // The mplayer command line options can be found at either of these sites:
  //   http://www.mplayerhq.hu/DOCS/HTML/en/commandline.html
  //   http://www.mplayerhq.hu/DOCS/man/en/mplayer.1.html
  // The Winamp command line parameters info:
  //   http://forums.winamp.com/showthread.php?threadid=180297
  // The Windows Media Player cmdline options can be found here:
  //  http://support.microsoft.com/default.aspx?scid=kb;EN-US;241422
  //  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/wmplay10/mmp_sdk/commandlineparameters.asp


} // End of   void SetAlarmDialog::initializeMediaExeMaps()


bool SetAlarmDialog::saveAlarmSettings() {
  // Save the easy ones, the alarm message and the alarm file from
  // the QLineEdit widgets.
  (*alarmMessage_) = alarmMessageLineEdit_->text();
  (*alarmFile_)    = alarmFileLineEdit_->text();

  // Now here's the more length part.  Extracting info from everything
  // to form a QTime and then saving it.
  int hour = hourSpinBox_->value();
  int minute = minuteSpinBox_->value();
  if(pmTime_->isChecked() && hour != 12) {
    hour += 12;  // Add 12 hours to the hour count, since it's pm.
  }
  else if(amTime_->isChecked() && hour == 12) {
    hour = 0;    // Set to 0 because 12:XX am is 0 hours military time.
  }
  int second = 0;
  alarmTime_->setHMS(hour, minute, second);
  
  // Set the alarming interval.
  (*alarmInterval_) = alarmIntervalSpinBox_->value();

  // Store the advanced settings now.

  // Check to see what radio button is selected to save the Index.
  if(customRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = CUSTOM;
  }
  else if(wmpRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = WINDOWS_MEDIA_PLAYER;
  }
  else if(winampRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = WINAMP;
  }
  else if(xmmsRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = XMMS;
  }
  else if(xineRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = XINE;
  }
  else if(mplayerRadioButton_->isChecked()) {
    (*alarmMediaExeIndex_) = MPLAYER;
  }
  
  // If the custom media player radio button is checked, save the strings
  // to the storage variable so it can be saved to a file soon below.
  if(customRadioButton_->isChecked()) {
    // Save'em.  These two are the currently selected parameters.
    (*alarmMediaExe_) = alarmMediaExeLineEdit_->text();
    (*alarmMediaExeOps_) = alarmMediaExeOpsLineEdit_->text();

    // Since the customRadioButton is selected, let's update the
    // variables we'll save in the save-file to hold the new custom
    // values.
    (*alarmMediaCustomExe_) = (*alarmMediaExe_);
    (*alarmMediaCustomExeOps_) = (*alarmMediaExeOps_);

    // Update the map value for the custom things so next time we open
    // the dialog it will be correct.
    alarmMediaExeMap_[CUSTOM] = (*alarmMediaExe_);
    alarmMediaExeOpsMap_[CUSTOM] = (*alarmMediaExeOps_);
  }


  // Actually hard-store the data.  Data consists of:  
  // MagicNumber
  // alarmTime_
  // alarmInterval_
  // alarmMessage_
  // alarmFile_
  // alarmMediaCustomExe_
  // alarmMediaCustomExeOps_
  // alarmMediaExe_
  // alarmMediaExeOps_
  // alarmMediaExeIndex_

  QFile file(saveDataFilename);
  if(!file.open(IO_WriteOnly)) {
    QMessageBox::warning(this, tr("Save Failed!"), 
			tr("Could not open file ") + saveDataFilename + 
			tr(" for writing."));
    return false;
  }

  QDataStream out(&file);
  out.setVersion(5);
  out << (Q_UINT32)MagicNumber;
  out << (*alarmTime_);
  out << (*alarmInterval_);
  out << (*alarmMessage_);
  out << (*alarmFile_);
  out << (*alarmMediaCustomExe_);    // Previous custom entries saved.
  out << (*alarmMediaCustomExeOps_);
  out << (*alarmMediaExe_);          // Previous media player exe saved.
  out << (*alarmMediaExeOps_);
  out << (*alarmMediaExeIndex_);     // Which radio button?  See the enum.

  if(file.status() != IO_Ok) {
    QMessageBox::warning(this, tr("Save Failed!"), 
			 tr("Error writing to file ") + saveDataFilename);
    return false;
  }
  else {
    return true;
  }
} // End of   bool SetAlarmDialog::saveAlarmSettings()
