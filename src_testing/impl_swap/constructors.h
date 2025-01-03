#ifndef CONSTRUCTORS_H
#define CONSTRUCTORS_H

// Reftabs

#include "zstdlib/reftab/reftab32.h"
#include "zstdlib/reftab/allocreftab.h"
#include "zstdlib/reftab/staticreftab.h"
#include "zstdlib/reftab/equreftab.h"
#include "zstdlib/reftab/strreftab.h"
#include "zstdlib/reftab/linkreftab.h"
#include "zstdlib/reftab/linksortreftab.h"
#include "zstdlib/input/zinputaction.h"
#include "zstdlib/input/zinputactions.h"
#include "zstdlib/input/zactionmap.h"
#include "zstdlib/fszip.h"

#include "zstdlib/zmastercontrol.h"

#include "zstdlib/trees/zrbtree.h"
#include "zstdlib/trees/zvaltree.h"
#include "zstdlib/zconsole/zconsole.h"
#include "drawing/zcounter.h"
#include "utilities.h"

#include "system/zsyscom.h"
#include "system/zsysfile.h"

namespace Constructors
{
    // renderopengl.dll
    static LPVOID linkSortRefTabAddress = (LPVOID)0x0FB94FF0;
    static LPVOID linkRefTabRenderingAddress = (LPVOID)0x0FB94700;
    static LPVOID zCounterAddress = (LPVOID)0x0FB9BFB0;

    // Addresses
    static LPVOID cFastLookup2Address = (LPVOID)0x0FFB9A00;
    static LPVOID refTabAddress = (LPVOID)0x0FFBED50;
    static LPVOID ref32RefTabAddress = (LPVOID)0x0FFBF5B0;
    static LPVOID staticRefTabAddress = (LPVOID)0x0FFBF710;
    static LPVOID allocRefTabAddress = (LPVOID)0x0FFC1350;

    static LPVOID equRefTabAddress = (LPVOID)0x0FFBFE80;
    static LPVOID strRefTabAddress = (LPVOID)0x0FFC0D30;
    static LPVOID linkRefTabAddress = (LPVOID)0x0FFC0050;

    static LPVOID zrbTreeAddress = (LPVOID)0x0FFC80D0;
    static LPVOID zvalTreeAddress = (LPVOID)0x0FFC8580;
    static LPVOID zConsoleAddress = (LPVOID)0x0FFC5400;

    static LPVOID zSysComAddress = (LPVOID)0x0FFA4570;
    static LPVOID zMasterControlAddress = (LPVOID)0x0FFC6060;

    static LPVOID zInputActionDefCtor = (LPVOID)0x0FFA3240;
    static LPVOID zActionMapAddress = (LPVOID)0x0FFA2640;
    static LPVOID zInputActionsCtor = (LPVOID)0x0FFA1000;

    static LPVOID fsZipCtor = (LPVOID)0x0FFC3490;

    // Typedefs
    typedef RefTab *(__fastcall *RefTabConstructor)(RefTab *_this, int p_poolSize, int p_size);
    typedef RefTab32 *(__fastcall *RefTab32Constructor)(RefTab32 *_this);

    typedef StaticRefTab *(__fastcall *StaticRefTabConstructor)(StaticRefTab *_this, int p_poolSize, int p_size);
    typedef AllocRefTab *(__fastcall *AllocRefTabConstructor)(AllocRefTab *_this);

    typedef EquRefTab *(__fastcall *EquRefTabConstructor)(EquRefTab *_this, int p_poolSize, int p_size);
    typedef StrRefTab *(__fastcall *StrRefTabConstructor)(StrRefTab *_this, int p_poolSize, int p_size);
    typedef LinkRefTab *(__fastcall *LinkRefTabConstructor)(LinkRefTab *_this, int p_poolSize, int p_size);

    typedef LinkSortRefTab *(__fastcall *LinkSortRefTabConstructor)(LinkSortRefTab *_this, int p_poolSize, int p_size);

    typedef ZCounter *(__fastcall *ZCounterConstructor)(ZCounter *_this);

    typedef ZRBTree *(__fastcall *ZRBTreeConstructor)(ZRBTree *_this);
    typedef ZValTree *(__fastcall *ZValTreeConstructor)(ZValTree *_this, int p_poolSize);

    typedef ZConsole *(__fastcall *ZConsoleConstructor)(ZConsole *_this);

    typedef ZSysCom *(__fastcall *ZSysComConstructor)(ZSysCom *_this);

    typedef ZInputAction *(__fastcall *ZInputActionDefCtor)(ZInputAction *_this, ZActionMap *p_actionMap, SActionOverride *p_actionOverride);
    typedef ZActionMap *(__fastcall *ZActionMapConstructor)(ZActionMap *_this, void *_EDX, ZInputActions *p_inputActions, ZActionMapDefinition *p_actionMapDef, char *p_mapName);
    typedef ZInputActions *(__fastcall *ZInputActionsConstructor)(ZInputActions *_this, void *_EDX);
    typedef ZMasterControl *(__fastcall *ZMasterControlConstructor)(ZMasterControl *_this);

    typedef FSZip *(__fastcall *FSZipConstructor)(FSZip *_this);

    static ZConsoleConstructor originalZConsole = 0;
    static RefTab32Constructor originalRefTab32 = 0;
    static StaticRefTabConstructor originalStaticRefTab = 0;
    static AllocRefTabConstructor originalAllocRefTab = 0;
    static ZCounterConstructor originalZCounter = 0;

    static ZSysComConstructor originalZSysCom = 0;
    static ZActionMapConstructor originalZActionMap = 0;
    static ZInputActionsConstructor originalZInputActions = 0;

    // Functions
    RefTab *__fastcall RefTabHook(RefTab *_this, void *_EDX, int p_poolSize, int p_size);
    RefTab32 *__fastcall RefTab32Hook(RefTab32 *_this, void *_EDX);
    StaticRefTab *__fastcall StaticRefTabHook(StaticRefTab *_this, void *_EDX, int p_poolSize, int p_size);
    AllocRefTab *__fastcall AllocRefTabHook(AllocRefTab *_this, void *_EDX);

    EquRefTab *__fastcall EquRefTabHook(EquRefTab *_this, void *_EDX, int p_poolSize, int p_size);
    StrRefTab *__fastcall StrRefTabHook(StrRefTab *_this, void *_EDX, int p_poolSize, int p_size);
    LinkRefTab *__fastcall LinkRefTabHook(LinkRefTab *_this, void *_EDX, int p_poolSize, int p_size);
    LinkSortRefTab *__fastcall LinkSortRefTabHook(LinkSortRefTab *_this, void *_EDX, int p_poolSize, int p_size);

    ZRBTree *__fastcall ZRBTreeConstructorHook(ZRBTree *_this, void *_EDX);
    ZValTree *__fastcall ZValTreeConstructorHook(ZValTree *_this, void *_EDX, int p_poolSize);

    ZConsole *__fastcall ZConsoleConstructorHook(ZConsole *_this, void *_EDX);

    ZSysCom *__fastcall ZSysComConstructorHook(ZSysCom *_this, void *_EDX);

    ZCounter *__fastcall ZCounterConstructorHook(ZCounter *_this, void *_EDX);

    ZInputAction *__fastcall ZInputActionDefCtorHook(ZInputAction *_this, void *_EDX, ZActionMap *p_actionMap, SActionOverride *p_actionOverride);
    ZActionMap *__fastcall ZActionMapConstructorHook(ZActionMap *_this, void *_EDX, ZInputActions *p_inputActions, ZActionMapDefinition *p_actionMapDef, char *p_mapName);
    ZInputActions *__fastcall ZInputActionsConstructorHook(ZInputActions *_this, void *_EDX);
    ZMasterControl *__fastcall ZMasterControlConstructorHook(ZMasterControl *_this, void *_EDX);
    FSZip *__fastcall FSZipConstructorHook(FSZip *_this, void *_EDX);

    void CreateHooks();
    void CreateRenderingHooks();
}

#endif