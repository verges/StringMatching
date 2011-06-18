/*
 * BoyerMoore.cpp
 *
 *  Created on: Mar 27, 2011
 *      Author: an
 */

#include "BoyerMoore.h"
#define ASIZE 255

QList<int> BoyerMoore::find(QString pattern, int m, QString & text, int n, QProgressBar * pBar)
{
	int i, j, bmGs[pattern.size()], bmBc[ASIZE];

	QByteArray   patternBytes  = pattern.toAscii();
	char * patternPtr = patternBytes.data();

	QByteArray   textBytes  = text.toAscii();
	char * textPtr = textBytes.data();

	QList<int> results;

	   /* Preprocessing */
	   this->preBmGs(patternPtr, m, bmGs, pattern.size());
	   this->preBmBc(patternPtr, m, bmBc);

	    // Progress bar
	    int refresher = 0;
	    pBar->setMaximum(n);
	    int period = n / 100;
	    // Progress bar end

	   /* Searching */
	   j = 0;
	   while (j <= n - m) {
              for (i = m - 1; i >= 0 && pattern[i] == text[i + j]; --i) {

              }

	      if (i < 0) {
	    	  results.append(j);
	         j += bmGs[0];
	      }
	      else
	         j += this->max(bmGs[i], bmBc[textPtr[i+j]] - m + 1 + i);

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

void BoyerMoore::preBmBc(char * x, int m, int bmBc[])
{
   int i;

   for (i = 0; i < ASIZE; ++i)
      bmBc[i] = m;
   for (i = 0; i < m - 1; ++i)
      bmBc[x[i]] = m - i - 1;
}

void BoyerMoore::preBmGs(char* x, int m, int bmGs[], int xsize)
{
   int i, j, suff[xsize];

   suffixes(x, m, suff);

   for (i = 0; i < m; ++i)
      bmGs[i] = m;
   j = 0;
   for (i = m - 1; i >= 0; --i)
      if (suff[i] == i + 1)
         for (; j < m - 1 - i; ++j)
            if (bmGs[j] == m)
               bmGs[j] = m - 1 - i;
   for (i = 0; i <= m - 2; ++i)
      bmGs[m - 1 - suff[i]] = m - 1 - i;
}

void BoyerMoore::suffixes(QString x,int m,int * suff)
{
	int f, g, i;

	   suff[m - 1] = m;
	   g = m - 1;
	   for (i = m - 2; i >= 0; --i) {
	      if (i > g && suff[i + m - 1 - f] < i - g)
	         suff[i] = suff[i + m - 1 - f];
	      else {
	         if (i < g)
	            g = i;
	         f = i;
	         while (g >= 0 && x[g] == x[g + m - 1 - f])
	            --g;
	         suff[i] = f - g;
	      }
	   }
}

int BoyerMoore::max(int x1, int x2)
{
  return (x1 > x2) ? x1 : x2;
}

