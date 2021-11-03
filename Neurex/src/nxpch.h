#pragma once

#include <algorithm>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "engine/core/Logger.h"
#include "engine/instrumentation/Instrumentor.h"

#ifdef NX_PT_WIN
#include <Windows.h>
#endif