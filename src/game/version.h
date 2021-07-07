/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_VERSION_H
#define GAME_VERSION_H
#include "generated/nethash.cpp"
#define GAME_VERSION "0.6.4"
#define GAME_NETVERSION "0.6" //GAME_NETVERSION_HASH
// 下面的这行,只要加入就报错,而且也没引用,干脆删了得了
//static const char GAME_RELEASE_VERSION[8] = {'0', '.', '6', '.', '4', 0};
//static const char GAME_RELEASE_VERSION = "0.6.4";
#endif
