#
#  $Id: Makefile,v 1.8 2004-06-17 00:28:04 ueshiba Exp $
#
#################################
#  User customizable macros	#
#################################
DEST		= $(LIBDIR)
INCDIR		= $(HOME)/include/TU
INCDIRS		= -I. -I$(HOME)/include

NAME		= $(shell basename $(PWD))

CPPFLAGS	= -DTUObjectPP_DEBUG
CFLAGS		= -O -g
CCFLAGS		= -g

LINKER		= $(CCC)

#########################
#  Macros set by mkmf	#
#########################
SUFFIX		= .cc:sC
HDRS		= Object++.h \
		Object++_.h
SRCS		= Desc.cc \
		Object++.cc \
		Object.cc \
		Page.cc \
		TUObject++.sa.cc
OBJS		= Desc.o \
		Object++.o \
		Object.o \
		Page.o \
		TUObject++.sa.o

#########################
#  Macros used by RCS	#
#########################
REV		= $(shell echo $Revision: 1.8 $	|		\
		  sed 's/evision://'		|		\
		  awk -F"."					\
		  '{						\
		      for (count = 1; count < NF; count++)	\
			  printf("%d.", $$count);		\
		      printf("%d", $$count + 1);		\
		  }')

include $(PROJECT)/lib/l.mk
include $(PROJECT)/lib/RCS.mk
###
Desc.o: Object++_.h /Users/ueshiba/include/TU/Object++.h \
	/Users/ueshiba/include/TU/types.h
Object++.o: /Users/ueshiba/include/TU/Object++.h \
	/Users/ueshiba/include/TU/types.h
Object.o: Object++_.h /Users/ueshiba/include/TU/Object++.h \
	/Users/ueshiba/include/TU/types.h
Page.o: Object++_.h /Users/ueshiba/include/TU/Object++.h \
	/Users/ueshiba/include/TU/types.h
TUObject++.sa.o: Object++_.h /Users/ueshiba/include/TU/Object++.h \
	/Users/ueshiba/include/TU/types.h
