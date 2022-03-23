QT += quick

CONFIG += c++11

# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

INCLUDEPATH += src

SOURCES += \
        src/Backend.cpp \
        src/NineSegDigit.cpp \
        src/Segment.cpp \
        src/main.cpp

HEADERS += \
    src/Backend.hpp \
    src/NineSegDigit.hpp \
    src/Segment.hpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
