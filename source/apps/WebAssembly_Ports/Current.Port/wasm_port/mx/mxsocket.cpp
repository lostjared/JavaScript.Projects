/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/



#include "mxsocket.h"


namespace mx
{


	mxSocket::mxSocket()
	{
		current_socket = 0;
		connected = false;
	}

	mxSocket::~mxSocket()
	{

	}

	mxSocket::mxSocket(mx_socket nsock)
	{
		current_socket = nsock;
		connected = true;
	}

	mxSocket::mxSocket(const mxSocket &nsock)
	{
		current_socket = nsock.current_socket;
		connected = nsock.connected;
	}

	mxSocket& mxSocket::operator=(const mxSocket &sock)
	{

		current_socket = sock.current_socket;
		connected = sock.connected;
		return *this;
	}

	bool mxSocket::createSocket()
	{

		current_socket = socket(AF_INET,SOCK_STREAM,0);
		return true;
	}

	bool mxSocket::listenAt(int port)
	{

		static struct sockaddr_in addy;
		memset((char*)&addy,0, sizeof(addy));
		addy.sin_port = htons(port);
		addy.sin_family = AF_INET;
		addy.sin_addr.s_addr = INADDR_ANY;
		int yup = 1;

		setsockopt(current_socket, SOL_SOCKET,SO_REUSEADDR,(const char*)&yup,sizeof(yup));

		bind(current_socket, (struct sockaddr *)&addy, sizeof(addy));
		listen(current_socket,5);

		return true;
	}


	mx_socket mxSocket::acceptNewSocket(std::string &ip)
	{
#ifdef _WIN32
		int caddy_len;
#else
		socklen_t caddy_len;
#endif
		static struct sockaddr_in caddy;
		int s;
		s = accept(current_socket, (struct sockaddr *)&caddy, &caddy_len);
		ip = std::string(inet_ntoa(caddy.sin_addr));
		connected = true;
		return s;
	}

	mx_socket mxSocket::acceptsocket()
	{
#ifdef _WIN32
		int caddy_len;
#else
		socklen_t caddy_len;
#endif

		mx_socket SOCK;
		struct sockaddr_in  caddy;
		SOCK = accept(current_socket, (struct sockaddr *)&caddy, &caddy_len);
		connected = true;
		return SOCK;
	}


	bool mxSocket::connectTo(std::string where, unsigned int port)
	{
		struct sockaddr_in saddy;
		saddy.sin_port = htons(port);
		saddy.sin_addr.s_addr = inet_addr(where.c_str());
		saddy.sin_family = AF_INET;

		if(connect(current_socket, (const sockaddr*)&saddy, sizeof(sockaddr_in)) == -1) {

			std::cout << "error: could not connect: \n";
			return false;
		}

		connected = true;
		return true;
	}


	ssize_t mxSocket::Read(void *data, size_t size)
	{

		size_t len = size;
		size_t total = 0;
		int cur_;
		char *cdata = (char*)data;

		while ( (len != 0) && ( (cur_ = recv(current_socket, cdata, len, 0) ) != 0) )
		{

			if(cur_ == -1) {


				std::cerr << "mxsocket: error on read\n";
				return -1;

			}
			len -= cur_;
			cdata += cur_;
			total += cur_;
		}

		return total;

	}

	ssize_t mxSocket::Write(void *data, size_t size)
	{

		size_t len = size;
		size_t total = 0;
		int cur_;
		char *cdata = (char*)data;

		while ( (len != 0) && (( cur_ = send(current_socket, cdata, len, 0) ) != 0) )
		{

			if(cur_ == -1) {


				std::cerr << "mxsocket: error on write\n";
				return -1;

			}
			len -= cur_;
			cdata += cur_;
			total += cur_;
		}

		return total;
	}


	int mxSocket::closeSocket()
	{

#ifdef _WIN32
		return closesocket(current_socket);
#else
		return close(current_socket);
#endif
		return 1;

	}


}




