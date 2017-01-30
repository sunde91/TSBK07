# set this variable to the director in which you saved the common files
commondir = ../common/
platform = Mac

all : lab1-1 

lab1-1 : lab1-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)$(platform)/MicroGlut.m
	gcc -Wall -o lab1-1 -I$(commondir) -I../common/$(platform) -DGL_GLEXT_PROTOTYPES lab1-1.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)$(platform)/MicroGlut.m -framework OpenGL -framework Cocoa -lm

clean :
	rm lab1-1 
