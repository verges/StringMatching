/*
 * Algorithm.h
 *
 *  Created on: Mar 22, 2011
 *      Author: an
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <QList>
#include <QtGui/QProgressBar>
#include <QCoreApplication>

class Algorithm {
public:
	virtual QList<int> find(QString pattern, int m, QString & text, int n, QProgressBar * pBar)=0;
};

#endif /* ALGORITHM_H_ */
