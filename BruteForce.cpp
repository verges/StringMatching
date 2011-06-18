/*
 * BruteForce.cpp
 *
 *  Created on: Mar 22, 2011
 *      Author: an
 */

#include "BruteForce.h"
#include <QCoreApplication>

BruteForce::BruteForce() : Algorithm() {

}

BruteForce::~BruteForce() {
    // TODO Auto-generated destructor stub
}

QList<int> BruteForce::find(QString pattern, int m, QString & text, int n, QProgressBar * pBar) {

    int i, j;

    QList<int> results;

    // Progress bar
    int refresher = 0;
    pBar->setMaximum(n);
    int period = n / 100;
    // Progress bar end

    for (j = 0; j <= n - m; ++j) {
        for (i = 0; i < m && pattern.at(i) == text.at(i + j); ++i) ;
        if (i >= m) {
            results.append(j);
        }

        // Progress bar
        refresher++;
        if (refresher == period) {
            pBar->setValue(j);
            QCoreApplication::processEvents();
            refresher = 0;
        }
    }

    pBar->setValue(n);
    QCoreApplication::processEvents();
    // Progress bar ends

    return results;

}
