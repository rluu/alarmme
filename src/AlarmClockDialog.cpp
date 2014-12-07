//////////////////////////////////////////////////////////////////////////////
//
//
#include "AlarmClockDialog.h"


AlarmClockDialog::AlarmClockDialog(QWidget *parent, const char *name) 
  : QDialog(parent, name) {

  // Create the two dialogs that this application uses.
  setAlarmDialog_ = new SetAlarmDialog(&alarmTime_, &alarmInterval_,
				       &alarmMessage_, &alarmFile_,
				       &alarmMediaExe_, &alarmMediaExeOps_,
				       &alarmMediaExeIndex_, this);

  alarmingDialog_ = new AlarmingDialog(&alarmTime_, &alarmInterval_,
				       &alarmMessage_, &alarmFile_,
				       &alarmMediaExe_, &alarmMediaExeOps_,
				       this);

  // Set the icon.
  setIcon( QPixmap::fromMimeSource("clockIcon.png") );

  // Create the widgets for this dialog.
  
  // Current time stuff.
  currTimeLabelFont_ = new QFont;
  currTimeLabelFont_->setBold(true);
  currTimeLabelFont_->setPointSize(16);
  currTimeLabel_ = new QLabel(tr("Current time:  "), this);
  currTimeLabel_->setFont(*currTimeLabelFont_);
  currTimeLineEdit_ = new QLineEdit(this);
  currTimeLineEdit_->setAlignment(Qt::AlignRight);
  
  // Alarm time stuff.
  alarmTimeLabelFont_ = new QFont;
  alarmTimeLabelFont_->setBold(true);
  alarmTimeLabelFont_->setPointSize(16);
  alarmTimeLabel_ = new QLabel(tr("Alarm time:  "), this);
  alarmTimeLabel_->setFont(*alarmTimeLabelFont_);
  alarmTimeLineEdit_ = new QLineEdit(this);
  alarmTimeLineEdit_->setAlignment(Qt::AlignRight);

  // Buttons
  setAlarmSettingsButton_ = new QPushButton(tr("Alarm &Settings"), this);
  aboutButton_ = new QPushButton(tr("&About"), this);
  exitButton_  = new QPushButton(tr("E&xit"), this);
  
  // Timer
  clockUpdateAndCheckTimer_ = new QTimer(this);
  clockUpdateAndCheckTimer_->start(500 /* milliseconds */, 
				   false /* single shot */);
  

  // Set up the layouts for the dialog.

  // There will be a top layout and a bottom layout within the main
  // layout.  The top layout will contain two columns: The left column
  // will have the labels for the current and alarm times.  The right
  // column will have the LineEdits that hold the current and alarm
  // times.  The bottom layout will have the buttons.

  // Top layout.
  // Left column:
  QVBoxLayout *timeLabelsLayout = new QVBoxLayout;
  timeLabelsLayout->addWidget(alarmTimeLabel_);
  timeLabelsLayout->addWidget(currTimeLabel_);
  // Right column:
  QVBoxLayout *timeLineEditsLayout = new QVBoxLayout;
  timeLineEditsLayout->addWidget(alarmTimeLineEdit_);
  timeLineEditsLayout->addWidget(currTimeLineEdit_);
  // Top layout:
  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addLayout(timeLabelsLayout);
  topLayout->addLayout(timeLineEditsLayout);

  // Buttons layout.  
  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(setAlarmSettingsButton_);
  buttonsLayout->addWidget(aboutButton_);
  buttonsLayout->addWidget(exitButton_);

  // Main layout.
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(topLayout);
  mainLayout->addLayout(buttonsLayout);


  // Connect signals.
  connect(clockUpdateAndCheckTimer_, SIGNAL(timeout()),
	  this, SLOT(clockUpdateAndCheck()));
  connect(setAlarmSettingsButton_, SIGNAL(clicked()),
	  this, SLOT(showSetAlarmDialog()));
  connect(exitButton_, SIGNAL(clicked()),
	  this, SLOT(close()));
  connect(aboutButton_, SIGNAL(clicked()),
	  this, SLOT(showAboutDialog()));
}


// Updates both the alarm time and the current time line edit's.
// Checks to see if the times match (by hour, min and seconds); if
// yes, alarm.
void AlarmClockDialog::clockUpdateAndCheck() {
  // Get the current time right now.
  currTime_ = QTime::currentTime();
  
  // Update the QLineEdit fields for alarmTime and currTime.
  alarmTimeLineEdit_->setText(alarmTime_.toString("h:mm:ss ap"));
  currTimeLineEdit_ ->setText(currTime_ .toString("h:mm:ss ap"));


  // Check to see if it's time to trigger the alarm.
  if(alarmTime_.hour()   == currTime_.hour() &&
     alarmTime_.minute() == currTime_.minute() &&
     alarmTime_.second() == currTime_.second()) {

    // Alarm!  But only if we're not already alarming.
    if(! alarmingDialog_->isShown()) {
      showAlarmingDialog();  // Alarm.
    }
  }
}


// Opens up the setAlarmTime dialog.
void AlarmClockDialog::showSetAlarmDialog() {
  setAlarmDialog_->open();
  //  setAlarmDialog_->show();
}

// Opens up the alarming dialog.
void AlarmClockDialog::showAlarmingDialog() {
  alarmingDialog_->show();
  alarmingDialog_->trigger();
}

void AlarmClockDialog::showAboutDialog() {
  // Create an about dialog.
  AboutDialog aboutDialog(this);
  aboutDialog.exec();
}

// Closes this dialog.
void AlarmClockDialog::close() {
  // Call QDialog::done(int r).
   done(0);
}


//////////////////////////////////////////////////////////////////////////////
// End of file
//////////////////////////////////////////////////////////////////////////////
