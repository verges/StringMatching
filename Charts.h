    /*
 * File:   Charts.h
 * Author: rafal
 *
 * Created on 16 marzec 2011, 13:02
 */

#ifndef CHARTS_H
#define	CHARTS_H

#define PerConst = 3.6;
#include <QObject>
#include <QPainter>
#include <QVector>
#include <QDebug>
#include <QLinearGradient>
#include <math.h>
#include <enums.h>

class pieceNC
{
public:
    explicit pieceNC();
    void addName(QString name);
    void setColor(Qt::GlobalColor);
    void setColor(QColor color);
    void setPerc(float Percentage);

    QString pname;
    QColor rgbColor;
    float pPerc;
    float xValue;
    float patternSize;
    float fileSize;
    symbol sym;

private:

};

class Charts
{
public:

    explicit Charts();
    ~Charts();
    enum type {Histogramm, Pie, Dpie, Scatter};
    enum legend_type{/*Horizontal,*/ Vertical, Round, ScatterLegend};
    int addPiece(QString name,Qt::GlobalColor,float Percentage, int fileSize, int patternSize, symbol sym);
    int addPiece(QString name,QColor, float Percentage, int fileSize, int patternSize, symbol sym);
    int setCords(double x, double y, double w, double h);
    int setLegendCords(int x, int y);
    int setType(Charts::type t);
    int setLegendType(Charts::legend_type t);
    int setShadows(bool ok = true);
    int setFont(QFont f);
    int draw(QPainter *painter);
    int drawLegend(QPainter *painter);
    double palpha;

private:
    double cX,cY,cW,cH,pW;
    int lX,lY;
    int nPiece;
    bool shadows;
    QVector<pieceNC> pieces;
    int ctype, cltype;
    QFont font;
    //QPainter *cpainter;
    QPointF GetPoint(double angle, double R1 = 0, double R2 = 0);
    int GetQuater(double angle);
    double Angle360(double angle);
    int getMaximumYValue();
    int getMaximumFileSizeValue();
    int getMaximumPatternSizeValue();


signals:

public slots:

};


#endif // CHARTS_H
