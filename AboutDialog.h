/* 
 * File:   AboutDialog.h
 * Author: rafal
 *
 * Created on 14 marzec 2011, 19:11
 */

#ifndef ABOUTDIALOG_H
#define	ABOUTDIALOG_H

#include <ui_AboutDialog.h>

class AboutDialog : public QDialog {
Q_OBJECT
public:
    AboutDialog();
    virtual ~AboutDialog();
private:
    Ui::Dialog aboutDialog;
};

#endif	/* ABOUTDIALOG_H */

