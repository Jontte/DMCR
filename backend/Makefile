
# Compilers to use
CPP = g++
CC = gcc

# C/C++ source files
CPP_SOURCES = src/main.cpp src/vector.cpp
CC_SOURCES = 

# Compiler options
CFLAGS= -Wall -Wextra -g 

# Linker options
LDFLAGS = 

# Target file
EXECUTABLE = dmcr

# C/C++-specific flags:
CPPFLAGS = $(CFLAGS)
CCFLAGS = $(CFLAGS)

# Where object files will be stored
OBJDIR = obj

SOURCES = $(CPP_SOURCES) $(CC_SOURCES)
OBJECTS = $(CPP_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CC_SOURCES:%.c=$(OBJDIR)/%.o)

DEPEND = makedepend -Y

all: $(SOURCES) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
		$(CPP) -c $(CPPFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c
		$(CPP) -c $(CPPFLAGS) $< -o $@

depend: $(SOURCES)
	$(DEPEND) $(SOURCES)

clean:
	-rm $(EXECUTABLE) $(OBJECTS)

# DO NOT DELETE

src/main.o: src/vector.h
src/vector.o: src/vector.h
