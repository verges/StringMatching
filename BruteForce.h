/*
 * BruteForce.h
 *
 *  Created on: Mar 22, 2011
 *      Author: an
 */

#ifndef BRUTEFORCE_H_
#define BRUTEFORCE_H_

#include "Algorithm.h"

class BruteForce : public Algorithm {
public:
    BruteForce();
    virtual ~BruteForce();
    QList<int> find(QString pattern, int m, QString & text, int n, QProgressBar * pBar);
};

#endif /* BRUTEFORCE_H_ */
