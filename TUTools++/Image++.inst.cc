/*
 *  $BJ?@.(B19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $BF1=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%Cx:n<T$K$h$k5v2D$J$7$K$3$N%W%m(B
 *  $B%0%i%`$rBh;0<T$X3+<(!$J#@=!$2~JQ!$;HMQ$9$kEy$NCx:n8"$r?/32$9$k9T0Y(B
 *  $B$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!"Cx:n<T$O@UG$(B
 *  $B$rIi$$$^$;$s!#(B 
 *
 *  Copyright 2007
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Author: Toshio UESHIBA
 *
 *  Confidentail and all rights reserved.
 *  This program is confidential. Any changing, copying or giving
 *  information about the source code of any part of this software
 *  and/or documents without permission by the authors are prohibited.
 *
 *  No Warranty.
 *  Authors are not responsible for any damages in the use of this program.
 *  
 *  $Id: Image++.inst.cc,v 1.10 2007-11-26 07:28:09 ueshiba Exp $
 */
#if defined(__GNUG__) || defined(__INTEL_COMPILER)

#include "TU/Image++.cc"

namespace TU
{
template class Image<u_char>;
template class Image<short>;
template class Image<int>;
template class Image<float>;
template class Image<double>;
template class Image<RGB>;
template class Image<BGR>;
template class Image<RGBA>;
template class Image<ABGR>;
template class Image<YUV444>;
template class Image<YUV422>;
template class Image<YUV411>;

template class IntegralImage<int>;
template IntegralImage<int>&
IntegralImage<int>::initialize(const Image<u_char>& image)		;
template const IntegralImage<int>&
IntegralImage<int>::crossVal(Image<int>& out, int cropSize)	const	;
template const IntegralImage<int>&
IntegralImage<int>::crossVal(Image<float>& out, int cropSize)	const	;
    
template class IntegralImage<float>;
template IntegralImage<float>&
IntegralImage<float>::initialize(const Image<u_char>& image)		;
template IntegralImage<float>&
IntegralImage<float>::initialize(const Image<float>& image)		;
template const IntegralImage<float>&
IntegralImage<float>::crossVal(Image<float>& out, int cropSize)	const	;

template class DiagonalIntegralImage<int>;
template DiagonalIntegralImage<int>&
DiagonalIntegralImage<int>::initialize(const Image<u_char>& image)	;
template const DiagonalIntegralImage<int>&
DiagonalIntegralImage<int>::crossVal(Image<int>& out, int cropSize)	const;
template const DiagonalIntegralImage<int>&
DiagonalIntegralImage<int>::crossVal(Image<float>& out, int cropSize)	const;
    
template class DiagonalIntegralImage<float>;
template DiagonalIntegralImage<float>&
DiagonalIntegralImage<float>::initialize(const Image<u_char>& image)	;
template DiagonalIntegralImage<float>&
DiagonalIntegralImage<float>::initialize(const Image<float>& image)	;
template const DiagonalIntegralImage<float>&
DiagonalIntegralImage<float>::crossVal(Image<float>& out, int cropSize)	const;
}

#endif
