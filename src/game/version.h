/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_VERSION_H
#define GAME_VERSION_H
#include "generated/nethash.cpp"
#define MMOTEE_VERSION "ST v0.3.4"
#define GAME_VERSION "0.6.4 " MMOTEE_VERSION
#define GAME_NETVERSION "0.6" //GAME_NETVERSION_HASH
// 下面的这行,只要加入就报错,而且也没引用,干脆删了得了
// FFS注：我修了，但是确实没用，所以还是不要了吧（）
// static const char GAME_RELEASE114514_VERSION[8] = {'0', '.', '6', '.', '4', 0};
// static const char GAME_RELEASE1919810_VERSION = "0.6.4";
#endif
