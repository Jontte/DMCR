UNAME = $(shell uname -s)

# Compilers to use
ifeq ($(UNAME), Darwin)
  CPP = x86_64-apple-darwin10-g++-mp-4.7
else
  CPP = g++
endif
CC = gcc
PROTOC = protoc

# Common C/C++ source files
CPP_SOURCES =		src/dmcr_protocol.pb.cpp src/socket.cpp \
								src/scene.cpp src/sceneobject.cpp src/dummyscene.cpp \
                3rdparty/json/jsoncpp.cpp src/loadbalancer.cpp\
								src/task.cpp src/taskmanager.cpp
CC_SOURCES = 

# Source files that belong explicitly to normal operation (disregard from testing)
MAIN_SOURCES = src/main.cpp

# Source files that belong to unit testing framework:
TEST_SOURCES = tests/main.cpp\
							 tests/test_sharedstream.cpp\
							 tests/test_loadbalancer.cpp

# 3rd party library flags
PROTOBUF_CFLAGS = $(shell pkg-config --cflags protobuf)
PROTOBUF_LDFLAGS = $(shell pkg-config --libs protobuf)

# Compiler options
CFLAGS += -Wall -Wextra -g $(PROTOBUF_CFLAGS) -I3rdparty

# Linker options
LDFLAGS = $(PROTOBUF_LDFLAGS)

# Target file
EXECUTABLE = dmcr
EXECUTABLE_UNITTEST = unittest

# C/C++-specific flags:
CPPFLAGS = $(CFLAGS) -std=gnu++0x
CCFLAGS = $(CFLAGS)

# Where object files will be stored
OBJDIR = obj
SRCDIR = src
ASSETDIR = assets

SOURCES = $(CPP_SOURCES) $(CC_SOURCES) $(MAIN_SOURCES) $(TEST_SOURCES)
MAIN_OBJECTS = $(MAIN_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CPP_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CC_SOURCES:%.c=$(OBJDIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CPP_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CC_SOURCES:%.c=$(OBJDIR)/%.o)

DEPEND = makedepend -Y -pobj/

all: builddirs $(SOURCES) $(EXECUTABLE) $(EXECUTABLE_UNITTEST)
$(EXECUTABLE): $(MAIN_OBJECTS)
	$(CPP) $(MAIN_OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS)

$(EXECUTABLE_UNITTEST): $(TEST_OBJECTS)
	$(CPP) $(TEST_OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS)

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
	-@rm $(EXECUTABLE) $(EXECUTABLE_UNITTEST) $(MAIN_OBJECTS) $(TEST_OBJECTS) 2>/dev/null || true

builddirs: $(OBJDIR)/src $(OBJDIR)/tests $(OBJDIR)/3rdparty/json

$(OBJDIR)/%:
	@mkdir -p $@

.PHONY: all builddirs depend clean

# DO NOT DELETE

obj/src/dmcr_protocol.pb.o: src/dmcr_protocol.pb.h
obj/src/socket.o: src/socket.h src/dmcr_protocol.pb.h
obj/src/scene.o: src/scene.h src/sceneobject.h src/vector.h
obj/src/sceneobject.o: src/sceneobject.h src/vector.h
obj/src/dummyscene.o: src/dummyscene.h src/scene.h src/sceneobject.h
obj/src/dummyscene.o: src/vector.h
obj/src/loadbalancer.o: src/loadbalancer.h
obj/src/task.o: src/task.h src/scene.h src/sceneobject.h src/vector.h
obj/src/taskmanager.o: src/taskmanager.h src/socket.h src/dmcr_protocol.pb.h
obj/src/taskmanager.o: src/scene.h src/sceneobject.h src/vector.h src/task.h
obj/src/taskmanager.o: src/loadbalancer.h
obj/src/main.o: src/vector.h src/socket.h src/dmcr_protocol.pb.h
obj/src/main.o: src/dummyscene.h src/scene.h src/sceneobject.h
obj/src/main.o: src/sharedstream.h
obj/tests/test_sharedstream.o: src/sharedstream.h
obj/tests/test_loadbalancer.o: src/sharedstream.h
