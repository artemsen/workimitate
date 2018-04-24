SOURCES =    *.cpp
LIBS =       -lmingw32
RESFILE =    WorkImitate.rc
OUTRESFILE = WorkImitateRes.obj
OUTFILE =    WorkImitate.exe
DEFINES =    -DWIN32 -DNDEBUG -DUNICODE
CC_OPTS =    -mwindows -Wall -Wextra -Wundef -Woverloaded-virtual -Wsign-promo -Wswitch-default -Wfloat-equal -Wcast-qual -Wcast-align -Wwrite-strings $(DEFINES)
BUILDOPT =	 $(SOURCES) -o $(OUTFILE) $(OUTRESFILE) $(DEFINES) $(CC_OPTS) $(LIBS)


all_win:
	windres $(RESFILE) -o $(OUTRESFILE)
	g++ $(BUILDOPT) -lWinmm
	strip $(OUTFILE)
