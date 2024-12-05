#include <chrono>

using int8   = char;
using uint8  = unsigned char;
using int16  = short;
using uint16 = unsigned short;
using int32  = int;
using uint32 = unsigned int;
using int64  = long long;
using uint64 = unsigned long long;

using size = size_t;

using MillisecondsU32 = std::chrono::duration<uint32, std::chrono::milliseconds::period>;
using MillisecondsS32 = std::chrono::duration<int32,  std::chrono::milliseconds::period>;
using MillisecondsU64 = std::chrono::duration<uint64, std::chrono::milliseconds::period>;
using MillisecondsS64 = std::chrono::duration<int64,  std::chrono::milliseconds::period>;
using MillisecondsF32 = std::chrono::duration<float,  std::chrono::milliseconds::period>;
using MillisecondsF64 = std::chrono::duration<double, std::chrono::milliseconds::period>;

using SecondsU32 = std::chrono::duration<uint32, std::chrono::seconds::period>;
using SecondsS32 = std::chrono::duration<int32,  std::chrono::seconds::period>;
using SecondsU64 = std::chrono::duration<uint64, std::chrono::seconds::period>;
using SecondsS64 = std::chrono::duration<int64,  std::chrono::seconds::period>;
using SecondsF32 = std::chrono::duration<float,  std::chrono::seconds::period>;
using SecondsF64 = std::chrono::duration<double, std::chrono::seconds::period>;
