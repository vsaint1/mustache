#pragma once

#ifdef _DEBUG
#define LOG(fmt, ...)\
{\
const auto filename = strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__;\
printf("[mustache-%s:%d] - " fmt "\n", filename, __LINE__, __VA_ARGS__);\
}
#else
#define LOG(fmt, ...)\
{\
printf("[mustache] - " fmt "\n", __VA_ARGS__);\
}
#endif