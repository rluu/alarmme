
#ifndef ALARMCLOCKDIALOG_H
#define ALARMCLOCKDIALOG_H



#include <qlabel.h>
#include <qlayout.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qdatetime.h>
#include <qpixmap.h>

// Dialogs spawned from this dialog.
#include "SetAlarmDialog.h"
#include "AlarmingDialog.h"
#include "AboutDialog.h"

class AlarmClockDialog : public QDialog {
  Q_OBJECT
public:
  AlarmClockDialog(QWidget *parent = 0, const char *name = 0);
  
private slots:
  void clockUpdateAndCheck();  // Update the current time and check if
			      // it's time to alarm.
  
  void showSetAlarmDialog();  // Opens up the setAlarmTime dialog and
			      // disables the button for this.

  void showAlarmingDialog();  // Opens up the alarming dialog.

  void showAboutDialog();     // Opens up the about dialog.

  void close();  // Closes this dialog (exiting).

private:
  SetAlarmDialog *setAlarmDialog_;  // Dialog for the alarm options
				    // and time settings.

  AlarmingDialog *alarmingDialog_;  // Dialog for when the alarm is going off.
  

  // Current time stuff.
  QFont  *currTimeLabelFont_;    // Font for the below label.
  QLabel *currTimeLabel_;        // "Current time is:"
  QLineEdit *currTimeLineEdit_;  // Uneditable field. Contains the
				 // current time.
  // Alarm time stuff.
  QFont  *alarmTimeLabelFont_;   // Font for the below label.
  QLabel *alarmTimeLabel_;       // "Alarm time is:"
  QLineEdit *alarmTimeLineEdit_; // Uneditable field. Contains the
				 // alarm time.

  // Buttons.
  QPushButton *setAlarmSettingsButton_; // Button to show the SetAlarmDialog. 
  QPushButton *aboutButton_;            // Button to show the about dialog.
  QPushButton *exitButton_;             // Button to close the application.


  // Timer used to update the clock and check for alarm time match.
  QTimer *clockUpdateAndCheckTimer_;
  
  QTime alarmTime_;  // Alarm time.
  QTime currTime_;   // Current time.


  unsigned int alarmInterval_;  // Interval time to realarm if alarm
				// is not turned off.

  QString alarmMessage_;
  QString alarmFile_;
  QString alarmMediaExe_;
  QString alarmMediaExeOps_;
  unsigned int alarmMediaExeIndex_;

};
#endif
