/*
 * MorrisPratt.cpp
 *
 *  Created on: Mar 23, 2011
 *      Author: an
 */

#include "MorrisPratt.h"
#include <QCoreApplication>

MorrisPratt::MorrisPratt() : Algorithm() {
}

MorrisPratt::~MorrisPratt() {
    // TODO Auto-generated destructor stub
}

void MorrisPratt::pre(QString pattern, int m, int mpNext[]) {
    int i, j;

    i = 0;
    j = mpNext[0] = -1;
    while (i < m) {
        while (j > -1 && pattern.at(i) != pattern.at(j))
            j = mpNext[j];
        mpNext[++i] = ++j;
    }
}

QList<int> MorrisPratt::find(QString pattern, int m, QString & text, int n, QProgressBar * pBar) {
    int i, j, mpNext[pattern.size()];
    QList<int> results;

    // Preprocessing
    this->pre(pattern, m, mpNext);

    // Progress bar
    int refresher = 0;
    pBar->setMaximum(n);
    int period = n / 100;
    // Progress bar end

    i = j = 0;
    while (j < n) {
        while (i > -1 && pattern.at(i) != text.at(j))
            i = mpNext[i];
        i++;
        j++;
        if (i >= m) {
            results.append(j - i);
            i = mpNext[i];
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
