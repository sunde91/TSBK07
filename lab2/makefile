# set this variable to the director in which you saved the common files
COMMON_DIR = ../common/
PLATFORM = Linux

ifeq (${PLATFORM}, Mac)
	DEPS = $(COMMON_DIR)$(PLATFORM)/MicroGlut.m $(COMMON_DIR)GL_utilities.c $(COMMON_DIR)VectorUtils3.c $(COMMON_DIR)loadobj.c $(COMMON_DIR)LoadTGA.c 
	LIBS = -framework OpenGL -framework Cocoa -lm
else
	DEPS = $(COMMON_DIR)$(PLATFORM)/MicroGlut.c $(COMMON_DIR)GL_utilities.c $(COMMON_DIR)VectorUtils3.c $(COMMON_DIR)loadobj.c $(COMMON_DIR)LoadTGA.c 
	LIBS = -lXt -lX11 -lGL -lm
endif

FLAGS = -DGL_GLEXT_PROTOTYPES 
INCLUDE = -I$(COMMON_DIR) -I$(COMMON_DIR)/$(PLATFORM)

SRC_DIR = src
COMPILE = gcc -Wall

PROGRAMS = lab2-1
all : $(PROGRAMS)

lab2-1 : $(SRC_DIR)/lab2-1.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)

.PHONY: clean
clean :
	rm $(PROGRAMS)
