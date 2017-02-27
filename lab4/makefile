# set this variable to the director in which you saved the common files
COMMON_DIR = ../common/
UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	PLATFORM = Mac
else
	PLATFORM = Linux
endif	

ifeq (${PLATFORM}, Mac)
	DEPS = $(COMMON_DIR)$(PLATFORM)/MicroGlut.m $(COMMON_DIR)GL_utilities.c $(COMMON_DIR)VectorUtils3.c $(COMMON_DIR)loadobj.c $(COMMON_DIR)LoadTGA.c 
	LIBS = -framework OpenGL -framework Cocoa -lm
else
	DEPS = $(COMMON_DIR)$(PLATFORM)/MicroGlut.c $(COMMON_DIR)GL_utilities.c $(COMMON_DIR)VectorUtils3.c $(COMMON_DIR)loadobj.c $(COMMON_DIR)LoadTGA.c $(COMMON_DIR)camera.h $(COMMON_DIR)init_object.h
	LIBS = -lXt -lX11 -lGL -lm
endif

FLAGS = -DGL_GLEXT_PROTOTYPES 
INCLUDE = -I$(COMMON_DIR) -I$(COMMON_DIR)/$(PLATFORM)

SRC_DIR = src
COMPILE = gcc -w 

PROGRAMS = lab4-1 lab4-2
all : $(PROGRAMS)

lab4-1 : src/lab4-1.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)
lab4-2 : src/lab4-2.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)

.PHONY: clean
clean :
	@ rm -rf $(PROGRAMS) *~
