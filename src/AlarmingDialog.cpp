//////////////////////////////////////////////////////////////////////////////
//

// If I ever get gifs working correctly, i may need this.
//#include <qimage.h>

// Debug used this.  (but the debug part is commented out right now).
//#include <qstringlist.h>

#include "AlarmingDialog.h"


AlarmingDialog::AlarmingDialog(QTime *alarmTime,
			       unsigned int *alarmInterval,
			       QString *alarmMessage,
			       QString *alarmFile,
			       QString *alarmMediaExe,
			       QString *alarmMediaExeOps,
			       QWidget *parent,
			       const char *name)
  : QDialog(parent, name, true, 
	    WStyle_Dialog | 
	    WStyle_StaysOnTop | WX11BypassWM),
    maxSnoozeTime_(180),
    minSnoozeTime_(1),
    defaultSnoozeTime_(15) {

  // Let's make sure none of these pointers are NULL:
  if(alarmTime == 0 || alarmInterval == 0 || alarmMessage == 0 || 
     alarmFile == 0 || alarmMediaExe == 0 || alarmMediaExeOps == 0) {
    QString errorMessage = tr("Pointers passed into constructor ");
    errorMessage += tr("'AlarmingDialog()' are NULL.  ") + QString(__FILE__);

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

  // Set the caption.
  setCaption(tr("Beep beep!"));

  // Set the icon.
  setIcon( QPixmap::fromMimeSource("clockIcon.png") );

  // Initialize the snooze time to the default.
  snoozeTime_ = defaultSnoozeTime_;  

  // Now create and initialize all the widgets.  Start with the little widgets.

  // QTimer.  This is the timer that will re-trigger the media player
  // and play the audio file again.
  alarmIntervalTimer_ = new QTimer(this);

  // QProcess.  This will spawn the media player process.
  mediaPlayerProcess_ = new QProcess(this);

  // Separators.
  QFrame *verticalSeparator = new QFrame(this);
  QFrame *horizontalSeparator = new QFrame(this);
  verticalSeparator->setFrameStyle(QFrame::VLine | QFrame::Plain);
  horizontalSeparator->setFrameStyle(QFrame::HLine | QFrame::Plain);

  // TODO:  Get gifs working.  and also, see if I can make it so it's only one instance?
  // This is a label that simply displays a gif of a ringing clock.
  alarmPicLabel_ = new QLabel(this);
  alarmPicLabel2_ = new QLabel(this);
  alarmPicLabel_->setPixmap( QPixmap::fromMimeSource("clockAlarming.gif"));
  alarmPicLabel2_->setPixmap( QPixmap::fromMimeSource("clockAlarming.gif"));

  // Sets the font for the alarm message.
  alarmFont_ = new QFont;
  alarmFont_->setBold(true);
  alarmFont_->setPointSize(24);

  // Label that displays the alarm message.
  alarmMessageLabel_ = new QLabel(*alarmMessage, this);
  alarmMessageLabel_->setFont(*alarmFont_);

  // Button that kills the current alarming audio and allows for
  // snoozing of a certain amount of time.
  snoozeButton_ = new QPushButton(QString(tr("&Snooze for ")) + 
				  getSnoozeTimeStr() +
				  QString(tr(" minutes")), this);
  
  // Button that kills the current alarming audio and hides the dialog.
  iAmUpButton_ = new QPushButton(QString(tr("&Okay, I am up!")), this);
  
  // Snooze time adjustment buttons.
  plus15MinButton_ = new QPushButton(tr("+15 min (p)"), this);
  plus1MinButton_ = new QPushButton(tr("+1 min (k)"), this);
  plus5MinButton_ = new QPushButton(tr("+5 min (l)"), this);
  minus5MinButton_ = new QPushButton(tr("-5 min (h)"), this);
  minus1MinButton_ = new QPushButton(tr("-1 min (j)"), this);
  minus15MinButton_ = new QPushButton(tr("-15 min (n)"), this);

  // Labels...
  snoozeTimeLabel_ = new QLabel(QString(tr("Give me")), this);
  snoozeTimeLabel2_ = new QLabel(QString(tr("  more minutes of sleep.")),this);
  
  // This spin box controls the amount of snooze time.
  snoozeTimeSpinBox_ = new QSpinBox(this);
  snoozeTimeSpinBox_->setWrapping(false);
  snoozeTimeSpinBox_->setMinValue(minSnoozeTime_);
  snoozeTimeSpinBox_->setMaxValue(maxSnoozeTime_);
  snoozeTimeSpinBox_->setValue(snoozeTime_);  // Display the default
					      // snoozeTime_.
  
  // Okay, let's set up the layout of everything.  The top will have
  // the alarm picture and alarm message prominently displayed in big
  // font.  The middle will have the snooze-adjusting features and
  // settings.  The bottom will have the snooze and i-am-up buttons.

  // Top layout.
  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(alarmPicLabel_);
  topLayout->addSpacing(5);
  topLayout->addWidget(alarmMessageLabel_);
  topLayout->addSpacing(5);
  topLayout->addWidget(alarmPicLabel2_);
  
  // Middle layout.  This one is a bit more complicated.

  // Contains the left and right middle layouts.
  QHBoxLayout *middleLayout = new QHBoxLayout;  

  // Contains the buttons for modifying the snooze time.
  QHBoxLayout *middleRightLayout = new QHBoxLayout; 
  // The middle right layout has 3 vertical layouts within it.
  QVBoxLayout *middleRightLeftLayout = new QVBoxLayout;
  middleRightLeftLayout->addWidget(minus5MinButton_);
  QVBoxLayout *middleRightMiddleLayout = new QVBoxLayout;
  middleRightMiddleLayout->addWidget(plus15MinButton_);
  middleRightMiddleLayout->addWidget(plus1MinButton_);
  middleRightMiddleLayout->addWidget(minus1MinButton_);
  middleRightMiddleLayout->addWidget(minus15MinButton_);
  QVBoxLayout *middleRightRightLayout = new QVBoxLayout;
  middleRightRightLayout->addWidget(plus5MinButton_);
  // Combine all of them to the middle right layout.
  middleRightLayout->addLayout(middleRightLeftLayout);
  middleRightLayout->addLayout(middleRightMiddleLayout);
  middleRightLayout->addLayout(middleRightRightLayout);
  // Contains the actual spin box for the snooze time.
  QHBoxLayout *middleLeftLayout = new QHBoxLayout;   
  middleLeftLayout->addWidget(snoozeTimeLabel_);
  middleLeftLayout->addWidget(snoozeTimeSpinBox_);
  middleLeftLayout->addWidget(snoozeTimeLabel2_);
  // Combine the left and right layouts to form the middle layout.
  middleLayout->addLayout(middleLeftLayout);
  middleLayout->addWidget(verticalSeparator);
  middleLayout->addLayout(middleRightLayout);
  
  // Bottom layout.
  QHBoxLayout *bottomLayout = new QHBoxLayout;
  bottomLayout->addWidget(snoozeButton_);
  bottomLayout->addWidget(iAmUpButton_);

  // Combine all three layouts into the main layout.
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(topLayout);
  mainLayout->addSpacing(5);
  mainLayout->addWidget(horizontalSeparator);
  mainLayout->addSpacing(5);
  mainLayout->addLayout(middleLayout);
  mainLayout->addSpacing(10);
  mainLayout->addLayout(bottomLayout);


  // Connecting buttons to actions:
  connect(plus5MinButton_, SIGNAL(clicked()), 
	  this, SLOT(plus5MinSnooze()));
  connect(minus5MinButton_, SIGNAL(clicked()), 
	  this, SLOT(minus5MinSnooze()));
  connect(plus1MinButton_, SIGNAL(clicked()), 
	  this, SLOT(plus1MinSnooze()));
  connect(minus1MinButton_, SIGNAL(clicked()), 
	  this, SLOT(minus1MinSnooze()));
  connect(plus15MinButton_, SIGNAL(clicked()), 
	  this, SLOT(plus15MinSnooze()));
  connect(minus15MinButton_, SIGNAL(clicked()), 
	  this, SLOT(minus15MinSnooze()));
  connect(snoozeButton_, SIGNAL(clicked()),
	  this, SLOT(snoozeButtonClicked()));
  connect(iAmUpButton_, SIGNAL(clicked()),
	  this, SLOT(iAmUpButtonClicked()));
  
  // Connect signal from the spinbox to adjust the snooze time.
  connect(snoozeTimeSpinBox_, SIGNAL(valueChanged(int)),
	  this, SLOT(adjustSnoozeTime(int)));
  // Connect the timer to the trigger slot.
  connect(alarmIntervalTimer_, SIGNAL(timeout()),
	  this, SLOT(trigger()));
}



QString AlarmingDialog::getSnoozeTimeStr() {
  char temp[32];
  sprintf(temp, "%d", snoozeTime_);
  return QString(temp);
}


void AlarmingDialog::keyPressEvent( QKeyEvent *k ) {
  switch( tolower(k->ascii()) ) {
  case 'j':
    minus1MinSnooze();
    break;
  case 'k':
    plus1MinSnooze();
    break;
  case 'h':
    minus5MinSnooze();    
    break;
  case 'l':
    plus5MinSnooze();
    break;
  case 'p':
    plus15MinSnooze();
    break;
  case 'n':
    minus15MinSnooze();
    break;
  }
}

void AlarmingDialog::trigger() {
  // Attempt to close the media player process if it's running.
  closeAlarmFile();

  // It shouldn't sit here long.  This is just to wait for the process
  // to complete die off before starting it up again.
  while(mediaPlayerProcess_->isRunning()) {
    // Do nothing and wait till the media player closes.
  }

  // Open the media player process.
  openAlarmFile();

  // Start the timer again if the restart-interval is enabled (non-zero).
  if((*alarmInterval_) != 0) {
      alarmIntervalTimer_->start((*alarmInterval_) * 1000 /* milliseconds */,
			     true /* single shot */);
  }
}



QString AlarmingDialog::addQuotes(const QString &str) {
  return QString(QString("\"") + str + QString("\""));
}



void AlarmingDialog::adjustSnoozeTime(int newVal) {
  // Check for out of bounds.
  if(newVal < int(minSnoozeTime_)) {
    snoozeTime_ = minSnoozeTime_;
  }
  else if(newVal > int(maxSnoozeTime_)) {
    snoozeTime_ = maxSnoozeTime_;
  }
  else {
    // We made a valid adjustment to the snooze time; save the change.
    snoozeTime_ = newVal;
  }

  // Change the value displayed on the snooze button as well:
  snoozeButton_->setText(QString(tr("&Snooze for ")) + 
			 getSnoozeTimeStr() +
			 QString(tr(" minutes")));
  
  // Change the value displayed in the spin box if it is different.  
  if(snoozeTimeSpinBox_->value() != int(snoozeTime_)) {
    snoozeTimeSpinBox_->setValue(snoozeTime_);
  }
}


void AlarmingDialog::focusDialog() {
  raise();
  setActiveWindow();
}


void AlarmingDialog::plus5MinSnooze() {
  adjustSnoozeTime(int(snoozeTime_)+5);
}
void AlarmingDialog::minus5MinSnooze() {
  adjustSnoozeTime(int(snoozeTime_)-5);
}
void AlarmingDialog::plus1MinSnooze() {
  adjustSnoozeTime(int(snoozeTime_)+1);
}
void AlarmingDialog::minus1MinSnooze() {
  adjustSnoozeTime(int(snoozeTime_)-1);
}
void AlarmingDialog::plus15MinSnooze() {
  adjustSnoozeTime(int(snoozeTime_)+15);
}
void AlarmingDialog::minus15MinSnooze() {
  adjustSnoozeTime(int(snoozeTime_)-15);
}



void AlarmingDialog::snoozeButtonClicked() {
  // Close the media player if it's open still.
  closeAlarmFile();

  // Stop the timer.
  alarmIntervalTimer_->stop();

  // Reset the alarm time to (current time) + (snoozeTime_ minutes).
  (*alarmTime_) = QTime::currentTime().addSecs( snoozeTime_ * 60 );

  // Hide this window.
  hide();
}


void AlarmingDialog::iAmUpButtonClicked() {
  // Close the media player if it's open still.
  closeAlarmFile();

  // Stop the timer.
  alarmIntervalTimer_->stop();

  // Hide this window.
  hide();
}


void AlarmingDialog::openAlarmFile() {

  // So do I need to add quotes around the arguments, or not?
  mediaPlayerProcess_->clearArguments();
  mediaPlayerProcess_->addArgument( (*alarmMediaExe_) );
  mediaPlayerProcess_->addArgument( (*alarmMediaExeOps_) );
  mediaPlayerProcess_->addArgument( (*alarmFile_) );
  //   mediaPlayerProcess_->clearArguments();
  //   mediaPlayerProcess_->addArgument( addQuotes(*alarmMediaExe_) );
  //   mediaPlayerProcess_->addArgument( addQuotes(*alarmMediaExeOps_) );
  //   mediaPlayerProcess_->addArgument( addQuotes(*alarmFile_) );

//   QString debugFullCmd;
//   QStringList list = mediaPlayerProcess_->arguments();
//   for(QStringList::Iterator it = list.begin();
//       it != list.end(); it++) {
//     debugFullCmd += (*it) + " ";
//   }
//   QMessageBox::information(this, "DEBUG",
// 			   tr("The following is the command to be executed:\n")
// 			   + debugFullCmd);
  
  if(! mediaPlayerProcess_->start()) {
    QMessageBox::critical(0, tr("Spawn process failed!"),
			  tr("The media player below failed to open!  \n") +
			  (*alarmMediaExe_),
			  QMessageBox::Ok, 
			  QMessageBox::NoButton,
			  QMessageBox::NoButton);
  }
  
  // Change focus so it's on top.
  QTimer::singleShot(3000, this, SLOT(focusDialog()));
  
}



//  NOTES: Right now, tryTerminate seems to work well with killing
//  xmms and mplayer.  Also, using a timer with singleShot won't work,
//  because this function (closeAlarmFile()) will return right away,
//  and then openAlarmFile() will open... which waits for the process
//  to stop running.  It will eventually find that the process
//  finished via the tryTerminate, and then open up the media player
//  again.  THIS one will get killed by the kill timer.  So the
//  oneshot timer thing here won't work.  I guess we can just hope
//  that tryTerminate will work.

void AlarmingDialog::closeAlarmFile() {
  if(mediaPlayerProcess_->isRunning()) {
    // tryTerminate() will only work in Linux.
    // kill() will work on both Linux and Windows.
    // 
    //     mediaPlayerProcess_->tryTerminate();
    mediaPlayerProcess_->kill();
  }
}


//////////////////////////////////////////////////////////////////////////////
// End of file
//////////////////////////////////////////////////////////////////////////////
