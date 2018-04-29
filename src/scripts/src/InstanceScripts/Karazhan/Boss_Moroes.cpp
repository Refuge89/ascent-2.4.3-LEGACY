/*
 * Sun++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Sun++ Team <http://www.Sunscripting.info/>
 * Copyright (C) 2011-2012 Tbc-Ascent <http://www.bitbucket.org/bloodfangx/wow-tbc-ascent.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "../../InstanceScripts/Base/setup.h"
#include "../../InstanceScripts/Base/base.h"


enum creatures
{

BOSS_MOROES = 15687,

};

enum Spells
{

MOROES_GOUGE   = 28456,
MOROES_VANISH  = 29448,
MOROES_BLIND   = 34654,
MOROES_ENRAGE  = 37023,
MOROES_GARROTE = 37066,


};

class MoroesAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MoroesAI, MoonScriptBossAI);
	MoroesAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
	    _unit->MechanicsDispels[ DISPEL_MECHANIC_CHARM ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_FEAR ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_ROOT ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_SLEEP ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_SNARE ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_STUN ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_KNOCKOUT ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_POLYMORPH ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_BANISH ] = 1;
		
		//Initialize timers
		mVanishTimer = mGarroteTimer = INVALIDATE_TIMER;

		//Phase 1 spells
		AddPhaseSpell(1, AddSpell(MOROES_GOUGE, Target_Current, 20, 0, 10, 0, 5));
		AddPhaseSpell(1, AddSpell(MOROES_BLIND, Target_ClosestPlayerNotCurrent, 20, 0, 10, 0, 10, true));
		mVanish = AddSpell(MOROES_VANISH, Target_Self, 0, 12, 0);
		mVanish->AddEmote("Now, where was I? Oh yes...", Text_Yell, 9215);
		mVanish->AddEmote("You rang?", Text_Yell, 9316);
		mEnrage = AddSpell(MOROES_ENRAGE, Target_Self, 0, 0, 0);

		//Phase 2 spells
		mGarrote = AddSpell(MOROES_GARROTE, Target_RandomPlayer, 0, 0, 0);

		//Emotes
		AddEmote(Event_OnCombatStart, "Hm, unannounced visitors. Preparations must be made...", Text_Yell, 9211);
		AddEmote(Event_OnDied, "How terribly clumsy of me...", Text_Yell, 9213);
		AddEmote(Event_OnTargetDied, "One more for dinner this evening.", Text_Yell, 9214);
		AddEmote(Event_OnTargetDied, "Time... Never enough time.", Text_Yell, 9314);
		AddEmote(Event_OnTargetDied, "I've gone and made a mess.", Text_Yell, 9315);
	}

	void OnCombatStart(Unit *pTarget)
	{
		mEnrage->mEnabled = true;
		mVanishTimer = AddTimer(35000);	//First vanish after 35sec
		ParentClass::OnCombatStart(pTarget);
	}

	void OnDied(Unit* pKiller)
	{
		RemoveAuraOnPlayers(MOROES_GARROTE);
		ParentClass::OnDied(pKiller);
	}

	void AIUpdate()
	{
		if( GetPhase() == 1 )
		{
			if( mEnrage->mEnabled && GetHealthPercent() <= 30 && !IsCasting() )
			{
				CastSpell(mEnrage);
				mEnrage->mEnabled = false;
			}
			else if( IsTimerFinished(mVanishTimer) && !IsCasting() )
			{
				SetPhase(2, mVanish);
				mGarroteTimer = AddTimer(12000);
				ResetTimer(mVanishTimer, 35000);
			}
		}
		else if( GetPhase() == 2 )
		{
			if( IsTimerFinished(mGarroteTimer) && !IsCasting() )
			{
				SetPhase(1, mGarrote);
				RemoveAura(MOROES_VANISH);
				RemoveTimer(mGarroteTimer);
			}
		}
		ParentClass::AIUpdate();
	}

	SpellDesc*	mVanish;
	SpellDesc*	mGarrote;
	SpellDesc*	mEnrage;
	int32		mVanishTimer, mGarroteTimer;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Lord Robin Daris//

#define CN_DARIS                        19876
#define DARIS_MORTAL_STRIKE             29572
#define DARIS_WHIRLWIND                 15589
#define DARIS_WHIRLWINDTWO              29573
#define DARIS_HAMSTRING                 9080


class LordRobinDarisAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LordRobinDarisAI, MoonScriptCreatureAI);
	LordRobinDarisAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(DARIS_MORTAL_STRIKE, Target_Current, 100, 0, 15, 0, 7);
		AddSpell(DARIS_WHIRLWIND, Target_Current, 100, 0, 15, 0, 15);
		AddSpell(DARIS_WHIRLWINDTWO , Target_Current, 100, 0, 15, 0, 15);
		AddSpell(DARIS_HAMSTRING, Target_Current, 100, 0, 15, 0, 20);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Baroness Dorothea Millstipe//

#define CN_DOROTHEA                        19875
#define DOROTHEA_MIND_FLAY                 29570
#define DOROTHEA_MANA_BURN                 29405
#define DOROTHEA_SHADOW_WORD_PAIN          29386



class BaronessDorotheaMillstipeAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BaronessDorotheaMillstipeAI, MoonScriptCreatureAI);
	BaronessDorotheaMillstipeAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(DOROTHEA_MIND_FLAY, Target_RandomPlayer, 100, 0, 15, 0, 10);
		AddSpell(DOROTHEA_MANA_BURN, Target_RandomPlayer, 100, 0, 15, 0, 20);
		AddSpell(DOROTHEA_SHADOW_WORD_PAIN , Target_RandomPlayer, 100, 0, 15, 0, 15);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Baron Rafe Dreuger//

#define CN_BARON_RAFE                      19874
#define BARON_HAND_OF_JUSTICE              13005
#define BARON_SEAL_OF_COMMAND              29385
#define BARON_JUSTICE_OF_COMMAND           29386



class BaronRafeDreugerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BaronRafeDreugerAI, MoonScriptCreatureAI);
	BaronRafeDreugerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(BARON_HAND_OF_JUSTICE, Target_Current, 100, 0, 15, 0, 14);
		AddSpell(BARON_SEAL_OF_COMMAND, Target_Self, 100, 0, 15, 0, 10);
		AddSpell(BARON_JUSTICE_OF_COMMAND , Target_RandomPlayer, 100, 0, 15, 0, 20);
	}
};




void SetupBoss_Moroes(ScriptMgr* pScriptMgr)
{
	pScriptMgr->register_creature_script(BOSS_MOROES, &MoroesAI::Create);
	pScriptMgr->register_creature_script(CN_DARIS, &LordRobinDarisAI::Create);
	pScriptMgr->register_creature_script(CN_DOROTHEA, &BaronessDorotheaMillstipeAI::Create);
	pScriptMgr->register_creature_script(CN_BARON_RAFE, &BaronRafeDreugerAI::Create);
}
