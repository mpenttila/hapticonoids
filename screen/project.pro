SOURCES += Main.cpp airhockeywidget.cpp malletwidget.cpp puckwidget.cpp contactlistener.cpp hapticfeedback.cpp

CONFIG += release
CONFIG -= qt
CONFIG -= app_bundle

TARGET = AirHockey

HEADERS += airhockeywidget.hpp contactlistener.hpp

macx {
  LIBS += -framework Nimble 
  LIBS += -framework Radiant -framework Luminous -framework Screenplay -framework Valuable
  LIBS += -framework MultiTouch -framework MultiWidgets -framework Fluffy
  LIBS += -lSDL -lSDLmain

  LIBS += -framework Cocoa
  LIBS += -lBox2D
}

linux-* {
  LIBS += -lMultiWidgets -lRadiant -lLuminous -lPoetic -lValuable -lFluffy -lResonant -lNimble -lMultiStateDisplay -lVideoDisplay -lMultiTouch -lScreenplay -lPatterns -lBox2D -lbluetooth
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


