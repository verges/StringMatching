/*
 * BoyerMoore.h
 *
 *  Created on: Mar 27, 2011
 *      Author: an
 */

#ifndef BOYERMOORE_H_
#define BOYERMOORE_H_

#include "Algorithm.h"

class BoyerMoore: public Algorithm {
private:
	int max(int x1, int x2);
	void preBmBc(char * x, int m, int bmBc[]);
	void preBmGs(char * x, int m, int bmGs[], int xsize);
	void suffixes(QString x, int m, int *suff);
public:
	QList<int> find(QString pattern, int m, QString & text, int n, QProgressBar * pBar);

};

#endif /* BOYERMOORE_H_ */
