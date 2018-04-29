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


//M'uru
#define BOSS_MURU						25960
#define CREATURE_BERSERKER	25798
#define CREATURE_FURY_MAGE	25799
#define CREATURE_VOID_SENTINEL			25772
#define CREATURE_ENTROPIUS				25840
#define MURU_NEGATIVE_ENERGY		46285
#define MURU_DARKNESS				45996
#define MURU_SUMMON_BERSERKER		46037
#define MURU_SUMMON_FURY_MAGE		46038
#define MURU_SUMMON_VOID_SENTINEL	45988

// Muru & Entropius's spells
enum MuruSpells
{
    SPELL_MURU_ENRAGE                = 26662,

    // Muru's spells
    SPELL_NEGATIVE_ENERGY        = 46009, //(this trigger 46008)
    SPELL_DARKNESS                = 45999,
    SPELL_OPEN_ALL_PORTALS        = 46177,
    SPELL_OPEN_PORTAL            = 45977,
    SPELL_OPEN_PORTAL_2            = 45976,
    SPELL_SUMMON_BERSERKER        = 46037,
    SPELL_SUMNON_FURY_MAGE        = 46038,
    SPELL_SUMMON_VOID_SENTINEL    = 45988,
    SPELL_SUMMON_ENTROPIUS        = 46217,

    // Entropius's spells
    SPELL_DARKNESS_E            = 46269,
    SPELL_BLACKHOLE             = 46282,
    SPELL_NEGATIVE_ENERGY_E     = 46284,
    SPELL_ENTROPIUS_SPAWN        = 46223,

    // Shadowsword Berserker's spells
    SPELL_FLURRY                = 46160,
    SPELL_SB_DUAL_WIELD         = 29651,

    // Shadowsword Fury Mage's spells
    SPELL_FEL_FIREBALL          = 46101,
    SPELL_SPELL_FURY            = 46102,

    // Void Sentinel's spells
    SPELL_SHADOW_PULSE          = 46087,
    SPELL_VOID_BLAST            = 46161,

    // Void Spawn's spells
    SPELL_SHADOW_BOLT_VOLLEY    = 46082,

    //Dark Fiend Spells
    SPELL_DARKFIEND_AOE            = 45944,
    SPELL_DARKFIEND_VISUAL        = 45936,
    SPELL_DARKFIEND_SKIN        = 45934,

    //Black Hole Spells
    SPELL_BLACKHOLE_SPAWN        = 46242,
    SPELL_BLACKHOLE_GROW        = 46228
};

enum Creatures
{


    CREATURE_DARK_FIEND         = 25744,

};

float DarkFiends[8][4] =
{
    {1819.9f,    609.80f,    69.74f,    1.94f},
    {1829.39f,   617.89f,    69.73f,    2.61f},
    {1801.98f,   633.62f,    69.74f,    5.71f},
    {1830.88f,   629.99f,    69.73f,    3.52f},
    {1800.38f,   621.41f,    69.74f,    0.22f},
    {1808.3f,   612.45f,    69.73f,    1.02f},
    {1823.9f,   639.69f,    69.74f,    4.12f},
    {1811.85f,   640.46f,    69.73f,    4.97f}
};

float Humanoides[6][5] =
{
    {CREATURE_FURY_MAGE, 1780.16f,    666.83f,    71.19f,    5.21f},
    {CREATURE_FURY_MAGE, 1847.93f,    600.30f,    71.30f,    2.57f},
    {CREATURE_BERSERKER, 1779.97f,    660.64f,    71.19f,    5.28f},
    {CREATURE_BERSERKER, 1786.2f,    661.01f,    71.19f,    4.51f},
    {CREATURE_BERSERKER, 1845.17f,    602.63f,    71.28f,    2.43f},
    {CREATURE_BERSERKER, 1842.91f,    599.93f,    71.23f,    2.44f}
};


enum BossTimers{
    TIMER_DARKNESS                = 0,
    TIMER_HUMANOIDES              = 1,
    TIMER_PHASE                   = 2,
    TIMER_SENTINEL                = 3
};

class MuruAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(MuruAI, MoonScriptBossAI);
    MuruAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
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


        AddSpell(MURU_NEGATIVE_ENERGY, Target_SecondMostHated, 25, 0, 1, 0, 40);
        AddSpell(MURU_DARKNESS, Target_Current, 25, 0, 10, 0, 10);


    }

    void OnCombatStart(Unit* mTarget)
    {
        SetAllowMelee(false);
        //doors
        ParentClass::OnCombatStart(mTarget);

    }

    void OnCombatStop(Unit* mTarget)
    {
        DarkFiend = false;
        Phase = 1;

        EnrageTimer = 600000;
        Timer[TIMER_DARKNESS] = 45000;
        Timer[TIMER_HUMANOIDES] = 10000;
        Timer[TIMER_PHASE] = 2000;
        Timer[TIMER_SENTINEL] = 31500;

        _unit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        ParentClass::OnCombatStop(mTarget);
    }


    void OnLoad()
    {
        spawnplace = 1;
        summoningst = false;
        timmer = 0;
        SetAllowMelee(false);
        _unit->GetAIInterface()->m_canMove = false;
        ParentClass::OnLoad();
    }

    void AIUpdate()
    {
        SetAllowMelee(false);
        timmer++;
        if (summoningst == true && (timmer == 20 || timmer == 60 || timmer == 100))
        {
            SpawnCreature(CREATURE_BERSERKER, 1871, 650, 71, 0, false);
            SpawnCreature(CREATURE_FURY_MAGE, 1871, 650, 71, 0, false);
            SpawnCreature(CREATURE_FURY_MAGE, 1748, 700, 71, 0, false);
            SpawnCreature(CREATURE_BERSERKER, 1748, 700, 71, 0, false);
        }

        if (timmer == 120)
        {
            timmer = 0;
            summoningst = true;
            switch (spawnplace)
            {
            case 1:
                {
                    SpawnCreature(CREATURE_VOID_SENTINEL, 1800, 652, 71, 0, false);
                    ++spawnplace;
                }
                break;
            case 2:
                {
                    SpawnCreature(CREATURE_VOID_SENTINEL, 1798, 605, 71, 0, false);
                    ++spawnplace;
                }
                break;
            case 3:
                {
                    SpawnCreature(CREATURE_VOID_SENTINEL, 1826, 650, 71, 0, false);
                    ++spawnplace;
                }
                break;
            case 4:
                {
                    SpawnCreature(CREATURE_VOID_SENTINEL, 1783, 625, 71, 0, false);
                    ++spawnplace;
                }
                break;
            case 5:
                {
                    SpawnCreature(CREATURE_VOID_SENTINEL, 1816, 595, 71, 0, false);
                    ++spawnplace;
                }
                break;
            case 6:
                {
                    SpawnCreature(CREATURE_VOID_SENTINEL, 1844, 641, 71, 0, false);
                    spawnplace = 1;
                }
                break;

            }
        }

        if (GetHealthPercent() <= 35)
        {
            SpawnCreature(CREATURE_ENTROPIUS, true);
            Despawn(100,0);
        }

        ParentClass::AIUpdate();
    }

    void OnDied(Unit * mKiller)
    {
        ParentClass::OnDied(mKiller);
    };
    //protected:

    int timmer, spawnplace;
    bool summoningst;
    uint32 BlackHoleSummonTimer;

    uint8 Phase;
    uint32 Timer[4];

    uint32 EnrageTimer;

    bool DarkFiend;
};



/////////////////////////////////////////////////////////////
#define BERSERK_FLURRY 46160

class ShadowswordBerserkerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowswordBerserkerAI, MoonScriptCreatureAI);
    ShadowswordBerserkerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(BERSERK_FLURRY, Target_Current, 30, 0, 30);
    }

    void OnCombatStart(Unit* mTarget)
    {
        ParentClass::OnCombatStart(mTarget);
    }

    void OnLoad()
    {
        _unit->GetAIInterface()->m_moveRun = true;
        Creature *Muruu = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1815, 625, 69, BOSS_MURU);
        if (Muruu)
        {
            MoveTo(Muruu);
        }
        ParentClass::OnLoad();
    }

    void AIUpdate()
    {
        ParentClass::AIUpdate();
    }

    void OnDied(Unit * mKiller)
    {
        Despawn(100,0);
        ParentClass::OnDied(mKiller);
    }

protected:
    Creature *Muruu;

};


////////////////////////////////////////////////////////////////////////////////////////
#define FEL_FIREBALL 46101
#define SPELL_FURY 46102

class ShadowswordFuryMageAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowswordFuryMageAI, MoonScriptCreatureAI);

    ShadowswordFuryMageAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(FEL_FIREBALL, Target_RandomPlayer, 100, 2, 2, 4, 40); 
        AddSpell(SPELL_FURY, Target_Self, 30, 0, 50); 
    }

    void OnCombatStart(Unit* mTarget)
    {
        ParentClass::OnCombatStart(mTarget);
    }

    void OnLoad()
    {
        _unit->GetAIInterface()->m_moveRun = true;
        Creature *Muruu = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1815, 625, 69, BOSS_MURU);
        if (Muruu)
        {
            MoveTo(Muruu);
        }
        ParentClass::OnLoad();
    }

    void AIUpdate()
    {
        ParentClass::AIUpdate();
    }

    void OnDied(Unit * mKiller)
    {
        Despawn(100,0);
        ParentClass::OnDied(mKiller);
    }

protected:
    Creature *Muruu;
};


//////////////////////////////////////////////////////////////////////////
#define SHADOW_PULSE 46087
#define WOID_BLAST 46161

class VoidSentinelAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(VoidSentinelAI, MoonScriptCreatureAI);

    VoidSentinelAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(SHADOW_PULSE, Target_Current, 50, 0, 3);
        AddSpell(WOID_BLAST, Target_Current, 20, 1.50f, 15);
    }

    void OnCombatStart(Unit* mTarget)
    {
        ParentClass::OnCombatStart(mTarget);
    }

    void OnLoad()
    {
        _unit->GetAIInterface()->m_moveRun = true;
        Creature *Muruu = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1815, 625, 69, BOSS_MURU);
        if (Muruu)
        {
            MoveTo(Muruu);
        }
        ParentClass::OnLoad();
    }

    void AIUpdate()
    {
        ParentClass::AIUpdate();
    }

    void OnDied(Unit * mKiller)
    {
        Despawn(100,0);
        ParentClass::OnDied(mKiller);
    }


protected:
    Creature *Muruu;
};

class npc_dark_fiendAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(npc_dark_fiendAI, MoonScriptCreatureAI);

    npc_dark_fiendAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)

    {

    }

    uint32 WaitTimer;
    bool InAction;

    void OnCombatStart(Unit* mTarget)
    {

        ParentClass::OnCombatStart(mTarget);
    }


    void OnCombatStop(Unit* mTarget)
    {
        WaitTimer = 2000;
        InAction = false;

        //_unit->SetFlag(UNIT_FLAG_STUNNED);

        //ParentClass::OnCombatStop(mTarget);
    }

    void AIUpdate()
    {

        if (WaitTimer <= mAIUpdateFrequency)
        {
            if (!InAction)
            {
                //_unit->ClearUnitState(UNIT_STATE_STUNNED);
                //DoCastAOE(SPELL_DARKFIEND_SKIN, false);
                //AttackStart(rand, 0, 100, true));
                InAction = true;
                WaitTimer = 500;
            }
            else
            {
                //if (_unit->IsWithinDist(_unit->getVictim(), 5))
                //{
                //   DoCastAOE(SPELL_DARKFIEND_AOE, false);
                //    _unit->DisappearAndDie();
                //}
                WaitTimer = 500;
            }
        } 
        else WaitTimer -= mAIUpdateFrequency;

        ParentClass::AIUpdate();
    }


};




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Entropius
#define ENTROPIUS_NEGATIVE_ENERGY	46289 //SpellID taken from Wowhead
#define ENTROPIUS_NEGATIVE_ENERGY1	46008
#define ENTROPIUS_NEGATIVE_ENERGY2	46285
#define ENTROPIUS_BLACK_HOLE		46242 //SpellID taken from Wowhead

uint32 EnrageTimer = 600000;

class EntropiusAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(EntropiusAI, MoonScriptBossAI);
    EntropiusAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
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


        AddSpell(ENTROPIUS_NEGATIVE_ENERGY1, Target_RandomDestination, 15, 0, 1, 0, 40);
        AddSpell(ENTROPIUS_BLACK_HOLE, Target_RandomDestination, 10, 0, 10, 0, 40);


    }

    void OnCombatStart(Unit* mTarget)
    {
        _unit->CastSpell( _unit, SPELL_NEGATIVE_ENERGY_E, true );//Bfx Need to add new function CastAoeSpell

        ParentClass::OnCombatStart(mTarget);

    }

    void OnCombatStop(Unit* mTarget)
    {
        BlackHoleSummonTimer = 15000;

        _unit->CastSpell( _unit, SPELL_ENTROPIUS_SPAWN, false );
        _unit->CastSpell( _unit, SPELL_NEGATIVE_ENERGY_E, false );//Bfx Need to add new function CastAoeSpell

        ParentClass::OnCombatStop(mTarget);

    }

    void OnLoad()
    {
        ParentClass::OnLoad();
    }

    void AIUpdate()
    {

        if (EnrageTimer < mAIUpdateFrequency && !_unit->HasAura(SPELL_MURU_ENRAGE))
        {
            _unit->CastSpell(_unit, SPELL_MURU_ENRAGE, false);
        } 
        else EnrageTimer -= mAIUpdateFrequency;

        if (BlackHoleSummonTimer <= mAIUpdateFrequency)
        {
            Unit* rand = GetRandomPlayer();
            if (!rand)
                return;

            _unit->CastSpell(rand, SPELL_DARKNESS_E, false);

            rand = GetRandomPlayer();
            if (!rand)
                return;

            rand->CastSpell(rand, SPELL_BLACKHOLE, false);
            BlackHoleSummonTimer = 15000;
        } 
        else BlackHoleSummonTimer -= mAIUpdateFrequency;


        ParentClass::AIUpdate();
    }

    void OnDied(Unit * mKiller)
    {
        //doors
        ParentClass::OnDied(mKiller);
    }

    uint32 BlackHoleSummonTimer;
    uint32 EnrageTimer;
};

void SetupMuru(ScriptMgr* pScriptMgr)
{

    pScriptMgr->register_creature_script(BOSS_MURU, &MuruAI::Create);
    pScriptMgr->register_creature_script(CREATURE_BERSERKER, &ShadowswordBerserkerAI::Create);
    pScriptMgr->register_creature_script(CREATURE_FURY_MAGE, &ShadowswordFuryMageAI::Create);
    pScriptMgr->register_creature_script(CREATURE_VOID_SENTINEL, &VoidSentinelAI::Create);
    pScriptMgr->register_creature_script(CREATURE_ENTROPIUS, &EntropiusAI::Create);
    pScriptMgr->register_creature_script(CREATURE_DARK_FIEND, &npc_dark_fiendAI::Create);
}