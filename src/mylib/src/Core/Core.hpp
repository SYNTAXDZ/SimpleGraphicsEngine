#pragma once

#include <memory>
#include <functional>

#define PLATFORM_LINUX

#define BIT( x ) ( x << 1 )
#define BIND_EVENT( fn ) std::bind(&fn, this, std::placeholders::_1) 