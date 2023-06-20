#pragma once

#include <vector>
#include <string>
using namespace std;

#include "cart.h"
#include "Input.h"
#include "Audio.h"
#include "host.h"

//extern "C" {
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
  #include <fix32.h>
//}

using namespace z8;

class Vm {
    Host* _host;
    PicoRam* _memory;
    uint8_t _drawStateCopy[64];

    Graphics* _graphics;
    Audio* _audio;
    Input* _input;

    Cart* _loadedCart;
    lua_State* _luaState;

    bool _cleanupDeps;

    int _targetFps;

    int _picoFrameCount;
    //bool _hasUpdate;
    //bool _hasDraw;

    bool _cartChangeQueued;
    bool _pauseMenu;
    string _prevCartKey;
    string _nextCartKey;
    const unsigned char* _nextCartData;
    size_t _nextCartSize;

    string _cartLoadError;

    string _cartdataKey;

    string _cartBreadcrumb;
    string _cartParam;

    vector<string> _cartList;

    bool loadCart(Cart* cart);
    void vm_reload(int destaddr, int sourceaddr, int len, Cart* cart);


    public:
    Vm(
       Host* host,
       PicoRam* memory = nullptr,
       Graphics* graphics = nullptr,
       Input* input = nullptr,
       Audio* audio = nullptr);
    ~Vm();

    void LoadBiosCart();
    
    void LoadSettingsCart();

    void LoadCart(string filename, bool loadBiosOnFail = true);
    void LoadCart(const unsigned char* cartData, size_t size, bool loadBiosOnFail = true);

    void UpdateAndDraw();

    uint8_t* GetPicoInteralFb();
    uint8_t* GetScreenPaletteMap();

    void FillAudioBuffer(void *audioBuffer, size_t offset, size_t size);

    void CloseCart();

    void QueueCartChange(string newcart);
    void QueueCartChange(const unsigned char* cartData, size_t size);

    int GetTargetFps();

    int GetFrameCount();

    void GameLoop();

    void SetCartList(vector<string> cartList);
    vector<string> GetCartList();
    string GetBiosError();

    bool ExecuteLua(string luaString, string callbackFunction);

    PicoRam* getPicoRam();

    string CurrentCartFilename();

    void togglePauseMenu();

    std::string getSerializedCartData();
    void deserializeCartDataToMemory(std::string cartDataStr);

    uint8_t vm_peek(int addr);
    int16_t vm_peek2(int addr);
    fix32 vm_peek4(int addr);

    void vm_poke(int addr, uint8_t value);
    void vm_poke2(int addr, int16_t value);
    void vm_poke4(int addr, fix32 value);

    bool vm_cartdata(string key);
    fix32 vm_dget(uint8_t n);
    void vm_dset(uint8_t n, fix32 value);

    void vm_reload(int destaddr, int sourceaddr, int len, string filename);

    void vm_memset(int destaddr, uint8_t val, int len);
    void vm_memcpy(int destaddr, int sourceaddr, int len);

    void update_prng();

    fix32 api_rnd();
    fix32 api_rnd(fix32 inRange);
    void api_srand(fix32 seed);

    void update_buttons();

    void vm_flip();
    void vm_run();
    void vm_extcmd(string  cmd);

    void vm_load(string filename, string breadcrumb, string param);

    void vm_reset();

    void setTargetFps(int targetFps);
    int getFps();
    int getTargetFps();

    int getYear();
    int getMonth();
    int getDay();
    int getHour();
    int getMinute();
    int getSecond();
    
    //settings
    int getSetting(string sname);
    void setSetting(string sname, int sval);
    
    void installPackins();
    
    //label
    void loadLabel(string filename, bool mini, int minioffset);
    
    string getLuaLine(string filename, int linenumber);
    
    string getCartBreadcrumb();
    string getCartParam();

    size_t serializeLuaState(char* dest);
    void deserializeLuaState(const char* src, size_t len);
};

