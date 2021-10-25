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

#include "Neurex/core/Logger.h"
#include "Neurex/debug/Instrumentor.h"

#ifdef NX_PT_WIN
#include <Windows.h>
#endif