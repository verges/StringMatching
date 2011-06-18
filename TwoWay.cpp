/*
 * TwoWay.cpp
 *
 *  Created on: Mar 27, 2011
 *      Author: an
 */

#include "TwoWay.h"

QList<int> TwoWay::find(QString pattern, int m, QString & text, int n, QProgressBar * pBar)
{
	int i, j, ell, memory, p, per, q;
	QList<int> results;


	QByteArray   patternBytes  = pattern.toAscii();
	char * patternPtr = patternBytes.data();

	QByteArray   textBytes  = text.toAscii();
	char * textPtr = textBytes.data();

	   /* Preprocessing */
	   i = maxSuf(patternPtr, m, &p);
	   j = maxSufTilde(patternPtr, m, &q);
	   if (i > j) {
	      ell = i;
	      per = p;
	   }
	   else {
	      ell = j;
	      per = q;
	   }

	   // Progress bar
	  	    int refresher = 0;
	  	    pBar->setMaximum(n);
	  	    int period = n / 100;
	  // Progress bar end

	   /* Searching */
	   if (memcmp(textPtr, textPtr + per, ell + 1) == 0) {
	      j = 0;
	      memory = -1;
	      while (j <= n - m) {
	         i = this->max(ell, memory) + 1;
	         while (i < m && pattern[i] == text[i + j])
	            ++i;
	         if (i >= m) {
	            i = ell;
	            while (i > memory && pattern[i] == text[i + j])
	               --i;
	            if (i <= memory)
	               results.append(j);
	            j += per;
	            memory = m - per - 1;
	         }
	         else {
	            j += (i - ell);
	            memory = -1;
	         }
	      // Progress bar
		  refresher++;
		  if (refresher == period) {
		    pBar->setValue(j);
		    QCoreApplication::processEvents();
		    refresher = 0;
		  }
	      }
	   }
	   else {
	      per = this->max(ell + 1, m - ell - 1) + 1;
	      j = 0;
	      while (j <= n - m) {
	         i = ell + 1;
	         while (i < m && pattern[i] == text[i + j])
	            ++i;
	         if (i >= m) {
	            i = ell;
	            while (i >= 0 && pattern[i] == text[i + j])
	               --i;
	            if (i < 0)
	               results.append(j);
	            j += per;
	         }
	         else
	            j += (i - ell);
	      // Progress bar
		  refresher++;
		  if (refresher == period) {
		    pBar->setValue(j);
		    QCoreApplication::processEvents();
		    refresher = 0;
		  }
	      }


	   }

	    pBar->setValue(n);
	    QCoreApplication::processEvents();
	    // Progress bar ends

	   return results;
}

int TwoWay::maxSuf(char * x, int m, int * p)
{
	  int ms, j, k;
	   char a, b;

	   ms = -1;
	   j = 0;
	   k = *p = 1;
	   while (j + k < m) {
	      a = x[j + k];
	      b = x[ms + k];
	      if (a < b) {
	         j += k;
	         k = 1;
	         *p = j - ms;
	      }
	      else
	         if (a == b)
	            if (k != *p)
	               ++k;
	            else {
	               j += *p;
	               k = 1;
	            }
	         else { /* a > b */
	            ms = j;
	            j = ms + 1;
	            k = *p = 1;
	         }
	   }
	   return(ms);
}

int TwoWay::maxSufTilde(char *x, int m, int *p)
{
	  int ms, j, k;
	   char a, b;

	   ms = -1;
	   j = 0;
	   k = *p = 1;
	   while (j + k < m) {
	      a = x[j + k];
	      b = x[ms + k];
	      if (a > b) {
	         j += k;
	         k = 1;
	         *p = j - ms;
	      }
	      else
	         if (a == b)
	            if (k != *p)
	               ++k;
	            else {
	               j += *p;
	               k = 1;
	            }
	         else { /* a < b */
	            ms = j;
	            j = ms + 1;
	            k = *p = 1;
	         }
	   }
	   return(ms);
}

int TwoWay::max(int x1, int x2)
{
	return (x1 > x2) ? x1 : x2;
}
