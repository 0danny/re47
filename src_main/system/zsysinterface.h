#ifndef ZSYSINTERFACE_H
#define ZSYSINTERFACE_H

#include "zstdlib/zconsole/zconsole.h"
#include "zstdlib/zdynamicloader.h"
#include "zstdlib/mystr.h"
#include "zstdlib/reftab/reftab.h"
#include "zstdlib/input/zinputactions.h"
#include "zstdlib/zmastercontrol.h"
#include "common.h"

class ZSysInterface
{
public:
    ZSysInterface(HINSTANCE p_dllHinst);

    virtual boolean UnkFunc0();

    virtual ~ZSysInterface();

    virtual i32 InitializeResources();
    virtual boolean ProcessWindowMessages(HWND p_hWnd);
    virtual void UnkFunc3();
    virtual LRESULT DispatchAndAllocateMessage(WPARAM, const void *, i32);
    virtual char UnkFunc5(const char *, i32);
    virtual void ChangeDisplaySettings();
    virtual char PrintStatus(char);
    virtual i32 UnkFunc8();
    virtual void UnkFunc9();

    virtual void *InitActionMap();
    virtual void FreeActionMap();

    virtual u8 *UnkFunc12();
    virtual void UnkFunc13();
    virtual char UnkFunc14(i32);
    virtual char UnkFunc15(i32);
    virtual char UnkFunc16(i32);

    virtual boolean IsKeyPressed(i32 p_vKey);
    virtual char UnkFunc18(i32);
    virtual char UnkFunc19(i32);
    virtual WORD ConvertVirtualKeyToAscii(UINT p_keyCode);

    virtual char **UnkFunc21(char **, i32);
    virtual BOOL SetWindowTitle(const char *);
    virtual i32 UnkFunc23(i32);
    virtual i32 UnkFunc24();
    virtual void UnkFunc25(const char *);
    virtual void UnkFunc26(const char *);
    virtual void UnkFunc27(const char *);
    virtual void UnkFunc28(const char *);
    virtual void UnkFunc29(const char *);
    virtual char **UnkFunc30(char **, i32);
    virtual i32 UnkFunc31();
    virtual void GenerateUniqueFilename();
    virtual void UnkFunc33();
    virtual double UnkFunc34();
    virtual i32 UnkFunc35();
    virtual i32 UnkFunc36();
    virtual void UnkFunc37();

    virtual void Sleep(u32 p_milliseconds);
    virtual void Init(char *p_cmdLineArgs);
    virtual char Restart(i32);

    virtual void UnkFunc40(i32, i32, i32);
    virtual void UnkFunc41(i32, i32);
    virtual i32 UnkFunc42(char *);
    virtual char UnkFunc43(i32);
    virtual void UnkFunc44();
    virtual i32 UnkFunc45(i32, i32);
    virtual void UnkFunc46(i32, i32, i32, i32, char *, ...);
    virtual void UnkFunc47(i32, i32, i32, char *, ...);
    virtual void UnkFunc48(i32, char *, ...);
    virtual i32 UnkFunc49(i32);
    virtual i32 UnkFunc50(i32);
    virtual i32 UnkFunc51(i32);
    virtual ZConsole *GetConsole();
    virtual i32 UnkFunc53(i32, i32, i32);
    virtual i32 UnkFunc54(i32, i32);
    virtual double UnkFunc55(i32, i32);
    virtual double UnkFunc56(i32, i32);
    virtual i32 UnkFunc57(const void *, u32, i32, const char *, i32);
    virtual char UnkFunc58(char *, u32, i32, i32, i32);
    virtual i32 UnkFunc59(i32, i32, i32, i32, i32);
    virtual i32 UnkFunc60(i32, i32);
    virtual i32 UnkFunc61();
    virtual void UnkFunc62(const char *);
    virtual void UnkFunc63(const char *);
    virtual void UnkFunc64();
    virtual void UnkFunc65();
    virtual ZSysInterface *UnkFunc66(const void *, i32, i32, i32);
    virtual u32 UnkFunc67(i32, i32);
    virtual FARPROC UnkFunc68(i32);

    static LRESULT CALLBACK WndProc(HWND hwnd, u32 uMsg, WPARAM wParam, LPARAM lParam);

    u8 m_currentState;
    u8 m_previousState;
    u8 m_isPaused;
    u8 m_unkByte4;
    u8 m_disablePack;
    u8 m_stereoView;
    u8 m_antiAlias;
    u8 m_disableDXT;
    u8 m_disablePAL;
    u8 m_disableWBuffer;
    u8 m_disableMultiTexture;
    u8 m_unkByte12;
    u8 m_disableLight;
    u8 m_trilinearFiltering;
    u8 m_windowFlag;
    u8 m_unkByte12_1;
    u8 m_disableHWTnl;
    u8 m_enableEMBM;
    u8 m_enableTripleBuffering;
    u8 m_debugVideo;
    u8 m_unkByte12_3;
    i32 m_resWidth;
    i32 m_resHeight;
    i32 m_unkInt3;
    i32 m_unkInt4;
    i32 m_colorDepth;
    i32 m_unkInt6;
    i32 m_refreshRate;
    i32 m_unkInt6_1;
    i32 m_startUpperPosX;
    i32 m_startUpperPosY;
    i32 m_backColor;
    i32 m_anisotropy;
    i32 m_shadowDetail;
    u8 m_noBlood;
    u8 m_pathFinderInof;
    u8 m_enableScriptRTC;
    u8 m_disableOptions;
    i32 m_textureResolution;
    i32 m_levelOfDetail;
    i32 m_unkInt14;
    i32 m_unkInt15;
    MyStr m_myStr1;
    MyStr m_curProjectStr;
    MyStr m_toolsStr;
    MyStr m_activeProjectStr;
    MyStr m_masterStr;
    MyStr m_projectFile;
    MyStr m_myStr2;
    MyStr m_drawDLLStr;
    MyStr m_scriptDllStr;
    MyStr m_directPlayStr;
    MyStr m_soundDllStr;
    MyStr m_localeDllStr;
    MyStr m_animFileStr;
    MyStr m_sceneStr;
    MyStr m_soundStr;
    MyStr m_myStr8;
    MyStr m_myStr9;
    MyStr m_recordFileStr;
    MyStr m_playFileStr;
    MyStr m_playVideoStr;
    float m_unkInt55;
    i32 m_unkInt56;
    i32 m_unkInt58;
    i32 m_unkInt59;
    float m_enableTimers;
    float m_enableDebugOptions;
    i32 m_info;
    HINSTANCE m_dllHinst;
    i32 m_unkInt63;
    HWND m_messageWindow;
    HWND m_mainHwnd;
    i32 m_unkInt66;
    i32 m_unkInt67;
    i32 m_unkInt68;
    i32 m_unkInt69;
    float m_unkInt70;
    i32 m_unkInt71;
    i32 m_unkInt72;
    i32 m_unkInt73;
    i32 m_unkInt74;
    u8 m_unkPad22[3840];
    u8 m_unkPad23[7680];
    ZInputActions *m_zInputActions;
    i32 m_unkInt76;
    i32 m_unkInt77;
    i32 m_unkInt78;
    i32 m_unkInt79;
    float m_unkInt80;
    i32 m_unkInt81;
    i32 m_unkInt82;
    i32 m_unkInt83;
    u8 m_unkPad23_1[8];
    i32 m_unkInt84;
    i32 m_unkInt85;
    i32 m_zRender;
    ZMasterControl *m_zMasterControl;
    MyStr m_masterProjectStr;
    MyStr m_unkMyStr;
    u8 m_unkByte22;
    u8 m_disableTextures;
    u8 m_unkByte25;
    u8 m_isInitialized;
    boolean m_isMessagingEnabled;
    u8 m_disableConfig;
    u8 m_unkByte27;
    u8 m_unkByte28;
    ZDynamicLoader *m_soundDLLBase;
    ZDynamicLoader *m_drawDLLBase;
    ZDynamicLoader *m_scriptDLLBase;
    ZDynamicLoader *m_directPlayDLLBase;
    ZDynamicLoader *m_localeDLLBase;
    MyStr m_consoleCmdStr;
    u8 m_scriptDebug;
    u8 m_scriptDebugPrint;
    u8 m_unkPad27[2];
    float m_timeMultiplier;
    i32 m_unkInt99;
    u8 m_unkByte30;
    u8 m_unkPad28[128];
    i32 m_unkInt100;
    i32 m_unkInt101;
    i32 m_unkInt102;
    i32 m_unkInt103;
    u8 m_saveRecordRuntime;
    i32 m_unkInt104;
    i32 m_unkInt105;
    i32 m_unkInt106;
    RefTab *m_unkInt107;
    i32 m_unkInt108;
    ZConsole *m_console;
    u8 m_unkByte31_1;
    ZDynamicLoader *m_engineDataDLLBase;
    u8 m_messageFlag;
    MyStr m_errorLogStr;
    u8 m_useTryCatchLoop;
    i32 m_unkInt113;
    MyStr m_windowTitleStr;
    MyStr m_autoDumpStr;
    u8 m_reloadEngine;
    i32 m_unkInt118;
    i32 m_unkInt119;
    float m_unkFloat2;
    i32 m_unkInt120;
    void *m_subFFAE110;
};

extern __declspec(dllimport) ZSysInterface *g_pSysInterface;

#endif