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


//Bfx Script Completion 60% TODO Spawns for Shadow images , Flame spawn on conflag fix up intro timers and announces

//Grand Warlock Alythess && Lady Sacrolash

enum Creatures
{

    CREATURE_LADY_SACROLASH			=	25165,
    CREATURE_GRAND_WARLOCK_ALYTHESS	=	25166,
    CREATURE_SHADOW_IMAGE           =   25214,

};

enum Spells
{

    ENRAGE					=		26662,	
    EMPOWER					=		45366,
    DARK_TOUCHED			=		45347,
    SHADOW_BLADES			=		45248,
    SHADOW_FURY				=	    45270,
    CONFOUNDING_BLOW		=		45256,

    PYROGENICS				=		45230,
    FLAME_TOUCHED			=		45348,
    CONFLAGRATION			=		45342,
    BLAZE					=		45235,
    FLAME_SEAR				=		46771,

};

class LadySacrolashAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(LadySacrolashAI, MoonScriptBossAI);
    LadySacrolashAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
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

        Emote( "Misery...", Text_Yell, 12484 );
        AddEmote( Event_OnTargetDied, "Shadows, engulf!", Text_Yell, 12486 );
    }

    void Init()
    {
        phase = 1;
        ShadowbladesTimer = 10000;
        ShadowfuryTimer = 30000;
        ConfoundingblowTimer = 25000;
        ConflagrationTimer = 30000;
        BerserkTimer = 360000;
    };

    void AIUpdate()
    {
        if( IsInCombat() == false || IsCasting() )
            return;

        if( BerserkTimer <= mAIUpdateFrequency )
        {
            _unit->CastSpell( _unit, ENRAGE, false );
            BerserkTimer = 360000;
            return;
        }
        else BerserkTimer -= mAIUpdateFrequency;

        if( ShadowbladesTimer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetCurrentTarget();
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SHADOW_BLADES, false );
                pTarget->RemoveAllAuras( FLAME_TOUCHED, 0 );
                _unit->CastSpell( pTarget, DARK_TOUCHED, true );
            }
            ShadowbladesTimer = 10000;
            return;
        }else ShadowbladesTimer -= mAIUpdateFrequency;

        if( ConfoundingblowTimer <= mAIUpdateFrequency )
        {
            Player* pTarget = TO_PLAYER( GetRandomPlayerNotCurrent() );
            if( pTarget )
            {
                _unit->CastSpell( pTarget, CONFOUNDING_BLOW, false );
                pTarget->RemoveAllAuras( FLAME_TOUCHED, 0 );
                _unit->CastSpell( pTarget, DARK_TOUCHED, true );
            };

            ConfoundingblowTimer = 20000 + (rand()%5000);
            return;
        }else ConfoundingblowTimer -= mAIUpdateFrequency;

        if( phase == 0 )
        {
            if( ShadowfuryTimer <= mAIUpdateFrequency )
            {
                Player* pTarget = TO_PLAYER(GetRandomPlayerNotCurrent() );
                if( pTarget )
                {
                    _unit->CastSpell( pTarget, SHADOW_FURY, false );

                    char msg[256];
                    snprintf((char*)msg, 256, "Lady Sacrolash directs Shadow Fury at %s", pTarget->GetName());
                    pTarget->SendChatMessage( CHAT_MSG_RAID_BOSS_EMOTE, LANG_UNIVERSAL,  msg );
                    Emote( "Shadow to the aid of fire!", Text_Yell, 12485);

                    pTarget->RemoveAllAuras( FLAME_TOUCHED, 0 );
                    _unit->CastSpell( pTarget, DARK_TOUCHED, true );

                };

                ShadowfuryTimer = 30000 +( rand()%5000 );
                return;
            }
            else
                ShadowfuryTimer -= mAIUpdateFrequency;
        }
        else
        {
            if( ConflagrationTimer <= mAIUpdateFrequency )
            {
                Player* pTarget = TO_PLAYER( GetRandomPlayer() );
                if( pTarget )
                {
                    _unit->CastSpell( pTarget, CONFLAGRATION, false );

                    pTarget->RemoveAllAuras( FLAME_TOUCHED, 0 );
                    _unit->CastSpell( pTarget, DARK_TOUCHED, true );
                }

                ConflagrationTimer = 30000 +( rand()%5000 );
                return;
            }
            else
                ConflagrationTimer -= mAIUpdateFrequency;
        };
    };

    void OnCombatStop( Unit* pTarget )
    {
        ParentClass::OnCombatStop( pTarget );
        Init();
    };

    uint32	ShadowfuryTimer;
    uint32	ConfoundingblowTimer;
    uint32	ConflagrationTimer;
    uint32  BerserkTimer;
    uint32	ShadowbladesTimer;
    uint8	phase;
};

class GrandWarlockAlythessAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(GrandWarlockAlythessAI, MoonScriptBossAI);
    GrandWarlockAlythessAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
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

        mLadySacrolash = NULL;
        mRespond = 0;
        mRespondTimer = AddTimer( 1100 );
        RegisterAIUpdateEvent( 500 );
        SetCanMove( false );

        AddEmote(Event_OnTargetDied, "Fire, consume!", Text_Yell, 12490 );
    }

    void Init()
    {
        ConflagrationTimer = 45000;
        BlazeTimer = 20000;
        PyrogenicsTimer = 15000;
        ShadowfuryTimer = 40000;
        FlamesearTimer = 15000;
        ShadowimageTimer = 20000;

        phase = 0;
    };

    void OnCombatStart( Unit* pTarget )
    {
        SetBehavior(Behavior_Melee);
        RegisterAIUpdateEvent(mAIUpdateFrequency);
        SetCanMove( false );
    };

    void AIUpdate()
    {
        ParentClass::AIUpdate();

        if( IsTimerFinished( mRespondTimer ) )
            Respond();

        if( !IsInCombat() )
            return;

        if( BerserkTimer <= mAIUpdateFrequency )
        {
            _unit->CastSpell( _unit, ENRAGE, false );
            Emote("Your luck has run its course!" , Text_Yell, 12490 );
            BerserkTimer = 360000;
            return;
        }
        else BerserkTimer -= mAIUpdateFrequency;

        if( phase = 0 && ( mLadySacrolash == NULL || !mLadySacrolash->IsAlive() ) )
        {
            phase = 1;
            Emote( "Sacrolash!", Text_Yell, 12488 );
        };

        if( IsCasting() )
            return;

        if( FlamesearTimer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell( pTarget, FLAME_SEAR, false );

                pTarget->RemoveAllAuras( DARK_TOUCHED, 0 );
                _unit->CastSpell( pTarget, FLAME_TOUCHED, true );
            };

            FlamesearTimer = 15000;
            return;
        }else FlamesearTimer -= mAIUpdateFrequency;

        if ( PyrogenicsTimer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetRandomPlayer();
            if( pTarget )
            {
                _unit->CastSpell( pTarget, PYROGENICS, false );

                pTarget->RemoveAllAuras( DARK_TOUCHED, 0 );
                _unit->CastSpell( pTarget, FLAME_TOUCHED, true );
            };

            PyrogenicsTimer = 15000;
            return;
        }
        else PyrogenicsTimer -= mAIUpdateFrequency;

        if (ShadowimageTimer <= mAIUpdateFrequency)
        {
            Unit* target = NULL;
            Creature* temp = NULL;
            for (uint8 i = 0; i<3; ++i)
            {
                Unit* target = GetRandomPlayer();
                (MoonScriptBossAI*)SpawnCreature(CREATURE_SHADOW_IMAGE);
                if (temp && target)
                {
                    //temp->AddThreat(target, 1000000);//don't change target(healers)
                    //temp->AI()->AttackStart(target);
                }
            }
            ShadowimageTimer = 20000;
        } 
        else ShadowimageTimer -=mAIUpdateFrequency;

        if ( BlazeTimer <= mAIUpdateFrequency )
        {
            Unit* pTarget = GetCurrentTarget();
            if( pTarget )
            {
                _unit->CastSpell( pTarget, BLAZE, false );

                _unit->RemoveAllAuras( DARK_TOUCHED, 0 );
                _unit->CastSpell( pTarget, FLAME_TOUCHED, true );
            };

            BlazeTimer = 20000;
            return;
        }
        else BlazeTimer -= mAIUpdateFrequency;

        if( phase == 0 )
        {
            if( ConflagrationTimer <= mAIUpdateFrequency )
            {
                Player* pTarget = TO_PLAYER( GetRandomPlayerNotCurrent() );
                if( pTarget )
                {
                    _unit->CastSpell( pTarget, CONFLAGRATION, false );

                    char msg[256];
                    snprintf((char*)msg, 256, "Lady Sacrolash directs Confligration at %s", pTarget->GetName());
                    _unit->SendChatMessage( CHAT_MSG_RAID_BOSS_EMOTE, LANG_UNIVERSAL,  msg );
                    Emote( "Fire to the aid of shadow!", Text_Yell, 12489 );

                    pTarget->RemoveAllAuras( DARK_TOUCHED, 0 );
                    _unit->CastSpell( pTarget, FLAME_TOUCHED, true );
                };

                ConflagrationTimer = 30000 +( rand()%5000 );
                return;
            }
            else ConflagrationTimer -= mAIUpdateFrequency;
        }
        else
        {
            if( ShadowfuryTimer <= mAIUpdateFrequency )
            {
                Player* pTarget = TO_PLAYER( GetRandomPlayerNotCurrent() );
                if( pTarget )
                {
                    _unit->CastSpell( pTarget, SHADOW_FURY, false );
                    char msg[256];
                    snprintf((char*)msg, 256, "Lady Sacrolash directs A ShadowFury at %s", pTarget->GetName());
                    _unit->SendChatMessage( CHAT_MSG_RAID_BOSS_EMOTE, LANG_UNIVERSAL,  msg );
                    pTarget->RemoveAllAuras( FLAME_TOUCHED, 0 );
                    _unit->CastSpell( pTarget, DARK_TOUCHED, true );
                };

                ShadowfuryTimer = 30000 +( rand()%5000 );
                return;
            }
            else
                ShadowfuryTimer -= mAIUpdateFrequency;
        };
    };

    void OnDied(Unit* pKiller)
    {
        if( mLadySacrolash && mLadySacrolash->IsAlive() )
        {
            mLadySacrolash->Emote( "Alythess! Your fire burns within me!", Text_Yell, 12488 );
            mLadySacrolash->ApplyAura( EMPOWER );
            mLadySacrolash->phase = 1;
        };

        ParentClass::OnDied(pKiller);
    }

    void Respond()
    {
        ++mRespond;
        switch( mRespond )
        {
        case 1:
            {
                mLadySacrolash = static_cast<LadySacrolashAI*>( GetNearestCreature( CREATURE_LADY_SACROLASH ) );
                Emote( "Depravity...", Text_Yell );
                ResetTimer( mRespondTimer, 1600 );
            }break;
        case 2:
            {
                if( mLadySacrolash && mLadySacrolash->IsAlive() )
                    mLadySacrolash->Emote( "Confusion...", Text_Yell );

                ResetTimer( mRespondTimer, 1600 );
            }break;
        case 3:
            {
                Emote( "Hatred...", Text_Yell );
                ResetTimer( mRespondTimer, 1600 );
            }break;
        case 4:
            {
                if( mLadySacrolash && mLadySacrolash->IsAlive() )
                    mLadySacrolash->Emote( "Mistrust...", Text_Yell );

                ResetTimer( mRespondTimer, 1600 );
            }break;
        case 5:
            {
                Emote( "Chaos...", Text_Yell );
                ResetTimer( mRespondTimer, 1600 );
            }break;
        case 6:
            {
                if( mLadySacrolash && mLadySacrolash->IsAlive() )
                    mLadySacrolash->Emote( "These are the hallmarks...", Text_Yell );

                ResetTimer( mRespondTimer, 2600 );
            }break;
        case 7:
            {
                Emote( "These are the pillars...", Text_Yell );
                RemoveTimer( mRespondTimer );
            }break;
        };
    };

    uint32					ConflagrationTimer;
    uint32					BlazeTimer;
    uint32					PyrogenicsTimer;
    uint32					ShadowfuryTimer;
    uint32                  ShadowimageTimer;
    uint32					FlamesearTimer;
    uint8					phase;
    uint32                  BerserkTimer;

    int32					mRespondTimer;
    uint32					mRespond;
    LadySacrolashAI*		mLadySacrolash;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SPELL_SHADOW_IMAGE_VISUAL 45263
#define SPELL_DARKSTRIKE 5271
#define SPELL_SHADOW_FURY 45270

class mob_Shadow_imageAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(mob_Shadow_imageAI, MoonScriptCreatureAI);
    mob_Shadow_imageAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {

    }


    void OnCombatStart(Unit* mTarget)
    {
        ParentClass::OnCombatStart(mTarget);
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->CastSpell(_unit, dbcSpell.LookupEntry(SPELL_SHADOW_IMAGE_VISUAL), true);
        _unit->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );
        ParentClass::OnCombatStop(mTarget);
        Shadow_fury_Timer = 5000 + (rand()%15000);
        Dark_Strike_Timer = 3000;
        KillTimer = 15000;
    }

    void AIUpdate()
    {

        if( Dark_Strike_Timer <= mAIUpdateFrequency )
        {
            Unit* pTarget = _unit->GetAIInterface()->GetMostHated();
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SPELL_DARKSTRIKE, true );
            }
            Dark_Strike_Timer = 3000;
            return;
        }
        else Dark_Strike_Timer -= mAIUpdateFrequency;

        if( Shadow_fury_Timer <= mAIUpdateFrequency )
        {
            Unit* pTarget = _unit->GetAIInterface()->GetMostHated();
            if( pTarget )
            {
                _unit->CastSpell( pTarget, SPELL_SHADOW_FURY, true );
            }
            Shadow_fury_Timer = 10000;
            return;
        }
        else Shadow_fury_Timer -= mAIUpdateFrequency;


        ParentClass::AIUpdate();

    };

    uint32 Dark_Strike_Timer;
    uint32 Shadow_fury_Timer;
    uint32 KillTimer;


};
void SetupBoss_Twins(ScriptMgr* pScriptMgr)
{

    pScriptMgr->register_creature_script(CREATURE_GRAND_WARLOCK_ALYTHESS, &GrandWarlockAlythessAI::Create);
    pScriptMgr->register_creature_script(CREATURE_LADY_SACROLASH, &LadySacrolashAI::Create);
    pScriptMgr->register_creature_script(CREATURE_SHADOW_IMAGE, &mob_Shadow_imageAI::Create);

}