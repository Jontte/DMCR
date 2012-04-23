#
# This file is a part of the DMCR project and is subject to the terms and
# conditions defined in file 'LICENSE.txt', which is a part of this source
# code package.
#

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
CPP_SOURCES = src/dmcr_protocol.pb.cpp src/socket.cpp \
src/scene.cpp src/sceneobject.cpp src/dummyscene.cpp \
3rdparty/json/jsoncpp.cpp \
src/task.cpp src/taskmanager.cpp src/ray.cpp src/renderer.cpp src/camera.cpp \
src/backendapplication.cpp src/octreescene.cpp src/aabb.cpp src/box.cpp \
src/sphere.cpp src/octree.cpp src/raycastresult.cpp \
src/threadparallelrenderer.cpp src/kdtreescene.cpp src/random.cpp \
src/settings.cpp src/util.cpp src/texture.cpp
CC_SOURCES = 

# Source files that belong explicitly to normal operation (disregard from testing)
MAIN_SOURCES = src/main.cpp

# Source files that belong to unit testing framework:
TEST_SOURCES =	tests/main.cpp\
								tests/test_sharedstream.cpp \
								tests/test_tasks.cpp tests/test_intersections.cpp\
								tests/test_vector.cpp tests/test_random.cpp\
                                tests/test_settings.cpp

BENCHMARK_SOURCES = tests/benchmark.cpp

# 3rd party library flags
PROTOBUF_CFLAGS = $(shell pkg-config --cflags protobuf)
PROTOBUF_LDFLAGS = $(shell pkg-config --libs protobuf)
PNGPP_LDFLAGS = $(shell pkg-config --libs libpng)

# Compiler options
CFLAGS += -Wall -Wextra $(PROTOBUF_CFLAGS) -I3rdparty

DEBUG ?= 1
PROFILE ?= 0

ifeq ($(PROFILE), 1)
CFLAGS += -pg -g
endif

ifeq ($(DEBUG), 1)
CFLAGS += -DDEBUG -g
else
# TODO: Profile-guided optimization
CFLAGS += -DNDEBUG -Ofast -ffast-math -mtune=native -march=native -mfpmath=sse -flto -funroll-loops -fweb -frename-registers -fomit-frame-pointer -fno-stack-protector
endif

# Linker options
LDFLAGS = $(PROTOBUF_LDFLAGS) $(PNGPP_LDFLAGS)

# Target file
EXECUTABLE = dmcr
EXECUTABLE_UNITTEST = unittest
EXECUTABLE_BENCHMARK = benchmark

# C/C++-specific flags:
CPPFLAGS = $(CFLAGS) -std=gnu++0x
CCFLAGS = $(CFLAGS)

# Where object files will be stored
OBJDIR = obj
SRCDIR = src
ASSETDIR = assets

SOURCES = $(CPP_SOURCES) $(CC_SOURCES) $(MAIN_SOURCES) $(TEST_SOURCES) $(BENCHMARK_SOURCES)
MAIN_OBJECTS = $(MAIN_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CPP_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CC_SOURCES:%.c=$(OBJDIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CPP_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CC_SOURCES:%.c=$(OBJDIR)/%.o)
BENCHMARK_OBJECTS = $(BENCHMARK_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CPP_SOURCES:%.cpp=$(OBJDIR)/%.o) $(CC_SOURCES:%.c=$(OBJDIR)/%.o)

DEPEND = makedepend -Y -pobj/

all: builddirs $(SOURCES) $(EXECUTABLE) $(EXECUTABLE_UNITTEST)
$(EXECUTABLE): $(MAIN_OBJECTS)
	$(CPP) $(MAIN_OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS)

$(EXECUTABLE_UNITTEST): $(TEST_OBJECTS)
	$(CPP) $(TEST_OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS)

$(EXECUTABLE_BENCHMARK): $(BENCHMARK_OBJECTS)
	$(CPP) $(BENCHMARK_OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS)

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

builddirs: $(OBJDIR) $(OBJDIR)/src $(OBJDIR)/tests $(OBJDIR)/3rdparty/json

$(OBJDIR) $(OBJDIR)/src $(OBJDIR)/tests $(OBJDIR)/3rdparty/json:
	@mkdir -p $@

.PHONY: all builddirs depend clean

# DO NOT DELETE

obj/src/dmcr_protocol.pb.o: src/dmcr_protocol.pb.h
obj/src/socket.o: src/dmcr_protocol.pb.h src/settings.h src/socket.h
obj/src/socket.o: src/itasklistener.h src/itaskprovider.h src/renderer.h
obj/src/socket.o: src/random.h src/vector.h src/scene.h src/camera.h
obj/src/socket.o: src/ray.h src/raycastresult.h src/sceneobject.h
obj/src/socket.o: src/material.h src/texture.h src/unique_ptr
obj/src/scene.o: src/sphere.h src/aabb.h src/ray.h src/vector.h
obj/src/scene.o: src/sceneobject.h src/raycastresult.h src/material.h
obj/src/scene.o: src/texture.h src/box.h src/scene.h src/camera.h
obj/src/scene.o: src/unique_ptr
obj/src/sceneobject.o: src/sceneobject.h src/raycastresult.h src/vector.h
obj/src/sceneobject.o: src/material.h src/texture.h
obj/src/dummyscene.o: src/dummyscene.h src/raycastresult.h src/vector.h
obj/src/dummyscene.o: src/scene.h src/camera.h src/ray.h src/sceneobject.h
obj/src/dummyscene.o: src/material.h src/texture.h src/unique_ptr
obj/src/task.o: src/task.h src/renderer.h src/random.h src/vector.h
obj/src/task.o: src/scene.h src/camera.h src/ray.h src/raycastresult.h
obj/src/task.o: src/sceneobject.h src/material.h src/texture.h src/unique_ptr
obj/src/task.o: src/taskmanager.h src/itasklistener.h
obj/src/task.o: src/threadparallelrenderer.h
obj/src/taskmanager.o: src/itaskprovider.h src/renderer.h src/random.h
obj/src/taskmanager.o: src/vector.h src/scene.h src/camera.h src/ray.h
obj/src/taskmanager.o: src/raycastresult.h src/sceneobject.h src/material.h
obj/src/taskmanager.o: src/texture.h src/unique_ptr src/taskmanager.h
obj/src/taskmanager.o: src/itasklistener.h src/task.h
obj/src/ray.o: src/ray.h src/vector.h
obj/src/renderer.o: src/util.h src/renderer.h src/random.h src/vector.h
obj/src/renderer.o: src/scene.h src/camera.h src/ray.h src/raycastresult.h
obj/src/renderer.o: src/sceneobject.h src/material.h src/texture.h
obj/src/renderer.o: src/unique_ptr
obj/src/camera.o: src/camera.h src/vector.h src/ray.h
obj/src/backendapplication.o: src/kdtreescene.h src/vector.h
obj/src/backendapplication.o: src/sceneobject.h src/raycastresult.h
obj/src/backendapplication.o: src/material.h src/texture.h src/scene.h
obj/src/backendapplication.o: src/camera.h src/ray.h src/unique_ptr
obj/src/backendapplication.o: src/backendapplication.h src/itasklistener.h
obj/src/backendapplication.o: src/socket.h src/dmcr_protocol.pb.h
obj/src/backendapplication.o: src/itaskprovider.h src/renderer.h src/random.h
obj/src/backendapplication.o: src/taskmanager.h src/task.h
obj/src/octreescene.o: src/octreescene.h src/octree.h src/aabb.h src/ray.h
obj/src/octreescene.o: src/vector.h src/sceneobject.h src/raycastresult.h
obj/src/octreescene.o: src/material.h src/texture.h src/unique_ptr
obj/src/octreescene.o: src/scene.h src/camera.h
obj/src/aabb.o: src/aabb.h src/ray.h src/vector.h src/sceneobject.h
obj/src/aabb.o: src/raycastresult.h src/material.h src/texture.h
obj/src/box.o: src/box.h src/aabb.h src/ray.h src/vector.h src/sceneobject.h
obj/src/box.o: src/raycastresult.h src/material.h src/texture.h src/random.h
obj/src/sphere.o: src/ray.h src/vector.h src/sphere.h src/aabb.h
obj/src/sphere.o: src/sceneobject.h src/raycastresult.h src/material.h
obj/src/sphere.o: src/texture.h src/random.h
obj/src/octree.o: src/octree.h src/aabb.h src/ray.h src/vector.h
obj/src/octree.o: src/sceneobject.h src/raycastresult.h src/material.h
obj/src/octree.o: src/texture.h src/unique_ptr
obj/src/raycastresult.o: src/raycastresult.h src/vector.h
obj/src/threadparallelrenderer.o: src/renderer.h src/random.h src/vector.h
obj/src/threadparallelrenderer.o: src/scene.h src/camera.h src/ray.h
obj/src/threadparallelrenderer.o: src/raycastresult.h src/sceneobject.h
obj/src/threadparallelrenderer.o: src/material.h src/texture.h src/unique_ptr
obj/src/threadparallelrenderer.o: src/threadparallelrenderer.h src/settings.h
obj/src/threadparallelrenderer.o: src/util.h
obj/src/kdtreescene.o: src/kdtreescene.h src/vector.h src/sceneobject.h
obj/src/kdtreescene.o: src/raycastresult.h src/material.h src/texture.h
obj/src/kdtreescene.o: src/scene.h src/camera.h src/ray.h src/unique_ptr
obj/src/kdtreescene.o: src/aabb.h
obj/src/random.o: src/random.h src/vector.h
obj/src/settings.o: src/util.h src/settings.h
obj/src/util.o: src/util.h
obj/src/texture.o: src/texture.h src/vector.h
obj/src/main.o: src/backendapplication.h src/itasklistener.h src/socket.h
obj/src/main.o: src/dmcr_protocol.pb.h src/itaskprovider.h src/renderer.h
obj/src/main.o: src/random.h src/vector.h src/scene.h src/camera.h src/ray.h
obj/src/main.o: src/raycastresult.h src/sceneobject.h src/material.h
obj/src/main.o: src/texture.h src/unique_ptr src/taskmanager.h src/task.h
obj/src/main.o: src/settings.h
obj/tests/test_sharedstream.o: src/sharedstream.h
obj/tests/test_tasks.o: src/taskmanager.h src/itasklistener.h src/renderer.h
obj/tests/test_tasks.o: src/random.h src/vector.h src/scene.h src/camera.h
obj/tests/test_tasks.o: src/ray.h src/raycastresult.h src/sceneobject.h
obj/tests/test_tasks.o: src/material.h src/texture.h src/unique_ptr
obj/tests/test_tasks.o: src/task.h src/task.h src/scene.h src/dummyscene.h
obj/tests/test_tasks.o: src/kdtreescene.h src/itaskprovider.h
obj/tests/test_intersections.o: src/aabb.h src/ray.h src/vector.h
obj/tests/test_intersections.o: src/sceneobject.h src/raycastresult.h
obj/tests/test_intersections.o: src/material.h src/texture.h src/box.h
obj/tests/test_intersections.o: src/aabb.h src/sphere.h src/ray.h
obj/tests/test_vector.o: src/vector.h
obj/tests/test_random.o: src/random.h src/vector.h
obj/tests/test_settings.o: src/settings.h
obj/tests/benchmark.o: src/taskmanager.h src/itasklistener.h src/renderer.h
obj/tests/benchmark.o: src/random.h src/vector.h src/scene.h src/camera.h
obj/tests/benchmark.o: src/ray.h src/raycastresult.h src/sceneobject.h
obj/tests/benchmark.o: src/material.h src/texture.h src/unique_ptr src/task.h
obj/tests/benchmark.o: src/task.h src/scene.h src/kdtreescene.h
obj/tests/benchmark.o: src/octreescene.h src/octree.h src/aabb.h
obj/tests/benchmark.o: src/dummyscene.h src/itaskprovider.h src/box.h
obj/tests/benchmark.o: src/sphere.h
