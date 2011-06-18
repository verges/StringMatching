/*
 * MorrisPratt.h
 *
 *  Created on: Mar 23, 2011
 *      Author: an
 */

#ifndef MORRISPRATT_H_
#define MORRISPRATT_H_

#include "Algorithm.h"
#include <QString>

class MorrisPratt : public Algorithm {
private:
    void pre(QString pattern, int m, int mpNext[]);
public:
    MorrisPratt();
    ~MorrisPratt();
    QList<int> find(QString pattern, int m, QString & text, int n, QProgressBar * pBar);

};

#endif /* MORRISPRATT_H_ */
