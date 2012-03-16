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
3rdparty/json/jsoncpp.cpp src/loadbalancer.cpp\
src/task.cpp src/taskmanager.cpp src/ray.cpp src/renderer.cpp src/camera.cpp \
src/backendapplication.cpp src/octreescene.cpp src/aabb.cpp src/box.cpp \
src/sphere.cpp src/octree.cpp src/raycastresult.cpp \
src/threadparallelrenderer.cpp
CC_SOURCES = 

# Source files that belong explicitly to normal operation (disregard from testing)
MAIN_SOURCES = src/main.cpp

# Source files that belong to unit testing framework:
TEST_SOURCES =	tests/main.cpp\
								tests/test_sharedstream.cpp tests/test_loadbalancer.cpp\
								tests/test_tasks.cpp tests/test_intersections.cpp\
								tests/test_vector.cpp

BENCHMARK_SOURCES = tests/benchmark.cpp

# 3rd party library flags
PROTOBUF_CFLAGS = $(shell pkg-config --cflags protobuf)
PROTOBUF_LDFLAGS = $(shell pkg-config --libs protobuf)

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
CFLAGS += -DNDEBUG -O3 -ffast-math -mtune=native -march=native -mfpmath=sse
endif

# Linker options
LDFLAGS = $(PROTOBUF_LDFLAGS)

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
obj/src/socket.o: src/socket.h src/dmcr_protocol.pb.h src/itaskprovider.h
obj/src/socket.o: src/renderer.h src/scene.h src/sceneobject.h src/vector.h
obj/src/socket.o: src/ray.h src/aabb.h src/raycastresult.h src/camera.h
obj/src/socket.o: src/unique_ptr src/itasklistener.h
obj/src/scene.o: src/scene.h src/sceneobject.h src/vector.h src/ray.h
obj/src/scene.o: src/aabb.h src/raycastresult.h src/camera.h src/unique_ptr
obj/src/scene.o: src/box.h src/sphere.h
obj/src/sceneobject.o: src/sceneobject.h src/vector.h src/ray.h src/aabb.h
obj/src/sceneobject.o: src/raycastresult.h
obj/src/dummyscene.o: src/dummyscene.h src/scene.h src/sceneobject.h
obj/src/dummyscene.o: src/vector.h src/ray.h src/aabb.h src/raycastresult.h
obj/src/dummyscene.o: src/camera.h src/unique_ptr
obj/src/loadbalancer.o: src/loadbalancer.h
obj/src/task.o: src/task.h src/scene.h src/sceneobject.h src/vector.h
obj/src/task.o: src/ray.h src/aabb.h src/raycastresult.h src/camera.h
obj/src/task.o: src/unique_ptr src/renderer.h src/taskmanager.h
obj/src/task.o: src/itasklistener.h src/threadparallelrenderer.h
obj/src/task.o: src/loadbalancer.h
obj/src/taskmanager.o: src/taskmanager.h src/scene.h src/sceneobject.h
obj/src/taskmanager.o: src/vector.h src/ray.h src/aabb.h src/raycastresult.h
obj/src/taskmanager.o: src/camera.h src/unique_ptr src/task.h src/renderer.h
obj/src/taskmanager.o: src/itasklistener.h src/loadbalancer.h
obj/src/taskmanager.o: src/itaskprovider.h
obj/src/ray.o: src/ray.h src/vector.h
obj/src/renderer.o: src/renderer.h src/scene.h src/sceneobject.h src/vector.h
obj/src/renderer.o: src/ray.h src/aabb.h src/raycastresult.h src/camera.h
obj/src/renderer.o: src/unique_ptr
obj/src/camera.o: src/camera.h src/vector.h src/ray.h
obj/src/backendapplication.o: src/backendapplication.h src/taskmanager.h
obj/src/backendapplication.o: src/scene.h src/sceneobject.h src/vector.h
obj/src/backendapplication.o: src/ray.h src/aabb.h src/raycastresult.h
obj/src/backendapplication.o: src/camera.h src/unique_ptr src/task.h
obj/src/backendapplication.o: src/renderer.h src/itasklistener.h src/socket.h
obj/src/backendapplication.o: src/dmcr_protocol.pb.h src/itaskprovider.h
obj/src/backendapplication.o: src/dummyscene.h
obj/src/octreescene.o: src/octreescene.h src/vector.h src/sceneobject.h
obj/src/octreescene.o: src/ray.h src/aabb.h src/raycastresult.h src/scene.h
obj/src/octreescene.o: src/camera.h src/unique_ptr src/octree.h
obj/src/aabb.o: src/aabb.h src/vector.h src/ray.h
obj/src/box.o: src/box.h src/sceneobject.h src/vector.h src/ray.h src/aabb.h
obj/src/box.o: src/raycastresult.h
obj/src/sphere.o: src/sphere.h src/sceneobject.h src/vector.h src/ray.h
obj/src/sphere.o: src/aabb.h src/raycastresult.h
obj/src/octree.o: src/octree.h src/aabb.h src/vector.h src/ray.h
obj/src/octree.o: src/sceneobject.h src/raycastresult.h src/unique_ptr
obj/src/raycastresult.o: src/raycastresult.h src/vector.h
obj/src/threadparallelrenderer.o: src/threadparallelrenderer.h src/scene.h
obj/src/threadparallelrenderer.o: src/sceneobject.h src/vector.h src/ray.h
obj/src/threadparallelrenderer.o: src/aabb.h src/raycastresult.h src/camera.h
obj/src/threadparallelrenderer.o: src/unique_ptr src/renderer.h
obj/src/threadparallelrenderer.o: src/loadbalancer.h
obj/src/main.o: src/vector.h src/dummyscene.h src/scene.h src/sceneobject.h
obj/src/main.o: src/ray.h src/aabb.h src/raycastresult.h src/camera.h
obj/src/main.o: src/unique_ptr src/sharedstream.h src/renderer.h src/util.h
obj/src/main.o: src/backendapplication.h src/taskmanager.h src/task.h
obj/src/main.o: src/itasklistener.h src/socket.h src/dmcr_protocol.pb.h
obj/src/main.o: src/itaskprovider.h
obj/tests/test_sharedstream.o: src/sharedstream.h
obj/tests/test_loadbalancer.o: src/loadbalancer.h
obj/tests/test_tasks.o: src/taskmanager.h src/scene.h src/sceneobject.h
obj/tests/test_tasks.o: src/vector.h src/ray.h src/aabb.h src/raycastresult.h
obj/tests/test_tasks.o: src/camera.h src/unique_ptr src/task.h src/renderer.h
obj/tests/test_tasks.o: src/itasklistener.h src/task.h src/scene.h
obj/tests/test_tasks.o: src/dummyscene.h src/octreescene.h src/octree.h
obj/tests/test_tasks.o: src/itaskprovider.h
obj/tests/test_intersections.o: src/box.h src/sceneobject.h src/vector.h
obj/tests/test_intersections.o: src/ray.h src/aabb.h src/raycastresult.h
obj/tests/test_intersections.o: src/ray.h
obj/tests/test_vector.o: src/vector.h
obj/tests/benchmark.o: src/taskmanager.h src/scene.h src/sceneobject.h
obj/tests/benchmark.o: src/vector.h src/ray.h src/aabb.h src/raycastresult.h
obj/tests/benchmark.o: src/camera.h src/unique_ptr src/task.h src/renderer.h
obj/tests/benchmark.o: src/itasklistener.h src/task.h src/scene.h
obj/tests/benchmark.o: src/octreescene.h src/octree.h src/itaskprovider.h
obj/tests/benchmark.o: src/box.h
