/* 
 * File:   MainWindow.h
 * Author: rafal
 *
 * Created on 6 marzec 2011, 15:29
 */

#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include <ui_MainWindow.h>
#include <ResultsChartWidget.h>
#include <enums.h>

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
private:
    bool fileLoaded;
    QString textFilePath;
    QString textFileContents;
    ResultsChartWidget * chartWidget;

    bool loadFile(QString & textFilePath, QString & textFileContents);
    /**
     * @brief Reference to GUI elements.
     */
    Ui::MainWindow mainWindow;

    //QList<int> karpRabin(QString & pattern, int m, QString & text, int n);

    //int preSo(char * pattern, int m, unsigned int S[]);
    //QList<int> searchOr(char * pattern, int m, char * text, int n);

    QString processOutput(QList<int> results, int elapsedTime);
    QString getNameShortcut(QString name);

    bool generalLock;

private slots:
    void showAboutDialog();
    void chooseTextFile();
    void processFile();
    void setChartType();
    void clearResults();
};

#endif	/* MAINWINDOW_H */

