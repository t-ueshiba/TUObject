/*
 *  $Id: Page.cc,v 1.2 2002-07-25 02:38:02 ueshiba Exp $
 */
#include "TU/Object++_.h"
#include <stdexcept>

namespace TU
{
/************************************************************************
*  class Page::Cell:		memory cells assigned to the objects	*
************************************************************************/
//! $B;XDj$5$l$?(Bblock$B?t0J>e$NBg$-$5$r;}$D(Bcell$B$r(Bfree list$B$+$iC5$9(B
/*!
  \param nblocks	block$B?t!%(B
  \param addition	false$B$J$i$P!$;XDj$5$l$?(Bblock$B?t0J>e$N(Bcell$B$,$_$D$+$k(B
			$B$^$GA4$F$N(Bfree list$B$rC5:w$9$k!%(Btrue$B$J$i$P!$;XDj$5$l(B
			$B$?(Bblock$B?t$r3JG<$9$k$N$K$U$5$o$7$$(Bfree list$B$N$_$rC5:w(B
			$B$7!$3JG<0LCV$ND>8e$N(Bcell$B$rJV$9!%(B
  \return		$B$_$D$+$C$?(Bcell$B$rJV$9!%$_$D$+$i$J$1$l$P(B0$B$rJV$9!%(B
*/
Page::Cell*
Page::Cell::find(u_int nblocks, bool addition)
{
  // Find i s.t. 2^i <= nblocks - 1 < 2^(i+1).
    u_int	i = 0;
    for (u_int n = nblocks - 1; n >>= 1; )
	++i;
    
  // Search for the smallest cell of size greater than nblocks.
    for (; i < TBLSIZ; ++i)
    {
	_head[i]._nb = nblocks;			// sentinel.
	Cell*	cell = _head[i]._nxt;
	while (cell->_nb < nblocks)
	    cell = cell->_nxt;
	if (addition || cell != &_head[i])
	    return cell;
    }
    return 0;
}

//! $B<+?H$r(Bfree list$B$K3JG<$9$k(B
/*!
  $B3F(Bfree list$B$NCf$G(Bcell$B$O$=$NBg$-$5(B(block$B?t(B)$B$N>:=g$K3JG<$5$l$k!%(Bthis == 0
  $B$b5v$5$l!$$b$A$m$s$3$N>l9g$O2?$b$7$J$$!%(B
  \return	this != 0$B$N>l9g$O<+?H$N(Bblock$B?t$,JV$5$l$k!%(Bthis == 0$B$N>l9g(B
		$B$O(B0$B$,JV$5$l$k!%(B
*/
u_int
Page::Cell::add()
{
    if (this)
    {
	Cell* cell = find(_nb, true);
	_nxt = cell;
	_prv = cell->_prv;
	_prv->_nxt = _nxt->_prv = this;
	_fr = 1;

	return _nb;
    }
    else
	return 0;
}
    
//! $B<+?H$r(Bfree list$B$+$i<h$j=P$9(B
/*!
  free list$B$K3JG<$5$l$F$$$k$3$H$rI=$9%U%i%0(B_fr$B$,(B1$B$N;~$N$_!$<B:]$N<h$j=P$7(B
  $B$,5/$3$j!$$b$A$m$s$3$N;~$O(B_fr$B$,(B0$B$K=q$-49$($i$l$k!%(B
  \return	$B<+J,<+?H$,JV$5$l$k!%(B
*/
Page::Cell*
Page::Cell::detach()
{
    if (_fr)
    {
	_nxt->_prv = _prv;
	_prv->_nxt = _nxt;
	_fr = 0;
    }
    return this;
}
    
//! $B<+?H$r(B2$B$D$N(Bcell$B$KJ,3d$9$k(B
/*!
  $B<+?H$N%5%$%:$r;XDj$5$l$?(Bblock$B?t$K@Z$j5M$a!$;D$j$r?7$?$J(Bcell$B$H$7$FJV$9!%(B
  $B$b$7$b;D$j$N%5%$%:$,(Bcell$B$=$N$b$N$N%5%$%:$h$j>.$5$$>l9g$OJ,3d$O@8$8$J$$!%(B
  \param blocks	$B;XDj(Bblock$B?t!%(B
  \return	$BJ,3d$,@.8y$9$l$P?7$?$J(Bcell$B$,!$<:GT$9$l$P(B0$B$,!$$=$l$>$lJV$5$l$k!%(B
*/
Page::Cell*
Page::Cell::split(u_int nblocks)
{
    const u_int	rest = _nb - nblocks;
    if (rest < nbytes2nblocks(sizeof(Cell)))
	return 0;
    _nb = nblocks;
    Cell* cell = new(forward()) Cell(rest);
    return cell;
}

//! $B%f!<%6B&$KJV$5$l$k%a%b%j$r$-$l$$$K$9$k(B
/*!
  Object::new$B$K$h$C$FF@$i$l$?%a%b%j$rMQ$$$F%f!<%6$,%*%V%8%'%/%H$r9=(B
  $BC[$9$k:]$K!$$=$N%*%V%8%'%/%H$NFbIt$KB>$N%*%V%8%'%/%H$X$N%]%$%s%?$,$"(B
  $B$k$H!$$=$N%]%$%s%?$N=i4|2=$,:Q$s$G$$$J$$;~E@$G(BGC$B$,@8$8$?>l9g$K%]%$%s(B
  $B%?$K%4%_$NCM$,F~$C$F$$$k$?$a$K(Bmarking$B$,K=Av$9$k2DG=@-$,$"$k!%$3$l$r(B
  $BKI$0$?$a$K!$(Bcell$B$NCf?HA4BN$r<+?H$X$N%]%$%s%?$GKd$a$F$*$/!%(B
*/
void*
Page::Cell::clean()
{
#ifdef TUObjectPP_DEBUG
    if (_gc || _fr)	// Must not be marked as in use or in freelist.
	throw std::domain_error("Page::Cell::clean: dirty cell!!");
#endif
    for (Cell **p = &_prv, **q = (Cell**)forward(); p < q; )
	*p++ = 0;
  /*    for (Cell **p = &_prv, **q = (Cell**)forward(); p < q; )
	*p++ = this;*/
    return this;
}

/************************************************************************
*  class Page:		memory page					*
************************************************************************/
//! $B?7$?$J%a%b%j%Z!<%8$r3NJ]$9$k(B
/*!
  $B%Z!<%8$r3NJ]$7$?$i!$<+?H$r%Z!<%8%j%9%H$KEPO?$9$k$H6&$K!$Cf?H$N%V%m%C%/(B
  $B$r(Bcell$B$H$7$F(Bfree list$B$K3JG<$9$k!%(B
*/
Page::Page()
    :_nxt(_root)
{
    _root = this;			// Register myself to the page list.
    
    Cell*	cell = new(&_block[0]) Cell(NBLOCKS);
    cell->add();
}

//! $BA4$F$N%a%b%j%Z!<%8$r(Bsweep$B$7$F;HMQ$5$l$F$$$J$$(Bcell$B$r2s<}$9$k(B
/*!
  \return	$B2s<}$7$?(Bblock$B?t$rJV$9!%(B
*/
u_int
Page::sweep()
{    
    u_int	nblocks = 0;
    
    for (Page* page = _root; page; page = page->_nxt)	// for all pages...
    {
#ifdef TUObjectPP_DEBUG
	std::cerr << "\tPage::sweep\tsweeping...." << std::endl;
#endif
	Cell	*garbage = 0;
	for (Cell *cell = (Cell*)(&page->_block[0]),
		  *end  = (Cell*)(&page->_block[NBLOCKS]);
	     cell < end; cell = cell->forward())
	    if (cell->_gc)		// $B;HMQCf!%(B
	    {
		cell->_gc = 0;			// $B%^!<%/$r$O$:$9$@$1!%(B
		nblocks += garbage->add();	// $B$3$l$^$G$K=8$a$?%4%_$r3JG<!%(B
		garbage = 0;
	    }
	    else			// free list$B$K$"$k$+Kt$O(Bdangling$B>uBV!%(B
	    {
#ifdef TUObjectPP_DEBUG
		if (cell->_nb == 0)
		    std::cerr << "size 0 cell!!" << std::endl;
#endif
		cell->detach();			// free list$B$K$"$l$P<h$j=P$9!%(B
	      // $B$3$l$^$G$K=8$a$?%4%_$H%^!<%8$9$k!%(B
		garbage = (garbage ? garbage->merge() : cell);
	    }
	nblocks += garbage->add();
    }
    return nblocks;
}
 
}
