QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    adminclubpage.cpp \
    admincoachpage.cpp \
    adminmainwindow.cpp \
    adminmatchpage.cpp \
    adminplayerpage.cpp \
    adminplayerstatspage.cpp \
    admintournamentpage.cpp \
           clubpage.cpp \
    coachpage.cpp \
    loginpage.cpp \
           mainwindow.cpp \
           databasemanager.cpp \
    playerpage.cpp \
    tournamentpage.cpp \
    utils.cpp

HEADERS += mainwindow.h \
    adminclubpage.h \
    admincoachpage.h \
    adminmainwindow.h \
    adminmatchpage.h \
    adminplayerpage.h \
    adminplayerstatspage.h \
    admintournamentpage.h \
           clubpage.h \
    coachpage.h \
           databasemanager.h \
    loginpage.h \
    playerpage.h \
    tournamentpage.h \
    utils.h

FORMS   += mainwindow.ui \
    adminclubpage.ui \
    admincoachpage.ui \
    adminmainwindow.ui \
    adminmatchpage.ui \
    adminplayerpage.ui \
    adminplayerstatspage.ui \
    admintournamentpage.ui \
           clubpage.ui \
    coachpage.ui \
    loginpage.ui \
    playerpage.ui \
    tournamentpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
