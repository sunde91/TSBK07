# set this variable to the director in which you saved the common files
COMMON_DIR = ../common/
PLATFORM = MAC

DEPS = $(COMMON_DIR)$(PLATFORM)/MicroGlut.m $(COMMON_DIR)GL_utilities.c $(COMMON_DIR)VectorUtils3.c $(COMMON_DIR)loadobj.c $(COMMON_DIR)LoadTGA.c 
ifeq (${PLATFORM}, MAC)
	LIBS = -framework OpenGL -framework Cocoa -lm
else
	-lXt -lX11 -lm
endif

FLAGS = -DGL_GLEXT_PROTOTYPES 
INCLUDE = -I$(COMMON_DIR) -I$(COMMON_DIR)/$(PLATFORM)

COMPILE = gcc -Wall

PROGRAMS = lab1-1 lab1-2 lab1-3 lab1-4 lab1-5
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

.PHONY: clean
clean :
	rm $(PROGRAMS)
