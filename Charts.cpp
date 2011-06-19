/*
 * File:   Charts.cpp
 * Author: rafal
 *
 * Created on 16 marzec 2011, 13:02
 */

#include "Charts.h"
#include <iostream>

Charts::Charts()

{
    font.setFamily("verdana");
    font.setPixelSize(15);

    cX = 20;
    cY = 0;
    cW = 100;
    cH = 100;
    shadows = true;
}
Charts::~Charts()
{
    pieces.clear();
}

int Charts::addPiece(QString name,Qt::GlobalColor color,float value, int fileSize, int patternSize, symbol sym)
{
    this->nPiece++;

    pieceNC piece;
    piece.addName(name);
    piece.setColor(color);
    piece.setChartValue(value);
    piece.fileSize = fileSize;
    piece.patternSize = patternSize;
    piece.sym = sym;
    pieces.append(piece);

    return 0;
}
int Charts::addPiece(QString name, QColor color, float value, int fileSize, int patternSize, symbol sym)
{
    this->nPiece++;
    pieceNC piece;
    piece.addName(name);
    piece.setColor(color);
    piece.setChartValue(value);
    piece.fileSize = fileSize;
    piece.patternSize = patternSize;
    piece.sym = sym;
    pieces.append(piece);

    return 0;
}
int Charts::setCords(double x, double y, double w, double h)
{
    this->cX = x;
    this->cY = y;
    this->cW = w;
    this->cH = h;

    return 0;
}
int Charts::setLegendCords(int x, int y)
{
    this->lX = x;
    this->lY = y;

    return 0;
}

int Charts::setType(Charts::type t)
{
    this->ctype = t;

    return 0;
}

int Charts::setLegendType(Charts::legend_type t)
{
    this->cltype = t;

    return 0;
}
int Charts::setFont(QFont f)
{
    this->font = f;

    return 0;
}

int Charts::setShadows(bool ok)
{
    this->shadows = ok;

    return 0;
}

int Charts::draw(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);

    if (this->ctype==Charts::Histogramm)
    {
        double pDist = 15;
        double firstDist = 10;
        double pW = 32;
        int maxYValue = getMaximumYValue();

        QLinearGradient gradient(cX+cW/2,cY,cX+cW/2,cY+cH);
        gradient.setColorAt(0,Qt::black);
        QPen pen;
        pen.setWidth(3);

        for (int i=0;i<pieces.size();i++)
        {
            if (shadows)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(Qt::darkGray);

                if (pieces[i].value!= 0) {
                    painter->drawRect(firstDist+cX+pDist+i*(pW + pDist)-pDist/2,cY+cH-1,pW,-(cH-20)/maxYValue*pieces[i].value+pDist/2 - 5);
                }
            }
            gradient.setColorAt(1,pieces[i].rgbColor);
            painter->setBrush(gradient);
            pen.setColor(pieces[i].rgbColor);
            painter->setPen(pen);
            if (pieces[i].value!= 0) {
                painter->drawRect(firstDist+cX+pDist+i*(pW + pDist),cY+cH,pW,-(cH-20)/maxYValue*pieces[i].value - 5);
            }
            QString label = QString::number(pieces[i].value);
            painter->setPen(Qt::SolidLine);
            if (pieces[i].value != 0) {
                painter->drawText(firstDist+cX+pDist+i*(pW + pDist)+pW/2-painter->fontMetrics().width(label)/2,cY+cH-(cH-20)/maxYValue*pieces[i].value-painter->fontMetrics().height()/2,label);
            } else {
                painter->drawText(firstDist+cX+pDist+i*(pW + pDist)+pW/2-painter->fontMetrics().width(label)/2,cY+cH,label);
            }
        }
        painter->setPen(Qt::SolidLine);
        painter->drawText(cX-10,cY+10,"t[ms]");
        painter->drawLine(cX,cY+cH,cX,cY+15);
        painter->drawLine(cX,cY+15,cX+4,cY+10+15);
        painter->drawLine(cX,cY+15,cX-4,cY+10+15);
        painter->drawLine(cX,cY+cH,cX+cW-200,cY+cH);

    }
    else if (this->ctype==Charts::Scatter)
    {
        int maxFileSizeValue = getMaximumFileSizeValue();
        int maxPatternSizeValue = getMaximumPatternSizeValue();

        QPen pen;

        int addX;
        for (int i=0;i<pieces.size();i++)
        {
            int xx;
            if ((int)pieces.at(i).fileSize == 0 || maxFileSizeValue == 0) {
                addX = 15;
                xx = 0;
            } else {
                addX = 0;
                xx = pieces.at(i).fileSize/maxFileSizeValue * (cW+20 - 220);
            }

            pen.setColor(pieces.at(i).rgbColor);

            if (pieces.at(i).sym == circle) {
                pen.setWidth(3);
                painter->setPen(pen);
                painter->drawEllipse(addX + xx + 10,
                                     cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35),
                                     10,10);
            } else if (pieces.at(i).sym == rectangle) {
                pen.setWidth(3);
                painter->setPen(pen);
                painter->drawRect(addX + xx + 10,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35),
                                  10,10);
            } else if (pieces.at(i).sym == dot) {
                pen.setWidth(3);
                painter->setPen(pen);
                painter->drawRect(addX + xx + 10,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35),
                                  10,10);
                pen.setWidth(8);
                painter->setPen(pen);
                painter->drawPoint(addX + xx + 10 + 5,
                                   cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35) + 5);
            } else if (pieces.at(i).sym == triangle) {
                pen.setWidth(3);
                painter->setPen(pen);
                painter->drawLine(addX + xx + 15,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35),
                                  addX + xx + 10,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35) + 10);
                painter->drawLine(addX + xx + 10,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35) + 10,
                                  addX + xx + 20,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35) + 10);
                painter->drawLine(addX + xx + 20,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35) + 10,
                                  addX + xx + 15,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35));
            } else if (pieces.at(i).sym == revTriangle) {
                pen.setWidth(3);
                painter->setPen(pen);
                painter->drawLine(addX + xx + 10,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35),
                                  addX + xx + 15,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35) + 10);
                painter->drawLine(addX + xx + 15,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35) + 10,
                                  addX + xx + 20,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35));
                painter->drawLine(addX + xx + 10,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35),
                                  addX + xx + 20,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35));
            } else if (pieces.at(i).sym == cross) {
                pen.setWidth(3);
                painter->setPen(pen);
                painter->drawLine(addX + xx + 10,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35),
                                  addX + xx + 20,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35) + 10);
                painter->drawLine(addX + xx + 10,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35) + 10,
                                  addX + xx + 20,
                                  cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35));

            }


            painter->setPen(Qt::SolidLine);
            painter->drawLine(addX + xx + 10 + 5,
                              cY+cH - 5,
                              addX + xx + 10 + 5,
                              cY+cH + 5);


            QString label = QString::number(pieces[i].fileSize);
            painter->drawText(addX + xx + 10 + 5 - painter->fontMetrics().width(label)/2,
                              cY+cH + painter->fontMetrics().height() + 3,
                              label);

            int zeroCoef = 0;
            if ((int)pieces.at(i).fileSize == 0 || maxFileSizeValue == 0) {
                zeroCoef = 15;
            }
            painter->drawLine(addX + 20 + cX - 5-zeroCoef,
                              cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35)+5,
                              addX + 20 + cX + 5-zeroCoef,
                              cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35)+5);


            QString label2 = QString::number(pieces[i].patternSize);
            painter->drawText(addX + cX + 12 - painter->fontMetrics().width(label2)-zeroCoef,
                              cY+cH - (pieces.at(i).patternSize/maxPatternSizeValue)*(cH-35) - painter->fontMetrics().height() + 8 + 17,
                              label2);
        }

        painter->setPen(Qt::SolidLine);
        painter->drawText(cX+20-10,cY+10,"pattern size");
        painter->drawLine(cX+20,cY+cH,cX+20,cY+15);
        painter->drawLine(cX+20,cY+15,cX+20+4,cY+10+15);
        painter->drawLine(cX+20,cY+15,cX+20-4,cY+10+15);
        painter->drawLine(cX+20,cY+cH,cX+20+cW-200,cY+cH);
        painter->drawLine(cX+20+cW-200-10,cY+cH-4,cX+20+cW-200,cY+cH);
        painter->drawLine(cX+20+cW-200-10,cY+cH+4,cX+20+cW-200,cY+cH);
        painter->drawText(cX+20+cW-195,cY+cH + painter->fontMetrics().height()/2 - 5,"file size[kB]");

    }
    return 0;
}

int Charts::getMaximumFileSizeValue() {
    int max = 0;

    for (int i=0;i<pieces.size();i++) {
        if (pieces.at(i).fileSize > max) {
            max = pieces.at(i).fileSize;
        }
    }

    return max;
}

int Charts::getMaximumPatternSizeValue() {
    int max = 0;

    for (int i=0;i<pieces.size();i++) {
        if (pieces.at(i).patternSize > max) {
            max = pieces.at(i).patternSize;
        }
    }

    return max;
}

int Charts::getMaximumYValue() {

    int max = 0;

    for (int i=0;i<pieces.size();i++) {
        if (pieces.at(i).value > max) {
            max = pieces.at(i).value;
        }
    }

    return max;
}

int Charts::drawLegend(QPainter *painter)
{
    //double ptext = 25;
    double angle = palpha;
    painter->setPen(Qt::SolidLine);

    switch(cltype)
    {
    case Charts::Vertical:
    {
        int dist = 5;
        painter->setBrush(Qt::white);
        if (pieces.size() > 0) {
            painter->drawText(lX+27, lY, "Algorithm\tPatt. size\tFile size");
        }
        for (int i=pieces.size()-1;i>=0;i--)
        {
            painter->setBrush(pieces[i].rgbColor);
            int x = lX+dist;
            int y = lY+dist+i*(painter->fontMetrics().height()+2*dist);
            painter->drawRect(x,y,painter->fontMetrics().height(),painter->fontMetrics().height());
            if (pieces[i].fileSize > 0) {
                painter->drawText(x+painter->fontMetrics().height()+dist,y+painter->fontMetrics().height()/2+dist,
                                  pieces[i].pname + "\t" + QString::number(pieces[i].patternSize) + "\t" + QString::number(pieces[i].fileSize) + " KB");
            } else {
                painter->drawText(x+painter->fontMetrics().height()+dist,y+painter->fontMetrics().height()/2+dist,
                                  pieces[i].pname + "\t" + QString::number(pieces[i].patternSize) + "\t" + " < 1 KB");
            }
        }
        break;
    }

    case Charts::ScatterLegend:
    {
        int dist = 5;
        if (pieces.size() > 0) {
            painter->drawText(lX+27, lY, "Algorithm\tSymbol");
            painter->drawText(lX+27, lY+200, "Blue: short time");
            painter->drawText(lX+27, lY+220, "Red: long time");
        }

        QList<QString> algList;

        for (int i=pieces.size()-1;i>=0;i--)
        {
            if (!algList.contains(pieces.at(i).pname)) {

                algList.append(pieces.at(i).pname);
                painter->setPen(Qt::SolidLine);
                int x = lX+dist+70;
                int y = lY+dist+(algList.size()-1)*(painter->fontMetrics().height()+2*dist) - 10;

                painter->drawText(x - 70 +painter->fontMetrics().height()+dist,y + 10 +painter->fontMetrics().height()/2+dist,
                                  pieces[i].pname + "\t");

                QPen pen;
                if (pieces.at(i).sym == circle) {
                    pen.setWidth(3);
                    painter->setPen(pen);
                    painter->drawEllipse(x+50,
                                         y+painter->fontMetrics().height()/2+dist,
                                         10,10);
                } else if (pieces.at(i).sym == rectangle) {
                    pen.setWidth(3);
                    painter->setPen(pen);
                    painter->drawRect(x+50,
                                      y+painter->fontMetrics().height()/2+dist,
                                      10,10);
                } else if (pieces.at(i).sym == dot) {
                    pen.setWidth(3);
                    painter->setPen(pen);
                    painter->drawRect(x+50,
                                      y+painter->fontMetrics().height()/2+dist,
                                      10,10);
                    pen.setWidth(8);
                    painter->setPen(pen);
                    painter->drawPoint(x+50+5,
                                       y+painter->fontMetrics().height()/2+dist+5);
                } else if (pieces.at(i).sym == triangle) {
                    pen.setWidth(3);
                    painter->setPen(pen);
                    painter->drawLine(x+50+5,
                                      y+painter->fontMetrics().height()/2+dist,
                                      x+50,
                                      y+painter->fontMetrics().height()/2+dist + 10);
                    painter->drawLine(x+50,
                                      y+painter->fontMetrics().height()/2+dist + 10,
                                      x+50+10,
                                      y+painter->fontMetrics().height()/2+dist + 10);
                    painter->drawLine(x+50+10,
                                      y+painter->fontMetrics().height()/2+dist + 10,
                                      x+50+5,
                                      y+painter->fontMetrics().height()/2+dist);
                } else if (pieces.at(i).sym == revTriangle) {
                    pen.setWidth(3);
                    painter->setPen(pen);
                    painter->drawLine(x+50,
                                      y+painter->fontMetrics().height()/2+dist,
                                      x+50+5,
                                      y+painter->fontMetrics().height()/2+dist + 10);
                    painter->drawLine(x+50+5,
                                      y+painter->fontMetrics().height()/2+dist + 10,
                                      x+50+10,
                                      y+painter->fontMetrics().height()/2+dist);
                    painter->drawLine(x+50,
                                      y+painter->fontMetrics().height()/2+dist,
                                      x+50+10,
                                      y+painter->fontMetrics().height()/2+dist);
                } else if (pieces.at(i).sym == cross) {
                    pen.setWidth(3);
                    painter->setPen(pen);
                    painter->drawLine(x+50,
                                      y+painter->fontMetrics().height()/2+dist,
                                      x+50+10,
                                      y+painter->fontMetrics().height()/2+dist + 10);
                    painter->drawLine(x+50,
                                      y+painter->fontMetrics().height()/2+dist + 10,
                                      x+50+10,
                                      y+painter->fontMetrics().height()/2+dist);

                }
            }
        }
        break;
    }
    }
    return 0;
}

pieceNC::pieceNC()
{
}
void pieceNC::addName(QString name)
{
    pname = name;
}
void pieceNC::setColor(Qt::GlobalColor color)
{
    rgbColor = color;
}
void pieceNC::setColor(QColor color)
{
    rgbColor = color;
}

void pieceNC::setChartValue(float value)
{
    this->value = value;
}
