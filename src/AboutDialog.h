#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <qfont.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qdialog.h>
#include <qimage.h>
#include <qtextedit.h>


#include "LicenseDialog.h"

#define VERSION "1.0.0"
#define YEAR "2005"


class AboutDialog : public QDialog {
  Q_OBJECT
public:
  // Default constructor.
  AboutDialog(QWidget *parent = 0, const char *name = 0);


private slots:
  void showLicenseButtonClicked();
  void closeButtonClicked();

private:
  void setAboutStrings();  // Sets the private member variables that
			   // contain the about info.

  // Buttons.
  QPushButton *closeButton_;
  QPushButton *showLicenseButton_;

  // Picture label.
  QLabel *aboutPicLabel_;

  // About- info labels.
  QLabel *aboutLicenseLabel_;
  QLabel *aboutInfoLabel_;
  QLabel *aboutDescriptionLabel_;

  // String to put in the above labels.
  QString aboutLicenseStr_;
  QString aboutInfoStr_;
  QString aboutDescriptionStr_;
};


#endif

//////////////////////////////////////////////////////////////////////////////
// End of file
//////////////////////////////////////////////////////////////////////////////
