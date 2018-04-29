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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Kalecgos Encounter
enum SunwellSounds
{

    KALEC_HUMAN_ENGAGE_SOUND = 12430,
    KALEC_HUMAN_LOW_HEALTH_SOUND = 12430,
    KALEC_HUMAN_MID_HEALTH_SOUND= 12430,

};

enum Creatures
{
    CREATURE_KALECGOS                   = 24850,
    CREATURE_SATHROVARR_THE_CORRUPTOR	= 24892,
    CREATURE_HUMAN_KALECGOS             = 24891,
};

enum EncounterDefines
{

    //Faction Related
    FRIENDLY_FACTION = 35,

    //Custom Script Target Flag defines
    SCRIPTTARGETFLAG_NONE         = 0x00,
    SCRIPTTARGETFLAG_NO_TANK      = 0x01,
    SCRIPTTARGETFLAG_NO_EXHAUTION = 0x02,

};

enum Spells
{
    AURA_SUNWELL_RADIANCE                        = 45769,
    AURA_SPECTRAL_EXHAUSTION                     = 44867,
    AURA_SPECTRAL_REALM                          = 46021,
    AURA_SPECTRAL_INVISIBILITY                   = 44801,
    AURA_DEMONIC_VISUAL                          = 44800,

    SPELL_SPECTRAL_BLAST                         = 44869,
    SPELL_TELEPORT_SPECTRAL                      = 46019,
    SPELL_ARCANE_BUFFET                          = 45018,
    SPELL_FROST_BREATH                           = 44799,
    SPELL_TAIL_LASH                              = 45122,

    SPELL_BANISH                                 = 44836,
    SPELL_TRANSFORM_KALEC                        = 44670,
    SPELL_ENRAGE                                 = 44807,

    SPELL_CORRUPTION_STRIKE                      = 45029,
    SPELL_AGONY_CURSE                            = 45032,
    SPELL_SHADOW_BOLT                            = 45031,

    SPELL_HEROIC_STRIKE                          = 45026,
    SPELL_REVITALIZE                             = 45027
};

struct coords
{
    float x,y,z;
    coords(float x,float y, float z)
    {
        coords::x = x;
        coords::y = y;
        coords::z = z;
    }

};

int ran = rand()%6;
uint32 WildMagic[] = { 44978, 45001, 45002, 45004, 45006, 45010 };

coords spectralCoords(1724.94f,909.265f,-74.5588f);
coords wp1(1703.69653f,869.224854f,65.416687f);
coords wp2(1703.131348f,842.142822f,91.754280f);
coords wp3(1752.924194f,754.499634f,181.749878f);

class KalecgosAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(KalecgosAI, MoonScriptBossAI);
    KalecgosAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {
        Init();
    }
	
	uint32 TailLashTimer;
    uint32 SpectralBlastTimer;
    uint32 WildMagicTimer;
    uint32 FrostBreathTimer;
    uint32 ArcaneBuffetTimer;
    uint32 darkelf;
    uint32 sathrovar;

    bool failed;
    bool  success;

    void Init()
    {
        ArcaneBuffetTimer = 8000;
        FrostBreathTimer = 15000;
        WildMagicTimer = 10000;
        TailLashTimer = 25000;
        SpectralBlastTimer = urand(20000, 25000);

    };

    void OnTargetDied(Unit* mTarget)
    {
        if (_unit->GetHealthPct() > 0)
        {
            int RandomSpeach = rand()%2;
            switch (RandomSpeach)
            {
            case 0:
                Emote( "You were warned!", Text_Yell, 12426);
                break;
            case 1:
                Emote( "In the name of Kil'jaeden!", Text_Yell, 12425);
                break;
            }
        }
    }

    void OnDied(Unit * mKiller)
    {
        Emote( "I am forever in your debt. Once we have triumphed over Kil'jaeden, this entire world will be in your debt as well.", Text_Yell, 12431);

        RemoveAIUpdateEvent();
    }


    void OnCombatStart(Unit* mTarget)
    {
        Emote( "No longer will I be a slave to Malygos! Challenge me and you will be destroyed!", Text_Yell, 12422);
        _unit->SetStandState( STANDSTATE_STAND);

        ParentClass::OnCombatStart(mTarget);

    };

    void OnCombatStop(Unit* mTarget)
    {
        _unit->SetStandState( STANDSTATE_SLEEP);

        ParentClass::OnCombatStop(mTarget);

    };

    /*void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
    switch(iWaypointId)
    {
    case 3: // 
    {
    cbMap.clear();
    saymap.clear();
    _unit->Despawn(100,0);
    if(sathrovar != NULL)
    sathrovar->Despawn(100,0);
    if(darkelf != NULL)
    darkelf->Despawn(100,0);
    }
    }
    };*/

    void AIUpdate()
    {

        if( ArcaneBuffetTimer <= mAIUpdateFrequency )
        {
            Unit* target = GetRandomPlayer();
            if( target )
            {
                _unit->CastSpell(target, SPELL_ARCANE_BUFFET, true );
            }
            ArcaneBuffetTimer = 8000;
            return;
        }
        else ArcaneBuffetTimer -= mAIUpdateFrequency;

        if( FrostBreathTimer <= mAIUpdateFrequency )
        {
            Unit* target = GetRandomPlayer();
            if( target )
            {
                _unit->CastSpell(target, SPELL_FROST_BREATH, true );
            }
            FrostBreathTimer = 15000;
            return;
        }
        else FrostBreathTimer -= mAIUpdateFrequency;

        if( WildMagicTimer <= mAIUpdateFrequency )
        {
            Player* target = TO_PLAYER( GetRandomPlayerNotCurrent() );
            if( pTarget )
            {
                _unit->CastSpell(_unit, WildMagic[rand()%6], false);
            }
            WildMagicTimer = 20000;
            return;
        }
        else WildMagicTimer -= mAIUpdateFrequency;

        if( TailLashTimer <= mAIUpdateFrequency )
        {
            Player* pTarget = TO_PLAYER( GetRandomPlayerNotCurrent() );
            if( pTarget )
            {
                _unit->CastSpell(target, SPELL_TAIL_LASH, true );

            }
            TailLashTimer = 15000;
            return;
        }
        else TailLashTimer -= mAIUpdateFrequency;

        if( SpectralBlastTimer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell(target, SPELL_SPECTRAL_BLAST, true );
            }
            SpectralBlastTimer = 11000;
            return;
        }
        else SpectralBlastTimer -= mAIUpdateFrequency;


        ParentClass::AIUpdate();

    };
};

//Kalecgos Human Form
class KalecgosHumanAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(KalecgosHumanAI, MoonScriptBossAI);
    KalecgosHumanAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {

    }
	
	uint32 RevitalizeTimer;
    uint32 HeroicStrikeTimer;
    uint32 YellTimer;
    uint32 YellSequence;

    void Init()
    {
        RevitalizeTimer = 5000;
        HeroicStrikeTimer = 3000;
        YellTimer = 5000;
        YellSequence = 0;
    };

    void OnTargetDied(Unit* mTarget)
    {
        ParentClass::OnTargetDied(mTarget);
    }

    void OnDied(Unit * mKiller)
    {
        ParentClass::OnDied(mKiller);
    }

    void OnCombatStart(Unit* mTarget)
    {
        ParentClass::OnCombatStart(mTarget);
        Init();
    };

    void AIUpdate()
    {

        if (RevitalizeTimer <= mAIUpdateFrequency)
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell(target, SPELL_REVITALIZE, true );
                RevitalizeTimer = 5000;
            }
        }
        else HeroicStrikeTimer -= mAIUpdateFrequency;

        if (HeroicStrikeTimer <= mAIUpdateFrequency)
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell(target, SPELL_HEROIC_STRIKE, true );
                HeroicStrikeTimer = 5000;
            }
        }
        else HeroicStrikeTimer -= mAIUpdateFrequency;

        if (YellTimer <= mAIUpdateFrequency )
        {
            switch (YellSequence)
            {
            case 0:
                Emote( "I need... your help... Cannot... resist him... much longer...", Text_Yell, KALEC_HUMAN_ENGAGE_SOUND);
                ++YellSequence;
                break;
            case 1:
                if (GetHealthPercent() <= 50)
                {
                    Emote( "Aaahhh! Help me, before I lose my mind!", Text_Yell, KALEC_HUMAN_MID_HEALTH_SOUND);
                    ++YellSequence;
                }
                break;
            case 2:
                if (GetHealthPercent() <= 10)
                {
                    Emote( "Hurry! There is not much of me left!", Text_Yell, KALEC_HUMAN_LOW_HEALTH_SOUND);
                    ++YellSequence;
                }	
                break;		
            } 
        }
		else YellTimer -= mAIUpdateFrequency;

        ParentClass::AIUpdate();
    }
};


/* Sathrovarr the Corruptor */
class SathrovarrTheCorruptorAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SathrovarrTheCorruptorAI, MoonScriptBossAI);
    SathrovarrTheCorruptorAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {
        Init();
    }
	
	uint32 ShadowBoltTimer;
    uint32 AgonyCurseTimer;
    uint32 CorruptionStrikeTimer;
    uint32 CheckTimer;
    uint32 ResetThreat;

    uint64 KalecGUID;
    uint64 KalecgosGUID;

    bool isEnraged;
    bool isBanished;

    void Init()
    {
        ShadowBoltTimer = 7000 + rand()%3 * 1000;
        AgonyCurseTimer = 20000;
        CorruptionStrikeTimer = 13000;
        CheckTimer = 1000;
        ResetThreat = 1000;
        isEnraged = false;
        isBanished = false;

    };

    void OnTargetDied(Unit* mTarget)
    {
        if (_unit->GetHealthPct() > 0)
        {
            int RandomSpeach = rand()%2;
            switch (RandomSpeach)
            {
            case 0:
                Emote( "Pitious mortal", Text_Yell, 12455);
                break;
            case 1:
                Emote( "Haven't you heard? I always win!", Text_Yell, 12456);
                break;
            }
        }
    }

    void OnDied(Unit * mKiller)
    {
        Emote( "I'm... never on... the losing... side...", Text_Yell, 12452);

        RemoveAIUpdateEvent();
    }

    void OnCombatStart(Unit* mTarget)
    {
        Emote( "Gyahaha... There will be no reprieve. My work here is nearly finished.", Text_Yell, 12451);

        ParentClass::OnCombatStart(mTarget);

        RegisterAIUpdateEvent(1000);

    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->CastSpell(_unit, AURA_DEMONIC_VISUAL, true);
        ParentClass::OnCombatStop(mTarget);
    }

    void OnLoad()
    {
        _unit->CastSpell(_unit,AURA_SPECTRAL_INVISIBILITY,true);
    }

    void AIUpdate()
    {

        if (AgonyCurseTimer  <= mAIUpdateFrequency)
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell(target, SPELL_AGONY_CURSE, true );
                AgonyCurseTimer = 20000;
            }
        }
        else AgonyCurseTimer  -= mAIUpdateFrequency;

        if (ShadowBoltTimer <= mAIUpdateFrequency)
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell(target, SPELL_SHADOW_BOLT, true );
                ShadowBoltTimer = 7000+(rand()%3000);
            }
        }
        else ShadowBoltTimer -= mAIUpdateFrequency;

        if (CorruptionStrikeTimer <= mAIUpdateFrequency)
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell(target, SPELL_CORRUPTION_STRIKE, true );
                CorruptionStrikeTimer = 13000;
            }
        }
        else CorruptionStrikeTimer -= mAIUpdateFrequency;

        ParentClass::AIUpdate();
    };
};

void SetupKalecgos(ScriptMgr* pScriptMgr)
{
    pScriptMgr->register_creature_script(CREATURE_KALECGOS, &KalecgosAI::Create);
    pScriptMgr->register_creature_script(CREATURE_SATHROVARR_THE_CORRUPTOR, &SathrovarrTheCorruptorAI::Create);
    pScriptMgr->register_creature_script(CREATURE_HUMAN_KALECGOS, &KalecgosHumanAI::Create);
}
