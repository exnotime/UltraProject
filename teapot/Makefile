# 	NuSYSTEM samples nu3 Makefile
#       Copyright (C) 1997-1999, NINTENDO Co,Ltd.

include $(ROOT)/usr/include/make/PRdefs

# The directory which has the include file and library of NuSYSTEM
#
N64KITDIR    = c:\nintendo\n64kit
NUSYSINCDIR  = $(N64KITDIR)/nusys/include
NUSYSLIBDIR  = $(N64KITDIR)/nusys/lib
NUSTDINCDIR  = $(N64KITDIR)/nustd/include
NUSTDLIBDIR  = $(N64KITDIR)/nustd/lib

LIB = $(ROOT)/usr/lib
LPR = $(LIB)/PR
INC = $(ROOT)/usr/include
CC  = gcc
LD  = ld
MAKEROM = mild

# When you use the former audio micro codes,
# please comment this out.
N_AUDIO = yes

ifdef N_AUDIO
NUAUDIOLIB = -lnualsgi_n_d -lgn_audio_d
else
NUAUDIOLIB = -lnualsgi_d
endif

LCDEFS =	-DNU_DEBUG -DF3DEX_GBI_2
LCINCS =	-I. -I$(NUSYSINCDIR) -I$(NUSTDINCDIR) -I$(ROOT)/usr/include/PR
LCOPTS =	-G 0 -nostdinc
LDFLAGS =	$(MKDEPOPT) -L$(LIB) -L$(NUSYSLIBDIR) -L$(NUSTDLIBDIR) $(NUAUDIOLIB) -lnustd_d -lnusys_d -lgultra_d -L$(GCCDIR)/mipse/lib -lkmc

OPTIMIZER =	-g

APP =		teapot.out

TARGETS =	teapot.n64

HFILES =	main.h graphic.h segment.h vector3.h model.h

CODEFILES   = 	main.c stage00.c graphic.c gfxinit.c model.c

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys.o

DATAFILES   =	

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)


default:        $(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(TARGETS):	$(OBJECTS)
		$(MAKEROM) spec -I$(NUSYSINCDIR) -r $(TARGETS) -e $(APP)
