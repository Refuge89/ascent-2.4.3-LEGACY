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

/* Bfx TO DO  Work on updating target selection methods, possibly refactor some or create duplicates but returning the previous method should work, the current target selection methods look strange and misleading.*/


enum TrashNpcs
{

    CREATURE_SUNBLADE_CABALIST     = 25369,
    CREATURE_SUNBLADE_VINDICATOR	= 25369,
    CREATURE_SUNBLADE_PROTECTOR	= 25507,
    CREATURE_SUNBLADE_DUSK_PRIEST  = 25370,
    CREATURE_SHADOWSWORD_ASSASSIN  = 25484,
    CREATURE_SHADOWSWORD_COMMANDER = 25837,
    CREATURE_SHADOWSWORN_GUARDIAN  = 25508,

};


enum TrashNpcSpells
{

    /*       Sunblade Cabalist            */
    SUNBLADE_CABALIST_IGNITE_MANA             = 46543,
    SUNBLADE_CABALIST_SUMMON_IMP              = 46544,
    SUNBLADE_CABALIST_SHADOWBOLT              = 47248,

    /*       Sunblade Vindicator          */
    SUNBLADE_VINDICATOR_BRUTAL_STRIKE         = 58460,
    SUNBLADE_VINDICATOR_CLEAVE                = 46559,


    /*        Sunblade Protector          */
    SUNBLADE_PROTECTOR_FEL_LIGHTNING	        = 46480,

    SHADOWSWORN_GUARDIAN_BEARDOWN             = 46239,
    SHADOWSWORN_GUARDIAN_EARTHQUAKE           = 46240,

};

enum SunwellSounds
{

    KALEC_HUMAN_ENGAGE_SOUND = 12430,
    KALEC_HUMAN_LOW_HEALTH_SOUND = 12430,
    KALEC_HUMAN_MID_HEALTH_SOUND= 12430,

};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Sunblade Cabalist//

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SunbladeCabalistAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SunbladeCabalistAI, MoonScriptCreatureAI);
    SunbladeCabalistAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {

        init();

    }

    void init()
    {
        Ignite_mana_timer = urand(5000,  10000);
        Summon_imp_timer  = urand(30000, 40000);
        Shadow_Bolt_timer = urand(10000, 15000);
    }

    void AIUpdate()
    {

        if( Ignite_mana_timer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx Need to get Correct Mana Target not sure on the Target selection methods :|
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SUNBLADE_CABALIST_IGNITE_MANA, true );
            }
            Ignite_mana_timer = 5000;
            return;
        }
        else Ignite_mana_timer -= mAIUpdateFrequency;

        if( Summon_imp_timer  <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell( _unit, SUNBLADE_CABALIST_SUMMON_IMP, true );
            }
            Summon_imp_timer = 30000;
            return;
        }
        else Summon_imp_timer  -= mAIUpdateFrequency;

        if( Shadow_Bolt_timer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SUNBLADE_CABALIST_SHADOWBOLT, true );
            }
            Shadow_Bolt_timer = 7000;
            return;
        }
        else Shadow_Bolt_timer -= mAIUpdateFrequency;



    }

    uint32 Ignite_mana_timer;
    uint32 Summon_imp_timer;
    uint32 Shadow_Bolt_timer;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Sunblade Vindicator//

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SunbladeVindicatorAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SunbladeVindicatorAI, MoonScriptCreatureAI);
    SunbladeVindicatorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {

        init();

    }


    void init()
    {
        Brutal_strike_Timer = urand(6500,  10000);
        Vindicator_Cleave_Timer  = urand(5000, 11000);
    }

    void AIUpdate()
    {

        if( Brutal_strike_Timer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx Need to get Correct Mana Target not sure on the Target selection methods :|
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SUNBLADE_VINDICATOR_BRUTAL_STRIKE, true );
            }
            Brutal_strike_Timer = 5000;
            return;
        }
        else Brutal_strike_Timer  -= mAIUpdateFrequency;

        if( Vindicator_Cleave_Timer  <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SUNBLADE_VINDICATOR_CLEAVE, true );
            }
            Vindicator_Cleave_Timer= 6000;
            return;
        }
        else Vindicator_Cleave_Timer -= mAIUpdateFrequency;

    }
    uint32 Vindicator_Cleave_Timer;
    uint32 Brutal_strike_Timer;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Sunblade Protector//


class Sunblade_ProtectorAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(Sunblade_ProtectorAI, MoonScriptCreatureAI);
    Sunblade_ProtectorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
        init();
    }

    void init()
    {
        Fel_Lightning_Timer = urand(13000,  20000);
    }

    void AIUpdate()
    {

        if( Fel_Lightning_Timer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx This Needs to be Random Target
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SUNBLADE_PROTECTOR_FEL_LIGHTNING, true );
            }
            Fel_Lightning_Timer = 13000;
            return;
        }
        else Fel_Lightning_Timer -= mAIUpdateFrequency;

    }
    uint32 Fel_Lightning_Timer;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Sunblade Dusk Priest//

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Sunblade Dusk Priest//
#define SUNBLADE_DUSKPRIEST_FEAR         	46561
#define SUNBLADE_DUSKPRIEST_MIND_FLAY       46562
#define SUNBLADE_DUSKPRIEST_SHADOWWORDPAIN  46560

class SunbladeDuskPriestAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SunbladeDuskPriestAI, MoonScriptCreatureAI);
    SunbladeDuskPriestAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
        init();
    }

    void init()
    {
        Fear_Timer = urand(15000, 20000);
        Mind_Flay_Timer = urand(5000, 15000);
        Shadow_word_pain_Timer = urand(10000, 25000);
    }

    void AIUpdate()
    {

        if( Fear_Timer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx This Needs to be Random Target
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SUNBLADE_DUSKPRIEST_FEAR , true );
            }
            Fear_Timer = 15000;
            return;
        }
        else Fear_Timer-= mAIUpdateFrequency;

        if( Mind_Flay_Timer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx This Needs to be Random Target
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SUNBLADE_DUSKPRIEST_MIND_FLAY, true );
            }
            Mind_Flay_Timer = 5000;
            return;
        }
        else Mind_Flay_Timer-= mAIUpdateFrequency;

        if( Shadow_word_pain_Timer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx This Needs to be Random Target
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SUNBLADE_DUSKPRIEST_SHADOWWORDPAIN, true );
            }
            Shadow_word_pain_Timer = 10000;
            return;
        }
        else Shadow_word_pain_Timer-= mAIUpdateFrequency;

    }
    uint32 Fear_Timer;
    uint32 Mind_Flay_Timer ;
    uint32 Shadow_word_pain_Timer;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Shadowsword Assassin
#define SHADOWSWORD_ASSASSIN_ASSASSINS_MARK			46459
#define SHADOWSWORD_ASSASSIN_AIMED_SHOT				46460
#define SHADOWSWORD_ASSASSIN_SHADOWSTEP				46463
#define SHADOWSWORD_ASSASSIN_GREATER_INVISIBILITY	16380

class ShadowswordAssassinAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowswordAssassinAI, MoonScriptCreatureAI);
    ShadowswordAssassinAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
        init();

        _unit->CastSpell( _unit, SHADOWSWORD_ASSASSIN_GREATER_INVISIBILITY, true );
    }

    void init()
    {
        Assassins_Mark_Timer = urand(9000, 10000);
        Aimed_Shot_Timer = urand(3500, 7000);
        Shadow_Step_Timer = urand(14000, 20000);
    }

    void AIUpdate()
    {

        if( Assassins_Mark_Timer  <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx This Needs to be Random Target
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SHADOWSWORD_ASSASSIN_ASSASSINS_MARK , true );
            }
            Assassins_Mark_Timer  = 15000;
            return;
        }
        else Assassins_Mark_Timer -= mAIUpdateFrequency;

        if( Aimed_Shot_Timer  <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx This Needs to be Random Target
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SHADOWSWORD_ASSASSIN_AIMED_SHOT, true );
            }
            Aimed_Shot_Timer  = 5000;
            return;
        }
        else Aimed_Shot_Timer = mAIUpdateFrequency;

        if( Shadow_Step_Timer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx This Needs to be Random Target
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SHADOWSWORD_ASSASSIN_SHADOWSTEP, true );
            }
            Shadow_Step_Timer = 10000;
            return;
        }
        else Shadow_Step_Timer-= mAIUpdateFrequency;

    }
    uint32 Assassins_Mark_Timer;
    uint32 Aimed_Shot_Timer;
    uint32 Shadow_Step_Timer;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Shadowsword Commander
#define SHADOWSWORD_COMMANDER_SHIELD_SLAM	46762
#define SHADOWSWORD_COMMANDER_BATTLESHOUT	46763

class ShadowswordCommanderAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowswordCommanderAI, MoonScriptCreatureAI);
    ShadowswordCommanderAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
        init();
    }

    void init()
    {
        Shield_Slam_Timer = urand(5000, 10000);
        Battle_Shout_Timer = urand(15000, 30000);
    }

    void AIUpdate()
    {

        if( Shield_Slam_Timer  <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx This Needs to be Random Target
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SHADOWSWORD_COMMANDER_SHIELD_SLAM , true );
            }
            Shield_Slam_Timer  = 5000;
            return;
        }
        else Shield_Slam_Timer -= mAIUpdateFrequency;

        if( Battle_Shout_Timer  <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell( _unit, SHADOWSWORD_COMMANDER_BATTLESHOUT, true );
            }
            Battle_Shout_Timer  = 15000;
            return;
        }
        else Battle_Shout_Timer = mAIUpdateFrequency;

    }
    uint32 Shield_Slam_Timer;
    uint32 Battle_Shout_Timer;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Shadowsword Guardian
class ShadowswordGuardianAI: public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowswordGuardianAI, MoonScriptCreatureAI);
    ShadowswordGuardianAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
        init();
    }

    void init()
    {
        Earthquake_Timer = urand(9000,  13000);
        Beardown_timer = urand(5000,  15000);
    }

    void AIUpdate()
    {

        if( Earthquake_Timer  <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx This Needs to be Random Target
            if( pTarget )
            {
                _unit->CastSpell( _unit, SHADOWSWORN_GUARDIAN_EARTHQUAKE, true );
            }
            Earthquake_Timer  = 9000;
            return;
        }
        else Earthquake_Timer -= mAIUpdateFrequency;

        if( Beardown_timer  <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();// Bfx This Needs to be Random Target
            if( pTarget )
            {
                _unit->CastSpell( _unit, SHADOWSWORN_GUARDIAN_EARTHQUAKE, true );
            }
            Beardown_timer  = 5000;
            return;
        }
        else Beardown_timer -= mAIUpdateFrequency;

    }
    uint32 Earthquake_Timer;
    uint32 Beardown_timer;
};

void SetupSunwellPlateauTrash(ScriptMgr* pScriptMgr)
{
    pScriptMgr->register_creature_script(CREATURE_SUNBLADE_PROTECTOR, &Sunblade_ProtectorAI::Create);
    pScriptMgr->register_creature_script(CREATURE_SUNBLADE_VINDICATOR, &SunbladeVindicatorAI::Create);
    pScriptMgr->register_creature_script(CREATURE_SUNBLADE_DUSK_PRIEST, &SunbladeDuskPriestAI::Create);
    pScriptMgr->register_creature_script(CREATURE_SUNBLADE_CABALIST, &SunbladeCabalistAI::Create);	
}