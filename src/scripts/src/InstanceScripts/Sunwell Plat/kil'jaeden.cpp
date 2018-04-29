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
#include "Setup.h"
#include "Base.h"

static Coords HandSpawn[] = 
{
	{ 1684.7f, 614.42f, 28.058f, 0.698392f },
	{ 1682.9f, 637.75f, 27.9231f, 5.71709f },
	{ 1707.6f, 612.15f, 27.7946f, 1.99037f }
};

static Coords OrbAddSpawn[] =
{
	{ 1678.633179f, 630.463013f, 44.456699f, 4.669198f }, // North East
	{ 1708.744385f, 612.506165f, 43.298355f, 3.412562f }, // Nort West
	{ 1714.956299f, 629.360107f, 42.220497f, 0.148447f }, // South west
	{ 1703.361572f, 642.184021f, 42.416416f, 4.033029f }  // South East
};

typedef std::vector<Creature*> EncounterArray;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EVENT - Sun Well
enum Creatures
{
CREATURE_ANVEENA	            =	26046,
CREATURE_KILJAEDEN			    =   25315,
CREATURE_VOLATILE_FELFIRE_FIEND	=   25598,
CREATURE_HAND_OF_THE_DECEIVER	=	25955,
CREATURE_FELFIRE_PORTAL		    =  	25603,
CREATURE_SHIELD_ORB				=	25502,

};

enum Spells
{

ANVEENAS_PRISON		=		    46367,
SHADOW_CHANNELING	=		    46757,
FELFIRE_FISSION		=			45779,
SHADOW_BOLT_VOLLEY	=			45770,
SHADOW_INFUSION		=			45772,
FELFIRE_PORTAL		=			46875,

//Phase two
SOUL_FLAY			=			45442,
LEGION_LIGHTNING	=			45664,
FIRE_BLOOM			=			45641,

SINISTER_REFLECTION =           45892,

}

class AnveenaAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AnveenaAI, MoonScriptBossAI);
	AnveenaAI(Creature* pCreature) :MoonScriptBossAI(pCreature)
	{
		SetFlyMode( true );
		SetCanMove( false );
		RegisterAIUpdateEvent( 500 );
	};

	void AIUpdate()
	{
		ApplyAura( ANVEENAS_PRISON );
		RemoveAIUpdateEvent();
	};

	void Destroy()
	{
		delete this;
	};
};

class KJDummyAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KJDummyAI, MoonScriptBossAI);
	KJDummyAI(Creature* pCreature) :MoonScriptBossAI(pCreature)
	{
		RestartEncounter();
		_unit->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		mStartCount = 0;
		RegisterAIUpdateEvent( 500 );
	};

	void AIUpdate()
	{
		ApplyAura( 46410 );
		RemoveAIUpdateEvent();
	};

	void UpdateCount()
	{
		mStartCount++;

		if( mStartCount == 3 )
			StartEncounter();
	};

	void StartEncounter()
	{
		RemoveAura( 46410 );
		SpawnCreature( CREATURE_KILJAEDEN, false );
	};

	void RestartEncounter()
	{
		for( vector<Creature*>::iterator CreatureIter = mEncounterArray.begin(); CreatureIter != mEncounterArray.end(); ++CreatureIter )
		{
			Creature* pCreature = (*CreatureIter);
			if( pCreature != NULL )
			{
				if( pCreature->isAlive() )
					pCreature->Despawn( 1000, 0);
				else
					sEventMgr.AddEvent( pCreature, &Creature::SafeDelete, EVENT_CREATURE_REMOVE_CORPSE, 1000, 1, 0);
			}
		};

		mEncounterArray.clear();

		// lets remove adds =>
		for( std::set< Object* >::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr )
		{
			if( (*itr)->IsInWorld() && (*itr)->IsCreature() )
			{
				Creature* pAdd = static_cast<Creature*>(*itr);
				if( pAdd->isAlive() && ( pAdd->GetEntry() ==  CREATURE_FELFIRE_PORTAL || pAdd->GetEntry() == CREATURE_VOLATILE_FELFIRE_FIEND ) )
					pAdd->Despawn( 1000, 0 );
			};
		};

		MoonScriptCreatureAI* Anveena = SpawnCreature(CREATURE_ANVEENA, 1698.550049f, 628.033020f, 72.541901f, 3.864760f);
		if( Anveena && Anveena->GetUnit() )
			AddEncounterCreature( Anveena->GetUnit() );

		for( int i = 0; i < 3; ++i )
		{
			MoonScriptCreatureAI* pHand = SpawnCreature( CREATURE_HAND_OF_THE_DECEIVER, HandSpawn[i].mX, HandSpawn[i].mY, HandSpawn[i].mZ, HandSpawn[i].mO );
			if( pHand && pHand->GetUnit() )
				AddEncounterCreature( pHand->GetUnit() );
		};
	
		mStartCount = 0;
		ApplyAura( 46410 );
	};

	void AddEncounterCreature( Creature* pCreature )
	{
		mEncounterArray.push_back( pCreature );
	};

	EncounterArray::iterator GetEncounterVectorBegin()
	{
		return mEncounterArray.begin();
	};

	EncounterArray::iterator GetEncounterVectorEnd()
	{
		return mEncounterArray.end();
	};

	void Destroy()
	{
		delete this;
	};

private:
	uint32			mStartCount;
	EncounterArray	mEncounterArray;
};

class HandOfTheDeceiverAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HandOfTheDeceiverAI, MoonScriptBossAI);
	HandOfTheDeceiverAI(Creature* pCreature) :MoonScriptBossAI(pCreature)
	{
		AddSpell( FELFIRE_PORTAL, Target_Self, 40, 0, 12 );
		AddSpell( SHADOW_BOLT_VOLLEY, Target_Self, 50, 0, 5 );

		Unit* pAvneena = ForceCreatureFind( CREATURE_ANVEENA, 1698.202393f, 629.104187f, 69.121910f );
		if( pAvneena )
		{	
			SetTargetToChannel( pAvneena, SHADOW_CHANNELING );
		};

		mEnraged = false;
	};

	void OnCombatStart( Unit* pTarget )
	{
		SetTargetToChannel( NULL, 0 );
		ParentClass::OnCombatStart(pTarget);

		KJDummyAI* pKjDummy = static_cast<KJDummyAI*>( GetNearestCreature( CREATURE_KIL_JADEN_DUMMY ) );
		if( pKjDummy )
		{
			// chain aggro
			for( EncounterArray::iterator itr = pKjDummy->GetEncounterVectorBegin(); itr != pKjDummy->GetEncounterVectorEnd(); ++itr )
			{
				if( (*itr) && (*itr)->GetEntry() == CREATURE_HAND_OF_THE_DECEIVER && (*itr)->isAlive() && pTarget )
					(*itr)->GetAIInterface()->AttackReaction( pTarget, 1 );
			};
		};
	};

	void AIUpdate()
	{
		if( mEnraged == false && GetHealthPercent() <= 20 )
		{
			ApplyAura(SHADOW_INFUSION);
			mEnraged = true;
		};

		ParentClass::AIUpdate();
	};

	void OnDied(Unit* mKiller)
	{
		KJDummyAI* pKjDummy = static_cast<KJDummyAI*>( GetNearestCreature( CREATURE_KIL_JADEN_DUMMY ) );
		if( pKjDummy )
			pKjDummy->UpdateCount();

		ParentClass::OnDied(mKiller);
	};

	void OnCombatStop(Unit* pTarget)
	{
		KJDummyAI* pKjDummy = static_cast<KJDummyAI*>( GetNearestCreature( CREATURE_KIL_JADEN_DUMMY ) );
		if( pKjDummy )
			pKjDummy->RestartEncounter();

		ParentClass::OnCombatStop(pTarget);
	};

	void Destroy()
	{
		delete this;
	};

	bool		mEnraged;
};

class FellFireFiendAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FellFireFiendAI, MoonScriptCreatureAI);
	FellFireFiendAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		_unit->GetAIInterface()->SetAllowedToEnterCombat( false );
		Player* randPlr = static_cast<Player*>(GetBestPlayerTarget());
		Coords _coord;
		_coord.mX = randPlr->GetPositionX();
		_coord.mY = randPlr->GetPositionY();
		_coord.mZ = randPlr->GetPositionZ();
		_coord.mO = randPlr->GetOrientation();
		AddWaypoint( CreateWaypoint( 1, 0, Flag_Run, _coord ) );

		mForceDied = false;
	};

	void OnReachWP( uint32 iWaypointId, bool bForwards )
	{
		ApplyAura( FELFIRE_FISSION );
		mForceDied = true;
		_unit->DealDamage( _unit, 3000, 0, 0, 0, true );
	};

	void OnDied( Unit* pKiller )
	{
		ParentClass::OnDied( pKiller );
		if( !mForceDied )
			ApplyAura( FELFIRE_FISSION );

		Despawn( 10000, 0 );
	};

	void Destroy()
	{
		delete this;
	};

	bool mForceDied;
};

class FellfirePortalAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FellfirePortalAI, MoonScriptCreatureAI);
	FellfirePortalAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		_unit->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );
		_unit->GetAIInterface()->SetAllowedToEnterCombat( false );
		RegisterAIUpdateEvent( 100 );
	};

	void AIUpdate()
	{
		ApplyAura( 46464 );
		RemoveAIUpdateEvent();
		Despawn( 7000, 0 );
	};

	void Destroy()
	{
		delete this;
	};
};

#define CREATURE_SHIELD_ORB					25502
#define SHIELD_ORB_SHADOWBOLT			45680


//Phase 3
#define SINISTER_REFLECTION				45892
#define SHADOW_SPIKE					45885
#define FLAME_DART_EXPLOSION			45746
#define DARKNESS_OF_A_THOUSAND_SOULS	45657

//Phase 4
#define CREATURE_ARMAGEDDON					25735

//Phase 5
#define SACRIFICE_OF_ANVEENA			46474

class KilJaedenAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(KilJaedenAI, MoonScriptBossAI);
	KilJaedenAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{	
		SetCanEnterCombat( false );

		DelayNextAttack( 11600 );
		SetAllowMelee( false );
		SetAllowSpell( false );
		SetAllowTargeting( false );
		SetCanMove( false );
		TriggerCooldownOnAllSpells(); 

		SetCanMove( false );
		Emote( "The expendible have perished... So be it! Now I shall succeed where Sargeras could not! I will bleed this wretched world and secure my place as the true master of the Burning Legion. The end has come! Let the unraveling of this world commence!", Text_Yell, 12500 );
		mStarted = false;
		RegisterAIUpdateEvent( 100 );

		pKJDummy = static_cast<KJDummyAI*>( GetNearestCreature( CREATURE_KIL_JADEN_DUMMY ) );
		if( pKJDummy )
			pKJDummy->AddEncounterCreature( _unit );

		AddPhaseSpell( 1, AddSpell( SOUL_FLAY, Target_Current, 50, 0, 6 ) );
		AddPhaseSpell( 1, AddSpell( LEGION_LIGHTNING, Target_RandomPlayer, 25, 2.0f, 8 ) );
		AddPhaseSpell( 1, AddSpell( FIRE_BLOOM, Target_RandomPlayer, 20, 0, 10 ) );
	};

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart( pTarget );
		SetCanMove( false );
	};

	void AIUpdate()
	{
		if( mStarted == false ) 
		{
			ApplyAura( 44200 );
			ApplyAura( 26586 );
			mStarted = true;
			RegisterAIUpdateEvent( 11500 );
			return;
		}

		if( mStarted == true && GetCanEnterCombat() == false ) //start part 
		{
			SetCanEnterCombat( true );
			SetAllowMelee( true );
			SetAllowSpell( true );
			SetAllowTargeting( true );
			SetCanMove( false );
			AggroNearestUnit( 1 );
		};

		ParentClass::AIUpdate();
	};

	void OnCombatStop( Unit* pTarget )
	{
		if( pKJDummy )
			pKJDummy->RestartEncounter();
	};

private:
	bool		mStarted;
	KJDummyAI*  pKJDummy;
	int32		mFireBloomTimer;
};

#define ORB_OF_BLUE						188415
#define POSSES_DRAGON					45839

class OrbOfBlueGO : public GameObjectAIScript
{
public:
	static GameObjectAIScript *Create(GameObject* GO) { return new OrbOfBlueGO(GO); }
	OrbOfBlueGO(GameObject* goinstance) : GameObjectAIScript(goinstance) 
	{
		//_gameobject->Update( 1 );
		_gameobject->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
	}
	
	void OnActivate(Player* pPlayer)
	{
		Creature* _unit = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature( 25653, _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), _gameobject->GetOrientation(), false, true, 0, 0 ); 
	};
};

class BlueDragonAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(BlueDragonAI, MoonScriptCreatureAI);
	BlueDragonAI(Creature* pCreature) : MoonScriptCreatureAI( pCreature )
	{	
		AI_Spell* NewSpell = new AI_Spell;
		NewSpell->spell = dbcSpell.LookupEntry( 45848 );
		NewSpell->agent = AGENT_SPELL;
		NewSpell->entryId = _unit->GetEntry();
		NewSpell->maxrange = GetMaxRange( dbcSpellRange.LookupEntry( NewSpell->spell->rangeIndex ) );
		NewSpell->minrange = GetMinRange( dbcSpellRange.LookupEntry( NewSpell->spell->rangeIndex ) );
		NewSpell->spelltargetType = TTYPE_SINGLETARGET;
		NewSpell->spellType = STYPE_DAMAGE;
		NewSpell->cooldown = objmgr.GetPetSpellCooldown( NewSpell->spell->Id );
		NewSpell->cooldowntime = 0;
		NewSpell->autocast_type = AUTOCAST_EVENT_NONE;
		NewSpell->floatMisc1 = 0;
		NewSpell->Misc2 = 0;
		NewSpell->procChance = 0;
		NewSpell->procCount=0;
		_unit->GetAIInterface()->addSpellToList( NewSpell );

		NewSpell = new AI_Spell;
		NewSpell->spell = dbcSpell.LookupEntry( 45856 );
		NewSpell->agent = AGENT_SPELL;
		NewSpell->entryId = _unit->GetEntry();
		NewSpell->maxrange = GetMaxRange( dbcSpellRange.LookupEntry( NewSpell->spell->rangeIndex ) );
		NewSpell->minrange = GetMinRange( dbcSpellRange.LookupEntry( NewSpell->spell->rangeIndex ) );
		NewSpell->spelltargetType = TTYPE_CASTER;
		NewSpell->spellType = STYPE_BUFF;
		NewSpell->cooldown = objmgr.GetPetSpellCooldown( NewSpell->spell->Id );
		NewSpell->cooldowntime = 0;
		NewSpell->autocast_type = AUTOCAST_EVENT_NONE;
		NewSpell->floatMisc1 = 0;
		NewSpell->Misc2 = 0;
		NewSpell->procChance = 0;
		NewSpell->procCount=0;
		_unit->GetAIInterface()->addSpellToList( NewSpell );
	
		NewSpell = new AI_Spell;
		NewSpell->spell = dbcSpell.LookupEntry( 45860 );
		NewSpell->agent = AGENT_SPELL;
		NewSpell->entryId = _unit->GetEntry();
		NewSpell->maxrange = GetMaxRange( dbcSpellRange.LookupEntry( NewSpell->spell->rangeIndex ) );		
		NewSpell->minrange = GetMinRange( dbcSpellRange.LookupEntry( NewSpell->spell->rangeIndex ) );
		NewSpell->spelltargetType = TTYPE_SINGLETARGET;
		NewSpell->spellType = STYPE_BUFF;
		NewSpell->cooldown = objmgr.GetPetSpellCooldown( NewSpell->spell->Id );
		NewSpell->cooldowntime = 0;
		NewSpell->autocast_type = AUTOCAST_EVENT_NONE;
		NewSpell->floatMisc1 = 0;
		NewSpell->Misc2 = 0;
		NewSpell->procChance = 0;
		NewSpell->procCount=0;
		_unit->GetAIInterface()->addSpellToList( NewSpell );

		NewSpell = new AI_Spell;
		NewSpell->spell = dbcSpell.LookupEntry( 45862 );
		NewSpell->agent = AGENT_SPELL;
		NewSpell->entryId = _unit->GetEntry();
		NewSpell->maxrange = GetMaxRange( dbcSpellRange.LookupEntry( NewSpell->spell->rangeIndex ) );		
		NewSpell->minrange = GetMinRange( dbcSpellRange.LookupEntry( NewSpell->spell->rangeIndex ) );
		NewSpell->spelltargetType = TTYPE_SINGLETARGET;
		NewSpell->spellType = STYPE_BUFF;
		NewSpell->cooldown = objmgr.GetPetSpellCooldown( NewSpell->spell->Id );
		NewSpell->cooldowntime = 0;
		NewSpell->autocast_type = AUTOCAST_EVENT_NONE;
		NewSpell->floatMisc1 = 0;
		NewSpell->Misc2 = 0;
		NewSpell->procChance = 0;
		NewSpell->procCount=0;
		_unit->GetAIInterface()->addSpellToList( NewSpell );

		RegisterAIUpdateEvent( 500 );
	};

	void AIUpdate()
	{
		GetNearestPlayer()->Possess( GetUnit() );
		GetUnit()->CastSpell( GetNearestPlayer(), POSSES_DRAGON, true ); 
	};
};

void SetupKJScript(ScriptMgr* pScriptMgr)
{
	pScriptMgr->register_creature_script( CREATURE_ANVEENA, &AnveenaAI::Create );
	pScriptMgr->register_creature_script( CREATURE_KIL_JADEN_DUMMY, &KJDummyAI::Create );
	pScriptMgr->register_creature_script( CREATURE_KILJAEDEN, &KilJaedenAI::Create );
	pScriptMgr->register_creature_script( CREATURE_HAND_OF_THE_DECEIVER, &HandOfTheDeceiverAI::Create );
	pScriptMgr->register_creature_script( CREATURE_VOLATILE_FELFIRE_FIEND, &FellFireFiendAI::Create );
	pScriptMgr->register_creature_script( CREATURE_FELFIRE_PORTAL, &FellfirePortalAI::Create );

	pScriptMgr->register_creature_script( 25653, &BlueDragonAI::Create );
	pScriptMgr->register_gameobject_script( ORB_OF_BLUE, &OrbOfBlueGO::Create );
}