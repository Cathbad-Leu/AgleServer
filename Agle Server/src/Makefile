##+++++++++++++++++++++++ Flags, librerie e percorsi ++++++++++++++++++++++++++++##
CC	 = g++
TARGET	 = AgleServer
CPPINCS  = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/mysql -L/usr/lib/mysql
CPPFLAGS = -Wall -ansi -g -DLINUX -D_POSIX_PTHREAD_SEMANTICS $(CPPINCS)
FLAGS 	 = -lm -lpthread -lglib-2.0 -lgthread-2.0 -I/usr/include/mysql -L/usr/lib/mysql -lmysqlclient

##+++++++++++++++++++++++++++ Files sorgenti ++++++++++++++++++++++++++++++++++++##
SRCS	 = World.cpp System.cpp Main.cpp UdpSocket.cpp TcpSocket.cpp Math3d.cpp \
           Client.cpp Character.cpp  DataBase.cpp 

##+++++++++++++++++++++++++++ Files oggetto ++++++++++++++++++++++++++++++++++++++##
OBJS	 = World.o System.o Main.o UdpSocket.o TcpSocket.o Math3d.o Client.o \
           Character.o DataBase.o

##+++++++++++++++++++++++++++ Comandi per make ++++++++++++++++++++++++++++++++++##
all: $(TARGET)
	chmod g+rw *.o

clean:
	rm -f $(TARGET) $(OBJS) core

$(TARGET): ${OBJS}
	$(CC) -o $@ ${OBJS} ${FLAGS}

depend:
	makedepend ${SRCS}
