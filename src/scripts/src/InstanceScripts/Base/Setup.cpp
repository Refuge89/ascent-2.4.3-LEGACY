#include "StdAfx.h"
#include "Setup.h"
//#define SKIP_ALLOCATOR_SHARING 1
#include <ScriptSetup.h>

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_MISC;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)	// Comment any script to disable it
{
    SetupDeadmines(mgr);
	SetupAlteracValleyBattleground(mgr);
	SetupSunwellPlateau(mgr);
    SetupSethek_halls(mgr);
    //Karazhan
    SetupKarazhan(mgr);
    SetupBoss_Moroes(mgr);
    SetupBoss_Maiden(mgr);
    SetupNightbane(mgr);
    SetupBossPrince_Malchezaar(mgr);
    SetupBossNetherSpite(mgr);
    SetupBoss_Attumen(mgr);
    SetupBoss_Curator(mgr);
    SetupOpera(mgr);
    SetupBoss_Illhoof(mgr);

    //Bfx Boss Script file Reform
    //Sunwell
    SetupMuru(mgr);
    SetupKalecgos(mgr);
    SetupSunwellPlateauTrash(mgr);
    SetupBoss_Twins(mgr);

    //UnderBog
    SetupBoss_hungarfen(mgr);

    //Steamvault
    SetupBoss_Hydromancer_thespia(mgr);

    //Gruuls Lair
    SetupBoss_Gruul(mgr);
    SetupBoss_Maulgar(mgr);
    SetupInstanceGruuls_Lair(mgr);

    //Magisters Terrace
    SetupSelinFireheart(mgr);
    SetupDelrissa(mgr);
    SetupVexallus(mgr);
    SetupKaelthas(mgr);
    SetupMagistersTerrace(mgr);

}

#ifdef WIN32

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    return TRUE;
}

#endif
