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

//Attumen the Huntsman (and Midnight)
#define CN_MIDNIGHT					16151
#define CN_ATTUMEN					15550
#define ATTUMEN_SHADOW_CLEAVE		29832
#define ATTUMEN_BERSERKER_CHARGE	22886
#define ATTUMEN_INTANGIBLE_PRESENCE	29833

class AttumenTheHuntsmanAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AttumenTheHuntsmanAI, MoonScriptBossAI);
	AttumenTheHuntsmanAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		mMidnight = NULL;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_CHARM ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_FEAR ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_ROOT ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_SLEEP ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_SNARE ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_STUN ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_KNOCKOUT ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_POLYMORPH ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_BANISH ] = 1;

		//All phase spells
		AddSpell(ATTUMEN_SHADOW_CLEAVE, Target_Current, 15, 0, 6, 0, 5, true);
		AddSpell(ATTUMEN_INTANGIBLE_PRESENCE, Target_Current, 15, 0, 12, 0, 5, true);

		//Phase 2 spells
		AddPhaseSpell(2, AddSpell(ATTUMEN_BERSERKER_CHARGE, Target_RandomPlayerNotCurrent, 15, 0, 6, 15, 40, true));

		//Emotes
		AddEmote(Event_OnCombatStart, "Cowards! Wretches!", Text_Yell, 9167);
		AddEmote(Event_OnCombatStart, "Who dares attack the steed of the Huntsman?", Text_Yell, 9298);
		AddEmote(Event_OnCombatStart, "Perhaps you would rather test yourselves against a more formidable opponent!", Text_Yell, 9299);
		AddEmote(Event_OnTargetDied, "It was... inevitable.", Text_Yell, 9169);
		AddEmote(Event_OnTargetDied, "Another trophy to add to my collection!", Text_Yell, 9300);
		AddEmote(Event_OnDied, "Always knew... someday I would become... the hunted.", Text_Yell, 9165);
		AddEmote(Event_OnTaunt, "Such easy sport.", Text_Yell, 9170);
		AddEmote(Event_OnTaunt, "Amatures! Do not think you can best me! I kill for a living.", Text_Yell, 9304);

		AggroNearestUnit(); //Aggro on spawn
	}

	void OnCombatStart(Unit* pTarget)
	{
		mMidnight = (MoonScriptBossAI*)GetNearestCreature(CN_MIDNIGHT);
		ParentClass::OnCombatStart(pTarget);
	}

	void OnCombatStop(Unit* pTarget)
	{
		Despawn(10000);
		ParentClass::OnCombatStop(pTarget);
	}

	void AIUpdate()
	{
		if( GetPhase() == 1 )
		{
			if( mMidnight && mMidnight->IsAlive() && GetHealthPercent() <= 25 && !IsCasting() )
			{
				SetPhase(2);
				SetAllowMelee(false);
				SetAllowSpell(false);
				Emote("Come Midnight, let's disperse this petty rabble!", Text_Yell, 9168);
				mMidnight->SetPhase(2);
				mMidnight->MoveTo(this);
				mMidnight->SetAllowMelee(false);
			}
		}
		ParentClass::AIUpdate();
	}

	MoonScriptBossAI* mMidnight;
};

class MidnightAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MidnightAI, MoonScriptBossAI);
	MidnightAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		mAttumen = NULL;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_CHARM ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_FEAR ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_ROOT ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_SLEEP ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_SNARE ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_STUN ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_KNOCKOUT ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_POLYMORPH ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_BANISH ] = 1;
	}

	void OnCombatStart(Unit* pTarget)
	{
		mAttumen = NULL;
		ParentClass::OnCombatStart(pTarget);
	}

	void OnCombatStop(Unit* pTarget)
	{
		mAttumen = NULL;
		SetAllowMelee(true);
		SetAllowSpell(true);
		ParentClass::OnCombatStop(pTarget);
	}

	void OnTargetDied(Unit* pTarget)
	{
		if( mAttumen && mAttumen->IsAlive() )
		{
			mAttumen->Emote("Well done Midnight!", Text_Yell, 9173);
		}
		ParentClass::OnTargetDied(pTarget);
	}

	void AIUpdate()
	{
		if( GetPhase() == 1 )
		{
			if( !mAttumen && GetHealthPercent() <= 95 && !IsCasting() )
			{
				Emote("Midnight calls for her master!", Text_Emote);
				mAttumen = (MoonScriptBossAI*)SpawnCreature(CN_ATTUMEN);
			}
			else if( mAttumen && mAttumen->IsAlive() && GetHealthPercent() <= 25 && !IsCasting() )
			{
				SetPhase(2);
				MoveTo(mAttumen);
				SetAllowMelee(false);
				mAttumen->SetPhase(2);
				mAttumen->SetAllowMelee(false);
				mAttumen->SetAllowSpell(false);
				mAttumen->Emote("Come Midnight, let's disperse this petty rabble!", Text_Yell, 9168);
			}
		}
		else if( GetPhase() == 2 )
		{
			if( mAttumen && mAttumen->IsAlive() )
			{
				if( GetRange(mAttumen) <= 15 )
				{
					mAttumen->Regenerate();
					mAttumen->SetDisplayId(16040);
					mAttumen->ClearHateList();
					mAttumen->SetAllowMelee(true);
					mAttumen->SetAllowSpell(true);
					mAttumen = NULL;
					Despawn();
				}
				else MoveTo(mAttumen);
			}
		}
		ParentClass::AIUpdate();
	}

	MoonScriptBossAI* mAttumen;
};

void SetupBoss_Attumen(ScriptMgr* pScriptMgr)
{
	pScriptMgr->register_creature_script(CN_ATTUMEN, &AttumenTheHuntsmanAI::Create);
	pScriptMgr->register_creature_script(CN_MIDNIGHT, &MidnightAI::Create);
}