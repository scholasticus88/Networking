#ifndef __COMMON_INCLUDES__

#define __COMMON_INCLUDES__

#include <ws2tcpip.h>
#include <winsock.h>
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <map>

using namespace std;

#define IS_DIGIT(x) ((x >= '0') && (x <= '9'))

#define IP_ADDRESS_BUFFER_LEN    20
#define MAX_NAME_LEN             20 
#define NETWORK_NAME_BUFFER_LEN 256

#pragma comment(lib, "ws2_32.lib")

typedef unsigned short PORT ;

#endif