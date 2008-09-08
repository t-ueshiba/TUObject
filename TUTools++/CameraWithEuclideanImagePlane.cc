/*
 *  $BJ?@.(B9-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$kEy$NCx(B
 *  $B:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the creator are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holders or the creator are not responsible for any
 *  damages in the use of this program.
 *  
 *  $Id: CameraWithEuclideanImagePlane.cc,v 1.10 2008-09-08 08:06:13 ueshiba Exp $
 */
#include "TU/Camera.h"

namespace TU
{
/************************************************************************
*  class CameraWithEuclideanImagePlane					*
************************************************************************/
CameraBase&
CameraWithEuclideanImagePlane::setProjection(const Matrix34d& PP)
{
    throw std::runtime_error("CameraWithEuclideanImagePlane::setProjection: Not implemented!!");
    return *this;
}

/*
 *  private members
 */
const CameraBase::Intrinsic&
CameraWithEuclideanImagePlane::intrinsic() const
{
    return _intrinsic;
}

CameraBase::Intrinsic&
CameraWithEuclideanImagePlane::intrinsic()
{
    return _intrinsic;
}

/************************************************************************
*  class CameraWithEuclideanImagePlane::Intrinsic			*
************************************************************************/
Point2d
CameraWithEuclideanImagePlane::Intrinsic::operator ()(const Point2d& xc) const
{
    return Point2d(k() * xc[0] + _principal[0], k() * xc[1] + _principal[1]);
}

Matrix<double>
CameraWithEuclideanImagePlane::Intrinsic::jacobianK(const Point2d& xc) const
{
    Matrix<double>	J(2, 3);
    J[0][0] = xc[0];
    J[1][0] = xc[1];
    J[0][1] = J[1][2] = 1.0;

    return J;
}

Point2d
CameraWithEuclideanImagePlane::Intrinsic::xcFromU(const Point2d& u) const
{
    return Point2d((u[0] - _principal[0]) / k(), (u[1] - _principal[1]) / k());
}

Matrix33d
CameraWithEuclideanImagePlane::Intrinsic::K() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = k();
    mat[0][2] = _principal[0];
    mat[1][2] = _principal[1];
    mat[2][2] = 1.0;

    return mat;
}

Matrix33d
CameraWithEuclideanImagePlane::Intrinsic::Kt() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = k();
    mat[2][0] = _principal[0];
    mat[2][1] = _principal[1];
    mat[2][2] = 1.0;

    return mat;
}

Matrix33d
CameraWithEuclideanImagePlane::Intrinsic::Kinv() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = 1.0 / k();
    mat[0][2] = -_principal[0] / k();
    mat[1][2] = -_principal[1] / k();
    mat[2][2] = 1.0;

    return mat;
}

Matrix33d
CameraWithEuclideanImagePlane::Intrinsic::Ktinv() const
{
    Matrix33d	mat;
    mat[0][0] = mat[1][1] = 1.0 / k();
    mat[2][0] = -_principal[0] / k();
    mat[2][1] = -_principal[1] / k();
    mat[2][2] = 1.0;

    return mat;
}

u_int
CameraWithEuclideanImagePlane::Intrinsic::dof() const
{
    return 3;
}

Point2d
CameraWithEuclideanImagePlane::Intrinsic::principal() const
{
    return _principal;
}

CameraBase::Intrinsic&
CameraWithEuclideanImagePlane::Intrinsic::setPrincipal(double u0, double v0)
{
    _principal[0] = u0;
    _principal[1] = v0;
    return *this;
}

CameraBase::Intrinsic&
CameraWithEuclideanImagePlane::Intrinsic::update(const Vector<double>& dp)
{
    CameraWithFocalLength::Intrinsic::update(dp(0, 1));
    _principal[0] -= dp[1];
    _principal[1] -= dp[2];
    return *this;
}

std::istream&
CameraWithEuclideanImagePlane::Intrinsic::get(std::istream& in)
{
    CameraWithFocalLength::Intrinsic::get(in);
    return in >> _principal;
}

std::ostream&
CameraWithEuclideanImagePlane::Intrinsic::put(std::ostream& out) const
{
    CameraWithFocalLength::Intrinsic::put(out);
    std::cerr << "Principal point:";
    return out << _principal << std::endl;
}
 
}
