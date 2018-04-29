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

/* TODO Bfx coardinates of mushrooms are wrong*/
 
 
 enum Creatures
 {
 
 CREATURE_MUSHROOM = 17990,
 BOSS_HUNGRAFEN    = 17770,
 
 };
 
 enum Spells
 {
 
 SPELL_FOUL_SPORES     =  31673,
 SPELL_ACID_GEYSER     =  38739,
 SPELL_SPORE_CLOUD     =  34168,
 SPELL_PUTRID_MUSHROOM =  31690,
 SPELL_GROW            =  31698,
 
 };
 
class Boss_hungarfenAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Boss_hungarfenAI, MoonScriptBossAI);
	Boss_hungarfenAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{	
        init();
	}
	
	void init()
	{
	   
	}
	
	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
	};
	
	void OnCombatStop(Unit* pTarget)
	{
	    Mushroom_Timer = 5000;                              // 1 mushroom after 5s, then one per 10s. This should be different in heroic mode
        AcidGeyser_Timer = 10000;
		SetCanMove(true);
		ParentClass::OnCombatStop(pTarget);
	};
	
	void AIUpdate()
	{
		if (GetHealthPercent() <= 20)
		{
			if( GetCanMove() == true )
			{
			    Unit* pTarget = GetCurrentTarget();
			    if( pTarget )
			    {
			      SetCanMove( false );
				  StopMovement();
				  _unit->CastSpell( pTarget, SPELL_FOUL_SPORES, true);
                  Emote( "Casts Foul spores", Text_Emote);
			    }
		    }
			return;
	
	    if( Mushroom_Timer <= mAIUpdateFrequency )
		{
			Unit* pTarget = GetCurrentTarget();
			if( pTarget )
			{
				SpawnCreature(CREATURE_MUSHROOM);
			}
			else
			{
			    SpawnCreature(CREATURE_MUSHROOM);
			}
            Mushroom_Timer = 10000;
			return;
		}
		else Mushroom_Timer -= mAIUpdateFrequency;
		
		if( AcidGeyser_Timer <= mAIUpdateFrequency )
		{
			Unit* pTarget = GetCurrentTarget();
			if( pTarget )
			{
				_unit->CastSpell( pTarget, SPELL_ACID_GEYSER, true );
			}
            AcidGeyser_Timer = 10000+rand()%7500;
			return;
		}
		else AcidGeyser_Timer  -= mAIUpdateFrequency;
		
		ParentClass::AIUpdate();
    }
	
}
	bool Root;
    uint32 Mushroom_Timer;
    uint32 AcidGeyser_Timer;
	
};


class Creature_underbog_mushroomAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Creature_underbog_mushroomAI, MoonScriptCreatureAI);
	Creature_underbog_mushroomAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
	
	}
	
	void OnCombatStart(Unit* pTarget)
	{
	
	    ParentClass::OnCombatStart(pTarget);
	};
	
	void OnCombatStop(Unit* pTarget)
	{
		Stop = false;
        Grow_Timer = 0;
        Shrink_Timer = 20000;

        _unit->CastSpell(_unit, SPELL_PUTRID_MUSHROOM, true);
        _unit->CastSpell(_unit, SPELL_SPORE_CLOUD, true);

		//ParentClass::OnCombatStop(pTarget);

	};
	
	void AIUpdate()
	{
		if (Stop)
        return;
		
	    if( Grow_Timer <= mAIUpdateFrequency )
		{
			_unit->CastSpell( _unit, SPELL_GROW, true );
            Grow_Timer = 3000;
		}
		else Grow_Timer -= mAIUpdateFrequency;
		
		if( Shrink_Timer<= mAIUpdateFrequency )
		{
				RemoveAura(SPELL_GROW);
				Stop = true;
		}
		else Shrink_Timer -= mAIUpdateFrequency;
		
		ParentClass::AIUpdate();
		
    }
	
	bool Stop;
    uint32 Grow_Timer;
    uint32 Shrink_Timer;

	
};

void SetupBoss_hungarfen(ScriptMgr* pScriptMgr)
{
    pScriptMgr->register_creature_script(BOSS_HUNGRAFEN,&Boss_hungarfenAI::Create);
	pScriptMgr->register_creature_script(CREATURE_MUSHROOM,&Creature_underbog_mushroomAI::Create);
};