
#-----------------------------------------------------------------------

APP           := XGame
TARGET        := ConfigServer
CONFIG        := 
STRIP_FLAG    := N
TARS2CPP_FLAG := 
CFLAGS        += -lm
CXXFLAGS      += -lm

INCLUDE   += -I/usr/local/cpp_modules/protobuf/include
LIB       += -L/usr/local/cpp_modules/protobuf/lib -lprotobuf

INCLUDE   += -I/usr/local/cpp_modules/wbl/include
LIB       += -L/usr/local/cpp_modules/wbl/lib -lwbl

INCLUDE   += -I/usr/local/mysql/include
LIB       += -L/usr/local/mysql/lib/mysql -lmysqlclient

#-----------------------------------------------------------------------
include /home/tarsproto/XGame/Comm/Comm.mk
include /home/tarsproto/XGame/util/util.mk
include /home/tarsproto/XGame/protocols/protocols.mk
include /home/tarsproto/XGame/RouterServer/RouterServer.mk
include /home/tarsproto/XGame/ConfigServer/ConfigServer.mk
include /usr/local/tars/cpp/makefile/makefile.tars

#-----------------------------------------------------------------------

xgame:
	cp -f $(TARGET) /usr/local/app/tars/tarsnode/data/XData.ConfigServer/bin/

100:
	sshpass -p 'awzs2022' scp ./ConfigServer  root@10.10.10.100:/home/yuj/server/configserver
