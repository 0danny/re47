#include "csystemprobe.h"

/* ----------- CGPUClass ----------- */
CGPUClass::CGPUClass()
{
    m_next = 0;

    m_settings.renderer = DEVICE_UNSELECTED;
    m_settings.resolution = RES_UNSELECTED;
    m_settings.colorDepth = COLOR_UNSELECTED;
    m_settings.shadowQuality = SHADOW_UNSELECTED;
    m_settings.textureQuality = TEXTURE_UNSELECTED;
    m_settings.unkSetting = SETTING_UNSELECTED;
    m_settings.anisotropicFiltering = SETTING_UNSELECTED;
    m_settings.antiAliasing = SETTING_UNSELECTED;
    m_settings.textureCompression = SETTING_UNSELECTED;
    m_settings.paletteCompression = SETTING_UNSELECTED;
    m_settings.vertexLighting = SETTING_UNSELECTED;
    m_settings.fullscreen = SETTING_UNSELECTED;
}

void CGPUClass::Destroy()
{
    if (m_next)
    {
        m_next->Destroy();
        delete m_next;
    }
}

CGPUClass *CGPUClass::GetNext()
{
    CGPUClass *l_this = this;

    for (CGPUClass *l_gpuClass = m_next; l_gpuClass; l_gpuClass = l_gpuClass->m_next)
        l_this = l_gpuClass;

    l_this->m_next = new CGPUClass();

    return l_this->m_next;
}

/* ----------- CSystemProbe ----------- */

CSystemProbe::CSystemProbe() : ZDynamicLoader()
{
    g_ddrawHandle = 0;
    g_openGLHandle = 0;
    g_glideHandle = 0;
    g_unused = 0;

    m_flags = 0;
    m_cyclesPerSec = 0.0;
    m_cpuLevel = 0;
    m_sysMem = 0;
    m_videoMem = 0;
    m_gpuFlags = 0;
    m_unused = 0;

    g_overallDeviceCount = 0;

    m_deviceCount = 0;

    memset(m_enumContext, 0, sizeof(m_enumContext));
    memset(&m_settings, 255u, sizeof(m_settings));

    m_arrayIndex = 0;

    CSettings *l_array = m_settingsList;
    i32 l_count = 9;

    do
    {
        l_array->renderer = DEVICE_UNSELECTED;
        l_array->resolution = RES_UNSELECTED;
        l_array->colorDepth = COLOR_UNSELECTED;
        l_array->shadowQuality = SHADOW_UNSELECTED;
        l_array->textureQuality = TEXTURE_UNSELECTED;
        l_array->unkSetting = SETTING_UNSELECTED;
        l_array->anisotropicFiltering = SETTING_UNSELECTED;
        l_array->antiAliasing = SETTING_UNSELECTED;
        l_array->textureCompression = SETTING_UNSELECTED;
        l_array->paletteCompression = SETTING_UNSELECTED;
        l_array->vertexLighting = SETTING_UNSELECTED;
        l_array->fullscreen = SETTING_UNSELECTED;

        ++l_array;
        --l_count;

    } while (l_count);

    m_gpuClass = 0;

    InitializeGPUDatabase();

    m_memHigher97MB = -1;
    m_unkInt4 = -1;
}

CSystemProbe::~CSystemProbe()
{
    Destroy();
}

void CSystemProbe::Destroy()
{
    if (m_gpuClass)
    {
        m_gpuClass->Destroy();
        delete m_gpuClass;
    }
}

void CSystemProbe::LogSystemInfo()
{
    if (m_deviceCount)
    {
        i32 l_curCount = 0;

        if (m_deviceCount > 0)
        {
            do
            {
                DWORD l_deviceType = m_enumContext[l_curCount].deviceType;

                if (l_deviceType)
                {
                    if (l_deviceType == 1)
                    {
                        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 216);
                        l_sysCom->LogMessage("CSystemProbeDLL: Found OpenGL graphics device\n", m_enumContext[l_curCount].deviceDescription, m_enumContext[l_curCount].driverName);
                    }
                    else if (l_deviceType == 2)
                    {
                        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 218);
                        l_sysCom->LogMessage("CSystemProbeDLL: Found Glide3D graphics device\n", m_enumContext[l_curCount].deviceDescription);
                    }
                }
                else
                {
                    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 214);
                    l_sysCom->LogMessage("CSystemProbeDLL: Found DirectX graphics device\n- %s\n- %s\n", m_enumContext[l_curCount].deviceDescription, m_enumContext[l_curCount].driverName);
                }

                ++l_curCount;

            } while (l_curCount < m_deviceCount);
        }
    }

    if ((m_flags & 1) != 0)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 223);
        l_sysCom->LogMessage("CSystemProbeDLL: DirectX detected\n");
    }
    else
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 225);
        l_sysCom->LogMessage("CSystemProbeDLL: DirectX NOT detected\n");
    }

    if ((m_flags & 2) != 0)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 231);
        l_sysCom->LogMessage("CSystemProbeDLL: OpenGL detected\n");
    }
    else
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 233);
        l_sysCom->LogMessage("CSystemProbeDLL: OpenGL NOT detected\n");
    }

    if ((m_flags & 4) != 0)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 238);
        l_sysCom->LogMessage("CSystemProbeDLL: Glide3D detected\n");
    }
    else
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 240);
        l_sysCom->LogMessage("CSystemProbeDLL: Glide3D NOT detected\n");
    }

    ZSysCom *l_sysCom1 = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 250);
    l_sysCom1->LogMessage("CSystemProbeDLL: Total System Memory %i(Kb)\n", m_sysMem);

    if (m_videoMem)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 253);
        l_sysCom->LogMessage("CSystemProbeDLL: Total Video Memory %i(Kb)\n", m_videoMem);
    }

    ZSysCom *l_sysCom2 = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 256);
    l_sysCom2->LogMessage("CSystemProbeDLL: Cycles per second %f\n", m_cyclesPerSec);

    ZSysCom *l_sysCom3 = g_pSysCom->SetPathAndLine("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 257);
    l_sysCom3->LogMessage("CSystemProbeDLL: Processor speed ~ %s\n", g_detectionStrs[m_cpuLevel]);
}

void CSystemProbe::PrintSystemInfo()
{
    if (m_deviceCount)
    {
        i32 l_curCount = 0;

        if (m_deviceCount > 0)
        {
            do
            {
                u32 l_deviceType = m_enumContext[l_curCount].deviceType;

                if (l_deviceType)
                {
                    if (l_deviceType == 1)
                    {
                        printf("CSystemProbeDLL: Found OpenGL graphics device\n", m_enumContext[l_curCount].deviceDescription, m_enumContext[l_curCount].driverName);
                    }
                    else if (l_deviceType == 2)
                    {
                        printf("CSystemProbeDLL: Found Glide3D graphics device\n", m_enumContext[l_curCount].deviceDescription);
                    }
                }
                else
                {
                    printf("CSystemProbeDLL: Found DirectX graphics device\n- %s\n- %s\n", m_enumContext[l_curCount].deviceDescription, m_enumContext[l_curCount].driverName);
                }

                ++l_curCount;

            } while (l_curCount < m_deviceCount);
        }
    }

    if ((m_flags & 1) != 0)
    {
        printf("CSystemProbeDLL: DirectX detected\n");
    }
    else
    {
        printf("CSystemProbeDLL: DirectX NOT detected\n");
    }

    if ((m_flags & 2) != 0)
    {
        printf("CSystemProbeDLL: OpenGL detected\n");
    }
    else
    {
        printf("CSystemProbeDLL: OpenGL NOT detected\n");
    }

    if ((m_flags & 4) != 0)
    {
        printf("CSystemProbeDLL: Glide3D detected\n");
    }
    else
    {
        printf("CSystemProbeDLL: Glide3D NOT detected\n");
    }

    printf("CSystemProbeDLL: Total System Memory %i(Kb)\n", m_sysMem);

    if (m_videoMem)
    {
        printf("CSystemProbeDLL: Total Video Memory %i(Kb)\n", m_videoMem);
    }

    printf("CSystemProbeDLL: Cycles per second %f\n", m_cyclesPerSec);

    printf("CSystemProbeDLL: Processor speed ~ %s\n", g_detectionStrs[m_cpuLevel]);
}

void CSystemProbe::WriteSystemInfoToFile(char *p_fileName)
{
    FILE *l_file = LockedFileOpen(p_fileName, "w");

    if (m_deviceCount)
    {
        i32 l_curCount = 0;

        if (m_deviceCount > 0)
        {
            do
            {
                DWORD l_deviceType = m_enumContext[l_curCount].deviceType;

                if (l_deviceType)
                {
                    if (l_deviceType == 1)
                    {
                        fprintf(l_file, "CSystemProbeDLL: Found OpenGL graphics device\n", m_enumContext[l_curCount].deviceDescription, m_enumContext[l_curCount].driverName);
                    }
                    else if (l_deviceType == 2)
                    {
                        fprintf(l_file, "CSystemProbeDLL: Found Glide3D graphics device\n", m_enumContext[l_curCount].deviceDescription);
                    }
                }
                else
                {
                    fprintf(l_file, "CSystemProbeDLL: Found DirectX graphics device\n- %s\n- %s\n", m_enumContext[l_curCount].deviceDescription, m_enumContext[l_curCount].driverName);
                }

                ++l_curCount;

            } while (l_curCount < m_deviceCount);
        }
    }

    if ((m_flags & 1) != 0)
    {
        fprintf(l_file, "CSystemProbeDLL: DirectX detected\n");
    }
    else
    {
        fprintf(l_file, "CSystemProbeDLL: DirectX NOT detected\n");
    }

    if ((m_flags & 2) != 0)
    {
        fprintf(l_file, "CSystemProbeDLL: OpenGL detected\n");
    }
    else
    {
        fprintf(l_file, "CSystemProbeDLL: OpenGL NOT detected\n");
    }

    if ((m_flags & 4) != 0)
    {
        fprintf(l_file, "CSystemProbeDLL: Glide3D detected\n");
    }
    else
    {
        fprintf(l_file, "CSystemProbeDLL: Glide3D NOT detected\n");
    }

    fprintf(l_file, "CSystemProbeDLL: Total System Memory %i(Kb)\n", m_sysMem);

    if (m_videoMem)
    {
        fprintf(l_file, "CSystemProbeDLL: Total Video Memory %i(Kb)\n", m_videoMem);
    }

    fprintf(l_file, "CSystemProbeDLL: Cycles per second %f\n", m_cyclesPerSec);
    fprintf(l_file, "CSystemProbeDLL: Processor speed ~ %s\n", g_detectionStrs[m_cpuLevel]);

    fclose(l_file);
}

i32 CSystemProbe::DetectSystemCapabilities()
{
    ClassifyCPU();

    LoadDDraw();
    LoadOpenGL();
    LoadGlide();

    GetSysMem();
    GetAllSettings();

    CalculateAverageSettings();
    SetDefaultSettings();

    return 0;
}

i32 CSystemProbe::GetRenderer()
{
    return m_settings.renderer;
}

i32 CSystemProbe::GetResolution()
{
    return m_settings.resolution;
}

i32 CSystemProbe::GetColorDepthSetting()
{
    return m_settings.colorDepth;
}

i32 CSystemProbe::GetUnknownSetting()
{
    return m_settings.unkSetting;
}

i32 CSystemProbe::GetShadowQuality()
{
    return m_settings.shadowQuality;
}

i32 CSystemProbe::GetTextureQuality()
{
    return m_settings.textureQuality;
}

i32 CSystemProbe::GetFiltering()
{
    return m_settings.anisotropicFiltering;
}

i32 CSystemProbe::GetAntiAliasing()
{
    return m_settings.antiAliasing;
}

i32 CSystemProbe::GetTextureCompression()
{
    return m_settings.textureCompression;
}

i32 CSystemProbe::GetPaletteCompression()
{
    return m_settings.paletteCompression;
}

i32 CSystemProbe::GetVertexLighting()
{
    return m_settings.vertexLighting;
}

i32 CSystemProbe::GetFullscreen()
{
    return m_settings.fullscreen;
}

CSettings *CSystemProbe::GetSettings()
{
    return &m_settings;
}

i32 *CSystemProbe::GetMemHigher()
{
    return &m_memHigher97MB;
}

i32 CSystemProbe::DetectMemoryCapabilities()
{
    ClassifyCPU();
    GetSysMem();
    MemTest();
    SetUnkInt4();

    return 0;
}

i32 CSystemProbe::InitializeGPUDatabase()
{
    if (!m_gpuClass)
    {
        m_gpuClass = new CGPUClass();
    }

    strcpy(m_gpuClass->m_gpuName, "Diamond FIRE GL 1000 PRO");

    CGPUClass *l_gpu1 = m_gpuClass->GetNext();
    strcpy(l_gpu1->m_gpuName, "Diamond Stealth III S540 for Windows 98");
    l_gpu1->m_settings.resolution = RES_640x480;
    l_gpu1->m_settings.renderer = DEVICE_OPENGL;
    l_gpu1->m_settings.fullscreen = SETTING_ON;

    CGPUClass *l_gpu2 = m_gpuClass->GetNext();
    strcpy(l_gpu2->m_gpuName, "Matrox Millennium G200 AGP - English");
    l_gpu2->m_settings.resolution = RES_640x480;
    l_gpu2->m_settings.renderer = DEVICE_OPENGL;
    l_gpu2->m_settings.fullscreen = SETTING_ON;

    CGPUClass *l_gpu3 = m_gpuClass->GetNext();
    strcpy(l_gpu3->m_gpuName, "Matrox Millennium G400 DualHead - English");
    l_gpu3->m_settings.resolution = RES_800x600;
    l_gpu3->m_settings.renderer = DEVICE_GLIDE3D;
    l_gpu3->m_settings.fullscreen = SETTING_ON;

    CGPUClass *l_gpu4 = m_gpuClass->GetNext();
    strcpy(l_gpu4->m_gpuName, "NVIDIA GeForce2 GTS/GeForce2 Pro");
    l_gpu4->m_settings.fullscreen = SETTING_ON;
    l_gpu4->m_settings.resolution = RES_800x600;
    l_gpu4->m_settings.renderer = DEVICE_GLIDE3D;

    CGPUClass *l_gpu5 = m_gpuClass->GetNext();
    strcpy(l_gpu5->m_gpuName, "NVIDIA GeForce2 MX");
    l_gpu5->m_settings.fullscreen = SETTING_ON;
    l_gpu5->m_settings.resolution = RES_800x600;
    l_gpu5->m_settings.renderer = DEVICE_GLIDE3D;

    CGPUClass *l_gpu6 = m_gpuClass->GetNext();
    strcpy(l_gpu6->m_gpuName, "NVIDIA GeForce 256");
    l_gpu6->m_settings.fullscreen = SETTING_ON;
    l_gpu6->m_settings.renderer = DEVICE_GLIDE3D;

    CGPUClass *l_gpu7 = m_gpuClass->GetNext();
    strcpy(l_gpu7->m_gpuName, "NVIDIA RIVA TNT");
    l_gpu7->m_settings.fullscreen = SETTING_ON;
    l_gpu7->m_settings.renderer = DEVICE_GLIDE3D;

    CGPUClass *l_gpu8 = m_gpuClass->GetNext();
    strcpy(l_gpu8->m_gpuName, "Voodoo3 AGP");
    l_gpu8->m_settings.fullscreen = SETTING_OFF;
    l_gpu8->m_settings.resolution = RES_640x480;

    CGPUClass *l_gpu9 = m_gpuClass->GetNext();
    strcpy(l_gpu9->m_gpuName, "Voodoo5 5500 AGP");
    l_gpu9->m_settings.resolution = RES_800x600;
    l_gpu9->m_settings.fullscreen = SETTING_OFF;

    CGPUClass *l_gpu10 = m_gpuClass->GetNext();
    strcpy(l_gpu10->m_gpuName, "All-In-Wonder 128 Pro AGP (English)");
    l_gpu10->m_settings.fullscreen = SETTING_ON;
    l_gpu10->m_settings.renderer = DEVICE_GLIDE3D;
    l_gpu10->m_settings.resolution = RES_800x600;

    CGPUClass *l_gpu11 = m_gpuClass->GetNext();
    strcpy(l_gpu11->m_gpuName, "Radeon DDR");
    l_gpu11->m_settings.fullscreen = SETTING_ON;
    l_gpu11->m_settings.resolution = RES_800x600;
    l_gpu11->m_settings.renderer = DEVICE_GLIDE3D;

    return 0;
}

i32 CSystemProbe::SetDefaultSettings()
{
    if (m_settings.resolution == RES_UNSELECTED)
        m_settings.resolution = RES_1024x768;

    if (m_settings.colorDepth == COLOR_UNSELECTED)
        m_settings.colorDepth = COLOR_32BIT;

    if (m_settings.shadowQuality == SHADOW_UNSELECTED)
        m_settings.shadowQuality = SHADOW_HITMAN;

    if (m_settings.textureQuality == TEXTURE_UNSELECTED)
        m_settings.textureQuality = TEXTURE_FULL;

    if (m_settings.unkSetting == SETTING_UNSELECTED)
        m_settings.unkSetting = SETTING_ON;

    if (m_settings.anisotropicFiltering == SETTING_UNSELECTED)
        m_settings.anisotropicFiltering = SETTING_OFF;

    if (m_settings.antiAliasing == SETTING_UNSELECTED)
        m_settings.antiAliasing = SETTING_OFF;

    if (m_settings.textureCompression == SETTING_UNSELECTED)
        m_settings.textureCompression = SETTING_ON;

    if (m_settings.paletteCompression == SETTING_UNSELECTED)
        m_settings.paletteCompression = SETTING_ON;

    if (m_settings.vertexLighting == SETTING_UNSELECTED)
        m_settings.vertexLighting = SETTING_OFF;

    if (m_settings.fullscreen == SETTING_UNSELECTED)
        m_settings.fullscreen = SETTING_ON;

    return 0;
}

i32 CSystemProbe::CalculateAverageSettings()
{
    i32 l_arrayIndex = m_arrayIndex;

    if (l_arrayIndex > 0)
    {
        CSettings *l_settingsPtr = m_settingsList;
        i32 l_iteration1 = l_arrayIndex;

        do
        {
            ++l_settingsPtr;
            --l_iteration1;
        } while (l_iteration1);
    }

    if ((m_flags & 4) != 0)
        m_settings.renderer = DEVICE_GLIDE3D;
    else
        m_settings.renderer = DEVICE_DIRECTX;

    f32 l_resFloat = 0.0;
    f32 l_resCount = 0;
    f32 l_resResult = 0.0;

    if (l_arrayIndex > 0)
    {
        Resolution *l_resPtr = &m_settingsList[0].resolution;

        do
        {
            if (*l_resPtr != RES_UNSELECTED)
            {
                l_resFloat = l_resFloat + (f32)*l_resPtr;
                ++l_resCount;
            }

            l_resPtr += CSETTINGS_INT_SIZE;
            --l_arrayIndex;

        } while (l_arrayIndex);

        l_resResult = l_resFloat;
    }

    i32 l_resSetting = IntDivide(l_resResult, l_resCount);

    f32 l_colorFloat = 0.0;
    i32 l_colorIndex = m_arrayIndex;
    f32 l_colorResult = 0.0;
    i32 l_colorCount = 0;

    m_settings.resolution = (Resolution)l_resSetting;

    if (l_colorIndex > 0)
    {
        ColorDepth *l_colorPtr = &m_settingsList[0].colorDepth;

        do
        {
            if (*l_colorPtr != COLOR_UNSELECTED)
            {
                l_colorFloat = l_colorFloat + (f32)*l_colorPtr;
                ++l_colorCount;
            }

            l_colorPtr += CSETTINGS_INT_SIZE;
            --l_colorIndex;

        } while (l_colorIndex);

        l_colorResult = l_colorFloat;
    }

    i32 l_colorSetting = IntDivide(l_colorResult, l_colorCount);

    f32 l_textureFloat = 0.0;
    i32 l_textureIndex = m_arrayIndex;
    f32 l_textureResult = 0.0;
    i32 l_textureCount = 0;

    m_settings.colorDepth = (ColorDepth)l_colorSetting;

    if (l_textureIndex > 0)
    {
        TextureQuality *l_texturePtr = &m_settingsList[0].textureQuality;

        do
        {
            if (*l_texturePtr != TEXTURE_UNSELECTED)
            {
                l_textureFloat = l_textureFloat + (f32)*l_texturePtr;
                ++l_textureCount;
            }

            l_texturePtr += CSETTINGS_INT_SIZE;
            --l_textureIndex;

        } while (l_textureIndex);

        l_textureResult = l_textureFloat;
    }

    i32 l_textureSetting = IntDivide(l_textureResult, l_textureCount);

    f32 l_unkSettingFloat = 0.0;
    i32 l_unkSettingIndex = m_arrayIndex;
    f32 l_unkSettingResult = 0.0;
    i32 l_unkSettingCount = 0;

    m_settings.textureQuality = (TextureQuality)l_textureSetting;

    if (l_unkSettingIndex > 0)
    {
        GenericSetting *l_unkSettingPtr = &m_settingsList[0].unkSetting;

        do
        {
            if (*l_unkSettingPtr != SETTING_UNSELECTED)
            {
                l_unkSettingFloat = l_unkSettingFloat + (f32)*l_unkSettingPtr;
                ++l_unkSettingCount;
            }

            l_unkSettingPtr += CSETTINGS_INT_SIZE;
            --l_unkSettingIndex;

        } while (l_unkSettingIndex);

        l_unkSettingResult = l_unkSettingFloat;
    }

    i32 l_unkSetting = IntDivide(l_unkSettingResult, l_unkSettingCount);

    f32 l_shadowFloat = 0.0;
    i32 l_shadowIndex = m_arrayIndex;
    f32 l_shadowResult = 0.0;
    i32 l_shadowCount = 0;

    m_settings.unkSetting = (GenericSetting)l_unkSetting;

    if (l_shadowIndex > 0)
    {
        ShadowQuality *l_shadowPtr = &m_settingsList[0].shadowQuality;

        do
        {
            if (*l_shadowPtr != SETTING_UNSELECTED)
            {
                l_shadowFloat = l_shadowFloat + (f32)*l_shadowPtr;
                ++l_shadowCount;
            }

            l_shadowPtr += CSETTINGS_INT_SIZE;
            --l_shadowIndex;

        } while (l_shadowIndex);

        l_shadowResult = l_shadowFloat;
    }

    m_settings.shadowQuality = (ShadowQuality)CSystemProbe::IntDivide(l_shadowResult, l_shadowCount);

    i32 l_vertexIndex = m_arrayIndex;
    i32 l_vertexCount = 0;

    if (l_vertexIndex > 0)
    {
        GenericSetting *l_vertexPtr = &m_settingsList[0].vertexLighting;

        do
        {
            if (*l_vertexPtr == SETTING_ON)
                ++l_vertexCount;

            l_vertexPtr += CSETTINGS_INT_SIZE;

            --l_vertexIndex;

        } while (l_vertexIndex);

        if (l_vertexCount)
            m_settings.vertexLighting = SETTING_ON;
    }

    return 0;
}

i32 CSystemProbe::GetAllSettings()
{
    GetVideoMemorySettings();
    GetMemorySettings();
    GetCPUSettings();

    return 0;
}

i32 CSystemProbe::GetVideoMemorySettings()
{
    i32 l_curIndex = 0;
    i32 l_innerCount = 0;

    if ((m_flags & 1) == 0)
        return 0;

    if (g_overallDeviceCount <= 0)
    {
    GOTO_LOOP:

        i32 l_count = 0;

        if (g_overallDeviceCount - 1 > 0)
        {
            i32 l_nextStruct = 0;
            i32 l_increment = 0;

            u32 *l_videoMemoryPtr = &m_enumContext[0].vidMemoryKB;

            do
            {
                if (*l_videoMemoryPtr > *(&m_enumContext[0].vidMemoryKB + l_nextStruct))
                {
                    l_innerCount = l_count;
                    l_nextStruct = l_increment;
                }

                ++l_count;

                l_increment += CDEVICEINFO_SIZE;
                l_videoMemoryPtr += CDEVICEINFO_INT_SIZE;

            } while (l_count < g_overallDeviceCount - 1);

            l_curIndex = l_innerCount;
        }

        ClassifyVideoMem(m_enumContext[l_curIndex].vidMemoryKB);
        return 0;
    }

    CDeviceInfo *l_enumContext = m_enumContext;

    i32 l_index = 0;

    while (l_enumContext->has3D != 1)
    {
        ++l_index;
        ++l_enumContext;

        if (l_index >= g_overallDeviceCount)
            goto GOTO_LOOP;
    }

    ClassifyVideoMem(m_enumContext[l_index].vidMemoryKB);

    return 0;
}

i32 CSystemProbe::GetMemorySettings()
{
    if (m_sysMem)
    {
        if (m_sysMem < 65000)
        {
            m_settingsList[m_arrayIndex].resolution = RES_640x480;
            m_settingsList[m_arrayIndex].unkSetting = SETTING_OFF;
            m_settingsList[m_arrayIndex].textureQuality = TEXTURE_VERY_LOW;
            m_settingsList[m_arrayIndex].colorDepth = COLOR_16BIT;
            m_settingsList[m_arrayIndex++].shadowQuality = SHADOW_NONE;
            return 0;
        }

        if (m_sysMem < 128000)
        {
            m_settingsList[m_arrayIndex].unkSetting = SETTING_ON;
            m_settingsList[m_arrayIndex++].textureQuality = TEXTURE_MEDIUM;
            return 0;
        }

        m_settingsList[m_arrayIndex].unkSetting = SETTING_ON;
        m_settingsList[m_arrayIndex++].textureQuality = TEXTURE_FULL;
    }

    return 0;
}

i32 CSystemProbe::GetCPUSettings()
{
    if (m_cpuLevel)
    {
        if (m_cpuLevel == 1)
        {
            m_settingsList[m_arrayIndex].resolution = RES_640x480;
            m_settingsList[m_arrayIndex].unkSetting = SETTING_OFF;
            m_settingsList[m_arrayIndex].textureQuality = TEXTURE_VERY_LOW;
            m_settingsList[m_arrayIndex].colorDepth = COLOR_16BIT;
        }
        else
        {
            if (m_cpuLevel != 2)
            {
                switch (m_cpuLevel)
                {
                case 3:
                    m_settingsList[m_arrayIndex].unkSetting = SETTING_ON;
                    m_settingsList[m_arrayIndex].resolution = RES_1024x768;
                    m_settingsList[m_arrayIndex].shadowQuality = SHADOW_HITMAN;
                    break;

                case 4:
                    m_settingsList[m_arrayIndex].unkSetting = SETTING_ON;
                    m_settingsList[m_arrayIndex].resolution = RES_1024x768;
                    m_settingsList[m_arrayIndex].shadowQuality = SHADOW_HITMAN;
                    break;

                case 5:
                    m_settingsList[m_arrayIndex].unkSetting = SETTING_ON;
                    m_settingsList[m_arrayIndex].resolution = RES_1024x768;
                    m_settingsList[m_arrayIndex].shadowQuality = SHADOW_ALL;
                    break;
                }

                goto INCREMENT_INDEX;
            }

            m_settingsList[m_arrayIndex].unkSetting = SETTING_OFF;
            m_settingsList[m_arrayIndex].resolution = RES_800x600;
        }

        m_settingsList[m_arrayIndex].shadowQuality = SHADOW_NONE;

    INCREMENT_INDEX:

        ++m_arrayIndex;
        return 0;
    }

    return 0;
}

i32 CSystemProbe::SetUnkInt4()
{
    m_unkInt4 = 8;
    return 0;
}

void CSystemProbe::ClassifyCPU()
{
    f32 l_cpuSpeed = GetCPUSpeed();

    m_cyclesPerSec = l_cpuSpeed;

    i32 l_cpuDiv = (i32)(i64)l_cpuSpeed / 1000000;

    if (l_cpuDiv >= 400)
    {
        if (l_cpuDiv >= 500)
        {
            if (l_cpuDiv >= 650)
            {
                if (l_cpuDiv >= 750)
                    m_cpuLevel = 5;
                else
                    m_cpuLevel = 4;
            }
            else
            {
                m_cpuLevel = 3;
            }
        }
        else
        {
            m_cpuLevel = 2;
        }
    }
    else
    {
        m_cpuLevel = 1;
    }
}

i32 CSystemProbe::LoadDDraw()
{
    bool l_validLoad = 0;

    g_ddrawHandle = GetModuleHandleA("DDRAW");

    if (g_ddrawHandle || (l_validLoad = 1, (g_ddrawHandle = LoadLibraryA("ddraw.dll")) != 0))
    {
        if (EnumDevices())
        {
            if (l_validLoad)
                FreeLibrary(g_ddrawHandle);

            return 1002;
        }
        else
        {
            m_flags |= 1u;

            if (l_validLoad)
                FreeLibrary(g_ddrawHandle);

            return 0;
        }
    }
    else
    {
        m_gpuFlags = 1002;
        return 1000;
    }
}

HRESULT CSystemProbe::EnumDevices()
{
    HRESULT l_hresult;

    FARPROC l_ddEnumerate = GetProcAddress(g_ddrawHandle, "DirectDrawEnumerateExA");

    DirectDrawEnumerateExA_t DirectDrawEnumerateExA = reinterpret_cast<DirectDrawEnumerateExA_t>(l_ddEnumerate);

    if (!DirectDrawEnumerateExA || (l_hresult = DirectDrawEnumerateExA(CSystemProbe::EnumDDCallback, m_enumContext, 3)) != 0)
    {
        m_gpuFlags = 2000;
        return 1000;
    }
    else
    {
        m_deviceCount = g_deviceCount;
    }

    return l_hresult;
}

BOOL CALLBACK CSystemProbe::EnumDDCallback(GUID FAR *p_lpGUID, LPSTR p_lpDriverDescription, LPSTR p_lpDriverName, LPVOID p_lpContext, HMONITOR p_hMonitor)
{
    LPDIRECTDRAW l_lpDD;
    DDCAPS l_ddCaps;
    DDDEVICEIDENTIFIER2 l_ddDeviceIdentifier;

    CDeviceInfo *l_lpContext = reinterpret_cast<CDeviceInfo *>(p_lpContext);

    IDirectDraw7 *l_dd7Interface = 0;

    DirectDrawCreate_t DirectDrawCreate = reinterpret_cast<DirectDrawCreate_t>(GetProcAddress(g_ddrawHandle, "DirectDrawCreate"));

    if (DirectDrawCreate(p_lpGUID, &l_lpDD, 0) || l_lpDD->QueryInterface(IID_IDirectDraw7, reinterpret_cast<void **>(&l_dd7Interface)))
        return 0;

    memset(&l_ddCaps, 0, sizeof(l_ddCaps));

    l_ddCaps.dwSize = 380;

    l_dd7Interface->GetCaps(&l_ddCaps, 0);
    l_dd7Interface->GetDeviceIdentifier(&l_ddDeviceIdentifier, 0);

    l_lpContext[g_deviceCount].deviceType = 0;
    l_lpContext[g_deviceCount].vidMemoryKB = l_ddCaps.dwVidMemTotal >> 10;

    if (strlen(l_ddDeviceIdentifier.szDescription) <= 50)
    {
        strcpy(l_lpContext[g_deviceCount].deviceDescription, l_ddDeviceIdentifier.szDescription);
    }
    else
    {
        char *l_deviceDesc = l_lpContext[g_deviceCount].deviceDescription;
        memcpy(l_deviceDesc, l_ddDeviceIdentifier.szDescription, 48u);
        l_lpContext[g_deviceCount].driverName[0] = '\0';
    }

    if (strlen(l_ddDeviceIdentifier.szDriver) <= 50)
    {
        strcpy(l_lpContext[g_deviceCount].driverName, l_ddDeviceIdentifier.szDriver);
    }
    else
    {
        char *l_driverName = l_lpContext[g_deviceCount].driverName;
        memcpy(l_driverName, &l_ddDeviceIdentifier.szDriver, 48u);
        l_lpContext[g_deviceCount + 1].has3D = 0;
    }

    l_lpContext[g_deviceCount].has3D = (l_ddCaps.dwCaps & 1) != 0 ? 1 : 2;

    if (l_dd7Interface)
        l_dd7Interface->Release();

    ++g_overallDeviceCount;

    if (g_deviceCount >= 4)
        return 0;

    ++g_deviceCount;

    return 1;
}

i32 CSystemProbe::LoadOpenGL()
{
    bool l_flag = 0;

    g_openGLHandle = GetModuleHandleA("OPENGL32");

    if (g_openGLHandle || (l_flag = 1, (g_openGLHandle = LoadLibraryA("OPENGL32.DLL")) != 0))
    {
        if (CSystemProbe::InitOpenGL())
        {
            m_gpuFlags = 1003;

            if (l_flag)
                FreeLibrary(g_openGLHandle);

            return 1000;
        }
        else
        {
            m_flags |= 2u;

            if (l_flag)
                FreeLibrary(g_openGLHandle);

            return 0;
        }
    }
    else
    {
        m_gpuFlags = 1003;
        return 1000;
    }
}

i32 CSystemProbe::InitOpenGL()
{
    PIXELFORMATDESCRIPTOR l_ppfd = {0};
    HGLRC l_context;

    l_ppfd.nSize = 40;
    l_ppfd.nVersion = 1;
    l_ppfd.dwFlags = 37;
    l_ppfd.iPixelType = 0;
    l_ppfd.cColorBits = 16;
    l_ppfd.cDepthBits = 32;

    if (!g_pSysInterface)
    {
        HDC l_deviceContext = GetDC(0);

        i32 l_pixelFormat = ChoosePixelFormat(l_deviceContext, &l_ppfd);
        SetPixelFormat(l_deviceContext, l_pixelFormat, &l_ppfd);
        HGLRC l_createContext = wglCreateContext(l_deviceContext);

        if (l_createContext && !wglMakeCurrent(l_deviceContext, l_createContext))
        {
            wglDeleteContext(l_createContext);
            m_gpuFlags = 1003;
            return 1000;
        }
    }

    glGetString_t l_glGetString = reinterpret_cast<glGetString_t>(GetProcAddress(g_openGLHandle, "glGetString"));

    const char *l_vendor = (const char *)l_glGetString(GL_VENDOR);
    if (l_vendor)
    {
        strcpy(m_enumContext[g_deviceCount].deviceDescription, l_vendor);
    }

    const char *l_renderer = (const char *)l_glGetString(GL_RENDERER);
    if (l_renderer)
    {
        strcpy(m_enumContext[g_deviceCount].driverName, l_renderer);
    }

    m_enumContext[g_deviceCount].deviceType = 1;
    m_enumContext[g_deviceCount].has3D = 0;

    ++m_deviceCount;

    if (!g_pSysInterface && l_context)
        wglDeleteContext(l_context);

    return 0;
}

i32 CSystemProbe::LoadGlide()
{
    // TODO: Add ZRender VT call, see original function.

    i32 l_numBoards = 0;
    bool l_glideLoaded = 0;

    HMODULE l_glideHandle = GetModuleHandleA("GLIDE3X");
    g_glideHandle = l_glideHandle;

    if (l_glideHandle || (l_glideLoaded = 1, l_glideHandle = LoadLibraryA("GLIDE3X.DLL"), (g_glideHandle = l_glideHandle) != 0))
    {
        GrGetProc l_grGet = (GrGetProc)GetProcAddress(g_glideHandle, "_grGet@12");

        if (l_grGet)
        {
            l_grGet(GR_NUM_BOARDS, sizeof(i32), &l_numBoards);

            if (l_numBoards)
            {
                m_flags |= 4u;

                if (l_glideLoaded)
                    FreeLibrary(g_glideHandle);

                return 1000;
            }
            else
            {
                if (l_glideLoaded)
                    FreeLibrary(g_glideHandle);

                return 0;
            }
        }
        else
        {
            if (l_glideLoaded)
                FreeLibrary(g_glideHandle);

            return 1000;
        }
    }
    else
    {
        return 1004;
    }
}

void CSystemProbe::GetSysMem()
{
    struct _MEMORYSTATUS l_memStatus;

    GlobalMemoryStatus(&l_memStatus);
    m_sysMem = l_memStatus.dwTotalPhys >> 10;
}

void CSystemProbe::MemTest()
{
    m_memHigher97MB = m_sysMem >= 97000;
}

i32 CSystemProbe::ClassifyVideoMem(u32 p_vidMemKB)
{
    m_videoMem = p_vidMemKB;

    if (p_vidMemKB > 8200)
    {
        if (p_vidMemKB >= 16000)
        {
            m_settingsList[m_arrayIndex].textureQuality = TEXTURE_FULL;

            if (p_vidMemKB >= 32000)
                m_settingsList[m_arrayIndex].resolution = RES_1280x1024;
            else
                m_settingsList[m_arrayIndex].resolution = RES_1024x768;

            ++m_arrayIndex;
            return 0;
        }
        else
        {
            m_settingsList[m_arrayIndex].textureQuality = TEXTURE_MEDIUM;
            m_settingsList[m_arrayIndex++].resolution = RES_800x600;
            return 0;
        }
    }
    else
    {
        m_settingsList[m_arrayIndex].textureQuality = TEXTURE_LOW;
        m_settingsList[m_arrayIndex++].resolution = RES_640x480;
        return 0;
    }
}

i32 CSystemProbe::IntDivide(f32 p_float, i32 p_int)
{
    if (p_int != 0)
    {
        return static_cast<i32>(p_float / p_int);
    }
    else
    {
        return -1;
    }
}

f32 CSystemProbe::GetCPUSpeed()
{
    if (g_pSysInterface)
        return g_pSysInterface->m_unkInt55;

    i64 l_cycles = GetRDTSC();

    Sleep(1000u);

    return (f32)(i64)(GetRDTSC() - l_cycles);
}