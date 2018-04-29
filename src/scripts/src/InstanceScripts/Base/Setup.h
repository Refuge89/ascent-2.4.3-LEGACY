#include <string>
#ifndef INSTANCE_SCRIPTS_SETUP_H
#define INSTANCE_SCRIPTS_SETUP_H

void SetupDeadmines(ScriptMgr * mgr);
void SetupAlteracValleyBattleground(ScriptMgr * mgr);
void SetupSethek_halls(ScriptMgr * mgr);
void SetupKarazhan(ScriptMgr * mgr);

//Bfx Boss Script file Reform
//Sunwell
void SetupMuru(ScriptMgr * mgr);
void SetupBoss_Twins(ScriptMgr * mgr);
void SetupSunwellPlateauTrash(ScriptMgr * mgr);
void SetupKalecgos(ScriptMgr * mgr);
void SetupSunwellPlateau(ScriptMgr * mgr);
//The Underbog
void SetupBoss_hungarfen(ScriptMgr * mgr);
//Steamvault
void SetupBoss_Hydromancer_thespia(ScriptMgr * mgr);
//Magisters Terrace
void SetupSelinFireheart(ScriptMgr * mgr);
void SetupDelrissa(ScriptMgr * mgr);
void SetupVexallus(ScriptMgr * mgr);
void SetupKaelthas(ScriptMgr * mgr);
void SetupMagistersTerrace(ScriptMgr * mgr);
//gruuls lair
void SetupBoss_Gruul(ScriptMgr * mgr);
void SetupBoss_Maulgar(ScriptMgr * mgr);
void SetupInstanceGruuls_Lair(ScriptMgr * mgr);
//Karazhan
void SetupBoss_Moroes(ScriptMgr * mgr);
void SetupBoss_Maiden(ScriptMgr * mgr);
void SetupNightbane(ScriptMgr * mgr);
void SetupBossPrince_Malchezaar(ScriptMgr * mgr);
void SetupBossNetherSpite(ScriptMgr * mgr);
void SetupBoss_Attumen(ScriptMgr * mgr);
void SetupBoss_Curator(ScriptMgr * mgr);
void SetupOpera(ScriptMgr * mgr);
void SetupBoss_Illhoof(ScriptMgr * mgr);



struct SP_AI_Spell{
	SpellEntry *info; // spell info
	char targettype; // 0-self , 1-attaking target, ....
	bool instant; // does it is instant or not?
	float perctrigger; // % of the cast of this spell in a total of 100% of the attacks
	int attackstoptimer; // stop the creature from attacking
	float maxdist2cast;		// max dist from caster to victim to perform cast (dist from caster <= maxdist2cast)
	int soundid; // sound id in DBC
	std::string speech; // text displaied when spell was casted
	uint32 cooldown;	// spell cooldown
	uint32 casttime;	// "time" left to cast spell
	float mindist2cast;		// min dist from caster to victim to perform cast (dist from caster >= mindist2cast)
	int minhp2cast;			// min hp amount of victim to perform cast on it (health >= minhp2cast)
	int maxhp2cast;			// max hp amount of victim to perform cast on it (health <= maxhp2cast)
};

enum
{
	TARGET_SELF,
	TARGET_VARIOUS,
	TARGET_ATTACKING,
	TARGET_DESTINATION,
	TARGET_SOURCE,
	TARGET_RANDOM_SINGLE,
	TARGET_RANDOM_FRIEND,	// doesn't work yet
	TARGET_RANDOM_DESTINATION,

	//.....add
};

#endif
