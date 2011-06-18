/* 
 * File:   ResultsChartWidget.h
 * Author: rafal
 *
 * Created on 17 marzec 2011, 13:02
 */

#ifndef RESULTSCHARTWIDGET_H
#define	RESULTSCHARTWIDGET_H

#include <QWidget>
#include <enums.h>

class ResultsChartWidget : public QWidget {
    Q_OBJECT
public:
    ResultsChartWidget(QWidget *parent = 0);
    virtual ~ResultsChartWidget();
    void addResult(QString algName, QString fileName, QString pattern, int time, int fileSize);
    void setChartType(chartType cType);
    void clearResults();
private:

    chartType cType;
    void paintEvent(QPaintEvent *event);
    int getHighestTime();

    class Result {
    public:
        Result() {
            algName = "";
            time = 0;
            fileSize = 0;
            fileName = "";
            pattern = "";
        }
    
        QString algName;
        int time;
        int fileSize;
        QString fileName;
        QString pattern;
    };

    QList<Result> resList;
    QList<Qt::GlobalColor> colorList;
    QList<symbol> symList;
    QList<QString> algList;
};


#endif	/* RESULTSCHARTWIDGET_H */

