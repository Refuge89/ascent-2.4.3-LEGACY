/*
* Sun++ for Ascent MMORPG Server
* Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
* Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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
#include "Steamvault.h"
#include "unit.h"

enum Spells
{
    SPELL_LIGHTNING_CLOUD      = 25033,
    SPELL_LUNG_BURST           = 31481,
    SPELL_ENVELOPING_WINDS     = 31718,
    SPELL_WATER_BOLT_VOLLEY    = 34449,
    H_SPELL_WATER_BOLT_VOLLEY  = 37924
};

class boss_hydromancer_thespia : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(boss_hydromancer_thespia, MoonScriptCreatureAI);
    boss_hydromancer_thespia(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {        
        init();
    }

    uint32 LightningCloud_Timer;
    uint32 LungBurst_Timer;
    uint32 EnvelopingWinds_Timer;

    void init()
    {
        LightningCloud_Timer = 15000;
        LungBurst_Timer = 7000;
        EnvelopingWinds_Timer = 9000;
    }

    void AIUpdate()
    {
        if( LightningCloud_Timer <= mAIUpdateFrequency )
        {
            Unit* Target = GetRandomPlayer();
            if( Target )
            {
                _unit->CastSpell( Target, SPELL_LIGHTNING_CLOUD, true );
            };

            if( IsHeroic())
            if( Target )
            {
                _unit->CastSpell( Target, SPELL_LIGHTNING_CLOUD, true );
            }
            LightningCloud_Timer = 15000+rand()%10000;
        }
        else LightningCloud_Timer -= mAIUpdateFrequency;

        if( LungBurst_Timer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SPELL_LUNG_BURST, true );
            }
            LungBurst_Timer = 7000+rand()%5000;
        }
        else LungBurst_Timer -= mAIUpdateFrequency;

        if (EnvelopingWinds_Timer <= mAIUpdateFrequency)
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SPELL_ENVELOPING_WINDS, true );
            }

            //cast twice in Heroic mode
            if( IsHeroic())
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SPELL_ENVELOPING_WINDS, true );
            }
            EnvelopingWinds_Timer = 10000+rand()%5000;
        } 
        else EnvelopingWinds_Timer -= mAIUpdateFrequency;

        ParentClass::AIUpdate();
    }
};


class mob_coilfang_waterelemental : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(mob_coilfang_waterelemental, MoonScriptCreatureAI);
    mob_coilfang_waterelemental(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {        
        init();
    }

    uint32 WaterBoltVolley_Timer;

    void init()
    {
        WaterBoltVolley_Timer = 3000+rand()%3000;
    }

    void AIUpdate()
    {
        if( WaterBoltVolley_Timer <= mAIUpdateFrequency )
        {
            _unit->CastSpell( _unit, SPELL_WATER_BOLT_VOLLEY, true );
            WaterBoltVolley_Timer = 7000+rand()%5000;
        }
        else WaterBoltVolley_Timer -= mAIUpdateFrequency;

        ParentClass::AIUpdate();
    }
};

void SetupBoss_Hydromancer_thespia(ScriptMgr * mgr)
{
    mgr->register_creature_script(BOSS_HYDROMANCER_THESPIA, &boss_hydromancer_thespia::Create);
    mgr->register_creature_script(CREATURE_COILFANG_WATER_ELEMENTAL, &mob_coilfang_waterelemental::Create);
}