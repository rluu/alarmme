// This class takes care of the loading, modifying and saving of the
// alarm data.  

#ifndef SETALARMDIALOG_H
#define SETALARMDIALOG_H

#include<map>
using namespace std;

// Qt library.
#include <qmessagebox.h>
#include <qbuttongroup.h>
#include <qlineedit.h>
#include <qfiledialog.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qdialog.h>
#include <qdatetime.h>
#include <qpixmap.h>


class SetAlarmDialog : public QDialog {
  Q_OBJECT
public:

  SetAlarmDialog(QTime *alarmTime,
		 unsigned int *alarmInterval, 
		 QString *alarmMessage,
		 QString *alarmFile,
		 QString *alarmMediaExe,
		 QString *alarmMediaExeOps,
		 unsigned int *alarmMediaExeIndex,
		 QWidget *parent = 0, const char *name = 0);

  void open();  // Shows the dialog.

  void loadAlarmSettings();  // This function needs to be called each time
		             // before a show() on this dialog so that it can
		             // refresh the widgets in the dialog to sync
		             // with the variables stored for the alarm
		             // settings.
  
  void setAlarmTime(const QTime &newAlarmTime);

  int getAlarmInterval();
  QTime getAlarmTime();
  QString getAlarmMessage();
  QString getAlarmFile();
  QString getAlarmMediaExe();
  QString getAlarmMediaExeOps();
signals:
private slots:
  void browseAlarmFileClicked();  // This opens a file browser to
				  // select an audio file.
  void browseMediaExeClicked();  // This opens a file browser to
				 // select a select the media exe.
  void doneClicked();  // This saves the settings in the dialog.
  void cancelClicked();  // This discards the settings in the dialog.
  void showAdvancedClicked();  // This toggles between showing the
			       // advanced alarm settings.

  void formatMinutes(int min); // Formats the minutes so it's always 2 digits.
  void mediaPlayerRadioButtonClicked();  // Updates the line-edit
					 // fields and the
					 // enable/editable properties
					 // for the media player
					 // advanced options.
  //  void mediaMediaExeCustomChanged();  // Updates the stl map for what
				      // the current values are.
private:
  enum { MagicNumber = 0x98c58f22 };  // This is stored as the header
				      // to every data file saved, to
				      // ensure it's a valid saved file.

  void initializeMediaExeMaps();

  // This takes the current widget settings and saves them into the variables.
  bool saveAlarmSettings();
  void loadDefaultValues();
  const QString saveDataFilename;

  bool amOn_;
  bool pmOn_;

  QSize normalSize_;
  QSize advancedSize_;

  QString showAdvancedString_;
  QString hideAdvancedString_;

  // Actual variables that are used.
  unsigned int *alarmInterval_;  // Interval time to realarm if alarm
				 // is not turned off.

  // These are the current values to be used in the case of an alarm.
  QTime *alarmTime_;
  QString *alarmMessage_;
  QString *alarmFile_;
  QString *alarmMediaExe_;
  QString *alarmMediaExeOps_;
  unsigned int *alarmMediaExeIndex_;

  // This is the value of the custom choice which is always saved.
  // This may or may not be == to alarmMediaExe_ or alarmMediaExeOps_.
  QString *alarmMediaCustomExe_;
  QString *alarmMediaCustomExeOps_;

  // These are the labels used in the dialog.
  QLabel *alarmTimeLabel_;
  QLabel *colonLabel_;
  QLabel *alarmIntervalLabel_;
  QLabel *alarmMsgLabel_;
  QLabel *alarmFileLabel_;
  QLabel *alarmMediaExeLabel_;
  QLabel *alarmMediaExeOpsLabel_;

  // These are the input devices uses in the dialog.
  QSpinBox *hourSpinBox_;
  QSpinBox *minuteSpinBox_;

  // For am/pm selection.
  QButtonGroup *ampmRadioButtons_;
  QRadioButton *amTime_;
  QRadioButton *pmTime_;

  // For media player selection.
  QButtonGroup *mediaPlayerRadioButtons_;
  QRadioButton *customRadioButton_;
  QRadioButton *wmpRadioButton_;
  QRadioButton *winampRadioButton_;
  QRadioButton *xmmsRadioButton_;
  QRadioButton *xineRadioButton_;
  QRadioButton *mplayerRadioButton_;


  // Spinbox and line edit widgets.
  QSpinBox *alarmIntervalSpinBox_;
  QLineEdit *alarmMessageLineEdit_;
  QLineEdit *alarmFileLineEdit_;
  QLineEdit *alarmMediaExeLineEdit_;
  QLineEdit *alarmMediaExeOpsLineEdit_;

  // Buttons.  
  QPushButton *browseAlarmFileButton_;
  QPushButton *okayButton_;
  QPushButton *cancelButton_;
  QPushButton *advancedButton_;
  QPushButton *browseMediaExeButton_;

  // These maps store the hardcoded values for each media exe supported.
  map<int, QString> alarmMediaExeMap_;
  map<int, QString> alarmMediaExeOpsMap_;

  // These are the media types supported.  If you want to add one,
  // make sure you update the SetAlarmDialog.cpp to include checking for it
  // in ::mediaPlayerRadioButtonClicked() and also, make sure it gets added
  // to the stl maps in the void ::initializeMediaExeMaps() function.
  enum {
    CUSTOM = 0,
    WINDOWS_MEDIA_PLAYER,
    WINAMP,
    XMMS,
    XINE,
    MPLAYER,
  };
};

#endif
