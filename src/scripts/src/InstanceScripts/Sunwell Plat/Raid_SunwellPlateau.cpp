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

/*
//Kil'Jaeden sound IDs saved for future use
//Some of them are used anytime during the raid progression in the instance 
//Some others are used in the actual Kil'Jaeden encounter

12527 Sunwell Plateau - Kil Jaeden - "Spawning"
12495 Sunwell Plateau - Kil Jaeden - "All my plans have led to this"
12496 Sunwell Plateau - Kil Jaeden - "Stay on task, do not waste time"
12497 Sunwell Plateau - Kil Jaeden - "I've waited long enough!"
12498 Sunwell Plateau - Kil Jaeden - "Fail me, and suffer for eternity!"
12499 Sunwell Plateau - Kil Jaeden - "Drain the girl, drain her power, untill there is nothing but an ...something... shell"
12500 Sunwell Plateau - Kil Jaeden - Very long thing, basiclly he tells us that he will take control over the Burning Legion.
12501 Sunwell Plateau - Kil Jaeden - "Another step towards destruction!"
*/

enum SunwellSounds
{

    KALEC_HUMAN_ENGAGE_SOUND = 12430,
    KALEC_HUMAN_LOW_HEALTH_SOUND = 12430,
    KALEC_HUMAN_MID_HEALTH_SOUND= 12430,

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Brutallus
//#define CREATURE_Blaze 23259 should spawn when he casts burn on a random member at there location at least it does on trinitycore temp spawn for like 10-20 seconds

enum Ecreatures
{

    BOSS_BRUTALLUS        = 24882,
    CREATURE_FELMYST    = 25038, 


};

enum Espells
{
    AURA_SUNWELL_RADIANCE                        = 45769,
    BRUTALLUS_METEOR_SLASH = 45150,
    BRUTALLUS_BURN         = 45141,
    BRUTALLUS_STOMP        = 45185,
    BRUTALLUS_BERSERK      = 26662,
    SPELL_DUAL_WIELD       = 42459,

};

class BrutallusAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(BrutallusAI, MoonScriptBossAI);
    BrutallusAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
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

        Init();

        //Emotes
        AddEmote( Event_OnTaunt, "Bring the fight to me!", Text_Yell, 12467 );
        AddEmote( Event_OnTaunt, "Another day, another glorious battle!", Text_Yell, 12468 );
        AddEmote( Event_OnTaunt, "I live for this!", Text_Yell, 12469 );
    }

    void OnCombatStart(Unit* mTarget)
    {
        Emote( "Ah, more lambs to the slaughter!", Text_Yell, 12463);
        _unit->CastSpell( _unit, SPELL_DUAL_WIELD, true );
        GetUnit()->SetFloatValue(UNIT_FIELD_MINDAMAGE, 7000);//Bfx DB does not support offhands using this instead.
        GetUnit()->SetFloatValue(UNIT_FIELD_MAXDAMAGE, 8000);
        GetUnit()->SetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE, 6000);
        GetUnit()->SetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE, 7500);
        _unit->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 10);
        _unit->SetFloatValue(UNIT_FIELD_COMBATREACH, 10);
        ParentClass::OnCombatStart(mTarget);
    };

    void OnTargetDied(Unit* mTarget)
    {
        if (_unit->GetHealthPct() > 0)
        {
            int RandomSpeach = rand()%3;
            switch (RandomSpeach)
            {
            case 0:
                Emote( "You are meat!", Text_Yell, 12465);
                break;
            case 1:
                Emote( "Too easy!", Text_Yell, 12466);
                break;
            case 3:
                Emote( "Perish, insect!", Text_Yell, 12464);
                break;
            }
        }
    }

    void OnDied(Unit * mKiller)
    {
        Emote( "Gah! Well done... Now... this gets... interesting....", Text_Yell, 12471);
        SpawnCreature(CREATURE_FELMYST, 1800, 652, 71, 0, false);

        ParentClass::OnDied(mKiller);
    };

    void Init()
    {
        MeteorSlashTimer = 11000;
        BurnTimer = 30000;
        StompTimer = 25000;
        BeserkTimer = 360000;
    };

    void AIUpdate()
    {

        if( MeteorSlashTimer <= mAIUpdateFrequency )
        {
            Unit* target = _unit->GetAIInterface()->GetMostHated();
            if( target )
            {
                _unit->CastSpell(target, BRUTALLUS_METEOR_SLASH, true );
            }
            MeteorSlashTimer = 11000;
            return;
        }
        else MeteorSlashTimer -= mAIUpdateFrequency;

        if( StompTimer <= mAIUpdateFrequency )
        {
            Unit* target = _unit->GetAIInterface()->GetMostHated();
            if( target )
            {
                _unit->CastSpell(target, BRUTALLUS_STOMP, true );
            };
            StompTimer = 30000;
            return;
        }
        else StompTimer -= mAIUpdateFrequency;


        if( BurnTimer <= mAIUpdateFrequency )
        {
            Player* target = TO_PLAYER( GetBestPlayerTarget() );
            if( target )
            {
                _unit->CastSpell(target, BRUTALLUS_BURN, false );
            };
            BurnTimer = 60000;
            return;
        }
        else BurnTimer -= mAIUpdateFrequency;

        if( BeserkTimer <= mAIUpdateFrequency )
        {
            _unit->CastSpell( _unit, BRUTALLUS_BERSERK, false );
            BeserkTimer = 360000;
            return;
        }
        else BeserkTimer -= mAIUpdateFrequency;

        ParentClass::AIUpdate();

    };

    uint32	MeteorSlashTimer;
    uint32	BurnTimer;
    uint32	StompTimer;
    uint32  BeserkTimer;


};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Felmyst Encounter


// BFX TODO: Implement Phase Two air phase and waypoints
//       Implement Spawns and creatures

enum FelMystSpells
{
    //Aura
    AURA_NOXIOUS_FUMES                            = 47002,

    //Land phase
    SPELL_CLEAVE                                  = 19983,
    SPELL_CORROSION                               = 45866,
    SPELL_GAS_NOVA                                = 45855,
    SPELL_ENCAPSULATE_CHANNEL                     = 45661,
    SPELL_FELMYST_BERSERK                         = 26662,
    SPELL_ENCAPSULATE_EFFECT                      = 45665,
    // SPELL_ENCAPSULATE_AOE                         = 45662,

    //Flight phase
    SPELL_VAPOR_SELECT                            = 45391,   // fel to player, force cast 45392, 50000y selete target
    SPELL_VAPOR_SUMMON                            = 45392,   // player summon vapor, radius around caster, 5y,
    SPELL_VAPOR_FORCE                             = 45388,   // vapor to fel, force cast 45389
    SPELL_VAPOR_CHANNEL                           = 45389,   // fel to vapor, green beam channel
    SPELL_VAPOR_TRIGGER                           = 45411,   // linked to 45389, vapor to self, trigger 45410 and 46931
    SPELL_VAPOR_DAMAGE                            = 46931,   // vapor damage, 4000
    SPELL_TRAIL_SUMMON                            = 45410,   // vapor summon trail
    SPELL_TRAIL_TRIGGER                           = 45399,   // trail to self, trigger 45402
    SPELL_TRAIL_DAMAGE                            = 45402,   // trail damage, 2000 + 2000 dot
    SPELL_DEAD_SUMMON                             = 45400,   // summon blazing dead, 5min
    SPELL_DEAD_PASSIVE                            = 45415,
    SPELL_FOG_BREATH                              = 45495,   // fel to self, speed burst
    SPELL_FOG_TRIGGER                             = 45582,   // fog to self, trigger 45782
    SPELL_FOG_FORCE                               = 45782,   // fog to player, force cast 45714
    SPELL_FOG_INFORM                              = 45714,   // player let fel cast 45717, script effect
    SPELL_FOG_CHARM                               = 45717,   // fel to player
    SPELL_FOG_CHARM2                              = 45726,   // link to 45717

    SPELL_TRANSFORM_TRIGGER                       = 44885,   // madrigosa to self, trigger 46350
    SPELL_TRANSFORM_VISUAL                        = 46350,   // 46411stun?
    SPELL_TRANSFORM_FELMYST                       = 45068,   // become fel
    SPELL_FELMYST_SUMMON                          = 45069,

    //Other
    SPELL_BERSERK                                 = 45078,
    SPELL_CLOUD_VISUAL                            = 45212,
    SPELL_CLOUD_SUMMON                            = 45884,
};

enum PhaseFelmyst
{
    PHASE_NULL   = 0,
    PHASE_GROUND = 1,
    PHASE_FLIGHT = 2,
};

enum EventFelmyst
{
    EVENT_NULL          =   0,
    EVENT_BERSERK       =   1,

    EVENT_CLEAVE        =   2,
    EVENT_CORROSION     =   3,
    EVENT_GAS_NOVA      =   4,
    EVENT_ENCAPSULATE   =   5,
    EVENT_FLIGHT        =   6,

    EVENT_FLIGHT_SEQUENCE   =   2,
    EVENT_SUMMON_DEAD       =   3,
    EVENT_SUMMON_FOG        =   4
};

enum FelMystEncounterCreatures
{
    BOSS_FELMYST            = 25038,       
    CREATURE_DEAD           =   25268,
    CREATURE_MADRIGOSA      =   25160,
    CREATURE_FELMYST_VISUAL =   25041,
    CREATURE_FLIGHT_LEFT    =   25357,
    CREATURE_FLIGHT_RIGHT   =   25358,
    CREATURE_DEATH_CLOUD    =   25703,
    CREATURE_VAPOR          =   25265,
    CREATURE_VAPOR_TRAIL    =   25267
};


static EventFelmyst MaxTimer[]=
{
    EVENT_NULL,
    EVENT_FLIGHT,
    EVENT_SUMMON_FOG,
};



class FelmystAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(FelmystAI, MoonScriptBossAI);
    FelmystAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {
        _unit->MechanicsDispels[DISPEL_MECHANIC_CHARM ] = 1;
        _unit->MechanicsDispels[DISPEL_MECHANIC_FEAR ] = 1;
        _unit->MechanicsDispels[DISPEL_MECHANIC_ROOT ] = 1;
        _unit->MechanicsDispels[DISPEL_MECHANIC_SLEEP ] = 1;
        _unit->MechanicsDispels[DISPEL_MECHANIC_SNARE ] = 1;
        _unit->MechanicsDispels[DISPEL_MECHANIC_STUN ] = 1;
        _unit->MechanicsDispels[DISPEL_MECHANIC_KNOCKOUT ] = 1;
        _unit->MechanicsDispels[DISPEL_MECHANIC_POLYMORPH ] = 1;
        _unit->MechanicsDispels[DISPEL_MECHANIC_BANISH ] = 1;

        //SetFlyMode(true); Bfx Only needed for fly phase
        init();
        AddEmote(Event_OnTaunt, "I am stronger than ever before!", Text_Yell);
    }
	
	PhaseFelmyst Phase;
    uint32 BeserkTimer;
    EventFelmyst Event;
    uint32 Timer[EVENT_FLIGHT + 1];
    uint32 FlightCount;
    uint32 BreathCount;
    float BreathX, BreathY;
    uint8	phase;

    //Spell Timers
    uint32 Encapsulate_timer;
    uint32 Corrossion_Timer;
    uint32 Cleave_timer;
    uint32 Gas_Nova_timer;
    uint32 Summon_fog_timer;

    void OnCombatStart( Unit* target )
    {
        Emote( "Glory to Kil'jaeden! Death to all who oppose!", Text_Yell);//Bfx Sound id
        phase = PHASE_GROUND;
        _unit->CastSpell( _unit, AURA_NOXIOUS_FUMES, true );
        _unit->CastSpell( _unit, AURA_SUNWELL_RADIANCE, true );
        ParentClass::OnCombatStart(target);
    }

    void OnCombatStop( Unit* target )
    {
        phase = PHASE_NULL;
        Event = EVENT_NULL;
        Timer[EVENT_BERSERK] = 600000;
        FlightCount = 0;
        SetFlyMode(false);

        init();

        _unit->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 10);//Bfx why does this only work for the creature and not the player , we should be able to attack also :|
        _unit->SetFloatValue(UNIT_FIELD_COMBATREACH, 10);//Bfx why does this only work for the creature and not the player , we should be able to attack also :|
        ParentClass::OnCombatStop( target );
    };

    void init()
    {
        //Ground phase
        BeserkTimer = 600000;
        Encapsulate_timer= urand(20000,  25000);
        Cleave_timer= urand(5000,  10000);
        Corrossion_Timer= urand(20000, 30000);
        Gas_Nova_timer= urand(20000, 25000);
        //Air Phase
        Summon_fog_timer= urand(20000, 25000);
    }

    void OnDied(Unit* pKiller)
    {
        Emote( "Kil'jaeden... will... prevail...", Text_Yell);
        ParentClass::OnDied(pKiller);
    }


    void OnTargetDied(Unit* mTarget)
    {
        if (_unit->GetHealthPct() > 0)
        {
            int RandomSpeach = rand()%2;
            switch (RandomSpeach)
            {
            case 0:
                Emote( "I kill for the master!", Text_Yell, 12426);//Bfx Need sound Id
                break;
            case 1:
                Emote( "The end has come!", Text_Yell, 12425);//Bfx Need Sound ID
                break;
            }
        }
    }

    void AIUpdate()
    {
        if( !IsInCombat() )
            return;

        if( phase == PHASE_GROUND )
        {

            if( Encapsulate_timer <= mAIUpdateFrequency )
            {
                Player* target = TO_PLAYER( GetBestUnitTarget() );//Bfx Target Random Player and Channel Encapsulate
                if( target )
                {
                    _unit->CastSpell( target, SPELL_ENCAPSULATE_CHANNEL, false );
                    /* Bfx these spells need to be linked
                    _unit->CastSpell( target, SPELL_ENCAPSULATE_EFFECT, false );
                    ApplyAura(target, SPELL_ENCAPSULATE_EFFECT);*/
                };

                Encapsulate_timer = 20000 + (rand()%5000);
                return;
            }
            else Encapsulate_timer -= mAIUpdateFrequency;

            if( Cleave_timer <= mAIUpdateFrequency )
            {

                Player* target = TO_PLAYER( _unit->GetAIInterface()->GetNextTarget() );//Target Current Main Aggro
                if( target )
                {
                    _unit->CastSpell( target, SPELL_CLEAVE, false );
                };

                Cleave_timer = 5000 + (rand()%1000);
                return;
            }
            else Cleave_timer -= mAIUpdateFrequency;

            if( Corrossion_Timer <= mAIUpdateFrequency )
            {

                Player* target = TO_PLAYER( GetBestUnitTarget() );
                if( target )
                {
                    _unit->CastSpell( target, SPELL_CORROSION, false );
                };

                Corrossion_Timer = 20000;
                return;
            }
            else Corrossion_Timer -= mAIUpdateFrequency;

            if( Gas_Nova_timer <= mAIUpdateFrequency )
            {

                Unit* target = TO_UNIT( GetBestUnitTarget() );
                if( pTarget )
                {
                    _unit->CastSpell( _unit, SPELL_GAS_NOVA, false );
                };

                Gas_Nova_timer = 20000 + (rand()%5000);
                return;
            }
            else Gas_Nova_timer -= mAIUpdateFrequency;

            if( BeserkTimer <= mAIUpdateFrequency )
            {
                _unit->CastSpell( _unit, SPELL_FELMYST_BERSERK, false );
                BeserkTimer = 600000;
                return;
            }
            else BeserkTimer -= mAIUpdateFrequency;

        }

        if( phase == PHASE_FLIGHT )
        {

            if( BeserkTimer <= mAIUpdateFrequency )
            {
                _unit->CastSpell( _unit, SPELL_FELMYST_BERSERK, false );
                BeserkTimer = 600000;
                return;
            }
            else BeserkTimer -= mAIUpdateFrequency;

            if( Summon_fog_timer <= mAIUpdateFrequency )
            {
                //Fog->RemoveAurasDueToSpell(SPELL_TRAIL_TRIGGER);
                //Fog->CastSpell(Fog, SPELL_FOG_TRIGGER, true);
                //_unit->CastSpell(Fog, SPELL_FOG_FORCE, true); BfX need to figure out what fog is ? wth did trinity do :S
                return;
            }
            else Summon_fog_timer -= mAIUpdateFrequency;
        }

        ParentClass::AIUpdate();

    };
};

class mob_felmyst_vaporAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(mob_felmyst_vaporAI, MoonScriptCreatureAI);
    mob_felmyst_vaporAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
        _unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        _unit->m_speedModifier = -20;
        _unit->UpdateSpeed();
    }; 

    void OnCombatStop( Unit* pTarget )
    {
        //ParentClass::OnCombatStop( pTarget );

    };

    void AIUpdate()
    {
        ParentClass::AIUpdate();
    };
};

class mob_felmyst_trailAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(mob_felmyst_trailAI, MoonScriptCreatureAI);
    mob_felmyst_trailAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
        _unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        _unit->CastSpell( _unit, SPELL_TRAIL_TRIGGER, true );
        _unit->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.01f); // Bfx core bug
        //_unit->SetTarget(me->GetGUID());

    }; 

    void OnCombatStop(  Unit* pTarget )
    {

    };
    void AIUpdate()
    {
        ParentClass::AIUpdate();
    };
    void OnCombatStart( Unit* pTarget)
    {

    };

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*//M'uru
#define CREATURE_MURU						25960
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
SPELL_SB_DUAL_WIELD            = 29651,

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
Creature *Muruu = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1815, 625, 69, CREATURE_MURU);
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
Creature *Muruu = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1815, 625, 69, CREATURE_MURU);
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
Creature *Muruu = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1815, 625, 69, CREATURE_MURU);
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
uint32 WaitTimer;
bool InAction;
}

void OnCombatStop(Unit* mTarget)
{
WaitTimer = 2000;
InAction = false;

_unit->AddUnitState(UNIT_STATE_STUNNED);

ParentClass::OnCombatStop(mTarget);
}

void AIUpdate()
{
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

if (EnrageTimer < mAIUpdateFrequency && !_unit->HasAura(SPELL_ENRAGE))
{
_unit->CastSpell(_unit, SPELL_ENRAGE, false);
} 
else EnrageTimer -= mAIUpdateFrequency;

if (BlackHoleSummonTimer <= mAIUpdateFrequency)
{
Unit* rand = GetBestUnitTarget();
if (!rand)
return;

_unit->CastSpell(rand, SPELL_DARKNESS_E, false);

rand = GetBestUnitTarget();
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
};*/

void SetupSunwellPlateau(ScriptMgr* pScriptMgr)
{

    pScriptMgr->register_creature_script(BOSS_BRUTALLUS, &BrutallusAI::Create);
    pScriptMgr->register_creature_script(BOSS_FELMYST, &FelmystAI::Create);
    pScriptMgr->register_creature_script(CREATURE_VAPOR, &mob_felmyst_vaporAI::Create);
    pScriptMgr->register_creature_script(CREATURE_VAPOR_TRAIL, &mob_felmyst_trailAI::Create);

    /*pScriptMgr->register_creature_script(CREATURE_MURU, &MuruAI::Create);
    pScriptMgr->register_creature_script(CREATURE_BERSERKER, &ShadowswordBerserkerAI::Create);
    pScriptMgr->register_creature_script(CREATURE_FURY_MAGE, &ShadowswordFuryMageAI::Create);
    pScriptMgr->register_creature_script(CREATURE_VOID_SENTINEL, &VoidSentinelAI::Create);
    pScriptMgr->register_creature_script(CREATURE_ENTROPIUS, &EntropiusAI::Create);*/

}
