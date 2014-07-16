SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(SOURCES:.cpp=.d)
LDFLAGS = $(shell pkg-config --libs gtkmm-2.4 gtkglextmm-1.2 lua5.1) -llua5.1

# from GLUT makefile
INCLUDES = \
    -I. \
    -I/usr/include/SDL

OBJS = \
    #SoundManager.o \
    main.o

# end

# from GLUT
LDLIBS = -lGL -lGLU -lglut -lSDL -lSDL_mixer
# end

CPPFLAGS = $(shell pkg-config --cflags gtkmm-2.4 gtkglextmm-1.2 lua5.1)
CXXFLAGS = $(CPPFLAGS) -W -Wall -g

# CXX = g++
# from GLUT
CXX = g++ $(INCLUDES)
# end

MAIN = castle


all: $(MAIN)

depend: $(DEPENDS)

clean:
	rm -f *.o *.d $(MAIN)

$(MAIN): $(OBJECTS)
	@echo Creating $@...
	@$(CXX) -o $@ $(OBJECTS) $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	@echo Compiling $<...
	@$(CXX) -o $@ -c $(CXXFLAGS) $<

%.d: %.cpp
	@echo Building $@...
	@set -e; $(CC) -M $(CPPFLAGS) $< \
                  | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
                [ -s $@ ] || rm -f $@


#GLUT
compileobjs: $(OBJS)

HEADERS = \
    SoundManager.h

lib_tutorial: $(HEADERS) $(OBJS) Makefile
	ar rvs libsm $(OBJS)
	mv libsm libsm.a

sample_tutorial: $(HEADERS) $(OBJS) Makefile
	$(CPP) -o test $(OBJS) $(LDLIBS)

# end


include $(DEPENDS)



