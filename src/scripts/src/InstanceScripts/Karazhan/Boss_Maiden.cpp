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


//Maiden of Virtue
#define CN_MAIDENOFVIRTUE			16457
#define MAIDENOFVIRTUE_REPENTANCE	29511
#define MAIDENOFVIRTUE_HOLY_GROUND	29512
#define MAIDENOFVIRTUE_HOLY_FIRE	29522
#define MAIDENOFVIRTUE_HOLY_WRATH	32445

class MaidenOfVirtueAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MaidenOfVirtueAI, MoonScriptBossAI);
	MaidenOfVirtueAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
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
	
		//Spells
		AddSpell(MAIDENOFVIRTUE_HOLY_GROUND, Target_Self, 100, 0, 3);
		AddSpell(MAIDENOFVIRTUE_HOLY_FIRE, Target_RandomPlayer, 25, 1, 15, 0, 50);
		AddSpell(MAIDENOFVIRTUE_HOLY_WRATH, Target_RandomPlayer, 25, 0, 20, 0, 100);
		mRepentance = AddSpell(MAIDENOFVIRTUE_REPENTANCE, Target_Self, 25, 0.6f, 30);
		mRepentance->AddEmote("Cast out your corrupt thoughts.", Text_Yell, 9313);
		mRepentance->AddEmote("Your impurity must be cleansed.", Text_Yell, 9208);

		//Emotes
		AddEmote(Event_OnCombatStart, "Your behavior will not be tolerated.", Text_Yell, 9204);
		AddEmote(Event_OnTargetDied, "Ah ah ah...", Text_Yell, 9207);
		AddEmote(Event_OnTargetDied, "This is for the best.", Text_Yell, 9312);
		AddEmote(Event_OnTargetDied, "Impure thoughts lead to profane actions.", Text_Yell, 9311);
		AddEmote(Event_OnDied, "Death comes. Will your conscience be clear?", Text_Yell, 9206);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mRepentance->TriggerCooldown();	//No repentance at the beginning of the fight
		ParentClass::OnCombatStart(pTarget);
	}

	SpellDesc* mRepentance;
};

void SetupBoss_Maiden(ScriptMgr* pScriptMgr)
{
	pScriptMgr->register_creature_script(CN_MAIDENOFVIRTUE, &MaidenOfVirtueAI::Create);
}