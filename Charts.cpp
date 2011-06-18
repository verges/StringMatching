/*
 * File:   Charts.cpp
 * Author: rafal
 *
 * Created on 16 marzec 2011, 13:02
 */

#include "Charts.h"
//#include <qgraphicsscene>
#include <iostream>

Charts::Charts()//QPainter *painter)

{
    font.setFamily("verdana");
    font.setPixelSize(15);
    //painter = painter;
    //painter->setFont(font);
    ctype = Charts::Dpie;
    cltype = Charts::Vertical;
    cX = 20;
    cY = 0;
    cW = 100;
    cH = 100;
    //lX = cX+cW+20;
    //lY = cY;
    shadows = true;
}
Charts::~Charts()
{
    pieces.clear();
}

int Charts::addPiece(QString name,Qt::GlobalColor color,float Percentage, int fileSize, int patternSize, symbol sym)
{
    this->nPiece++;

    pieceNC piece;
    piece.addName(name);
    piece.setColor(color);
    piece.setPerc(Percentage);
    piece.fileSize = fileSize;
    piece.patternSize = patternSize;
    piece.sym = sym;
    pieces.append(piece);

    return 0;
}
int Charts::addPiece(QString name, QColor color, float Percentage, int fileSize, int patternSize, symbol sym)
{
    this->nPiece++;
    pieceNC piece;
    piece.addName(name);
    piece.setColor(color);
    piece.setPerc(Percentage);
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
    //this->lX = cX+cW+20;
    //this->lY = cY;

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
    if (this->ctype==Charts::Pie)
    {
      pW = 0;
      double pdegree = 0;

      //Options
      QLinearGradient gradient(cX+0.5*cW,cY,cX+0.5*cW,cY+cH*2.5);
      gradient.setColorAt(1,Qt::black);


      //Draw
      //pdegree = (360/100)*pieces[i].pPerc;
      if (shadows)
      {
          double sumangle = 0;
          for (int i=0;i<pieces.size();i++)
          {
              sumangle += 3.6*pieces[i].pPerc;
          }
          painter->setBrush(Qt::darkGray);
          painter->drawPie(cX,cY+pW+5,cW,cH,palpha*16,sumangle*16);
      }

      QPen pen;
      pen.setWidth(2);

      for (int i=0;i<pieces.size();i++)
      {
        gradient.setColorAt(0,pieces[i].rgbColor);
        painter->setBrush(gradient);
        pen.setColor(pieces[i].rgbColor);
        painter->setPen(pen);
        pdegree = 3.6*pieces[i].pPerc;
        painter->drawPie(cX,cY,cW,cH,palpha*16,pdegree*16);
        palpha += pdegree;
      }
    }
    else if (this->ctype==Charts::Dpie)
    {
        pW = 50;
        double pdegree = 0;
        QPointF p;

        QLinearGradient gradient(cX-0.5*cW,cY+cH/2,cX+1.5*cW,cY+cH/2);
        gradient.setColorAt(0,Qt::black);
        gradient.setColorAt(1,Qt::white);
        QLinearGradient gradient_side(cX,cY+cH,cX+cW,cY+cH);
        gradient_side.setColorAt(0,Qt::black);

        double sumangle = 0;
        for (int i=0;i<pieces.size();i++)
        {
            sumangle += 3.6*pieces[i].pPerc;
        }
        if (shadows)
        {
            painter->setBrush(Qt::darkGray);
            painter->drawPie(cX,cY+pW+5,cW,cH,palpha*16,sumangle*16);
        }
        int q = GetQuater(palpha+sumangle);

        if (q ==2 || q==3)
        {
            QPointF p = GetPoint(palpha+sumangle);
            QPointF points[4] =
            {
                QPointF(p.x(),p.y()),
                QPointF(p.x(),p.y()+pW),
                QPointF(cX+cW/2,cY+cH/2+pW),
                QPointF(cX+cW/2,cY+cH/2)
            };
            gradient_side.setColorAt(1,pieces[pieces.size()-1].rgbColor);
            painter->setBrush(gradient_side);
            painter->drawPolygon(points,4);
        }
        p = GetPoint(palpha);
        q = GetQuater(palpha);
        if (q ==1 || q==4)
        {
            QPointF points[4] =
            {
                QPointF(p.x(),p.y()),
                QPointF(p.x(),p.y()+pW),
                QPointF(cX+cW/2,cY+cH/2+pW),
                QPointF(cX+cW/2,cY+cH/2)
            };
            gradient_side.setColorAt(1,pieces[0].rgbColor);
            painter->setBrush(gradient_side);
            painter->drawPolygon(points,4);
        }

        for (int i=0;i<pieces.size();i++)
        {
          gradient.setColorAt(0.5,pieces[i].rgbColor);
          painter->setBrush(gradient);
          pdegree = 3.6*pieces[i].pPerc;
          painter->drawPie(cX,cY,cW,cH,palpha*16,pdegree*16);

          double a_ = Angle360(palpha);
          int q_ = GetQuater(palpha);

          palpha += pdegree;

          double a = Angle360(palpha);
          int q = GetQuater(palpha);

          QPainterPath path;
          p = GetPoint(palpha);

          if((q == 3 || q == 4) && (q_ == 3 || q_ == 4))
          {
              // 1)
              if (a>a_)
              {
                  QPointF p_old = GetPoint(palpha-pdegree);
                  path.moveTo(p_old.x()-1,p_old.y());
                  path.arcTo(cX,cY,cW,cH,palpha-pdegree,pdegree);
                  path.lineTo(p.x(),p.y()+pW);
                  path.arcTo(cX,cY+pW,cW,cH,palpha,-pdegree);
              }
              // 2)
              else
              {
                  path.moveTo(cX,cY+cH/2);
                  path.arcTo(cX,cY,cW,cH,180,Angle360(palpha)-180);
                  path.lineTo(p.x(),p.y()+pW);
                  path.arcTo(cX,cY+pW,cW,cH,Angle360(palpha),-Angle360(palpha)+180);
                  path.lineTo(cX,cY+cH/2);

                  path.moveTo(p.x(),p.y());
                  path.arcTo(cX,cY,cW,cH,palpha-pdegree,360-Angle360(palpha-pdegree));
                  path.lineTo(cX+cW,cY+cH/2+pW);
                  path.arcTo(cX,cY+pW,cW,cH,0,-360+Angle360(palpha-pdegree));
              }

          }
          // 3)
          else if((q == 3 || q == 4) && (q_ == 1 || q_ == 2) && a>a_ )
          {
              path.moveTo(cX,cY+cH/2);
              path.arcTo(cX,cY,cW,cH,180,Angle360(palpha)-180);
              path.lineTo(p.x(),p.y()+pW);
              path.arcTo(cX,cY+pW,cW,cH,Angle360(palpha),-Angle360(palpha)+180);
              path.lineTo(cX,cY+cH/2);
          }
          // 4)
          else if((q == 1 || q == 2) && (q_ == 3 || q_ == 4) && a<a_)
          {
              p = GetPoint(palpha-pdegree);
              path.moveTo(p.x(),p.y());
              path.arcTo(cX,cY,cW,cH,palpha-pdegree,360-Angle360(palpha-pdegree));
              path.lineTo(cX+cW,cY+cH/2+pW);
              path.arcTo(cX,cY+pW,cW,cH,0,-360+Angle360(palpha-pdegree));
          }
          // 5)
          else if((q ==1 || q==2) && (q_==1 || q_==2) && a<a_)
          {
              path.moveTo(cX,cY+cH/2);
              path.arcTo(cX,cY,cW,cH,180,180);
              path.lineTo(cX+cW,cY+cH/2+pW);
              path.arcTo(cX,cY+pW,cW,cH,0,-180);
              path.lineTo(cX,cY+cH/2);
          }
          if (!path.isEmpty())
          {
              gradient_side.setColorAt(1,pieces[i].rgbColor);
              painter->setBrush(gradient_side);
              painter->drawPath(path);
          }
        }
    }
    else if (this->ctype==Charts::Histogramm)
    {
        double pDist = 15;
        double firstDist = 10;
        //double pW = (cW-(pieces.size())*pDist)/pieces.size() - 200;
        double pW = 32;
        int maxYValue = getMaximumYValue();
        //cH = maxYValue;

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

                if (pieces[i].pPerc != 0) {
                    painter->drawRect(firstDist+cX+pDist+i*(pW + pDist)-pDist/2,cY+cH-1,pW,-(cH-20)/maxYValue*pieces[i].pPerc+pDist/2 - 5);
                }
            }
            gradient.setColorAt(1,pieces[i].rgbColor);
            painter->setBrush(gradient);
            pen.setColor(pieces[i].rgbColor);
            painter->setPen(pen);
            if (pieces[i].pPerc != 0) {
                painter->drawRect(firstDist+cX+pDist+i*(pW + pDist),cY+cH,pW,-(cH-20)/maxYValue*pieces[i].pPerc - 5);
            }
            //std::cout << maxYValue << std::endl;
            QString label = QString::number(pieces[i].pPerc);//+"ms";
            painter->setPen(Qt::SolidLine);
            if (pieces[i].pPerc != 0) {
                painter->drawText(firstDist+cX+pDist+i*(pW + pDist)+pW/2-painter->fontMetrics().width(label)/2,cY+cH-(cH-20)/maxYValue*pieces[i].pPerc-painter->fontMetrics().height()/2,label);
            } else {
                painter->drawText(firstDist+cX+pDist+i*(pW + pDist)+pW/2-painter->fontMetrics().width(label)/2,cY+cH,label);
            }
        }
        painter->setPen(Qt::SolidLine);
        /*for (int i=1;i<10;i++)
        {
            painter->drawLine(cX-3,cY+cH/10*i,cX+3,cY+cH/10*i);    //������� �� ��� Y
            //painter->drawText(cX-20,cY+cH/10*i,QString::number((10-i)*10)+"%");
        }*/
        painter->drawText(cX-10,cY+10,"t[ms]");
        painter->drawLine(cX,cY+cH,cX,cY+15);         //���
        painter->drawLine(cX,cY+15,cX+4,cY+10+15);       //�������
        painter->drawLine(cX,cY+15,cX-4,cY+10+15);
        painter->drawLine(cX,cY+cH,cX+cW-200,cY+cH);   //��� �

    }
    else if (this->ctype==Charts::Scatter)
    {
        //double pDist = 15;
        //double firstDist = 10;
        //double pW = (cW-(pieces.size())*pDist)/pieces.size() - 200;
        //double pW = 32;
        //int maxYValue = getMaximumYValue();
        int maxFileSizeValue = getMaximumFileSizeValue();
        int maxPatternSizeValue = getMaximumPatternSizeValue();
        //cH = maxYValue;

        //QLinearGradient gradient(cX+20+20+cW/2,cY,cX+20+20+cW/2,cY+cH);
        //gradient.setColorAt(0,Qt::black);
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
        painter->drawLine(cX+20,cY+cH,cX+20,cY+15);         //
        painter->drawLine(cX+20,cY+15,cX+20+4,cY+10+15);       //
        painter->drawLine(cX+20,cY+15,cX+20-4,cY+10+15);
        painter->drawLine(cX+20,cY+cH,cX+20+cW-200,cY+cH);
        painter->drawLine(cX+20+cW-200-10,cY+cH-4,cX+20+cW-200,cY+cH); //
        painter->drawLine(cX+20+cW-200-10,cY+cH+4,cX+20+cW-200,cY+cH); //
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
        if (pieces.at(i).pPerc > max) {
            max = pieces.at(i).pPerc;
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
    /*case Nightcharts::Horizontal:
    {
        int dist = 5;
        painter->setBrush(Qt::white);
        float x = cX;
        float y = cY+cH+20+dist;
        //painter->drawRoundRect(cX+cW+20,cY,dist*2+200,pieces.size()*(fontmetr.height()+2*dist)+dist,15,15);
        for (int i=0;i<pieces.size();i++)
        {
            painter->setBrush(pieces[i].rgbColor);
            x += fontmetr.height()+2*dist;
            if (i%3 == 0)
            {
                x = cX;
                y += dist+fontmetr.height();
            }
            painter->drawRect(x,y,fontmetr.height(),fontmetr.height());
            QString label = pieces[i].pname + " - " + QString::number(pieces[i].pPerc)+"%";
            painter->drawText(x+fontmetr.height()+dist,y+fontmetr.height()/2+dist,label);
            x += fontmetr.width(label);
        }
        break;
    }*/
    case Charts::Vertical:
    {
        int dist = 5;
        painter->setBrush(Qt::white);
        if (pieces.size() > 0) {
            painter->drawText(lX+27, lY, "Algorithm\tPatt. size\tFile size");
        }
        //painter->drawRoundRect(cX+cW+20,cY,dist*2+200,pieces.size()*(painter->fontMetrics().height()+2*dist)+dist,15,15);
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

                //painter->drawRect(x,y,painter->fontMetrics().height(),painter->fontMetrics().height());

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

    case Charts::Round:
        for (int i=pieces.size()-1;i>=0;i--)
        {
            float len = 100;
            double pdegree = 3.6*pieces[i].pPerc;
            angle -= pdegree/2;
            QPointF p = GetPoint(angle);
            QPointF p_ = GetPoint(angle, cW+len,cH+len);
            int q = GetQuater(angle);
            if (q == 3 || q == 4)
            {
                p.setY(p.y()+pW/2);
                p_.setY(p_.y()+pW/2);
            }
            painter->drawLine(p.x(),p.y(),p_.x(),p_.y());
            QString label = pieces[i].pname + " - " + QString::number(pieces[i].pPerc)+"%";
            float recW = painter->fontMetrics().width(label)+10;
            float recH = painter->fontMetrics().height()+10;
            p_.setX(p_.x()-recW/2 + recW/2*cos(angle*M_PI/180));
            p_.setY(p_.y()+recH/2 + recH/2*sin(angle*M_PI/180));
            painter->setBrush(Qt::white);
            painter->drawRoundRect(p_.x() ,p_.y(), recW, -recH);
            painter->drawText(p_.x()+5, p_.y()-recH/2+5, label);
            angle -= pdegree/2;
         }
        break;
    }

    return 0;
}

QPointF Charts::GetPoint(double angle, double R1, double R2)
{
    if (R1 == 0 && R2 == 0)
    {
        R1 = cW;
        R2 = cH;
    }
    QPointF point;
    double x = R1/2*cos(angle*M_PI/180);
    x+=cW/2+cX;
    double y = -R2/2*sin(angle*M_PI/180);
    y+=cH/2+cY;
    point.setX(x);
    point.setY(y);
    return point;
}

int Charts::GetQuater(double angle)
{
    angle = Angle360(angle);

    if(angle>=0 && angle<90)
        return 1;
    if(angle>=90 && angle<180)
        return 2;
    if(angle>=180 && angle<270)
        return 3;
    if(angle>=270 && angle<360)
        return 4;

    return 0;
}

double Charts::Angle360(double angle)
{
    int i = (int)angle;
    double delta = angle - i;
    return (i%360 + delta);
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

void pieceNC::setPerc(float Percentage)
{
    pPerc = Percentage;
}
