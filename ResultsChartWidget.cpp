/* 
 * File:   ResultsChartWidget.cpp
 * Author: rafal
 * 
 * Created on 17 marzec 2011, 13:02
 */

#include <ResultsChartWidget.h>
#include <QPaintEvent>
#include <QPainter>
#include <QFont>
#include <Charts.h>

ResultsChartWidget::ResultsChartWidget(QWidget * parent) : QWidget(parent) {
    colorList.append(Qt::blue);
    colorList.append(Qt::green);
    colorList.append(Qt::red);
    colorList.append(Qt::cyan);
    colorList.append(Qt::magenta);
    colorList.append(Qt::yellow);
    colorList.append(Qt::black);
    colorList.append(Qt::gray);
    colorList.append(Qt::color0);
    colorList.append(Qt::darkBlue);

    symList.append(rectangle);
    symList.append(dot);
    symList.append(circle);
    symList.append(cross);
    symList.append(triangle);
    symList.append(revTriangle);
}

ResultsChartWidget::~ResultsChartWidget() {
    
}

void ResultsChartWidget::clearResults() {
    resList.clear();
}

void ResultsChartWidget::setChartType(chartType cType) {
    this->cType = cType;
}

void ResultsChartWidget::paintEvent(QPaintEvent * event) {
    QWidget::paintEvent(event);
    QPainter painter;
    painter.begin(this);
    Charts resultsChart;

    if (cType == histogram) {
        resultsChart.setType(Charts::Histogramm);
        resultsChart.setLegendType(Charts::Vertical);
        resultsChart.setLegendCords(500,10);
    } else if (cType == scatter) {
        resultsChart.setType(Charts::Scatter);
        resultsChart.setLegendType(Charts::ScatterLegend);
        resultsChart.setLegendCords(590,10);
    }

    resultsChart.setCords(10,0,this->width() - 20,this->height() - 20);

    int i = 0;

    if (cType == histogram) {
        if (resList.size() > 10) {
            i = resList.size()-10;
        }
    }

    for (; i < resList.size(); i++) {
        if (cType == histogram) {
            resultsChart.addPiece(resList[i].algName,
                                  colorList.at(i % 10), resList[i].time, resList[i].fileSize,
                                  resList[i].pattern.size(), dot);
        } else if (cType == scatter) {

            int highest = getHighestTime();
            float red;
            if (highest == 0 || resList[i].time == 0) {
                red = 0;
            } else {
                red = resList[i].time/(float)highest * 255;
            }

            int symNum;

            if (!algList.contains(resList[i].algName)) {
                algList.append(resList[i].algName);
                symNum = algList.size()-1;
            } else {
                symNum = algList.indexOf(resList[i].algName);
            }

            if (symList.size() > symNum) {
                resultsChart.addPiece(resList[i].algName, QColor::fromRgb(red, 0 , 255-red, 255),
                                      resList[i].time, resList[i].fileSize,
                                      resList[i].pattern.size(), symList[symNum]);
            } else {
                //should throw exception (not enough symbols)
            }
        }
    }
    
    resultsChart.draw(&painter);
    resultsChart.drawLegend(&painter);
}

int ResultsChartWidget::getHighestTime() {

    int highest = 0;

    for (int i = 0; i < resList.size(); i++) {
        if (resList[i].time > highest) {
            highest = resList[i].time;
        }
    }

    return highest;
}

void ResultsChartWidget::addResult(QString algName, QString fileName, QString pattern, int time, int fileSize) {
    Result result = Result();

    result.algName = algName;
    result.fileName = fileName;
    result.pattern = pattern;
    result.time = time;
    result.fileSize = fileSize;

    resList.append(result);
}

