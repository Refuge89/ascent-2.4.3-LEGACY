#include "StdAfx.h"
#include "../../InstanceScripts/Base/setup.h"
#include "../../InstanceScripts/Base/base.h"

//Priestess Delrissa
#define BOSS_Priestess_Delrissa 24560

#define PRIESTESS_DELRISSA_DISPEL_MAGIC 27609
#define PRIESTESS_DELRISSA_FLASH_HEAL 17843
#define PRIESTESS_DELRISSA_SHADOWWORD_PAIN 15654
#define PRIESTESS_DELRISSA_POWERWORD_SHIELD 44291
#define PRIESTESS_DELRISSA_RENEW 44174

class Priestess_DelrissaAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(Priestess_DelrissaAI, MoonScriptBossAI);
		Priestess_DelrissaAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(PRIESTESS_DELRISSA_DISPEL_MAGIC, Target_RandomFriendly, 35, 0, 5, 0, 30);
			AddSpell(PRIESTESS_DELRISSA_FLASH_HEAL, Target_RandomFriendly, 40, 1.5, 7, 0, 40);
			AddSpell(PRIESTESS_DELRISSA_SHADOWWORD_PAIN, Target_RandomPlayer, 45, 0, 18, 0, 30);
			AddSpell(PRIESTESS_DELRISSA_POWERWORD_SHIELD, Target_RandomFriendly, 32, 0, 15, 0, 40);
			AddSpell(PRIESTESS_DELRISSA_RENEW, Target_RandomFriendly, 30, 0, 18, 0, 40);

			AddEmote(Event_OnDied, "Not what I had... planned...", Text_Yell, 12397);

			mClearHateList = AddTimer(15000);
			mKilledPlayers = 0;
		};

		void OnCombatStart(Unit* pTarget)
		{
			Emote("Annihilate them!", Text_Yell, 12395);
			//AggroRandomUnit();	// Want to aggro random unit ? Set it instead of calling premade
			// method that in this case recursively loops this procedure

			ParentClass::OnCombatStart(pTarget);
		};

		void OnTargetDied(Unit* pTarget)
		{
			if(!pTarget || !pTarget->IsPlayer())
				return;

			++mKilledPlayers;

			if(mKilledPlayers == 1)
				Emote("I call that a good start.", Text_Yell, 12405);
			else if(mKilledPlayers == 2)
				Emote("I could have sworn there were more of you.", Text_Yell, 12407);
			else if(mKilledPlayers == 3)
				Emote("Not really much of a group, anymore, is it?", Text_Yell, 12409);
			else if(mKilledPlayers == 4)
				Emote("One is such a lonely number.", Text_Yell, 12410);
			else if(mKilledPlayers == 5)
				Emote("It's been a kick, really.", Text_Yell, 12411);

			ParentClass::OnTargetDied(pTarget);
		};

		void OnCombatStop(Unit* pTarget)
		{
			Emote("It's been a kick, really.", Text_Yell, 12411);
			mKilledPlayers = 0;

			ParentClass::OnCombatStop(pTarget);
		};

		void AIUpdate()
		{
			if(IsTimerFinished(mClearHateList))
			{
				ClearHateList();
				AggroRandomUnit();
				ResetTimer(mClearHateList, 15000);
			};

			ParentClass::AIUpdate();
		};

	protected:
		uint8		mKilledPlayers;
		int32		mClearHateList;
};

//Kagani Nightstrike
#define CN_KaganiNightstrike 24557

#define KAGANI_NIGHTSTRIKE_Eviscerate 46189
#define KAGANI_NIGHTSTRIKE_KidneyShot 27615
#define KAGANI_NIGHTSTRIKE_Gouge	  12540

class Kagani_NightstrikeAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(Kagani_NightstrikeAI, MoonScriptBossAI);
		Kagani_NightstrikeAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(KAGANI_NIGHTSTRIKE_KidneyShot, Target_Current, 80, 0, 25, 0, 30);
			AddSpell(KAGANI_NIGHTSTRIKE_Gouge, Target_ClosestPlayer, 20, 0, 18, 0, 30);
			AddSpell(KAGANI_NIGHTSTRIKE_Eviscerate, Target_Current, 8, 0, 45, 0, 30);
		}

};

//Ellrys Duskhallow
#define CN_EllrysDuskhallow 14558

#define EllrysDuskhallow_Immolate 44267
#define EllrysDuskhallow_ShadowBolt 12471
#define EllrysDuskhallow_CurseofAgony 14875
#define EllrysDuskhallow_Fear 38595

class Ellrys_DuskhallowAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(Ellrys_DuskhallowAI, MoonScriptBossAI);
		Ellrys_DuskhallowAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(EllrysDuskhallow_Immolate, Target_Current, 75, 2, 15, 0, 30);
			AddSpell(EllrysDuskhallow_ShadowBolt, Target_RandomPlayer, 75, 3, 5, 4, 40);
			AddSpell(EllrysDuskhallow_CurseofAgony, Target_RandomPlayer, 75, 0, 4, 0, 30);
			AddSpell(EllrysDuskhallow_Fear, Target_RandomPlayer, 75, 1.5, 9, 0, 20);
		}

};

//Eramas Brightblaze
#define CN_EramasBrightblaze 24554

#define ERAMAS_BRIGHTBLAZE_KNOCKDOWN 11428
#define ERAMAS_BRIGHTBLAZE_SNAP_KICK 46182

class Eramas_BrightblazeAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(Eramas_BrightblazeAI, MoonScriptBossAI);
		Eramas_BrightblazeAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(ERAMAS_BRIGHTBLAZE_KNOCKDOWN, Target_Current, 25, 0, 5, 0, 5);
			AddSpell(ERAMAS_BRIGHTBLAZE_SNAP_KICK, Target_SecondMostHated, 40, 0, 2, 0, 5);
		}

};

//Yazzai
#define CN_YAZZAI 24561

#define YAZZAI_POLYMORPH 13323
#define YAZZAI_ICE_BLOCK 27619
#define YAZZAI_BLIZZARD 44178
#define YAZZAI_CONE_OF_COLD 38384
#define YAZZAI_FROSTBOLT 15530

class YazzaiAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(YazzaiAI, MoonScriptBossAI);
		YazzaiAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(YAZZAI_POLYMORPH, Target_RandomPlayer, 30, 1.5, 16, 0, 30);
			AddSpell(YAZZAI_ICE_BLOCK, Target_Self, 20, 0, 300, 0, 1);
			AddSpell(YAZZAI_BLIZZARD, Target_RandomPlayer, 25, 0, 20, 0, 30);
			AddSpell(YAZZAI_CONE_OF_COLD, Target_Self, 10, 0, 19, 0, 1);
			AddSpell(YAZZAI_FROSTBOLT, Target_RandomPlayer, 80, 3, 14, 0, 40);
		}

};

//Warlord Salaris
#define CN_WARLORD_SALARIS 24559

#define WARLORD_SALARIS_INTERCEPT 27577
#define WARLORD_SALARIS_DISARM 27581
#define WARLORD_SALARIS_PIERCING_HOWL 23600
#define WARLORD_SALARIS_FRIGHTENING_SHOUT 19134
#define WARLORD_SALARIS_HAMSTRING 27584
//#define WARLORD_SALARIS_BATTLE_SHOUT 27578
#define WARLORD_SALARIS_MORTAL_STRIKE 44268

class Warlord_SalarisAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(Warlord_SalarisAI, MoonScriptBossAI);
		Warlord_SalarisAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			//AddSpell(uint32 pSpellId, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange, float pMaxRange
			AddSpell(WARLORD_SALARIS_INTERCEPT, Target_RandomPlayer , 25, 0, 8, 8, 25);
			AddSpell(WARLORD_SALARIS_DISARM, Target_Current, 100, 0, 60, 0, 5);
			AddSpell(WARLORD_SALARIS_PIERCING_HOWL, Target_Self, 22, 0, 17, 0, 1);
			AddSpell(WARLORD_SALARIS_FRIGHTENING_SHOUT, Target_RandomPlayer, 30, 0, 9, 0, 10);
			AddSpell(WARLORD_SALARIS_HAMSTRING, Target_ClosestPlayer, 10, 0, 20, 0, 5);
			AddSpell(WARLORD_SALARIS_MORTAL_STRIKE, Target_Current, 100, 0, 6, 0, 5);
		}

};

//Geraxxas
#define CN_GARAXXAS 24555

#define GARAXXAS_AIMED_SHOT 44271
#define GARAXXAS_SHOOT 15620
#define GARAXXAS_CONCUSSIV_SHOT 27634
#define GARAXXAS_MULTI_SHOT 44285
#define GARAXXAS_WING_CLIP 44286


class GaraxxasAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(GaraxxasAI, MoonScriptBossAI);
		GaraxxasAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddSpell(GARAXXAS_AIMED_SHOT, Target_RandomPlayer, 90, 3, 6, 5, 35);
			AddSpell(GARAXXAS_SHOOT, Target_RandomPlayer, 90, 2.5, 5, 5, 30);
			AddSpell(GARAXXAS_CONCUSSIV_SHOT, Target_RandomPlayer, 40, 0, 8, 5, 35);
			AddSpell(GARAXXAS_MULTI_SHOT, Target_RandomPlayer, 25, 0, 12, 5, 30);
			AddSpell(GARAXXAS_WING_CLIP, Target_Current, 30, 0, 9, 0, 5);
		}

};

//Apoko
#define CN_APOKO 24553

#define APOKO_FROST_SHOCK 21401
#define APOKO_LESSER_HEALING_WAVE 44256
#define APOKO_PURGE 27626

class ApokoAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(ApokoAI, MoonScriptCreatureAI);
		ApokoAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(APOKO_FROST_SHOCK, Target_RandomPlayer, 40, 0, 8, 0, 20);
			AddSpell(APOKO_LESSER_HEALING_WAVE, Target_RandomFriendly, 50, 1.5, 10, 0, 40);
			AddSpell(APOKO_PURGE, Target_RandomUnit, 20, 0, 40, 0, 30);
		}

};

//Zelfan
#define CN_ZELFAN 24556

#define ZELFAN_GOBLIN_DRAGON_GUN 44272
#define ZELFAN_HIGH_EXPLOSIV_SHEEP 44276
#define ZELFAN_ROCKET_LAUNCH 44137

class ZelfanAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(ZelfanAI, MoonScriptCreatureAI);
		ZelfanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(ZELFAN_GOBLIN_DRAGON_GUN, Target_Current, 90, 0, 15, 0, 5);
			AddSpell(ZELFAN_HIGH_EXPLOSIV_SHEEP, Target_Self, 90, 2, 80);
			AddSpell(ZELFAN_ROCKET_LAUNCH, Target_RandomPlayer, 99, 3.5, 60, 0, 45);
		}

};

void SetupDelrissa(ScriptMgr* pScriptMgr) 
{
	pScriptMgr->register_creature_script(CN_KaganiNightstrike, &Kagani_NightstrikeAI::Create);
	pScriptMgr->register_creature_script(CN_EllrysDuskhallow, &Ellrys_DuskhallowAI::Create);
	pScriptMgr->register_creature_script(CN_EramasBrightblaze, &Eramas_BrightblazeAI::Create);
	pScriptMgr->register_creature_script(BOSS_Priestess_Delrissa, &Priestess_DelrissaAI::Create);
	pScriptMgr->register_creature_script(CN_YAZZAI, &YazzaiAI::Create);
	pScriptMgr->register_creature_script(CN_WARLORD_SALARIS, &Warlord_SalarisAI::Create);
	pScriptMgr->register_creature_script(CN_GARAXXAS, &GaraxxasAI::Create);
	pScriptMgr->register_creature_script(CN_APOKO, &ApokoAI::Create);
	pScriptMgr->register_creature_script(CN_ZELFAN, &ZelfanAI::Create);
}