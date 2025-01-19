############################### AGLE  ##################################

Agle means ANOTHER GAME LIBRARY and ENVIRONMENT
I've developed it for learning purpose, I wanted an
easy to use and comprensible game library with also online futures.
Agle has a client and a server, you don't need to run the server to run
the current client example only install gl4java library
This library is under GPL license and feel free to use it just
remeber me in your credits.

########################### AGLE SERVER ###############################

Server is linux based code, so u need to run it under linux distribution,
it uses also glib library for multithreading and use a mysql database so you
need to have one up and running.

How RUN agle server:
1> Start mysql
2> Create a user and password same as the one you have edited in globals.h
	default are agle and agle
3> Create a database same as the one you have edited in globals.h
	default is agle
4> Start ther server with command ./AgleServer in src directory

How COPILE agle server:
1> Install mysql and mysql-devel package (see makefile for the library needed)
3> Ensure you have installed cpp compiler etc.
4> Change in file globals.h the definition ( database host port name etc.)
5> If you want to change the test user that is being created the first time
	agle server is started change in DataBase.cpp the corresponding values in 
	function void CDataBase::db_create()
3> make -f Makefile under agle src directory