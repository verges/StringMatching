/* 
 * File:   MainWindow.cpp
 * Author: rafal
 * 
 * Created on 6 marzec 2011, 15:29
 */

#include "MainWindow.h"
#include <AboutDialog.h>
#include "BruteForce.h"
#include "MorrisPratt.h"
#include "BoyerMoore.h"
#include "TwoWay.h"
#include <QFileDialog>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>
#include <QTime>

MainWindow::MainWindow() {
    mainWindow.setupUi(this);

    chartWidget = new ResultsChartWidget(mainWindow.centralwidget);
    chartWidget->setObjectName(QString::fromUtf8("widget"));
    chartWidget->setGeometry(QRect(10, 280, 780, 300));
    chartWidget->setChartType(histogram);
    mainWindow.timeHistogram->setChecked(true);

    QObject::connect(mainWindow.About_StringMatching, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    QObject::connect(mainWindow.chooseFileButton, SIGNAL(clicked()), this, SLOT(chooseTextFile()));
    QObject::connect(mainWindow.processButton, SIGNAL(clicked()), this, SLOT(processFile()));
    QObject::connect(mainWindow.timeHistogram, SIGNAL(clicked()), this, SLOT(setChartType()));
    QObject::connect(mainWindow.pattVsFileVsTime, SIGNAL(clicked()), this, SLOT(setChartType()));
    QObject::connect(mainWindow.clearResultsButton, SIGNAL(clicked()), this, SLOT(clearResults()));
    fileLoaded = false;
    generalLock = false;
}

MainWindow::~MainWindow() {

}

void MainWindow::clearResults() {
    chartWidget->clearResults();
    chartWidget->repaint();
}

void MainWindow::setChartType() {
    if (mainWindow.timeHistogram->isChecked()) {
        this->chartWidget->setChartType(histogram);
    } else if (mainWindow.pattVsFileVsTime->isChecked()) {
        this->chartWidget->setChartType(scatter);
    }

    chartWidget->repaint();
}

void MainWindow::showAboutDialog() {
    AboutDialog *aboutDialog = new AboutDialog();
    aboutDialog->exec();
    delete aboutDialog;
}

void MainWindow::chooseTextFile() {
    textFilePath = QFileDialog::getOpenFileName(this, tr("Choose file..."),
                                                "~/",
                                                tr("Text files (*.txt *.html *.xml)"));
    mainWindow.fileBrowser->setText(textFilePath);

    if (loadFile(textFilePath, textFileContents)) {
        fileLoaded = true;
        mainWindow.statusLabel->setText("File loaded");
    } else {
        mainWindow.statusLabel->setText("Could not load the file");
    }
}

bool MainWindow::loadFile(QString & textFilePath, QString & textFileContents) {
    if (generalLock == false) {
        generalLock = true;
    } else {
        return false;
    }

    QFile textFile(textFilePath);
    QString line;
    if (textFile.open(QIODevice::ReadOnly)) {
        //textFileContents = textFile.readAll();
        //int fileSize = textFile.size();
        mainWindow.progressBar->setMaximum(textFile.size());
        textFileContents = "";
        QTextStream stream(&textFile);
        int refresher = 0;
        mainWindow.statusLabel->setText("Loading file...");
        while (!stream.atEnd()) {
            line = stream.read(300);
            textFileContents += line;

            refresher++;
            if (refresher == 300) {
                mainWindow.progressBar->setValue(textFileContents.size());
                QCoreApplication::processEvents();
                refresher = 0;
            }
        }
        mainWindow.progressBar->setValue(textFile.size());
        QCoreApplication::processEvents();
        textFile.close();
        generalLock = false;
        return true;
    } else {
        generalLock = false;
        return false;
    }
}

QString MainWindow::getNameShortcut(QString name) {
    if (name == "Brute Force") {
        return "B. Force";
    } else if (name == "Search Or") {
        return "Search Or";
    } else if (name == "Morris-Pratt") {
        return "M-P";
    } else if (name == "Boyer-Moore") {
        return "B-M";
    } else if (name == "TwoWay") {
        return "TW";
    }else {
        return "unknown";
    }
}

void MainWindow::processFile() {

    if (generalLock == false) {
        generalLock = true;
    } else {
        return;
    }

    QTime timer;

    if (!fileLoaded) {
        mainWindow.statusLabel->setText("No file loaded");
        return;
    } else {
        QString pattern = mainWindow.patternTextEdit->toPlainText();
        if (pattern == "") {
            mainWindow.statusLabel->setText("No pattern given");
        } else {
            QString output = "";
            QList<int> results;
            int elapsedTime;

            mainWindow.statusLabel->setText("Processing file contents...");
            QCoreApplication::processEvents();

            if (mainWindow.chooseAlgCombo->currentText() == "Brute Force") {
            	BruteForce bf= BruteForce();
                timer.start();
                results = bf.find(pattern, pattern.size(), textFileContents, textFileContents.size(), mainWindow.progressBar);
                elapsedTime = timer.elapsed();
            }
            else if (mainWindow.chooseAlgCombo->currentText() == "Morris-Pratt") {
                MorrisPratt mp = MorrisPratt();
                timer.start();
                results = mp.find(pattern, pattern.size(), textFileContents, textFileContents.size(), mainWindow.progressBar);
                elapsedTime = timer.elapsed();
            }
            else if (mainWindow.chooseAlgCombo->currentText() == "Boyer-Moore") {
            	BoyerMoore bm = BoyerMoore();
            	timer.start();
            	results = bm.find(pattern, pattern.size(), textFileContents, textFileContents.size(), mainWindow.progressBar);
            	elapsedTime = timer.elapsed();
            }
            else if (mainWindow.chooseAlgCombo->currentText() == "TwoWay") {
                TwoWay tw = TwoWay();
                timer.start();
                results = tw.find(pattern, pattern.size(), textFileContents, textFileContents.size(), mainWindow.progressBar);
                elapsedTime = timer.elapsed();
            }
            else {
                mainWindow.statusLabel->setText("Algorithm not supported");
                return;
            }



            mainWindow.statusLabel->setText("Preparing results...");
            QCoreApplication::processEvents();
            output = processOutput(results, elapsedTime);

            chartWidget->addResult(getNameShortcut(mainWindow.chooseAlgCombo->currentText()), "", pattern, elapsedTime, textFileContents.size() / 1024);
            chartWidget->repaint();

            mainWindow.statusLabel->setText("Processing finished");

            mainWindow.consoleBrowser->setText(output);
        }
    }
    generalLock = false;
}

QString MainWindow::processOutput(QList<int> results, int elapsedTime) {
    QString output = "";

    output += "Processing time: " + QString::number(elapsedTime) + " ms\n";

    if (results.size() == 0) {
        mainWindow.progressBar->setMaximum(100);
        output += "Pattern not found\n";
        mainWindow.progressBar->setValue(100);
        QCoreApplication::processEvents();
    } else {
        output += "Pattern found in " + QString::number(results.size()) + " places.\n";
        mainWindow.progressBar->setMaximum(results.size());
        if (mainWindow.listOfFoundCheckBox->isChecked()) {
            int refresher = 0;
            int period = results.size() / 100;
            for (int i = 0; i < results.size(); i++) {
                refresher++;
                output += QString::number(results.at(i));
                output += ", ";

                if (refresher == period) {
                    mainWindow.progressBar->setValue(i);
                    QCoreApplication::processEvents();
                    refresher = 0;
                }
            }
        }
        output += "\n";
        mainWindow.progressBar->setValue(results.size());
        QCoreApplication::processEvents();
    }


    return output;
}
