
# Compilers to use
CPP = g++
CC = gcc
PROTOC = protoc

# C/C++ source files
CPP_SOURCES = src/main.cpp src/vector.cpp src/dmcr_protocol.pb.cpp src/socket.cpp
CC_SOURCES = 

# 3rd party library flags
PROTOBUF_CFLAGS = $(shell pkg-config --cflags protobuf)
PROTOBUF_LDFLAGS = $(shell pkg-config --libs protobuf)

# Compiler options
CFLAGS= -Wall -Wextra -g $(PROTOBUF_CFLAGS)

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

DEPEND = makedepend -Y

all: builddirs $(SOURCES) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
		$(CPP) -c $(CPPFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c
		$(CPP) -c $(CPPFLAGS) $< -o $@

$(SRCDIR)/dmcr_protocol.pb.h: dmcr_protocol.pb.cpp

$(SRCDIR)/dmcr_protocol.pb.cpp: $(ASSETDIR)/dmcr_protocol.proto
		$(PROTOC) $< --cpp_out $(SRCDIR) --proto_path $(ASSETDIR)
		mv $(SRCDIR)/dmcr_protocol.pb.cc $@

depend: $(SOURCES)
	$(DEPEND) $(SOURCES)

clean:
	-rm $(EXECUTABLE) $(OBJECTS)

builddirs: $(OBJDIR)/src

$(OBJDIR)/%:
	@mkdir -p $@

.PHONY: all builddirs depend clean

# DO NOT DELETE

src/main.o: src/vector.h
src/vector.o: src/vector.h
src/dmcr_protocol.pb.o: src/dmcr_protocol.pb.h
src/socket.o: src/socket.h src/dmcr_protocol.pb.h
