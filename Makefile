CC=g++
 
# Les différents répertoires contenant respectivement les fichiers : Sources *.c, Headers *.h, Objets *.o, l'exécutable
SRCDIR=src
HEADDIR=include
LIBDIR=obj
BINDIR=bin
 
 
# Les différentes options de compilations, soyons strictes !
CFLAGS= -I$(HEADDIR) -g -std=c++11 
# Les différents FrameWorks et bibliothèques pour le linkage
GLLIBS = -ltiff -framework OPENGL -framework GLUT -framework GLUI -lobjc -lstdc++ -lm
SFML=-L/usr/share/doc/  -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s -lfreetype -lX11 -lXrandr -lxcb -lxcb-randr -lxcb-image -lGL -lFLAC -logg -lvorbis -lvorbisenc -lvorbisfile -lopenal -lpthread -ludev
# L'exécutable
BIN=main.x
 
# Où trouver les différents sources *.c qu'il faudra compiler pour créer les objets correspondants
SRC= $(wildcard $(SRCDIR)/*.cpp)
OBJ= $(SRC:$(SRCDIR)/%.cpp=$(LIBDIR)/%.o)
	

all: $(BIN)
 
# Création des différents *.o à partir des *.cpp
$(LIBDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -o $@ -c $< $(SFML) $(CFLAGS)
 
#Création de l'exécutable
$(BIN): $(OBJ)
	$(CC) -o $(BINDIR)/$@ $^ $(SFML) $(CFLAGS)
	
# Nettoyage des objets => Tout sera recompiler !
clean:
	rm $(LIBDIR)/*.o
 
# Nettoyage complet => clean + effacement du l'exécutable
Clean: clean
	rm $(BINDIR)/*
