##============================================================================
# makefile for the A.L.E: Atari Learing Environment (Powered by Stella)
##============================================================================

srcdir      ?= .
# Set this to 1 to enable SDL and display_screen
USE_SDL     := 1
# Set this to 1 to enable the RLGlue interface
USE_RLGLUE  := 0
DEFINES     := -DRLGENV_NOMAINLOOP
LDFLAGS     := 
INCLUDES    := -Isrc/controllers -Isrc/os_dependent -I/usr/include -Isrc/environment 
LIBS_SDL    := -lSDL -lSDL_gfx -lSDL_image 
LIBS_RLGLUE := -lrlutils -lrlgluenetdev
LIBS        :=
OBJS	    :=
PROF        :=
MODULES     :=
MODULE_DIRS :=
DISTNAME    := atari_learning_environment
DEBUG       := 0

# -------- Generated by configure -----------
CXX := g++
CXXFLAGS := 
LD := g++
LIBS += -lz 
RANLIB := ranlib
INSTALL := install
AR := ar cru
MKDIR := mkdir -p
ECHO := printf
CAT := cat
RM := rm -f
RM_REC := rm -f -r
ZIP := zip -q
CP := cp


# MODULES +=  src/unix 
MODULE_DIRS += 
EXEEXT := 
NASM := /usr/bin/nasm
NASMFLAGS := -f elf

PREFIX := /usr/local
BINDIR := /usr/local/bin
DOCDIR := /usr/local/share/doc/stella
DATADIR := /usr/local/share
PROFILE := 

HAVE_GCC3 = 1
HAVE_NASM = 1

INCLUDES += -Isrc/games -Isrc/emucore -Isrc/emucore/m6502/src -Isrc/emucore/m6502/src/bspf/src -Isrc/common -Isrc/controllers -Isrc/agents -Isrc/environment -Isrc/NEAT
OBJS += 
DEFINES +=  -DUNIX -DHAS_ALTIVEC -DUSE_NASM -DBSPF_UNIX -DHAVE_INTTYPES -DWINDOWED_SUPPORT  -DHAVE_GETTIMEOFDAY -DSNAPSHOT_SUPPORT 
LDFLAGS += 

ifeq ($(strip $(USE_SDL)), 1)
  DEFINES +=  -D__USE_SDL
  LIBS += $(LIBS_SDL)
endif

ifeq ($(strip $(USE_RLGLUE)), 1)
  DEFINES +=  -D__USE_RLGLUE
  LIBS += $(LIBS_RLGLUE)
endif


# Uncomment this for stricter compile time code verification
# CXXFLAGS+= -Werror

CXXFLAGS += -Wall -Wno-multichar -Wunused -fno-rtti -fPIC

ifeq ($(strip $(DEBUG)), 1)
  CXXFLAGS += -g -O0
else
  CXXFLAGS += -O3
endif

ifdef PROFILE
  PROF:= -g -pg -fprofile-arcs -ftest-coverage
  CXXFLAGS+= $(PROF)
else
  CXXFLAGS+= -fomit-frame-pointer
endif

# Even more warnings...
#CXXFLAGS+= -pedantic -Wpointer-arith -Wcast-qual -Wconversion
#CXXFLAGS+= -Wshadow -Wimplicit -Wundef -Wnon-virtual-dtor
#CXXFLAGS+= -Wno-reorder -Wwrite-strings -fcheck-new -Wctor-dtor-privacy 

#######################################################################
# Misc stuff - you should never have to edit this                     #
#######################################################################

EXECUTABLE  := ale$(EXEEXT)
LIBRARY := libale.so

all: tags $(EXECUTABLE) $(LIBRARY)


######################################################################
# Various minor settings
######################################################################

# The name for the directory used for dependency tracking
DEPDIR := .deps


######################################################################
# Module settings
######################################################################

MODULES := $(MODULES)

# After the game specific modules follow the shared modules
MODULES += \
	src \
	src/emucore \
	src/emucore/m6502 \
	src/common \
	src/controllers \
	src/os_dependent \
	src/agents \
  src/games \
  src/environment \
  src/NEAT


######################################################################
# The build rules follow - normally you should have no need to
# touch whatever comes after here.
######################################################################

# Concat DEFINES and INCLUDES to form the CPPFLAGS
CPPFLAGS:= $(DEFINES) $(INCLUDES)

# Include the build instructions for all modules
-include $(addprefix $(srcdir)/, $(addsuffix /module.mk,$(MODULES)))

# Depdir information
DEPDIRS = $(addsuffix /$(DEPDIR),$(MODULE_DIRS))
DEPFILES = 

# The build rule for the Stella executable
$(EXECUTABLE):  $(OBJS)
	$(LD) $(LDFLAGS) $(PRE_OBJS_FLAGS) $+ $(POST_OBJS_FLAGS) $(LIBS) $(PROF) -o $@

$(LIBRARY): $(OBJS)
	$(LD) $(LDFLAGS) -shared -o $(LIBRARY) $(OBJS)        

distclean: clean
	$(RM_REC) $(DEPDIRS)
	$(RM) build.rules config.h config.mak config.log

clean:
	$(RM) $(OBJS) $(EXECUTABLE) $(LIBRARY)




.PHONY: all clean dist distclean

.SUFFIXES: .cxx
ifndef HAVE_GCC3
# If you use GCC, disable the above and enable this for intelligent
# dependency tracking. 
.cxx.o:
	$(MKDIR) $(*D)/$(DEPDIR)
	$(CXX) -Wp,-MMD,"$(*D)/$(DEPDIR)/$(*F).d2" $(CXXFLAGS) $(CPPFLAGS) -c $(<) -o $*.o
	$(ECHO) "$(*D)/" > $(*D)/$(DEPDIR)/$(*F).d
	$(CAT) "$(*D)/$(DEPDIR)/$(*F).d2" >> "$(*D)/$(DEPDIR)/$(*F).d"
	$(RM) "$(*D)/$(DEPDIR)/$(*F).d2"

.c.o:
	$(MKDIR) $(*D)/$(DEPDIR)
	$(CXX) -Wp,-MMD,"$(*D)/$(DEPDIR)/$(*F).d2" $(CXXFLAGS) $(CPPFLAGS) -c $(<) -o $*.o
	$(ECHO) "$(*D)/" > $(*D)/$(DEPDIR)/$(*F).d
	$(CAT) "$(*D)/$(DEPDIR)/$(*F).d2" >> "$(*D)/$(DEPDIR)/$(*F).d"
	$(RM) "$(*D)/$(DEPDIR)/$(*F).d2"
else
# If you even have GCC 3.x, you can use this build rule, which is safer; the above
# rule can get you into a bad state if you Ctrl-C at the wrong moment.
# Also, with this GCC inserts additional dummy rules for the involved headers,
# which ensures a smooth compilation even if said headers become obsolete.
.cxx.o:
	$(MKDIR) $(*D)/$(DEPDIR)
	$(CXX) -Wp,-MMD,"$(*D)/$(DEPDIR)/$(*F).d",-MQ,"$@",-MP $(CXXFLAGS) $(CPPFLAGS) -c $(<) -o $*.o

.c.o:
	$(MKDIR) $(*D)/$(DEPDIR)
	$(CXX) -Wp,-MMD,"$(*D)/$(DEPDIR)/$(*F).d",-MQ,"$@",-MP $(CXXFLAGS) $(CPPFLAGS) -c $(<) -o $*.o
endif

ifdef HAVE_NASM
.SUFFIXES: .asm
.asm.o:
	$(NASM) -O1 $(NASMFLAGS) -g -o $*.o $(<)
endif

# Include the dependency tracking files. We add /dev/null at the end
# of the list to avoid a warning/error if no .d file exist
-include $(wildcard $(addsuffix /*.d,$(DEPDIRS))) /dev/null

# check if configure has been run or has been changed since last run
config.mak: $(srcdir)/configure
	@echo "You need to run ./configure before you can run make"
	@echo "Either you haven't run it before or it has changed."
	@exit 1

install: all
	$(INSTALL) -d "$(DESTDIR)$(BINDIR)"
	$(INSTALL) -c -s -m 755 "$(srcdir)/stella$(EXEEXT)" "$(DESTDIR)$(BINDIR)/stella$(EXEEXT)"
	$(INSTALL) -d "$(DESTDIR)$(DOCDIR)"
	$(INSTALL) -c -m 644 "$(srcdir)/Announce.txt" "$(srcdir)/Changes.txt" "$(srcdir)/Copyright.txt" "$(srcdir)/License.txt" "$(srcdir)/README-SDL.txt" "$(srcdir)/Readme.txt" "$(srcdir)/Todo.txt" "$(srcdir)/docs/index.html" "$(srcdir)/docs/debugger.html" "$(DESTDIR)$(DOCDIR)/"
	$(INSTALL) -d "$(DESTDIR)$(DOCDIR)/graphics"
	$(INSTALL) -c -m 644 $(wildcard $(srcdir)/docs/graphics/*.png) "$(DESTDIR)$(DOCDIR)/graphics"
	$(INSTALL) -d "$(DESTDIR)$(DATADIR)/applications"
	$(INSTALL) -c -m 644 "$(srcdir)/src/unix/stella.desktop" "$(DESTDIR)$(DATADIR)/applications"
	$(INSTALL) -d "$(DESTDIR)$(DATADIR)/icons"
	$(INSTALL) -d "$(DESTDIR)$(DATADIR)/icons/mini"
	$(INSTALL) -d "$(DESTDIR)$(DATADIR)/icons/large"
	$(INSTALL) -c -m 644 "$(srcdir)/src/common/stella.png" "$(DESTDIR)$(DATADIR)/icons"
	$(INSTALL) -c -m 644 "$(srcdir)/src/common/stella.png" "$(DESTDIR)$(DATADIR)/icons/mini"
	$(INSTALL) -c -m 644 "$(srcdir)/src/common/stella.png" "$(DESTDIR)$(DATADIR)/icons/large"

install-strip: install
	strip stella$(EXEEXT)

uninstall:
	rm -f  "$(DESTDIR)$(BINDIR)/stella$(EXEEXT)"
	rm -rf "$(DESTDIR)$(DOCDIR)/"
	rm -f  "$(DESTDIR)$(DATADIR)/applications/stella.desktop"
	rm -f  "$(DESTDIR)$(DATADIR)/icons/stella.png"
	rm -f  "$(DESTDIR)$(DATADIR)/icons/mini/stella.png"
	rm -f  "$(DESTDIR)$(DATADIR)/icons/large/stella.png"


# Use Exuberant ctags (the one from Slackware's vim package, for instance),
# not the one from emacs!
tags:
	ctags `find . -name '*.[ch]xx' -o -name '*.c' -o -name '*.y'` || true


