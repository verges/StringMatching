/*
 * TwoWay.h
 *
 *  Created on: Mar 27, 2011
 *      Author: an
 */

#ifndef TWOWAY_H_
#define TWOWAY_H_

#include "Algorithm.h"

class TwoWay: public Algorithm {
private:
	int maxSuf(char *x, int m, int *p);
	int maxSufTilde(char *x, int m, int *p);
	int max(int x1, int x2);
public:
	QList<int> find(QString pattern, int m, QString & text, int n, QProgressBar * pBar);
};

#endif /* TWOWAY_H_ */
