# Game
INCS=	-IAGE/source 							\
		-IAGE/source/window 					\
		-IAGE/source/io							\
		-IAGE/source/graphics 					\
		-IAGE/source/audio	 					\
		-IAGE/source/geometry 					\
		-Isource/COD							\
		-Isource/utilities						\
		-Isource	\
		-F/Library/Frameworks

LIBS=		-framework SDL2 				\
				-framework SDL2_image 			\
				-framework SDL2_mixer 			\
				-framework SDL2_ttf\
 	-lengine -L./AGE/bin

OBJS= 	source/main.cpp 						\
		source/simplegame.cpp					\
		source/utilities/statemanager.cpp		\
		source/utilities/screen.cpp				\
		source/utilities/screenobject.cpp		\
		source/COD/tileresource.cpp				\
		source/COD/splashscreen.cpp				\
		source/COD/winscreen.cpp				\
		source/COD/game.cpp						\
		source/COD/gamescreen.cpp				\
		source/COD/highscore.cpp				\
		source/COD/leveleditor.cpp				\
		source/COD/level.cpp					\
		source/COD/mainmenu.cpp

CXX=	g++

EXEC= 	-o bin/out

$(EXEC):	$(OBJS)
	$(CXX) $(EXEC) $(INCS) $(LIBS) $(OBJS)
