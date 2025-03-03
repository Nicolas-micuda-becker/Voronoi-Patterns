#  Makefile 

CC = gcc
ECHO = echo
RM = rm -f
TAR = tar
ZIP = zip
MKDIR = mkdir
CHMOD = chmod
CP = rsync -R

CFLAGS = -Wall -O2
CPPFLAGS = -I.
LDFLAGS = -lm
 
PACKNAME = sc_00_03
PROGNAME = circles
VERSION = 0.1
distdir = $(PACKNAME)_$(PROGNAME)-$(VERSION)
HEADERS = tiny4D.h
SOURCES = window.c
MSVCSRC = $(patsubst %,<ClCompile Include=\"%\\\" \\/>,$(SOURCES))
OBJ = $(SOURCES:.c=.o)
DOXYFILE = documentation/Doxyfile
VSCFILES = $(PROGNAME).vcxproj $(PROGNAME).sln
EXTRAFILES = COPYING $(wildcard shaders/*.?s images/*) $(VSCFILES)
DISTFILES = $(SOURCES) Makefile $(HEADERS) $(DOXYFILE) $(EXTRAFILES)
# Traitements automatiques pour ajout de chemins et options (ne pas modifier)
ifneq (,$(shell ls -d /usr/local/include 2>/dev/null | tail -n 1))
	CPPFLAGS += -I/usr/local/include
endif
ifneq (,$(shell ls -d $(HOME)/local/include 2>/dev/null | tail -n 1))
	CPPFLAGS += -I$(HOME)/local/include
endif
ifneq (,$(shell ls -d /usr/local/lib 2>/dev/null | tail -n 1))
	LDFLAGS += -L/usr/local/lib
endif
ifneq (,$(shell ls -d $(HOME)/local/lib 2>/dev/null | tail -n 1))
	LDFLAGS += -L$(HOME)/local/lib
endif
ifeq ($(shell uname),Darwin)
	MACOSX_DEPLOYMENT_TARGET = 10.8
        CFLAGS += -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET)
        LDFLAGS += -framework OpenGL -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET)
else
        LDFLAGS += -lGL
endif
CPPFLAGS += $(shell sdl2-config --cflags)
LDFLAGS  += -lGL4Dummies $(shell sdl2-config --libs)
all: $(PROGNAME)
$(PROGNAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(PROGNAME)
%.o: %.c $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
dist: distdir
	$(CHMOD) -R a+r $(distdir)
	$(TAR) zcvf $(distdir).tgz $(distdir)
	$(RM) -r $(distdir)
zip: distdir
	$(CHMOD) -R a+r $(distdir)
	$(ZIP) -r $(distdir).zip $(distdir)
	$(RM) -r $(distdir)
distdir: $(DISTFILES)
	$(RM) -r $(distdir)
	$(MKDIR) $(distdir)
	$(CHMOD) 777 $(distdir)
	$(CP) $(DISTFILES) $(distdir)
doc: $(DOXYFILE)
	cat $< | sed -e "s/PROJECT_NAME *=.*/PROJECT_NAME = $(PROGNAME)/" |\
	  sed -e "s/PROJECT_NUMBER *=.*/PROJECT_NUMBER = $(VERSION)/" >> $<.new
	mv -f $<.new $<
	cd documentation && doxygen && cd ..
msvc: $(VSCFILES)
	@echo "Now these files ($?) already exist. If you wish to regenerate them, you should first delete them manually."  
$(VSCFILES):
	@echo "Generating $@ ..."
	@cat $(GL4D_REPO)/Windows/templates/gl4dSample$(suffix $@) | sed -e "s/INSERT_PROJECT_NAME/$(PROGNAME)/g" | sed -e "s/INSERT_TARGET_NAME/$(PROGNAME)/" | sed -e "s/INSERT_SOURCE_FILES/$(MSVCSRC)/" > $@
clean:
	@$(RM) -r $(PROGNAME) $(OBJ) *~ $(distdir).tgz $(distdir).zip gmon.out	\
	  core.* documentation/*~ shaders/*~ documentation/html
