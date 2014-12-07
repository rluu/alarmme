
#include "LicenseDialog.h"

LicenseDialog::LicenseDialog(QWidget *parent, const char *name)
  : QDialog(parent, name) {

  // Set the caption:
  setCaption(tr("License"));

  // Set the icon.
  setIcon( QPixmap::fromMimeSource("clockIcon.png") );

  // Set the license string.
  setLicenseString();

  // Create a font for smaller text in the dialog.
  QFont courier("Courier", 10);

  // This is a label that will displayed as hidden.  I'm using this so
  // that the dialog will open up to an acceptable width.  It's also
  // serving as a separator between the close button and the text
  // edit.
  QLabel *hackLabel = new QLabel(tr("--------------------------------------------------------------------------------"), this);
  hackLabel->setFont(courier);

  // QTextEdit with the license inside.  (scrollable).
  licenseTextEdit_ = new QTextEdit(this);
  licenseTextEdit_->setReadOnly(true);
  licenseTextEdit_->setFont(courier);
  licenseTextEdit_->setWordWrap(QTextEdit::NoWrap);
  licenseTextEdit_->setText(license_);

  // The close button.
  closeButton_ = new QPushButton(tr("&Close"), this);

  // Setup the layout for this dialog.
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(licenseTextEdit_);
  mainLayout->addWidget(hackLabel);
  mainLayout->addWidget(closeButton_);

  // Connect the button to the done signal.
  connect(closeButton_, SIGNAL(clicked()),
	  this, SLOT(closeClicked()));

}

void LicenseDialog::closeClicked() {
  // Close with success.
  done(0);
}

void LicenseDialog::setLicenseString() {
  license_ =
    QString("\n") +
    QString("		    GNU GENERAL PUBLIC LICENSE\n") +
    QString("		       Version 2, June 1991\n") +
    QString("\n") +
    QString(" Copyright (C) 1989, 1991 Free Software Foundation, Inc.\n") +
    QString("                     59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n") +
    QString(" Everyone is permitted to copy and distribute verbatim copies\n") +
    QString(" of this license document, but changing it is not allowed.\n") +
    QString("\n") +
    QString("			    Preamble\n") +
    QString("\n") +
    QString("  The licenses for most software are designed to take away your\n") +
    QString("freedom to share and change it.  By contrast, the GNU General Public\n") +
    QString("License is intended to guarantee your freedom to share and change free\n") +
    QString("software--to make sure the software is free for all its users.  This\n") +
    QString("General Public License applies to most of the Free Software\n") +
    QString("Foundation's software and to any other program whose authors commit to\n") +
    QString("using it.  (Some other Free Software Foundation software is covered by\n") +
    QString("the GNU Library General Public License instead.)  You can apply it to\n") +
    QString("your programs, too.\n") +
    QString("\n") +
    QString("  When we speak of free software, we are referring to freedom, not\n") +
    QString("price.  Our General Public Licenses are designed to make sure that you\n") +
    QString("have the freedom to distribute copies of free software (and charge for\n") +
    QString("this service if you wish), that you receive source code or can get it\n") +
    QString("if you want it, that you can change the software or use pieces of it\n") +
    QString("in new free programs; and that you know you can do these things.\n") +
    QString("\n") +
    QString("  To protect your rights, we need to make restrictions that forbid\n") +
    QString("anyone to deny you these rights or to ask you to surrender the rights.\n") +
    QString("These restrictions translate to certain responsibilities for you if you\n") +
    QString("distribute copies of the software, or if you modify it.\n") +
    QString("\n") +
    QString("  For example, if you distribute copies of such a program, whether\n") +
    QString("gratis or for a fee, you must give the recipients all the rights that\n") +
    QString("you have.  You must make sure that they, too, receive or can get the\n") +
    QString("source code.  And you must show them these terms so they know their\n") +
    QString("rights.\n") +
    QString("\n") +
    QString("  We protect your rights with two steps: (1) copyright the software, and\n") +
    QString("(2) offer you this license which gives you legal permission to copy,\n") +
    QString("distribute and/or modify the software.\n") +
    QString("\n") +
    QString("  Also, for each author's protection and ours, we want to make certain\n") +
    QString("that everyone understands that there is no warranty for this free\n") +
    QString("software.  If the software is modified by someone else and passed on, we\n") +
    QString("want its recipients to know that what they have is not the original, so\n") +
    QString("that any problems introduced by others will not reflect on the original\n") +
    QString("authors' reputations.\n") +
    QString("\n") +
    QString("  Finally, any free program is threatened constantly by software\n") +
    QString("patents.  We wish to avoid the danger that redistributors of a free\n") +
    QString("program will individually obtain patent licenses, in effect making the\n") +
    QString("program proprietary.  To prevent this, we have made it clear that any\n") +
    QString("patent must be licensed for everyone's free use or not licensed at all.\n") +
    QString("\n") +
    QString("  The precise terms and conditions for copying, distribution and\n") +
    QString("modification follow.\n") +
    QString("\n") +
    QString("		    GNU GENERAL PUBLIC LICENSE\n") +
    QString("   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n") +
    QString("\n") +
    QString("  0. This License applies to any program or other work which contains\n") +
    QString("a notice placed by the copyright holder saying it may be distributed\n") +
    QString("under the terms of this General Public License.  The \"Program\", below,\n") +
    QString("refers to any such program or work, and a \"work based on the Program\"\n") +
    QString("means either the Program or any derivative work under copyright law:\n") +
    QString("that is to say, a work containing the Program or a portion of it,\n") +
    QString("either verbatim or with modifications and/or translated into another\n") +
    QString("language.  (Hereinafter, translation is included without limitation in\n") +
    QString("the term \"modification\".)  Each licensee is addressed as \"you\".\n") +
    QString("\n") +
    QString("Activities other than copying, distribution and modification are not\n") +
    QString("covered by this License; they are outside its scope.  The act of\n") +
    QString("running the Program is not restricted, and the output from the Program\n") +
    QString("is covered only if its contents constitute a work based on the\n") +
    QString("Program (independent of having been made by running the Program).\n") +
    QString("Whether that is true depends on what the Program does.\n") +
    QString("\n") +
    QString("  1. You may copy and distribute verbatim copies of the Program's\n") +
    QString("source code as you receive it, in any medium, provided that you\n") +
    QString("conspicuously and appropriately publish on each copy an appropriate\n") +
    QString("copyright notice and disclaimer of warranty; keep intact all the\n") +
    QString("notices that refer to this License and to the absence of any warranty;\n") +
    QString("and give any other recipients of the Program a copy of this License\n") +
    QString("along with the Program.\n") +
    QString("\n") +
    QString("You may charge a fee for the physical act of transferring a copy, and\n") +
    QString("you may at your option offer warranty protection in exchange for a fee.\n") +
    QString("\n") +
    QString("  2. You may modify your copy or copies of the Program or any portion\n") +
    QString("of it, thus forming a work based on the Program, and copy and\n") +
    QString("distribute such modifications or work under the terms of Section 1\n") +
    QString("above, provided that you also meet all of these conditions:\n") +
    QString("\n") +
    QString("    a) You must cause the modified files to carry prominent notices\n") +
    QString("    stating that you changed the files and the date of any change.\n") +
    QString("\n") +
    QString("    b) You must cause any work that you distribute or publish, that in\n") +
    QString("    whole or in part contains or is derived from the Program or any\n") +
    QString("    part thereof, to be licensed as a whole at no charge to all third\n") +
    QString("    parties under the terms of this License.\n") +
    QString("\n") +
    QString("    c) If the modified program normally reads commands interactively\n") +
    QString("    when run, you must cause it, when started running for such\n") +
    QString("    interactive use in the most ordinary way, to print or display an\n") +
    QString("    announcement including an appropriate copyright notice and a\n") +
    QString("    notice that there is no warranty (or else, saying that you provide\n") +
    QString("    a warranty) and that users may redistribute the program under\n") +
    QString("    these conditions, and telling the user how to view a copy of this\n") +
    QString("    License.  (Exception: if the Program itself is interactive but\n") +
    QString("    does not normally print such an announcement, your work based on\n") +
    QString("    the Program is not required to print an announcement.)\n") +
    QString("\n") +
    QString("These requirements apply to the modified work as a whole.  If\n") +
    QString("identifiable sections of that work are not derived from the Program,\n") +
    QString("and can be reasonably considered independent and separate works in\n") +
    QString("themselves, then this License, and its terms, do not apply to those\n") +
    QString("sections when you distribute them as separate works.  But when you\n") +
    QString("distribute the same sections as part of a whole which is a work based\n") +
    QString("on the Program, the distribution of the whole must be on the terms of\n") +
    QString("this License, whose permissions for other licensees extend to the\n") +
    QString("entire whole, and thus to each and every part regardless of who wrote it.\n") +
    QString("\n") +
    QString("Thus, it is not the intent of this section to claim rights or contest\n") +
    QString("your rights to work written entirely by you; rather, the intent is to\n") +
    QString("exercise the right to control the distribution of derivative or\n") +
    QString("collective works based on the Program.\n") +
    QString("\n") +
    QString("In addition, mere aggregation of another work not based on the Program\n") +
    QString("with the Program (or with a work based on the Program) on a volume of\n") +
    QString("a storage or distribution medium does not bring the other work under\n") +
    QString("the scope of this License.\n") +
    QString("\n") +
    QString("  3. You may copy and distribute the Program (or a work based on it,\n") +
    QString("under Section 2) in object code or executable form under the terms of\n") +
    QString("Sections 1 and 2 above provided that you also do one of the following:\n") +
    QString("\n") +
    QString("    a) Accompany it with the complete corresponding machine-readable\n") +
    QString("    source code, which must be distributed under the terms of Sections\n") +
    QString("    1 and 2 above on a medium customarily used for software interchange; or,\n") +
    QString("\n") +
    QString("    b) Accompany it with a written offer, valid for at least three\n") +
    QString("    years, to give any third party, for a charge no more than your\n") +
    QString("    cost of physically performing source distribution, a complete\n") +
    QString("    machine-readable copy of the corresponding source code, to be\n") +
    QString("    distributed under the terms of Sections 1 and 2 above on a medium\n") +
    QString("    customarily used for software interchange; or,\n") +
    QString("\n") +
    QString("    c) Accompany it with the information you received as to the offer\n") +
    QString("    to distribute corresponding source code.  (This alternative is\n") +
    QString("    allowed only for noncommercial distribution and only if you\n") +
    QString("    received the program in object code or executable form with such\n") +
    QString("    an offer, in accord with Subsection b above.)\n") +
    QString("\n") +
    QString("The source code for a work means the preferred form of the work for\n") +
    QString("making modifications to it.  For an executable work, complete source\n") +
    QString("code means all the source code for all modules it contains, plus any\n") +
    QString("associated interface definition files, plus the scripts used to\n") +
    QString("control compilation and installation of the executable.  However, as a\n") +
    QString("special exception, the source code distributed need not include\n") +
    QString("anything that is normally distributed (in either source or binary\n") +
    QString("form) with the major components (compiler, kernel, and so on) of the\n") +
    QString("operating system on which the executable runs, unless that component\n") +
    QString("itself accompanies the executable.\n") +
    QString("\n") +
    QString("If distribution of executable or object code is made by offering\n") +
    QString("access to copy from a designated place, then offering equivalent\n") +
    QString("access to copy the source code from the same place counts as\n") +
    QString("distribution of the source code, even though third parties are not\n") +
    QString("compelled to copy the source along with the object code.\n") +
    QString("\n") +
    QString("  4. You may not copy, modify, sublicense, or distribute the Program\n") +
    QString("except as expressly provided under this License.  Any attempt\n") +
    QString("otherwise to copy, modify, sublicense or distribute the Program is\n") +
    QString("void, and will automatically terminate your rights under this License.\n") +
    QString("However, parties who have received copies, or rights, from you under\n") +
    QString("this License will not have their licenses terminated so long as such\n") +
    QString("parties remain in full compliance.\n") +
    QString("\n") +
    QString("  5. You are not required to accept this License, since you have not\n") +
    QString("signed it.  However, nothing else grants you permission to modify or\n") +
    QString("distribute the Program or its derivative works.  These actions are\n") +
    QString("prohibited by law if you do not accept this License.  Therefore, by\n") +
    QString("modifying or distributing the Program (or any work based on the\n") +
    QString("Program), you indicate your acceptance of this License to do so, and\n") +
    QString("all its terms and conditions for copying, distributing or modifying\n") +
    QString("the Program or works based on it.\n") +
    QString("\n") +
    QString("  6. Each time you redistribute the Program (or any work based on the\n") +
    QString("Program), the recipient automatically receives a license from the\n") +
    QString("original licensor to copy, distribute or modify the Program subject to\n") +
    QString("these terms and conditions.  You may not impose any further\n") +
    QString("restrictions on the recipients' exercise of the rights granted herein.\n") +
    QString("You are not responsible for enforcing compliance by third parties to\n") +
    QString("this License.\n") +
    QString("\n") +
    QString("  7. If, as a consequence of a court judgment or allegation of patent\n") +
    QString("infringement or for any other reason (not limited to patent issues),\n") +
    QString("conditions are imposed on you (whether by court order, agreement or\n") +
    QString("otherwise) that contradict the conditions of this License, they do not\n") +
    QString("excuse you from the conditions of this License.  If you cannot\n") +
    QString("distribute so as to satisfy simultaneously your obligations under this\n") +
    QString("License and any other pertinent obligations, then as a consequence you\n") +
    QString("may not distribute the Program at all.  For example, if a patent\n") +
    QString("license would not permit royalty-free redistribution of the Program by\n") +
    QString("all those who receive copies directly or indirectly through you, then\n") +
    QString("the only way you could satisfy both it and this License would be to\n") +
    QString("refrain entirely from distribution of the Program.\n") +
    QString("\n") +
    QString("If any portion of this section is held invalid or unenforceable under\n") +
    QString("any particular circumstance, the balance of the section is intended to\n") +
    QString("apply and the section as a whole is intended to apply in other\n") +
    QString("circumstances.\n") +
    QString("\n") +
    QString("It is not the purpose of this section to induce you to infringe any\n") +
    QString("patents or other property right claims or to contest validity of any\n") +
    QString("such claims; this section has the sole purpose of protecting the\n") +
    QString("integrity of the free software distribution system, which is\n") +
    QString("implemented by public license practices.  Many people have made\n") +
    QString("generous contributions to the wide range of software distributed\n") +
    QString("through that system in reliance on consistent application of that\n") +
    QString("system; it is up to the author/donor to decide if he or she is willing\n") +
    QString("to distribute software through any other system and a licensee cannot\n") +
    QString("impose that choice.\n") +
    QString("\n") +
    QString("This section is intended to make thoroughly clear what is believed to\n") +
    QString("be a consequence of the rest of this License.\n") +
    QString("\n") +
    QString("  8. If the distribution and/or use of the Program is restricted in\n") +
    QString("certain countries either by patents or by copyrighted interfaces, the\n") +
    QString("original copyright holder who places the Program under this License\n") +
    QString("may add an explicit geographical distribution limitation excluding\n") +
    QString("those countries, so that distribution is permitted only in or among\n") +
    QString("countries not thus excluded.  In such case, this License incorporates\n") +
    QString("the limitation as if written in the body of this License.\n") +
    QString("\n") +
    QString("  9. The Free Software Foundation may publish revised and/or new versions\n") +
    QString("of the General Public License from time to time.  Such new versions will\n") +
    QString("be similar in spirit to the present version, but may differ in detail to\n") +
    QString("address new problems or concerns.\n") +
    QString("\n") +
    QString("Each version is given a distinguishing version number.  If the Program\n") +
    QString("specifies a version number of this License which applies to it and \"any\n") +
    QString("later version\", you have the option of following the terms and conditions\n") +
    QString("either of that version or of any later version published by the Free\n") +
    QString("Software Foundation.  If the Program does not specify a version number of\n") +
    QString("this License, you may choose any version ever published by the Free Software\n") +
    QString("Foundation.\n") +
    QString("\n") +
    QString("  10. If you wish to incorporate parts of the Program into other free\n") +
    QString("programs whose distribution conditions are different, write to the author\n") +
    QString("to ask for permission.  For software which is copyrighted by the Free\n") +
    QString("Software Foundation, write to the Free Software Foundation; we sometimes\n") +
    QString("make exceptions for this.  Our decision will be guided by the two goals\n") +
    QString("of preserving the free status of all derivatives of our free software and\n") +
    QString("of promoting the sharing and reuse of software generally.\n") +
    QString("\n") +
    QString("			    NO WARRANTY\n") +
    QString("\n") +
    QString("  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY\n") +
    QString("FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN\n") +
    QString("OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES\n") +
    QString("PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED\n") +
    QString("OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\n") +
    QString("MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS\n") +
    QString("TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE\n") +
    QString("PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,\n") +
    QString("REPAIR OR CORRECTION.\n") +
    QString("\n") +
    QString("  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING\n") +
    QString("WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR\n") +
    QString("REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,\n") +
    QString("INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING\n") +
    QString("OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED\n") +
    QString("TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY\n") +
    QString("YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER\n") +
    QString("PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE\n") +
    QString("POSSIBILITY OF SUCH DAMAGES.\n") +
    QString("\n") +
    QString("		     END OF TERMS AND CONDITIONS\n") +
    QString("");

}

//////////////////////////////////////////////////////////////////////////////
// End of file
//////////////////////////////////////////////////////////////////////////////

