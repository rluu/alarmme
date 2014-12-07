//////////////////////////////////////////////////////////////////////////////
// 
// PROGRAM:  AlarmME
//
// AUTHOR:   Ryan Luu
//
// DESCRIPTION: 
// AlarmME stands for Alarm Made Easy.  It is a
// clock alarm which wakes you up by opening an audio 
// or video file.  It is designed so you can (very) quickly
// reset the alarm for additional snooze sleep time.  The
// sourceforge website for this project is:  
// http://sourceforge.net/projects/alarmme/
//
// LICENSE:  GPLv2
//
//

#include <qapplication.h>

#include "AlarmClockDialog.h"


int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  
  AlarmClockDialog *dialog = new AlarmClockDialog;
  app.setMainWidget(dialog);
  dialog->show();
  return app.exec();

} // End of int main();
