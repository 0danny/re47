#pragma once

#include <windows.h>

// Virtual base class for the ZSysInterface
class ZSysInterfaceBase
{
public:
    int Func1();                            // VTABLE 0x0 0xFFA9890
    ~ZSysInterfaceBase();                   // VTABLE 0x4 0xFFA9290
    virtual int Func2();                    // VTABLE 0x8 0xFFAB510
    virtual int Func3();                    // VTABLE 0xC 0xFFB0B50
    virtual int Func4();                    // VTABLE 0x10 0xFFB0BF0
    virtual int Func5();                    // VTABLE 0x14 0xFFAB760
    virtual int Func6();                    // VTABLE 0x18 0xFFB0270
    virtual int Func7();                    // VTABLE 0x1C 0xFFADFF0
    virtual int Func8();                    // VTABLE 0x20 0xFFAB8C0
    virtual int Func9();                    // VTABLE 0x24 0xFFB09E0
    virtual void Func10();                  // VTABLE 0x28 0xFFADEB0
    virtual int Func11();                   // VTABLE 0x2C 0xFFACB20
    virtual int Func12();                   // VTABLE 0x30 0xFFACB90
    virtual int Func13();                   // VTABLE 0x34 0xFFACBE0
    virtual int Func14();                   // VTABLE 0x38 0xFFAE010
    virtual int Func15();                   // VTABLE 0x3C 0xFFACC50
    virtual int Func16();                   // VTABLE 0x40 0xFFACCB0
    virtual int Func17();                   // VTABLE 0x44 0xFFACD10
    virtual int Func18();                   // VTABLE 0x48 0xFFACD70
    virtual int Func19();                   // VTABLE 0x4C 0xFFACD90
    virtual int Func20();                   // VTABLE 0x50 0xFFACDC0
    virtual int Func21();                   // VTABLE 0x54 0xFFACDF0
    virtual int Func22();                   // VTABLE 0x58 0xFFACE50
    virtual int Func23();                   // VTABLE 0x5C 0xFFB0C50
    virtual int Func24();                   // VTABLE 0x60 0xFFAE2D0
    virtual int Func25();                   // VTABLE 0x64 0xFFAB910
    virtual int Func26();                   // VTABLE 0x68 0xFFABBE0
    virtual int Func27();                   // VTABLE 0x6C 0xFFABE10
    virtual int Func28();                   // VTABLE 0x70 0xFFAC040
    virtual int Func29();                   // VTABLE 0x74 0xFFAC270
    virtual int Func30();                   // VTABLE 0x78 0xFFAC4A0
    virtual int Func31();                   // VTABLE 0x7C 0xFFAC6D0
    virtual int Func32();                   // VTABLE 0x80 0xFFADF10
    virtual int Func33();                   // VTABLE 0x84 0xFFAD0D0
    virtual int Func34();                   // VTABLE 0x88 0xFFAE300
    virtual int Func35();                   // VTABLE 0x8C 0xFFB1010
    virtual int Func36();                   // VTABLE 0x90 0xFFB10F0
    virtual int Func37();                   // VTABLE 0x94 0xFFB1140
    virtual int Func38();                   // VTABLE 0x98 0xFFB1230
    virtual int Func39();                   // VTABLE 0x9C 0xFFB1310
    virtual void Func40(LPSTR commandLine); // VTABLE 0xA0 0xFFAD290
    virtual int Func41();                   // VTABLE 0xA4 0xFFAD1F0
    virtual int NullSub1();                 // VTABLE 0xA8 0xNULLSUB
    virtual int NullSub2();                 // VTABLE 0xAC 0xNULLSUB
    virtual int Func44();                   // VTABLE 0xB0 0xFFB0950
    virtual int NullSub3();                 // VTABLE 0xB4 0xNULLSUB
    virtual int Func46();                   // VTABLE 0xB8 0xFFB0990
    virtual int Func47();                   // VTABLE 0xBC 0xFFB1370
    virtual int Func48();                   // VTABLE 0xC0 0xFFB1330
    virtual int Func49();                   // VTABLE 0xC4 0xFFAD510
    virtual int NullSub4();                 // VTABLE 0xC8 0xNULLSUB
    virtual int Func51();                   // VTABLE 0xCC 0xFFB1730
    int Func52();                           // VTABLE 0xD0 0xFFA95E0
    virtual int Func53();                   // VTABLE 0xD4 0xFFB07F0
    virtual int Func54();                   // VTABLE 0xD8 0xFFB0860
    virtual int Func55();                   // VTABLE 0xDC 0xFFB08E0
    int Func56();                           // VTABLE 0xE0 0xFFAA720
    int Func57();                           // VTABLE 0xE4 0xFFAA750
    int Func58();                           // VTABLE 0xE8 0xFFAA780
    int Func59();                           // VTABLE 0xEC 0xFFA9B20
    int Func60();                           // VTABLE 0xF0 0xFFAA660
    int Func61();                           // VTABLE 0xF4 0xFFAA690
    int Func62();                           // VTABLE 0xF8 0xFFAA6C0
    int Func63();                           // VTABLE 0xFC 0xFFAA6F0
    int Func64();                           // VTABLE 0x100 0xFFA9690
    int Func65();                           // VTABLE 0x104 0xFFA98C0
    int Func66();                           // VTABLE 0x108 0xFFA9CF0
    virtual int Func67();                   // VTABLE 0x10C 0xFFB17A0
    int Func68();                           // VTABLE 0x110 0xFFA9600
    int Func69();                           // VTABLE 0x114 0xFFA9D40
    int Func70();                           // VTABLE 0x118 0xFFA9F80
    int Func71();                           // VTABLE 0x11C 0xFFAA1F0
    int Func72();                           // VTABLE 0x120 0xFFAA3D0
    int Func73();                           // VTABLE 0x124 0xFFAA5C0
    virtual int Func74();                   // VTABLE 0x128 0xFFB1740
};

class ZSysInterface : public ZSysInterfaceBase
{
public:
    ZSysInterface();

    // Override all of the virtual functions
    int Func2() override;                    // VTABLE 0x8
    int Func3() override;                    // VTABLE 0xC
    int Func4() override;                    // VTABLE 0x10
    int Func5() override;                    // VTABLE 0x14
    int Func6() override;                    // VTABLE 0x18
    int Func7() override;                    // VTABLE 0x1C
    int Func8() override;                    // VTABLE 0x20
    int Func9() override;                    // VTABLE 0x24
    void Func10() override;                  // VTABLE 0x28
    int Func11() override;                   // VTABLE 0x2C
    int Func12() override;                   // VTABLE 0x30
    int Func13() override;                   // VTABLE 0x34
    int Func14() override;                   // VTABLE 0x38
    int Func15() override;                   // VTABLE 0x3C
    int Func16() override;                   // VTABLE 0x40
    int Func17() override;                   // VTABLE 0x44
    int Func18() override;                   // VTABLE 0x48
    int Func19() override;                   // VTABLE 0x4C
    int Func20() override;                   // VTABLE 0x50
    int Func21() override;                   // VTABLE 0x54
    int Func22() override;                   // VTABLE 0x58
    int Func23() override;                   // VTABLE 0x5C
    int Func24() override;                   // VTABLE 0x60
    int Func25() override;                   // VTABLE 0x64
    int Func26() override;                   // VTABLE 0x68
    int Func27() override;                   // VTABLE 0x6C
    int Func28() override;                   // VTABLE 0x70
    int Func29() override;                   // VTABLE 0x74
    int Func30() override;                   // VTABLE 0x78
    int Func31() override;                   // VTABLE 0x7C
    int Func32() override;                   // VTABLE 0x80
    int Func33() override;                   // VTABLE 0x84
    int Func34() override;                   // VTABLE 0x88
    int Func35() override;                   // VTABLE 0x8C
    int Func36() override;                   // VTABLE 0x90
    int Func37() override;                   // VTABLE 0x94
    int Func38() override;                   // VTABLE 0x98
    int Func39() override;                   // VTABLE 0x9C
    void Func40(LPSTR commandLine) override; // VTABLE 0xA0
    int Func41() override;                   // VTABLE 0xA4
    int NullSub1() override;                 // VTABLE 0xA8
    int NullSub2() override;                 // VTABLE 0xAC
    int Func44() override;                   // VTABLE 0xB0
    int NullSub3() override;                 // VTABLE 0xB4
    int Func46() override;                   // VTABLE 0xB8
    int Func47() override;                   // VTABLE 0xBC
    int Func48() override;                   // VTABLE 0xC0
    int Func49() override;                   // VTABLE 0xC4
    int NullSub4() override;                 // VTABLE 0xC8
    int Func51() override;                   // VTABLE 0xCC

    int Func53() override; // VTABLE 0xD4
    int Func54() override; // VTABLE 0xD8
    int Func55() override; // VTABLE 0xDC
    int Func67() override; // VTABLE 0x10C
    int Func74() override; // VTABLE 0x128

    ~ZSysInterface(); // VTABLE 0x12C 0xFFADFD0
};

extern __declspec(dllimport) ZSysInterface *g_pSysInterface;