/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#ifndef __MXSOCKET_H__D
#define __MXSOCKET_H__D


#include<iostream>
#include<string>


#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>

typedef int mx_socket;

#else

#include<winsock.h>

typedef SOCKET mx_socket;

#endif

#include"mx_exception.h"


namespace mx
{

	/*! \class mxSocket cross platform socket class
	 */
	class mxSocket {

	public:
		/*! default constructor creates object */
		mxSocket();
		/*! destructor */
		~mxSocket();
		/*! creates object with already initalized socket
		 * \param nsock socket file descriptor
		 */
		mxSocket(mx_socket nsock);
		/*! copy constructor
		 * \param nsock init's this object with nsock
		 */
		mxSocket(const mxSocket &nsock);
		 /*! overloaded operator =
		  * \param nsock init's this object with nsock
		  * \return this object
		  */
		mxSocket &operator=(const mxSocket &nsock);

		/*!
		 * createSocket, creates the socket
		 * \return true on success
		 */
		bool createSocket();

		/*! isConnected
		 * \return true if connected
		 */
		bool isConnected() { return connected; }
		/*! connectTo connects to ip address and port
		 * \param ip ip address
		 * \param port outgoing port
		 * \return true on success
		 */
		bool connectTo(std::string ip, unsigned int port);

		/*!
		 * listen for connections at Port
		 * \return true if operations successful
		 */
		bool listenAt(int port);

		/*! acceptNewSocket accepts new socket when listening
		 * \param ip ip of accepted incoming connection
		 * \return returns file descriptor of established connection
		 */
		mx_socket acceptNewSocket(std::string &ip);
		/*! acceptsocket accepts a incoming connection
		 * \return returns file descriptor for incoming connection
		 */
		mx_socket acceptsocket();

		/*! Read all reads bytes of size until all have been read
		 * \param data pointer to location to hold bytes
		 * \param size how many bytes to read
		 */
		ssize_t Read(void *data, size_t size);
		/*! Write all reads bytes of size until all have been written
		* \param data pointer to location of bytes
		* \param size how many bytes to write
		*/
		ssize_t Write(void *data, size_t size);

		/*! getsocket return current socket file descriptor */
		const int getsocket() const { return current_socket; }
		/*! close the socket */
		int closeSocket();
	private:
		mx_socket current_socket;
		bool connected;
	};


#ifdef _WIN32

	struct InitWinsock32 {

	public:

		InitWinsock32()
		{
			WSADATA socka;
			WSAStartup(MAKEWORD(2,2), &socka);

		}

		~InitWinsock32()
		{
			WSACleanup();
		}
	};

	static InitWinsock32 socket_init_;
#endif

}

#endif


