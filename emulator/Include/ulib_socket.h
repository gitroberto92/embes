#ifndef __ULIB_SOCKET_H__
#define __ULIB_SOCKET_H__

#include "ulib_basestrtype.h"

#ifdef WIN32
#include "winsock2.h"
#else
/* Unix */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <ctype.h>
#include <stdint.h>
#if defined(_QNX4)
#include <sys/select.h>
#include <unix.h>
#endif

#define closesocket	closeSocket 
#define closeSocket close
// 추가 정의
#define SOCKET	int
#define INVALID_SOCKET  (SOCKET)(~0)
typedef struct sockaddr SOCKADDR;
#define SOCKET_ERROR            (-1)

#define WSABASEERR 10000
#define WSAETIMEDOUT            (WSABASEERR+60)

#define SD_BOTH         0x02

#endif

int static wsaGetLastError()  {
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
	return WSAGetLastError();
#else
	return WSABASEERR;
#endif
}

#endif
