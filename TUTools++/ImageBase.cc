/*
 *  $BJ?@.(B9$BG/(B $BEE;R5;=QAm9g8&5f=j(B $B?"<G=SIW(B $BCx:n8"=jM-(B
 *
 *  $BCx:n<T$K$h$k5v2D$J$7$K$3$N%W%m%0%i%`$NBh;0<T$X$N3+<(!"J#@=!"2~JQ!"(B
 *  $B;HMQEy$=$NB>$NCx:n?M3J8"$r?/32$9$k9T0Y$r6X;_$7$^$9!#(B
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!"Cx:n<T$O@UG$(B
 *  $B$rIi$$$^$;$s!#(B 
 *
 *
 *  Copyright 1996
 *  Toshio UESHIBA, Electrotechnical Laboratory
 *
 *  All rights reserved.
 *  Any changing, copying or giving information about source programs of
 *  any part of this software and/or documentation without permission of the
 *  authors are prohibited.
 *
 *  No Warranty.
 *  Authors are not responsible for any damage in use of this program.
 */

/*
 *  $Id: ImageBase.cc,v 1.11 2004-10-15 05:33:36 ueshiba Exp $
 */
#include "TU/Image++.h"
#include "TU/Manip.h"
#include "TU/Geometry++.h"
#include <stdexcept>
#ifndef STDC_HEADERS
#  define STDC_HEADERS
#endif


namespace TU
{
    enum EPBM_Sign {
	EPBM_UNSIGNED = 0,
	EPBM_SIGNED   = 1
    };
    enum EPBM_DataType {
	EPBM_CHAR8    = 0,
	EPBM_SHORT16  = 1,
	EPBM_INT32    = 2,
	EPBM_FLOAT32  = 3,
	EPBM_DOUBLE64 = 4
    };

/************************************************************************
*  static functions							*
************************************************************************/
inline u_int	bit2byte(u_int i)	{return ((i - 1)/8 + 1);}

/************************************************************************
*  class ImageBase							*
************************************************************************/
ImageBase::~ImageBase()
{
}

ImageBase::Type
ImageBase::restoreHeader(std::istream& in)
{
    using namespace	std;

  // Reset calibration parameters.
    P = 0.0;
    P[0][0] = P[1][1] = P[2][2] = 1.0;
    d1 = d2 = 0.0;
    
  // Read the magic number.
    int	magic = in.get();
    if (magic == EOF)
	return END;
    if (magic != 'P')
	throw runtime_error("TU::ImageBase::restoreHeader: not a pbm file!!");
    in >> magic >> ws; // Read pbm magic number and trailing white spaces.

    u_int	dataType = EPBM_CHAR8, sign = EPBM_UNSIGNED;
    int		c;
  // Process comment lines.
    for (; (c = in.get()) == '#'; in >> ign)
    {
	char	key[256], val[256];
	in >> key;
	if (!strcmp(key, "DataType:"))		// pixel data type
	{
	    in >> val;
	    if (!strcmp(val, "Char"))
		dataType = EPBM_CHAR8;
	    else if (!strcmp(val, "Short"))
		dataType = EPBM_SHORT16;
	    else if (!strcmp(val, "Int"))
		dataType = EPBM_INT32;
	    else if (!strcmp(val, "Float"))
		dataType = EPBM_FLOAT32;
	    else if (!strcmp(val, "Double"))
		dataType = EPBM_DOUBLE64;
	    else
		throw runtime_error("TU::ImageBase::restore_epbm: unknown data type!!");
	}
	else if (!strcmp(key, "Sign:"))		// signed- or unsigned-image
	{
	    in >> val;
	    sign = (!strcmp(val, "Unsigned") ? EPBM_UNSIGNED : EPBM_SIGNED);
	}
	else if (!strcmp(key, "Endian:"))	// big- or little-endian
	{
	    in >> val;
	    if (strcmp(val, "Big") && dataType != EPBM_CHAR8)
		throw runtime_error("TU::ImageBase::restore_epbm: little endian is not supported!!");
	}
	else if (!strcmp(key, "PinHoleParameterH11:"))
	    in >> P[0][0];
	else if (!strcmp(key, "PinHoleParameterH12:"))
	    in >> P[0][1];
	else if (!strcmp(key, "PinHoleParameterH13:"))
	    in >> P[0][2];
	else if (!strcmp(key, "PinHoleParameterH14:"))
	    in >> P[0][3];
	else if (!strcmp(key, "PinHoleParameterH21:"))
	    in >> P[1][0];
	else if (!strcmp(key, "PinHoleParameterH22:"))
	    in >> P[1][1];
	else if (!strcmp(key, "PinHoleParameterH23:"))
	    in >> P[1][2];
	else if (!strcmp(key, "PinHoleParameterH24:"))
	    in >> P[1][3];
	else if (!strcmp(key, "PinHoleParameterH31:"))
	    in >> P[2][0];
	else if (!strcmp(key, "PinHoleParameterH32:"))
	    in >> P[2][1];
	else if (!strcmp(key, "PinHoleParameterH33:"))
	    in >> P[2][2];
	else if (!strcmp(key, "PinHoleParameterH34:"))
	    in >> P[2][3];
	else if (!strcmp(key, "DistortionParameterA:"))
	    in >> d1;
	else if (!strcmp(key, "DistortionParameterB:"))
	    in >> d2;
    }
    in.putback(c);

    if (d1 != 0.0 || d2 != 0.0)
    {
	Camera	camera(P);
	double	k = camera.k();
	d1 *= (k * k);
	d2 *= (k * k * k * k);
    }

    u_int	w, h;
    in >> w;
    in >> h;
    _resize(h, w);				// set width & height
    in >> w >> ign;				// skip MaxValue

    switch (magic)
    {
      case U_CHAR:
	switch (dataType)
	{
	  case EPBM_CHAR8:
	    return U_CHAR;
	  case EPBM_SHORT16:
	    return SHORT;
	  case EPBM_FLOAT32:
	    return FLOAT;
	  case EPBM_DOUBLE64:
	    return DOUBLE;
	}
	break;
      case RGB_24:
	return RGB_24;
      case YUV_444:
	return YUV_444;
      case YUV_422:
	return YUV_422;
      case YUV_411:
	return YUV_411;
    }

    throw
      runtime_error("TU::ImageBase::restoreHeader: unknown data type!!");
}

std::ostream&
ImageBase::saveHeader(std::ostream& out, Type type) const
{
    using namespace	std;
    
    out << 'P';
    switch (type)
    {
      case U_CHAR:
      case SHORT:
      case FLOAT:
      case DOUBLE:
	out << (int)U_CHAR << endl;
	break;
      default:
	out << (int)type << endl;
	break;
    }

    const u_int	depth = type2depth(type);
    out << "# PixelLength: " << bit2byte(depth) << endl;
    out << "# DataType: ";
    switch (type)
    {
      case U_CHAR:
	out << "Char" << endl;
	break;
      case SHORT:
	out << "Short" << endl;
	break;
      case FLOAT:
	out << "Float" << endl;
	break;
      case DOUBLE:
	out << "Double" << endl;
	break;
      default:
	out << endl;
	break;
    }
    out << "# Sign: ";
    switch (type)
    {
      case U_CHAR:
      case RGB_24:
	out << "Unsigned" << endl;
	break;
      default:
	out << "Signed" << endl;
	break;
    }
    out << "# Endian: Big" << endl;
    out << "# PinHoleParameterH11: " << P[0][0] << endl
	<< "# PinHoleParameterH12: " << P[0][1] << endl
	<< "# PinHoleParameterH13: " << P[0][2] << endl
	<< "# PinHoleParameterH14: " << P[0][3] << endl
	<< "# PinHoleParameterH21: " << P[1][0] << endl
	<< "# PinHoleParameterH22: " << P[1][1] << endl
	<< "# PinHoleParameterH23: " << P[1][2] << endl
	<< "# PinHoleParameterH24: " << P[1][3] << endl
	<< "# PinHoleParameterH31: " << P[2][0] << endl
	<< "# PinHoleParameterH32: " << P[2][1] << endl
	<< "# PinHoleParameterH33: " << P[2][2] << endl
	<< "# PinHoleParameterH34: " << P[2][3] << endl
	<< "# PinHoleParameterF: 1.0" << endl
	<< "# PinHoleParameterM: 0.0" << endl;
    if (d1 != 0.0 || d2 != 0.0)
    {
	Camera	camera(P);
	double	k = camera.k();
	out << "# DistortionParameterA: " << d1 / (k * k) << endl
	    << "# DistortionParameterB: " << d2 / (k * k * k * k) << endl
	    << "# DistortionParameterCOLD: " << camera.principal()[0] << endl
	    << "# DistortionParameterROWD: " << camera.principal()[1] << endl;
    }
    out << _width() << ' ' << _height() << '\n'
	<< 255 << endl;
    
    return out;
}

u_int
ImageBase::type2depth(Type type)
{
    switch (type)
    {
      case SHORT:
	return 8*sizeof(short);
      case FLOAT:
	return 8*sizeof(float);
      case DOUBLE:
	return 8*sizeof(double);
      case RGB_24:
	return 24;
    }

    return 8;
}
 
}
