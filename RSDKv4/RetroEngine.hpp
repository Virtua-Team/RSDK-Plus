#ifndef RETROENGINE_H
#define RETROENGINE_H

// Disables POSIX use c++ name blah blah stuff
#pragma warning(disable : 4996)

// Setting this to true removes (almost) ALL changes from the original code, the trade off is that a playable game cannot be built, it is advised to
// be set to true only for preservation purposes
#ifndef RETRO_USE_ORIGINAL_CODE
#define RETRO_USE_ORIGINAL_CODE (0)
#endif

#ifndef RETRO_USE_MOD_LOADER
#define RETRO_USE_MOD_LOADER (!RETRO_USE_ORIGINAL_CODE && 1)
#endif

#ifndef RETRO_USE_NETWORKING
#define RETRO_USE_NETWORKING (!RETRO_USE_ORIGINAL_CODE && 1)
#endif

// Forces all DLC flags to be disabled, this should be enabled in any public releases
#ifndef RSDK_AUTOBUILD
#define RSDK_AUTOBUILD (0)
#endif

// ================
// STANDARD LIBS
// ================
#include <stdio.h>
#include <string.h>
#include <cmath>
#if RETRO_USE_MOD_LOADER
#include <regex>
#endif

// ================
// STANDARD TYPES
// ================
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;

// Platforms (RSDKv4 only defines these 7 (I assume), but feel free to add your own custom platform define for easier platform code changes)
#define RETRO_WIN      (0)
#define RETRO_OSX      (1)
#define RETRO_XBOX_360 (2)
#define RETRO_PS3      (3)
#define RETRO_iOS      (4)
#define RETRO_ANDROID  (5)
#define RETRO_WP7      (6)
// Custom Platforms start here
#define RETRO_UWP   (7)
#define RETRO_LINUX (8)

// Platform types (Game manages platform-specific code such as HUD position using this rather than the above)
#define RETRO_STANDARD (0)
#define RETRO_MOBILE   (1)

#if defined _WIN32

#if defined WINAPI_FAMILY
#if WINAPI_FAMILY != WINAPI_FAMILY_APP
#define RETRO_PLATFORM   (RETRO_WIN)
#define RETRO_DEVICETYPE (RETRO_STANDARD)
#else
#include <WInRTIncludes.hpp>

#define RETRO_PLATFORM   (RETRO_UWP)
#define RETRO_DEVICETYPE (UAP_GetRetroGamePlatform())
#endif
#else
#define RETRO_PLATFORM   (RETRO_WIN)
#define RETRO_DEVICETYPE (RETRO_STANDARD)
#endif

#elif defined __APPLE__
#if __IPHONEOS__
#define RETRO_PLATFORM   (RETRO_iOS)
#define RETRO_DEVICETYPE (RETRO_MOBILE)
#else
#define RETRO_PLATFORM   (RETRO_OSX)
#define RETRO_DEVICETYPE (RETRO_STANDARD)
#endif
#elif defined __ANDROID__
#define RETRO_PLATFORM   (RETRO_ANDROID)
#define RETRO_DEVICETYPE (RETRO_MOBILE)
#include <jni.h>
#elif defined(__linux__)
#define RETRO_PLATFORM   (RETRO_LINUX)
#define RETRO_DEVICETYPE (RETRO_STANDARD)
#else
// #error "No Platform was defined"
#define RETRO_PLATFORM   (RETRO_WIN)
#define RETRO_DEVICETYPE (RETRO_STANDARD)
#endif

#define DEFAULT_SCREEN_XSIZE 426
#define DEFAULT_FULLSCREEN   false
#define RETRO_USING_MOUSE
#define RETRO_USING_TOUCH

#ifndef BASE_PATH
#define BASE_PATH ""
#endif

#if !defined(RETRO_USE_SDL2) && !defined(RETRO_USE_SDL1)
#define RETRO_USE_SDL2 (1)
#endif

#if RETRO_PLATFORM == RETRO_WIN || RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_UWP                       \
    || RETRO_PLATFORM == RETRO_ANDROID
#ifdef RETRO_USE_SDL2
#define RETRO_USING_SDL1 (0)
#define RETRO_USING_SDL2 (1)
#elif defined(RETRO_USE_SDL1)
#define RETRO_USING_SDL1 (1)
#define RETRO_USING_SDL2 (0)
#endif
#else // Since its an else & not an elif these platforms probably aren't supported yet
#define RETRO_USING_SDL1 (0)
#define RETRO_USING_SDL2 (0)
#endif

#if RETRO_PLATFORM == RETRO_iOS || RETRO_PLATFORM == RETRO_ANDROID || RETRO_PLATFORM == RETRO_WP7
#define RETRO_GAMEPLATFORM (RETRO_MOBILE)
#elif RETRO_PLATFORM == RETRO_UWP
#define RETRO_GAMEPLATFORM (UAP_GetRetroGamePlatform())
#else
#define RETRO_GAMEPLATFORM (RETRO_STANDARD)
#endif

#define RETRO_SW_RENDER       (0)
#define RETRO_RENDERTYPE      (RETRO_SW_RENDER)
#define RETRO_SOFTWARE_RENDER (RETRO_RENDERTYPE == RETRO_SW_RENDER)

#define RETRO_USE_HAPTICS (1)

// NOTE: This is only used for rev00 stuff, it was removed in rev01 and later builds
#if RETRO_PLATFORM <= RETRO_WP7
#define RETRO_GAMEPLATFORMID (RETRO_PLATFORM)
#else

// use *this* macro to determine what platform the game thinks its running on (since only the first 7 platforms are supported natively by scripts)
#if RETRO_PLATFORM == RETRO_LINUX
#define RETRO_GAMEPLATFORMID (RETRO_WIN)
#elif RETRO_PLATFORM == RETRO_UWP
#define RETRO_GAMEPLATFORMID (UAP_GetRetroGamePlatformId())
#else
#error Unspecified RETRO_GAMEPLATFORMID
#endif

#endif

enum RetroLanguages {
    RETRO_EN = 0,
    RETRO_FR = 1,
    RETRO_IT = 2,
    RETRO_DE = 3,
    RETRO_ES = 4,
    RETRO_JP = 5,
    RETRO_PT = 6,
    RETRO_RU = 7,
    RETRO_KO = 8,
    RETRO_ZH = 9,
    RETRO_ZS = 10,
};

enum RetroEngineMessages {
    MESSAGE_NONE      = 0,
    MESSAGE_MESSAGE_1 = 1,
    MESSAGE_LOSTFOCUS = 2,
    MESSAGE_MESSAGE_3 = 3,
    MESSAGE_MESSAGE_4 = 4,
};

enum RetroStates {
    ENGINE_DEVMENU     = 0,
    ENGINE_MAINGAME    = 1,
    ENGINE_INITDEVMENU = 2,
    ENGINE_WAIT        = 3,
    ENGINE_SCRIPTERROR = 4,
    ENGINE_INITPAUSE   = 5,
    ENGINE_EXITPAUSE   = 6,
    ENGINE_ENDGAME     = 7,
    ENGINE_RESETGAME   = 8,
    ENGINE_VIDEOWAIT   = 9,

#if RETRO_USE_NETWORKING
    // Custom GameModes (required to make some features work)
    ENGINE_CONNECT2PVS = 0x80,
    ENGINE_WAIT2PVS    = 0x81,
#endif
#if RETRO_USE_MOD_LOADER
    ENGINE_INITMODMENU = 0x82,
#endif
};

enum RetroGameType {
    GAME_UNKNOWN = 0,
    GAME_SONIC1  = 1,
    GAME_SONIC2  = 2,
    GAME_SONIC3  = 3,
};

// General Defines
#define SCREEN_YSIZE   (240)
#define SCREEN_CENTERY (120)

#if RETRO_PLATFORM == RETRO_WIN || RETRO_PLATFORM == RETRO_UWP || RETRO_PLATFORM == RETRO_ANDROID
#if RETRO_USING_SDL2
#include <SDL.h>
#elif RETRO_USING_SDL1
#include <SDL.h>
#endif
#include <vorbis/vorbisfile.h>
#include <theora/theora.h>
// incredibly hacky fix, but msvc sucks lol, dunno why it can't find the header file
#include "../dependencies/all/theoraplay/theoraplay.h"
#elif RETRO_PLATFORM == RETRO_OSX
#include <SDL2/SDL.h>
#include <Vorbis/vorbisfile.h>
#include <Theora/theora.h>
#include "theoraplay.h"
#include "cocoaHelpers.hpp"
#elif RETRO_PLATFORM == RETRO_LINUX
#if RETRO_USING_SDL2
#include <SDL2/SDL.h>
#elif RETRO_USING_SDL1
#include <SDL/SDL.H>
#endif
#include <vorbis/vorbisfile.h>
#include <theora/theora.h>
#include "theoraplay.h"
#elif RETRO_USING_SDL2
#include <SDL2/SDL.h>
#include <vorbis/vorbisfile.h>
#include <theora/theora.h>
#include "theoraplay.h"
#else

#endif

extern bool usingCWD;
extern bool engineDebugMode;

// Utils
#include "Ini.hpp"

#include "Math.hpp"
#include "Reader.hpp"
#include "String.hpp"
#include "Animation.hpp"
#include "Audio.hpp"
#include "Input.hpp"
#include "Object.hpp"
#include "Palette.hpp"
#include "Drawing.hpp"
#include "Scene3D.hpp"
#include "Collision.hpp"
#include "Scene.hpp"
#include "Script.hpp"
#include "Sprite.hpp"
#include "Text.hpp"
#include "Networking.hpp"
#include "Video.hpp"
#include "Userdata.hpp"
#include "Debug.hpp"
#include "ModAPI.hpp"

class RetroEngine
{
public:
    RetroEngine()
    {
        if (RETRO_GAMEPLATFORM == RETRO_STANDARD) {
            gamePlatform   = "STANDARD";
            gameDeviceType = RETRO_STANDARD;
        }
        else {
            gamePlatform   = "MOBILE";
            gameDeviceType = RETRO_MOBILE;
        }
    }

    bool usingDataFile_Config = false;
    bool usingDataFile        = false;
    bool usingBytecode        = false;

    char dataFile[RETRO_PACKFILE_COUNT][0x80];

    bool initialised = false;
    bool running     = false;
    double deltaTime = 0;

    int gameMode          = ENGINE_MAINGAME;
    int language          = RETRO_EN;
    int message           = 0;
    int gameDeviceType    = RETRO_STANDARD;
    int globalBoxRegion   = RETRO_JP;
    bool nativeMenuFadeIn = false;

    bool trialMode        = false;
    bool onlineActive     = true;
    bool useHighResAssets = false;
#if RETRO_USE_HAPTICS
    bool hapticsEnabled = true;
#endif

    int frameSkipSetting = 0;
    int frameSkipTimer   = 0;

    // Ported from RSDKv5
    int startList_Game  = -1;
    int startStage_Game = -1;

    bool consoleEnabled  = false;
    bool devMenu         = false;
    int startList        = -1;
    int startStage       = -1;
    int startPlayer      = -1;
    int startSave        = -1;
    int gameSpeed        = 1;
    int fastForwardSpeed = 8;
    bool masterPaused    = false;
    bool frameStep       = false;
    int dimTimer         = 0;
    int dimLimit         = 0;
    float dimPercent     = 1.0;
    float dimMax         = 1.0;

    char startSceneFolder[0x10];
    char startSceneID[0x10];

    bool showPaletteOverlay = false;
    bool useHQModes         = true;

    bool hasFocus  = true;
    int focusState = 0;

    void Init();
    void Run();

    bool LoadGameConfig(const char *filepath);
#if RETRO_USE_MOD_LOADER
    void LoadXMLWindowText();
    void LoadXMLVariables();
    void LoadXMLPalettes();
    void LoadXMLObjects();
    void LoadXMLSoundFX();
    void LoadXMLPlayers(TextMenu *menu);
    void LoadXMLStages(TextMenu *menu, int listNo);
#endif

    char gameWindowText[0x40];
    char gameDescriptionText[0x100];
#ifdef DECOMP_VERSION
    const char *gameVersion = DECOMP_VERSION;
#else
    const char *gameVersion       = "1.0.0";
#endif
    const char *gamePlatform = nullptr;

    int gameTypeID             = 0;
    const char *releaseType    = "USE_STANDALONE";
    const char *gameRenderType = "SW_RENDERING";

#if RETRO_USE_HAPTICS
    const char *gameHapticSetting = "USE_F_FEEDBACK"; // None is default, but people with controllers exist
#else
    const char *gameHapticSetting = "NO_F_FEEDBACK";
#endif

    byte gameType = GAME_UNKNOWN;
#if RETRO_USE_MOD_LOADER
    bool modMenuCalled = false;
    bool forceSonic1   = false;
    bool forceSonic2   = false;
#endif

    ushort *frameBuffer   = nullptr;
    ushort *frameBuffer2x = nullptr;
    uint *texBuffer       = nullptr;
    uint *texBuffer2x     = nullptr;

    bool isFullScreen = false;

    bool startFullScreen  = false; // if should start as fullscreen
    bool borderless       = false;
    bool vsync            = false;
    int scalingMode       = 0;
    int windowScale       = 2;
    int refreshRate       = 60; // user-picked screen update rate
    int screenRefreshRate = 60; // hardware screen update rate
    int targetRefreshRate = 60; // game logic update rate

    int renderFrameIndex = 0;
    int skipFrameIndex   = 0;

    int windowXSize; // width of window/screen in the previous frame
    int windowYSize; // height of window/screen in the previous frame

#if RETRO_USING_SDL2
    SDL_Window *window          = nullptr;
    SDL_Renderer *renderer      = nullptr;
    SDL_Texture *screenBuffer   = nullptr;
    SDL_Texture *screenBuffer2x = nullptr;
    SDL_Texture *videoBuffer    = nullptr;
    SDL_Event sdlEvents;
#endif // RETRO_USING_SDL2

#if RETRO_USING_SDL1
    SDL_Surface *windowSurface  = nullptr;
    SDL_Surface *screenBuffer   = nullptr;
    SDL_Surface *screenBuffer2x = nullptr;
    SDL_Surface *videoBuffer    = nullptr;
    SDL_Event sdlEvents;
#endif // RETRO_USING_SDL1
};

extern RetroEngine Engine;
#endif // !RETROENGINE_H
