UNAME = $(shell uname -s)

# Compilers to use
ifeq ($(UNAME), Darwin)
  CPP = x86_64-apple-darwin10-g++-mp-4.7
else
  CPP = g++
endif
CC = gcc
PROTOC = protoc

# C/C++ source files
CPP_SOURCES = src/main.cpp src/vector.cpp src/dmcr_protocol.pb.cpp \
src/socket.cpp src/scene.cpp src/sceneobject.cpp src/dummyscene.cpp \
3rdparty/json/jsoncpp.cpp
CC_SOURCES = 

# 3rd party library flags
PROTOBUF_CFLAGS = $(shell pkg-config --cflags protobuf)
PROTOBUF_LDFLAGS = $(shell pkg-config --libs protobuf)

# Compiler options
CFLAGS= -Wall -Wextra -g $(PROTOBUF_CFLAGS) -I3rdparty

# Linker options
LDFLAGS = $(PROTOBUF_LDFLAGS)

# Target file
EXECUTABLE = dmcr

# C/C++-specific flags:
CPPFLAGS = $(CFLAGS) -std=gnu++0x
CCFLAGS = $(CFLAGS)

# Where object files will be stored
OBJDIR = obj
SRCDIR = src
ASSETDIR = assets

SOURCES = $(CPP_SOURCES) $(CC_SOURCES)
OBJECTS = $(CPP_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CC_SOURCES:%.c=$(OBJDIR)/%.o)

DEPEND = makedepend -Y -pobj/

all: builddirs $(SOURCES) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
		$(CPP) -c $(CPPFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c
		$(CPP) -c $(CPPFLAGS) $< -o $@

$(SRCDIR)/dmcr_protocol.pb.h: $(SRCDIR)/dmcr_protocol.pb.cpp

$(SRCDIR)/dmcr_protocol.pb.cpp: $(ASSETDIR)/dmcr_protocol.proto
		$(PROTOC) $< --cpp_out $(SRCDIR) --proto_path $(ASSETDIR)
		mv $(SRCDIR)/dmcr_protocol.pb.cc $@

depend: $(SOURCES)
	$(DEPEND) $(SOURCES)

clean:
	-rm $(EXECUTABLE) $(OBJECTS)

builddirs: $(OBJDIR)/src $(OBJDIR)/3rdparty/json

$(OBJDIR)/%:
	@mkdir -p $@

.PHONY: all builddirs depend clean

# DO NOT DELETE

obj/src/main.o: src/vector.h src/socket.h src/dmcr_protocol.pb.h
obj/src/main.o: src/dummyscene.h src/scene.h src/sceneobject.h
obj/src/vector.o: src/vector.h
obj/src/dmcr_protocol.pb.o: src/dmcr_protocol.pb.h
obj/src/socket.o: src/socket.h src/dmcr_protocol.pb.h
obj/src/scene.o: src/scene.h src/sceneobject.h src/vector.h
obj/src/sceneobject.o: src/sceneobject.h src/vector.h
obj/src/dummyscene.o: src/dummyscene.h src/scene.h src/sceneobject.h
obj/src/dummyscene.o: src/vector.h
