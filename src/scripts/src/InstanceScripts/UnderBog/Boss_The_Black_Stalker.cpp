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
 
 enum Creatures
 {
 BOSS_BLACK_STALKER     = 17990,
 CREATURE_SPORE_STRIDER = 22299,
 };
 
 enum Spells
 {
 
SPELL_LEVITATE             =   31704,
SPELL_SUSPENSION           =   31719,
SPELL_LEVITATION_PULSE     =   31701,
SPELL_MAGNETIC_PULL        =   31705,
SPELL_CHAIN_LIGHTNING      =   31717,
SPELL_STATIC_CHARGE        =   31715,
SPELL_SUMMON_SPORE_STRIDER =   38755,
 
 };
 
class Boss_hungarfenAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Boss_hungarfenAI, MoonScriptBossAI);
	Boss_hungarfenAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{

	
	}
	
	void OnCombatStart(Unit* pTarget)
	{

		ParentClass::OnCombatStart(pTarget);
	};
	
	void OnCombatStop(Unit* pTarget)
	{
		Levitate_Timer = 12000;
        ChainLightning_Timer = 6000;
        StaticCharge_Timer = 10000;
        SporeStriders_Timer = 10000+rand()%5000;
        check_Timer = 5000;
        LevitatedTarget = 0;
        LevitatedTarget_Timer = 0;
        Striders.clear();
		ParentClass::OnCombatStop(pTarget);
	};
	
	void AIUpdate()
	{
		if (GetHealthPercent() <= 20)
		{
			if (!Root)
			{
			  _unit->CastSpell( _unit, SPELL_FOUL_SPORES, true );
			  Root = true;
			}
		}
	
	    if( Mushroom_Timer <= mAIUpdateFrequency )
		{
			Unit* pTarget = GetRandomPlayer();
			if( pTarget )
			{
				SpawnCreature(CREATURE_MUSHROOM, 1800, 652, 71, 0, false);
			}
			else
			{
			    SpawnCreature(CREATURE_MUSHROOM, 1800, 652, 71, 0, false);
			}
            Mushroom_Timer = 10000;
			return;
		}
		else Mushroom_Timer -= mAIUpdateFrequency;
		
		if( AcidGeyser_Timer <= mAIUpdateFrequency )
		{
			Unit* pTarget = GetRandomPlayer();
			if( pTarget )
			{
				_unit->CastSpell( pTarget, SPELL_ACID_GEYSER, true );
			}
            AcidGeyser_Timer = 10000+rand()%7500;
			return;
		}
		else Mushroom_Timer -= mAIUpdateFrequency;
		
		ParentClass::AIUpdate();
		
		

    }
	
	    uint32 SporeStriders_Timer;
        uint32 Levitate_Timer;
        uint32 ChainLightning_Timer;
        uint32 StaticCharge_Timer;
        uint64 LevitatedTarget;
        uint32 LevitatedTarget_Timer;
        bool InAir;
        uint32 check_Timer;
        std::list<uint64> Striders;
	
};