// Simple class to display the license in a textedit widget.

#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <qlayout.h>
#include <qdialog.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlabel.h>

class LicenseDialog : public QDialog {
  Q_OBJECT
public:
  // Default constructor.
  LicenseDialog(QWidget *parent = 0, const char *name = 0);
private slots:
  void closeClicked();     // Close this dialog.
private:
 void setLicenseString();  // Set the license string below.

  QString license_;             // The license string.

  QTextEdit *licenseTextEdit_;  // Text Edit holding the license string.

  QPushButton *closeButton_;    // Close dialog button.

};

#endif

//////////////////////////////////////////////////////////////////////////////
// End of file
//////////////////////////////////////////////////////////////////////////////
