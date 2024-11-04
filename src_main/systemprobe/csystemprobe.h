#ifndef CSYSTEMPROBE_H
#define CSYSTEMPROBE_H

#include "common.h"
#include "zstdlib/zdynamicloader.h"
#include "system/zsyscom.h"
#include "system/zsysfile.h"
#include "system/zsysinterface.h"

#include <directx7/ddraw.h>
#include <gl/GL.h>
#include <glide3/glide.h>

#pragma pack(push, 1)

#define CSETTINGS_INT_SIZE 12
#define CDEVICEINFO_SIZE 112
#define CDEVICEINFO_INT_SIZE 28

const char g_detectionStrs[30][10] = {
    // Computer Speed
    "INVALID",
    "VERY_SLOW",
    "SLOW",
    "MEDIUM",
    "FAST",
    "VERY_FAST",

    // Resolutions
    "640x480",
    "800x600",
    "1024x768",
    "1280x1024",
    "1600x1200",

    // Qualities (No Idea)
    "Low",
    "Normal",
    "High",

    // Texture Quality
    "Very low",
    "Low",
    "Medium",
    "Full",

    // Shadow Quality
    "None",
    "Hitman",
    "All",

    // Renderers
    "Directx",
    "OpenGL",
    "Glide3x",

    // On/Off
    "Off",
    "On",

    // Color Depths
    "16bit",
    "32bit",
};

enum Renderer
{
    DEVICE_UNSELECTED = -1,
    DEVICE_DIRECTX = 0,
    DEVICE_OPENGL = 1,
    DEVICE_GLIDE3D = 2,
};

enum Resolution
{
    RES_UNSELECTED = -1,
    RES_640x480 = 0,
    RES_800x600 = 1,
    RES_1024x768 = 2,
    RES_1280x1024 = 3,
    RES_1600x1200 = 4,
};

enum ColorDepth
{
    COLOR_UNSELECTED = -1,
    COLOR_16BIT = 0,
    COLOR_32BIT = 1,
};

enum ShadowQuality
{
    SHADOW_UNSELECTED = -1,
    SHADOW_NONE = 0,
    SHADOW_HITMAN = 1,
    SHADOW_ALL = 2,
};

enum TextureQuality
{
    TEXTURE_UNSELECTED = -1,
    TEXTURE_VERY_LOW = 0,
    TEXTURE_LOW = 1,
    TEXTURE_MEDIUM = 2,
    TEXTURE_FULL = 3,
};

enum GenericSetting
{
    SETTING_UNSELECTED = -1,
    SETTING_OFF = 0,
    SETTING_ON = 1,
};

struct CSettings
{
    Renderer renderer;                   // 0 = Directx, 1 = OpenGL, 2 = Glide3x
    Resolution resolution;               // 0 = 640x480, 1 = 800x600, 2 = 1024x768, 3 = 1280x1024, 4 = 1600x1200
    ColorDepth colorDepth;               // 0 = 16bit, 1 = 32bit
    ShadowQuality shadowQuality;         // 0 = None, 1 = Hitman, 2 = All
    TextureQuality textureQuality;       // 0 = Very Low, 1 = Low, 2 = Medium, 3 = Full
    GenericSetting unkSetting;           // No idea what this is, nothing changes when it's set to 1 or 0
    GenericSetting anisotropicFiltering; // 0 = Off, 1 = On
    GenericSetting antiAliasing;         // 0 = Off, 1 = On
    GenericSetting textureCompression;   // 0 = Off, 1 = On
    GenericSetting paletteCompression;   // 0 = Off, 1 = On
    GenericSetting vertexLighting;       // 0 = Off, 1 = On
    GenericSetting fullscreen;           // 0 = Off, 1 = On
};

struct CDeviceInfo
{
    u32 has3D;
    u32 deviceType;
    u32 vidMemoryKB;
    char deviceDescription[50];
    char driverName[50];
};

class CGPUClass
{
public:
    CGPUClass *m_next;
    char m_gpuName[50];
    CSettings m_settings;

    CGPUClass();
    void Destroy();

    CGPUClass *GetNext();
};

class CSystemProbe : public ZDynamicLoader
{
private:
    typedef void(WINAPI *GrGetProc)(FxU32 pname, FxU32 plength, void *param);
    typedef const GLubyte *(WINAPI *glGetString_t)(GLenum);
    typedef HRESULT(WINAPI *DirectDrawCreate_t)(GUID *lpGUID, LPDIRECTDRAW *lplpDD, IUnknown *pUnkOuter);
    typedef HRESULT(WINAPI *DirectDrawEnumerateExA_t)(LPDDENUMCALLBACKEXA, LPVOID, DWORD);

public:
    i32 m_flags;
    i32 m_deviceCount;
    CDeviceInfo m_enumContext[4];
    i32 m_unused;
    CSettings m_settings;
    i32 m_memHigher97MB;
    i32 m_unkInt4;
    i32 m_arrayIndex;
    CSettings m_settingsList[9];
    float m_cyclesPerSec;
    i32 m_cpuLevel;
    i32 m_sysMem;
    i32 m_videoMem;
    i32 m_gpuFlags;
    CGPUClass *m_gpuClass;

    CSystemProbe();

    virtual ~CSystemProbe();
    void Destroy();

    virtual void LogSystemInfo();
    virtual void PrintSystemInfo();
    virtual void WriteSystemInfoToFile(char *p_fileName);

    virtual i32 DetectSystemCapabilities();

    virtual i32 GetRenderer();
    virtual i32 GetResolution();
    virtual i32 GetColorDepthSetting();
    virtual i32 GetUnknownSetting();
    virtual i32 GetShadowQuality();
    virtual i32 GetTextureQuality();
    virtual i32 GetFiltering();
    virtual i32 GetAntiAliasing();
    virtual i32 GetTextureCompression();
    virtual i32 GetPaletteCompression();
    virtual i32 GetVertexLighting();
    virtual i32 GetFullscreen();

    virtual CSettings *GetSettings();
    virtual i32 *GetMemHigher();

    virtual i32 DetectMemoryCapabilities();

    i32 InitializeGPUDatabase();
    i32 SetDefaultSettings();
    i32 CalculateAverageSettings();

    i32 GetAllSettings();

    i32 GetVideoMemorySettings();
    i32 GetMemorySettings();
    i32 GetCPUSettings();

    i32 SetUnkInt4();

    void ClassifyCPU();
    i32 ClassifyVideoMem(u32 p_vidMemKB);

    i32 LoadDDraw();
    i32 LoadOpenGL();
    i32 LoadGlide();
    i32 InitOpenGL();

    void GetSysMem();
    void MemTest();

    i32 IntDivide(float p_float, i32 p_int);
    float GetCPUSpeed();

    HRESULT EnumDevices();
    static BOOL CALLBACK EnumDDCallback(GUID FAR *p_lpGUID, LPSTR p_lpDriverDescription, LPSTR p_lpDriverName, LPVOID p_lpContext, HMONITOR p_hMonitor);
};

// Globals
static CSystemProbe *g_probeInstance;

static HMODULE g_ddrawHandle;
static HMODULE g_openGLHandle;
static HMODULE g_glideHandle;
static HMODULE g_unused;

static i32 g_overallDeviceCount;
static i32 g_deviceCount;

#pragma pack(pop)

#endif