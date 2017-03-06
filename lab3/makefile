# set this variable to the director in which you saved the common files
COMMON_DIR = ../common/
PLATFORM = Mac

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

PROGRAMS = lab3-1 lab3-2 lab3-3 lab3-4 lab3-5
all : $(PROGRAMS)

lab3-1 : $(SRC_DIR)/lab3-1.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)
lab3-2 : $(SRC_DIR)/lab3-2.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)
lab3-3 : $(SRC_DIR)/lab3-3.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)
lab3-4 : $(SRC_DIR)/lab3-4.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)
lab3-5 : $(SRC_DIR)/lab3-5.c $(DEPS)
	$(COMPILE) -o $@ $(INCLUDE) $(FLAGS) $< $(DEPS) $(LIBS)

.PHONY: clean
clean :
	@ rm -rf $(PROGRAMS) *~
