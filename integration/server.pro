TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread -llog4cplus -levent

SOURCES += \
    src/dealQueue.cpp \
    src/cmd/command.cpp \
    comm/safeQueue.cpp \
    comm/cpthread.cpp \
    comm/safeData.cpp \
    comm/cautoLock.cpp \
    comm/allocateMem.cpp \
    comm/Utility.cpp \
    main.cpp \
    comm/commu/myLibeventBase.cpp \
    comm/epollProxy.cpp \
    comm/socketInfoBase.cpp \
    comm/commu/myEpollBase.cpp \
    comm/commu/callBackFactory.cpp \
    comm/commu/callBackHttp.cpp \
    comm/commu/callBackTCP.cpp \
    comm/commu/callBackUdp.cpp

HEADERS += \
    include/log4cplus/appender.h \
    include/log4cplus/config.h \
    include/log4cplus/configurator.h \
    include/log4cplus/consoleappender.h \
    include/log4cplus/fileappender.h \
    include/log4cplus/fstreams.h \
    include/log4cplus/hierarchy.h \
    include/log4cplus/hierarchylocker.h \
    include/log4cplus/layout.h \
    include/log4cplus/logger.h \
    include/log4cplus/loggingmacros.h \
    include/log4cplus/loglevel.h \
    include/log4cplus/ndc.h \
    include/log4cplus/nullappender.h \
    include/log4cplus/socketappender.h \
    include/log4cplus/streams.h \
    include/log4cplus/syslogappender.h \
    include/log4cplus/tstring.h \
    include/log4cplus/helpers/appenderattachableimpl.h \
    include/log4cplus/helpers/loglog.h \
    include/log4cplus/helpers/logloguser.h \
    include/log4cplus/helpers/pointer.h \
    include/log4cplus/helpers/property.h \
    include/log4cplus/helpers/sleep.h \
    include/log4cplus/helpers/socket.h \
    include/log4cplus/helpers/socketbuffer.h \
    include/log4cplus/helpers/stringhelper.h \
    include/log4cplus/helpers/thread-config.h \
    include/log4cplus/helpers/threads.h \
    include/log4cplus/helpers/timehelper.h \
    include/log4cplus/spi/appenderattachable.h \
    include/log4cplus/spi/factory.h \
    include/log4cplus/spi/filter.h \
    include/log4cplus/spi/loggerfactory.h \
    include/log4cplus/spi/loggerimpl.h \
    include/log4cplus/spi/loggingevent.h \
    include/log4cplus/spi/objectregistry.h \
    include/log4cplus/spi/rootlogger.h \
    src/dealQueue.h \
    src/cmd/command.h \
    comm/safeQueue.h \
    comm/cpthread.h \
    comm/safeData.h \
    comm/cautoLock.h \
    comm/allocateMem.h \
    comm/Utility.h \
    comm/commu/myLibeventBase.h \
    comm/commu/callBackBase.h \
    comm/epollProxy.h \
    comm/socketInfoBase.h \
    comm/commu/myEpollBase.h \
    comm/commu/callBackFactory.h \
    comm/commu/callBackHttp.h \
    comm/commu/callBackTCP.h \
    comm/commu/callBackUdp.h

#unix:!macx: LIBS += -L$$PWD/lib/ -llog4cplus

#INCLUDEPATH += $$PWD/include
#DEPENDPATH += $$PWD/include

#unix:!macx: PRE_TARGETDEPS += $$PWD/lib/liblog4cplus.a

OTHER_FILES += \
    conf/log4cplus.properties
