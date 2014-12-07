
#ifndef ALARMINGDIALOG_H
#define ALARMINGDIALOG_H

// Qt library.
#include <qmessagebox.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qdialog.h>
#include <qdatetime.h>
#include <qprocess.h>
#include <qtimer.h>
#include <qpixmap.h>


class AlarmingDialog : public QDialog {
  Q_OBJECT
public:
  // Default constructor.
  AlarmingDialog(QTime *alarmTime,
		 unsigned int *alarmInterval,
		 QString *alarmMessage,
		 QString *alarmFile,
		 QString *alarmMediaExe,
		 QString *alarmMediaExeOps,
		 QWidget *parent = 0,
		 const char *name = 0);
 protected:
  void keyPressEvent( QKeyEvent * );
  public slots:  
  void trigger();  // Opens the alarmFile and (re)starts the alarmInterval
                   // countdown.
  private slots:
  void snoozeButtonClicked();
  void iAmUpButtonClicked();
  void adjustSnoozeTime(int);
  void focusDialog();         // Focuses the dialog with a hide and show.

  void plus5MinSnooze();
  void minus5MinSnooze();
  void plus1MinSnooze();
  void minus1MinSnooze();
  void plus15MinSnooze();
  void minus15MinSnooze();

private:
  // Default constants.
  const unsigned int maxSnoozeTime_;
  const unsigned int minSnoozeTime_;
  const unsigned int defaultSnoozeTime_;


  void openAlarmFile();  // Spawns a process to open the alarm file.
  void closeAlarmFile(); // Closes the media player if one it opened is open.

  QString addQuotes(const QString &str);  // Helper function that adds
					  // quotes to a QString.
  QString getSnoozeTimeStr();    // Helper function for the snooze button text.

  QProcess *mediaPlayerProcess_;
  QTimer *alarmIntervalTimer_;      // Timer used to respawn the media
				    // player process.

  unsigned int *alarmInterval_;  // Interval time to realarm if alarm is not 
                                 // turned off.(value == 0 for disabled).

  QTime *alarmTime_;
  QString *alarmMessage_;
  QString *alarmFile_;
  QString *alarmMediaExe_;
  QString *alarmMediaExeOps_;

  QFont  *alarmFont_;
  QLabel *alarmMessageLabel_;

  QLabel *alarmPicLabel_;
  QLabel *alarmPicLabel2_;

  unsigned int snoozeTime_;

  QLabel *snoozeTimeLabel_;
  QLabel *snoozeTimeLabel2_;
  QSpinBox *snoozeTimeSpinBox_;

  QPushButton *plus5MinButton_;
  QPushButton *minus5MinButton_;
  QPushButton *plus1MinButton_;
  QPushButton *minus1MinButton_;
  QPushButton *plus15MinButton_;
  QPushButton *minus15MinButton_;
  
  QPushButton *snoozeButton_;  // Kills alarm audio (spawned process),
			       // hides dialog, then adjusts the alarm
			       // time to be = current time + snooze
			       // time.
  QPushButton *iAmUpButton_; // Kills alarm audio (spawned process),
			     // hides dialog.

};




#endif
