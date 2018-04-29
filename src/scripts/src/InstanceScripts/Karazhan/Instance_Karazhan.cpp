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

enum spell
{
//SPECTRAL STALION
HOOF_STRIKE	                                = 29577,
ABSORB_VITALITY	                            = 29323,
SPECTRAL_CHARGE                             = 29320,
};

enum creatures
{
SPECTRAL_STALION = 15546,
SPECTRAL_CHARGER = 15547,

};

/************************************************************************/
/* Raid_Karazhan.cpp Script												*/
/************************************************************************/

// Partially by Plexor (I used a spell before, but changed to his method)
class Berthold : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4037, Plr);
		
		Menu->AddItem( 0, "What is this place?", 1 );
		Menu->AddItem( 0, "Where is Medivh?", 2 );
		Menu->AddItem( 0, "How do you navigate the tower?", 3 );

		//Killing the Shade of Aran makes a teleport to medivh's available from Berthold the Doorman.
		Unit* soa = pObject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-11165.2f, -1912.13f, 232.009f, 16524);
		if(!soa || !soa->isAlive())
			Menu->AddItem( 0, "Please teleport me to the Guardian's Library.", 4);

		if(AutoSend)
			Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object* pObject, Player * Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch (IntId)
		{
			case 0:
				GossipHello(pObject, Plr, true);
				break;

			case 4:
				Plr->SafeTeleport(Plr->GetMapId(), Plr->GetInstanceID(), -11165.123f, -1911.13f, 232.009f, 2.3255f);
				break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////
//SPECTRAL STALION


class SPECTRAL_STALIONAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SPECTRAL_STALIONAI, MoonScriptCreatureAI);
	SPECTRAL_STALIONAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{        
		init();
	}
	
	void init()
	{
	
	Spectralhoof_strike_Timer = 5000;
	Absorb_vitality_timer     = 11000;
	
	}
	
	void AIUpdate()
	{
	    if( Spectralhoof_strike_Timer <= mAIUpdateFrequency )
		{
			Unit* pTarget = GetCurrentTarget();
			if( pTarget )
			{
				_unit->CastSpell( pTarget, HOOF_STRIKE, true );
			}
            Spectralhoof_strike_Timer = 5000;
			return;
		}
		else Spectralhoof_strike_Timer -= mAIUpdateFrequency;
		
		if( Absorb_vitality_timer <= mAIUpdateFrequency )
		{
			Unit* pTarget = GetCurrentTarget();
			if( pTarget )
			{
				_unit->CastSpell( pTarget, ABSORB_VITALITY, true );
			}
            Absorb_vitality_timer = 11000;
			return;
		}
		else Absorb_vitality_timer -= mAIUpdateFrequency;
		
		ParentClass::AIUpdate();
	}
	uint32 Spectralhoof_strike_Timer;
	uint32 Absorb_vitality_timer;
};
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//SPECTRAL CHARGER
#define SPECTRAL_CHARGER				            15547
#define SPECTRAL_CHARGE	                            29320

void SpellFunc_Spectral_Charger_Charge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class spectral_chargerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(spectral_chargerAI, MoonScriptCreatureAI);
	spectral_chargerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
    init();
	}

    void init()
	{
	Spectral_charge_timer = 5000;
	}
	
	void AIUpdate()
	{
	    if( Spectral_charge_timer <= mAIUpdateFrequency )
		{
			Unit* pTarget = GetRandomPlayer();
			if( pTarget )
			{
				_unit->CastSpell( pTarget, SPECTRAL_CHARGE, true );
			}
            Spectral_charge_timer = 10000;
			return;
		}
		else Spectral_charge_timer -= mAIUpdateFrequency;
		
		ParentClass::AIUpdate();
	}
	uint32 Spectral_charge_timer;
	

    SpellDesc*	mCharge;
};


void SpellFunc_Spectral_Charger_Charge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	spectral_chargerAI *pSpectral_chargerAI = (pCreatureAI != NULL) ? (spectral_chargerAI*)pCreatureAI : NULL;
	if (pSpectral_chargerAI != NULL)
	{
		Unit* pCurrentTarget = pSpectral_chargerAI->GetUnit()->GetAIInterface()->GetNextTarget();
		if (pCurrentTarget != NULL && pCurrentTarget != pTarget)
		{
			pSpectral_chargerAI->GetUnit()->GetAIInterface()->AttackReaction(pTarget, 500);
			pSpectral_chargerAI->GetUnit()->GetAIInterface()->SetNextTarget(pTarget);
			pSpectral_chargerAI->GetUnit()->GetAIInterface()->RemoveThreatByPtr(pCurrentTarget);
		}

		pSpectral_chargerAI->CastSpell(pSpectral_chargerAI->mCharge);
	}
}
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//Concubine
#define CONCUBINE				            16461
#define CONCUBINE_SEDUCE	                29490

class CONCUBINEAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CONCUBINEAI, MoonScriptCreatureAI);
	CONCUBINEAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //Emotes
		AddEmote(Event_OnCombatStart, "They fall asleep after. Me, I fall asleep during....", Text_Yell, 0);
		AddEmote(Event_OnDied, "We Could have had so much Fun!", Text_Yell, 0);
		
        //spells
		AddSpell(CONCUBINE_SEDUCE, Target_RandomPlayer, 100, 0, 15, 0, 10);
	}
};
/////////////////////////////////////////////////////////////////////////////////
//Skeletal Usher//
/////////////////////////////////////////////////////////////////////////////////
//Skeletal Usher
#define SKELETAL_USHER             16471
#define SKELETAL_FROST_SHOCK       29666
#define SKELETAL_ICE_TOMB          29670
#define SKELETAL_MAGNETIC_PULL     29661


class SKELETAL_USHERAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SKELETAL_USHERAI, MoonScriptCreatureAI);
	SKELETAL_USHERAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(SKELETAL_FROST_SHOCK, Target_Current, 100, 0, 15, 0, 22);
		AddSpell(SKELETAL_ICE_TOMB, Target_RandomPlayer, 100, 0, 15, 0, 15);
		AddSpell(SKELETAL_MAGNETIC_PULL, Target_RandomPlayer, 100, 0, 15, 0, 30);
	}
};
/////////////////////////////////////////////////////////////////////////////////
//Spectral Patron
#define SPECTRAL_PATRON            16468
#define SPECTRAL_PATRON_KICK       29560
#define SPECTRAL_PATRON_LEFT_HOOK  29555



class SPECTRAL_PATRONAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SPECTRAL_PATRONAI, MoonScriptCreatureAI);
	SPECTRAL_PATRONAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(SPECTRAL_PATRON_LEFT_HOOK, Target_Current, 100, 0, 15, 0, 10);
		AddSpell(SPECTRAL_PATRON_KICK , Target_RandomPlayer, 100, 0, 15, 0, 15);
	}
};

///////////////////////////////////////////////////////////////////////////////
//Arcane Protector//

//Arcane Protector
#define ARCANE_PROTECTOR                     16504
#define ARCANE_PROTECTOR_FIST_OF_STONE       29560


class ARCANE_PROTECTORAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ARCANE_PROTECTORAI, MoonScriptCreatureAI);
	ARCANE_PROTECTORAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(ARCANE_PROTECTOR_FIST_OF_STONE, Target_Current, 100, 0, 15, 0, 15);
		
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Phantom Guest
#define PHANTOM_GUEST                    16504
#define PHANTOM_CURSE_OF_AGONY           29930
#define PHANTOM_DANCE_VIBE               29521
#define PHANTOM_HOLY_NOVA                29514

class PHANTOM_GUESTAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PHANTOM_GUESTAI, MoonScriptCreatureAI);
	PHANTOM_GUESTAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(PHANTOM_CURSE_OF_AGONY, Target_Current, 100, 0, 15, 0, 10);
		AddSpell(PHANTOM_DANCE_VIBE, Target_Self, 100, 0, 15, 0, 10);
		AddSpell(PHANTOM_HOLY_NOVA, Target_RandomPlayer, 100, 0, 15, 0, 12);
		
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Phantom Valet
#define PHANTOM_VALET                    16408
#define PHANTOM_DEMO_SHOUT               29584

class PHANTOM_VALETAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PHANTOM_VALETAI, MoonScriptCreatureAI);
	PHANTOM_VALETAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(PHANTOM_DEMO_SHOUT, Target_RandomPlayer, 100, 0, 15, 0, 10);
		
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Spectral Servant
#define SPECTRAL_SERVANT                 16408
#define SPECTRAl_CURSE_OB                29540

class SPECTRAL_SERVANTAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SPECTRAL_SERVANTAI, MoonScriptCreatureAI);
	SPECTRAL_SERVANTAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(SPECTRAl_CURSE_OB, Target_RandomPlayer, 100, 0, 15, 0, 10);
		
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Spectral Attendant
#define SPECTRAL_ATTEDANT                 16408
#define SPECTRAl_KICK                     29540
#define SPECTRAL_SHADOW_REJUV             29587

class SPECTRAL_ATTEDANTAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SPECTRAL_ATTEDANTAI, MoonScriptCreatureAI);
	SPECTRAL_ATTEDANTAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(SPECTRAl_KICK, Target_Current, 100, 0, 15, 0, 5);
		AddSpell(SPECTRAL_SHADOW_REJUV, Target_Self, 100, 0, 15, 0, 10);
		
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ghastly Haunt//

#define GHASTLY_HAUNT                     16481
#define GHASTLY_SHADOW_SHOCK              29712


class GHASTLY_HAUNTAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GHASTLY_HAUNTAI, MoonScriptCreatureAI);
	GHASTLY_HAUNTAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(GHASTLY_SHADOW_SHOCK, Target_Current, 100, 0, 15, 0, 10);
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Trapped Soul//

#define TRAPPED_SOUL                    16481
#define TRAPPED_CONE_OF_COLD            29712
#define TRAPPED_ELEMENTAL_ARMOR         29718


class TRAPPED_SOULAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TRAPPED_SOULAI, MoonScriptCreatureAI);
	TRAPPED_SOULAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
        //spells
		AddSpell(TRAPPED_CONE_OF_COLD, Target_Current, 100, 0, 15, 0, 10);
		AddSpell(TRAPPED_ELEMENTAL_ARMOR, Target_Self, 100, 0, 15, 0, 20);
	}
};


void SetupKarazhan(ScriptMgr* pScriptMgr)
{
	GossipScript * KBerthold = (GossipScript*) new Berthold();
	pScriptMgr->register_gossip_script(16153, KBerthold);

	pScriptMgr->register_creature_script(CONCUBINE, &CONCUBINEAI::Create);
	pScriptMgr->register_creature_script(SPECTRAL_PATRON, &SPECTRAL_PATRONAI::Create);
	pScriptMgr->register_creature_script(SKELETAL_USHER, &SKELETAL_USHERAI::Create);
	pScriptMgr->register_creature_script(ARCANE_PROTECTOR, &ARCANE_PROTECTORAI::Create);
	pScriptMgr->register_creature_script(SPECTRAL_SERVANT, &SPECTRAL_SERVANTAI::Create);
	pScriptMgr->register_creature_script(TRAPPED_SOUL, &TRAPPED_SOULAI::Create);
    pScriptMgr->register_creature_script(GHASTLY_HAUNT, &GHASTLY_HAUNTAI::Create);
	pScriptMgr->register_creature_script(SPECTRAL_ATTEDANT, &SPECTRAL_ATTEDANTAI::Create);
	pScriptMgr->register_creature_script(SPECTRAL_STALION, &SPECTRAL_STALIONAI::Create);
	pScriptMgr->register_creature_script(SPECTRAL_CHARGER, &spectral_chargerAI::Create);
	
}
