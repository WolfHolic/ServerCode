#pragma once
#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"mswsock.lib")
#pragma comment(lib,"Winmm.lib")

#include <WS2tcpip.h>
#include <winsock2.h>
#include <MSWSock.h>
#include <mmsystem.h>

#include <WS2spi.h>
#include <mstcpip.h>

#include <Windows.h>
#include <iostream>
#include <io.h>
#include <cstdlib>
#include <stdio.h>
#include <cstdint>

#include <assert.h>
#include <fcntl.h>
#include <algorithm>
#include <functional>

#include <thread>
#include <mutex>
#include <memory>

#include <string>
#include <tchar.h>

#include <ctime>
#include <random>
#include <typeinfo>

#include <array>
#include <map>

#if _DEBUG
#define CONTEXT_SWITCH			Sleep(1)
#else
#define CONTEXT_SWITCH			::SwitchToThread()
#endif

typedef void(*Function)(void *);

#include "Shutdown.h"

#include "./Util/tinyXml/tinyxml.h"
#include "./Util/tinyXml/tinystr.h"

#include "./Util/Clock.h"
#include "./Util/Random.h"
#include "./Util/Singleton.h"
#include "./Util/Type.h"
#include "./Util/Util.h"

#include "Util\Lock.h"
#include "Util\Thread.h"
#include "Util\Logger.h"
#include "Util\Config.h"