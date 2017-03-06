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

COMPILE = gcc -Wall

PROGRAMS = lab1-1 lab1-2 lab1-3 lab1-4 lab1-5 lab1-6
all : $(PROGRAMS)

lab1-1 : lab1-1.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)

lab1-2 : lab1-2.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)

lab1-3 : lab1-3.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)

lab1-4 : lab1-4.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)

lab1-5 : lab1-5.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)

lab1-6 : lab1-6.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)

.PHONY: clean
clean :
	rm $(PROGRAMS)
