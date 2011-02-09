SOURCES += Main.cpp

CONFIG += release
CONFIG -= qt
CONFIG -= app_bundle

TARGET = AirHockey

macx {
  LIBS += -framework Nimble 
  LIBS += -framework Radiant -framework Luminous -framework Screenplay -framework Valuable
  LIBS += -framework MultiTouch -framework MultiWidgets
  LIBS += -lSDL -lSDLmain

  LIBS += -framework Cocoa
}

linux-* {
  LIBS += -lMultiWidgets
}

win* {

  # You may need to edit CORNERSTONE_ROOT to match your installation.

  CORNERSTONE_ROOT="C:\CornerStone"

  INCLUDEPATH += $$CORNERSTONE_ROOT\include 
  INCLUDEPATH += $$CORNERSTONE_ROOT\src\MultiTouch\multitude
  INCLUDEPATH += $$CORNERSTONE_ROOT\src\MultiTouch\multitude\Win32x\include

  LIBS += -lMultiWidgets -lFluffy -lRadiant -lNimble -lSDLmain -lSDL
  LIBS += -L$$CORNERSTONE_ROOT\lib
  LIBS += -L$$CORNERSTONE_ROOT\src\MultiTouch\multitude\Win32x\lib

  CONFIG += console
}


