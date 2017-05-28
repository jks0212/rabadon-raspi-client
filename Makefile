
CC = g++
OPTIONS = -pthread -o # -lwiringPi -o
STDPP = -std=c++0x
SRCS = src/rabadon.cc src/Util.cc src/Client.cc src/SpiPino.cc src/Drone.cc src/jsoncpp.cpp src/http/SimpleHttp.cc src/setting/Setting.cc
OBJS = rabadon.o Util.o Client.o SpiPino.o Drone.o jsoncpp.o SimpleHttp.o Setting.o
TARGET = rabadon
TARGETSERV = rabadon.service
TARGETCONF = rabadon.conf
TARGETSETTINGS = settings.json
PIDFILE = /run/$(TARGET).pid
EXECPATH = /usr/local/bin/
ETCPATH = /usr/local/etc/rabadon/
SERVPATH = /lib/systemd/system/
SYMBPATH = /etc/systemd/system/multi-user.target.wants/

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OPTIONS) $(TARGET) $(OBJS) $(STDPP)

$(OBJS): $(SRCS)
	$(CC) -c $(SRCS) $(STDPP)


clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(PIDFILE)
##	rm -f $(EXECPATH)$(TARGET)
##	rm -f $(SYMBPATH)$(TARGETSERV)
##	rm -f $(SERVPATH)$(TARGETSERV)
#	rm -rf $(ETCPATH)
	rm -rf /run/rabadon

##install: $(TARGET)
##	cp $(CURDIR)/$(TARGET) $(EXECPATH)$(TARGET)
#	cp $(CURDIR)/$(TARGETSERV) $(SERVPATH)$(TARGETSERV)
#	ln -s $(SERVPATH)$(TARGETSERV) $(SYMBPATH)$(TARGETSERV)
##	mkdir $(ETCPATH)
##	cp $(CURDIR)/$(TARGETCONF) $(ETCPATH)$(TARGETCONF)
##	cp $(CURDIR)/$(TARGETSETTINGS) $(ETCPATH)$(TARGETSETTINGS)
