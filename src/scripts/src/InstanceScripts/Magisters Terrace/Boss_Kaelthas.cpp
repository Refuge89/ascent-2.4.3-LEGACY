/*
 * Sun++ Scripts  for SummitEmu MMORPG Server 
 * Copyright (C) 2005-2007 Arcemu Team 
 * Copyright (C) 2007 Moon++ Team <http://www.moonplusplus.com/>
 * Copyright (C) 2007 Sun++ Team <http://www.Sunscripting.com/>
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

/*Corrupt Kael'thas

/*** Spells ***/

// Phase 1 spells
#define SPELL_FIREBALL_NORMAL 44189 // Deals 2700-3300 damage at current target
#define SPELL_FIREBALL_HEROIC 46164 // 4950-6050

#define SPELL_PHOENIX 44194 // Summons a phoenix (Doesn't work?)
#define SPELL_PHOENIX_BURN 44197 // A spell Phoenix uses to damage everything around
#define SPELL_REBIRTH_DMG 44196 // DMG if a Phoenix rebirth happen

#define SPELL_FLAMESTRIKE1_NORMAL 44190 // Damage part
#define SPELL_FLAMESTRIKE1_HEROIC 46163 // Heroic damage part
#define SPELL_FLAMESTRIKE2 44191 // Flamestrike indicator before the damage
#define SPELL_FLAMESTRIKE3 44192 // Summons the trigger + animation (projectile)

#define SPELL_SHOCK_BARRIER 46165 // Heroic only; 10k damage shield, followed by Pyroblast
#define SPELL_PYROBLAST 36819 // Heroic only; 45-55k fire damage

// Phase 2 spells
#define SPELL_GRAVITY_LAPSE_INITIAL 44224 // Cast at the beginning of every Gravity Lapse
#define SPELL_GRAVITY_LAPSE_CHANNEL 44251 // Channeled; blue beam animation to every enemy in range
#define SPELL_TELEPORT_CENTER 44218 // Should teleport people to the center. Requires DB entry in spell_target_position.
#define SPELL_GRAVITY_LAPSE_FLY 44227 // Hastens flyspeed and allows flying for 1 minute. For some reason removes 44226.
#define SPELL_GRAVITY_LAPSE_DOT 44226 // Knocks up in the air and applies a 300 DPS DoT.
#define SPELL_ARCANE_SPHERE_PASSIVE 44263 // Passive auras on Arcane Spheres
#define SPELL_POWER_FEEDBACK 44233 // Stuns him, making him take 50% more damage for 10 seconds. Cast after Gravity Lapse

#define BOSS_Kaelthas 24664
/*** Creatures ***/
#define CREATURE_PHOENIX 24674
#define CREATURE_PHOENIX_EGG 24675
#define CREATURE_ARCANE_SPHERE 24708
#define CREATURE_FELKAEL_FLAMESTRIKE 24666

class KaelThasMTAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KaelThasMTAI, MoonScriptBossAI);
	KaelThasMTAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
	  
       AddPhaseSpell(1, AddSpell(SPELL_FIREBALL_NORMAL, Target_RandomPlayer, 40, 2, 6, 0, 40));

	   Phases = 3;

	}
	
	uint32 Phases; 

};

class mob_felkael_flamestrikeAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(mob_felkael_flamestrikeAI, MoonScriptCreatureAI);
	mob_felkael_flamestrikeAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
	
};

void SetupKaelthas(ScriptMgr* pScriptMgr) 
{
	pScriptMgr->register_creature_script(BOSS_Kaelthas, &KaelThasMTAI::Create);
	pScriptMgr->register_creature_script(CREATURE_FELKAEL_FLAMESTRIKE, &mob_felkael_flamestrikeAI::Create);
}