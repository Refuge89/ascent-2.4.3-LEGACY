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


// Prince Malchezaar
#define CN_MALCHEZAAR			15690
#define CN_INFERNAL				17646
#define CN_AXES					17650
#define CN_DUMMY				17644		

#define ENFEEBLE				30843
#define SHADOWNOVA				30852
#define SW_PAIN					30854
#define THRASH_AURA				12787
#define SUNDER_ARMOR			25225
#define AMPLIFY_DMG				39095 // old 12738
#define SUMMON_AXES				30891
#define WIELD_AXES				30857

// Extra creature info
#define INF_RAIN				33814
#define HELLFIRE				39131
#define DEMONIC_FRENZY			32851

// Item model info
#define AXE_ITEM_MODEL			40066
#define AXE_ITEM_INFO			33488898
#define AXE_ITEM_SLOT			768
/* Emotes:
SPECIAL? - 9223 - 9320
AXETOSS2? - 9317
*/

class MalchezaarAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MalchezaarAI);
	bool m_spellcheck[9];
	SP_AI_Spell spells[9];

	MalchezaarAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
	
		m_phase = 1;
		nrspells = 5;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SW_PAIN);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 15;	
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;
		
		spells[1].info = dbcSpell.LookupEntry(ENFEEBLE);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 25;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(INF_RAIN);
		spells[2].targettype = TARGET_DESTINATION;
		spells[2].instant = true;
		spells[2].cooldown = 43;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		
		spells[3].info = dbcSpell.LookupEntry(SUNDER_ARMOR);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = 15;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(AMPLIFY_DMG);
		spells[4].targettype = TARGET_RANDOM_SINGLE;
		spells[4].instant = true;
		spells[4].cooldown = 20;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;
		spells[4].mindist2cast = 0.0f;
		
		spells[5].info = dbcSpell.LookupEntry(SHADOWNOVA);
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = false;
		spells[5].cooldown = 4;	
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 2000;

		spells[6].info = dbcSpell.LookupEntry(THRASH_AURA);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = true;
		spells[6].cooldown = 1;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;

		spells[7].info = dbcSpell.LookupEntry(WIELD_AXES);
		spells[7].targettype = TARGET_SELF;
		spells[7].instant = true;
		spells[7].attackstoptimer = 1000;

		spells[8].info = dbcSpell.LookupEntry(SUMMON_AXES);
		spells[8].targettype = TARGET_SELF;
		spells[8].instant = true;
		spells[8].attackstoptimer = 1000;

		// Dummy initialization
		float dumX = -10938.56f;
		float dumY = -2041.26f;
		float dumZ = 305.132f;
		InfernalDummy = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_DUMMY, dumX, dumY, dumZ, 0, true, false, 0, 0);

		// Door initialization
		MDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-11018.5f, -1967.92f, 276.652f, 185134);
	}

	void OnCombatStart(Unit * mTarget)
	{
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		uint32 t = (uint32)time(NULL);
		spells[0].casttime = t + spells[0].cooldown;
		spells[1].casttime = t + spells[1].cooldown;
		spells[2].casttime = t + spells[2].cooldown;
		
		m_spawn_infernal = 0;
		m_infernal = false;

		_unit->PlaySoundToSet(9218);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Madness has brought you here to me. I shall be your undoing.");
		RegisterAIUpdateEvent(1000);

		if(MDoor != NULL)
		{
			MDoor->SetUInt32Value(GAMEOBJECT_STATE, 1);
			MDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
		}
	}

	void OnCombatStop(Unit * mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		// Reset weapon
		_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 0);
		_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 0);
		_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 0);

		// Off hand weapon
		_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 0);
		_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 0);
		_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 0);
		
		CreatureProto * cp = CreatureProtoStorage.LookupEntry(CN_MALCHEZAAR);
		if(!cp)
			return;

		_unit->SetFloatValue(UNIT_FIELD_MINDAMAGE, cp->MinDamage);
		_unit->SetFloatValue(UNIT_FIELD_MAXDAMAGE, cp->MaxDamage);

		for(int i = 0; i < 5; ++i)
			Enfeeble_Targets[i] = 0;

		InfernalDummy->Despawn(10000, 0);

		// Open door
		if(MDoor != NULL)
			MDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		
		Creature *MAxes = NULL;
		MAxes = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_AXES);
		if(MAxes != NULL)
			MAxes->Despawn(1000, 0);		
	}

	void OnDied(Unit * mKiller)
	{
		_unit->PlaySoundToSet(9221);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I refuse to concede defeat. I am a prince of the Eredar! I am...");
		RemoveAIUpdateEvent();

		if(InfernalDummy)
			InfernalDummy->Despawn(10000, 0);

		Creature *MAxes = NULL;
		MAxes = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), 
			_unit->GetPositionZ(), CN_AXES);
		if(MAxes)
			MAxes->Despawn(1000, 0);

		// Open door
		if(MDoor)
			MDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}

	void OnTargetDied(Unit * mTarget)
	{
		switch(rand()%3)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are, but a plaything, unfit even to amuse.");
			_unit->PlaySoundToSet(9319);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your greed, your foolishness has brought you to this end.");
			_unit->PlaySoundToSet(9318);
			break;

		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Surely you did not think you could win.");
			_unit->PlaySoundToSet(9222);
			break;
		}
	}

	void AIUpdate()
	{	
		switch(m_phase)
		{
		case 1:
			if(_unit->GetHealthPct() <= 60)
				PhaseTwo();
			break;
		case 2:
			PhaseTwo();
			if(_unit->GetHealthPct() <= 30)
				PhaseThree();
			break;
		case 3:
			PhaseThree();
			break;
		default:
			m_phase = 1;
			break;
		}
		uint32 t = (uint32)time(NULL);
		if(t > spells[1].casttime && _unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			Enfeebler();
			spells[1].casttime = t + spells[1].cooldown;
			spells[5].casttime = t + spells[5].cooldown;
		}
		else if(t > m_spawn_infernal && m_infernal == true && _unit->GetAIInterface()->GetNextTarget())
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_INFERNAL, ranX, ranY, 276.0f, 0, true, false, 0, 0);
			m_spawn_infernal = 0;
			m_infernal = false;
		}
		else if(t > spells[5].casttime && _unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			spells[5].casttime = -1;
			_unit->CastSpell(_unit, spells[5].info, spells[5].instant);
			m_enfeebleoff = t + 3;
		}
		else if(t > m_enfeebleoff)
		{
			EnfeebleOff();
			m_enfeebleoff = -1;
		}
		else if(t > spells[2].casttime)
		{
			SummonInfernal();
			if(m_phase==3)
				spells[2].casttime = t + 20;
			else
				spells[2].casttime = t + spells[2].cooldown;
		}
		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void PhaseTwo()
	{
		if(_unit->GetHealthPct() <= 60 && m_phase == 1)
		{
			uint32 t = (uint32)time(NULL);
			spells[0].casttime = -1;
			spells[3].casttime = t + spells[3].cooldown;
			spells[3].perctrigger = 50.0f;
			
			_unit->PlaySoundToSet(9220);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time is the fire in which you'll burn!");
			
			_unit->CastSpell(_unit, spells[6].info, spells[6].instant);
			_unit->CastSpell(_unit, spells[7].info, spells[6].instant);

			// Main hand weapon
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, AXE_ITEM_MODEL);
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, AXE_ITEM_INFO);
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, AXE_ITEM_SLOT);

			// Off hand weapon
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, AXE_ITEM_MODEL);
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, AXE_ITEM_INFO);
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, AXE_ITEM_SLOT);

			CreatureProto * cp = CreatureProtoStorage.LookupEntry(CN_MALCHEZAAR);
			if(!cp)
				return;

			_unit->SetFloatValue(UNIT_FIELD_MINDAMAGE, 1.5f*cp->MinDamage);
			_unit->SetFloatValue(UNIT_FIELD_MAXDAMAGE, 1.5f*cp->MaxDamage);

			m_phase = 2;
		}
		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void PhaseThree()
	{
		if(_unit->GetHealthPct() <= 30 && m_phase == 2)
		{
			uint32 t = (uint32)time(NULL);

			spells[0].targettype = TARGET_RANDOM_SINGLE;
			spells[0].casttime = t + spells[0].cooldown;

			spells[1].casttime = -1;
			spells[1].perctrigger = 0.0f;
			
			spells[4].casttime = t + spells[4].cooldown;
			spells[4].perctrigger = 50.0f;

			_unit->CastSpell(_unit, spells[8].info, spells[8].instant);
			
			_unit->RemoveAura(THRASH_AURA);
			_unit->RemoveAura(WIELD_AXES);
			
			_unit->PlaySoundToSet(9321);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "How can you hope to withstand against such overwhelming power?");
			
			// Main hand weapon
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 0);
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 0);
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 0);

			// Off hand weapon
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 0);
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 0);
			_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 0);

			CreatureProto * cp = CreatureProtoStorage.LookupEntry(CN_MALCHEZAAR);
			if(!cp)
				return;

			_unit->SetFloatValue(UNIT_FIELD_MINDAMAGE, cp->MinDamage);
			_unit->SetFloatValue(UNIT_FIELD_MAXDAMAGE, cp->MaxDamage);
			m_phase = 3;
		}
		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void SummonInfernal()
	{
		switch(rand()%2)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You face not Malchezaar alone, but the legions I command!");
			_unit->PlaySoundToSet(9322);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All realities, all dimensions are open to me!");
			_unit->PlaySoundToSet(9224);
			break;
		}

		ranX = RandomFloat(113.47f)-11019.37f;
		ranY = RandomFloat(36.951f)-2011.549f;
		InfernalDummy->CastSpellAoF(ranX, ranY, 275.0f, spells[2].info, spells[2].instant); // Shoots the missile
		float dist = InfernalDummy->CalcDistance(ranX, ranY, 275.0f);
		uint32 dtime = (uint32)(dist/spells[2].info->speed);
		m_spawn_infernal = (uint32)time(NULL) + dtime+1;
		m_infernal = true;
	}

	void Enfeebler()
	{
		std::vector<Player*> Targets;
		std::set<Player*>::iterator Itr = _unit->GetInRangePlayerSetBegin();
		std::advance(Itr, 1);
		for( ; Itr != _unit->GetInRangePlayerSetEnd(); ++Itr)
		{
			if(isHostile(_unit, (*Itr)) && (*Itr)->GetInstanceID() == _unit->GetInstanceID())
			{
				Player *RandomTarget = NULL;
				RandomTarget = static_cast< Player* >(*Itr);

				if(RandomTarget->isAlive() && isHostile(_unit, RandomTarget))
					Targets.push_back(RandomTarget);
			}
		}

		while(Targets.size() > 5)
			Targets.erase(Targets.begin()+rand()%Targets.size());

		int i = 0;
		for(std::vector<Player*>::iterator E_Itr = Targets.begin(); E_Itr != Targets.end(); ++E_Itr)
		{
			if(*E_Itr && (*E_Itr)->GetGUID() != _unit->GetAIInterface()->GetMostHated()->GetGUID())
			{
				Enfeeble_Targets[i] = (*E_Itr)->GetGUID();
				Enfeeble_Health[i] = (*E_Itr)->GetUInt32Value(UNIT_FIELD_HEALTH);

				_unit->CastSpell((*E_Itr), spells[1].info, spells[1].instant);
				(*E_Itr)->SetUInt32Value(UNIT_FIELD_HEALTH, 1);
			}
		}
	}

	void EnfeebleOff()
	{
		for(int i = 0; i < 5; ++i)
		{
			Unit *ETarget = _unit->GetMapMgr()->GetUnit(Enfeeble_Targets[i]);
			if(ETarget && ETarget->isAlive())
				ETarget->SetUInt64Value(UNIT_FIELD_HEALTH, Enfeeble_Health[i]);
			Enfeeble_Targets[i] = 0;
			Enfeeble_Health[i] = 0;
		}
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			std::vector<Player *> TargetTable;
			for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
			{ 
				Player* RandomTarget = NULL;
				RandomTarget = static_cast< Player* >(*itr);

				if (RandomTarget && RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast)
					TargetTable.push_back(RandomTarget);
			}

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:
	float ranX;
	float ranY;
	int nrspells;
	int m_phase;
	bool m_infernal;
	uint32 m_enfeebleoff;
	uint32 m_spawn_infernal;
	uint64 Enfeeble_Targets[5];
	uint64 Enfeeble_Health[5];
	Creature *InfernalDummy;
	GameObject *MDoor;
};

class NetherInfernalAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NetherInfernalAI);
	SP_AI_Spell spells[1];

	NetherInfernalAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->Root();
		_unit->DisableAI();
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;
		_unit->Despawn(175000, 0);
		RegisterAIUpdateEvent(6000);
		
		spells[0].info = dbcSpell.LookupEntry(HELLFIRE);
		spells[0].instant = false;
		spells[0].cooldown = 6;
		spells[0].casttime = (uint32)time(NULL) + spells[0].cooldown;

		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
	}

	void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if(t > spells[0].casttime)
		{
			_unit->CastSpell(_unit, spells[0].casttime, spells[0].instant);
			spells[0].casttime = t + spells[0].cooldown;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class InfernalDummyAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(InfernalDummyAI);
	InfernalDummyAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, 768));
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = -10938.56f;
		wp->y = -2041.26f;
		wp->z = 305.132f;
		wp->o = 0;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}

	void Destroy()
	{
		delete this;
	}
};

class MAxesAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MAxesAI);
	SP_AI_Spell spells[1];

	MAxesAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		
		spells[0].info = dbcSpell.LookupEntry(DEMONIC_FRENZY);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 1;
	}

	void OnCombatStart(Unit *mTarget)
	{
		RegisterAIUpdateEvent(6000);

		spells[0].casttime = (uint32)time(NULL) + spells[0].cooldown;

		std::vector<Unit *> TargetTable;
		for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
		{
			if(isHostile(_unit, (*itr)) && (*itr)->isAlive())
			{
				Player* RandomTarget = NULL;
				RandomTarget = static_cast< Player* >(*itr);

				if (RandomTarget && RandomTarget->isAlive() && isHostile(_unit, RandomTarget))
					TargetTable.push_back(RandomTarget);
			}
		}

		if (!TargetTable.size())
			return;

		size_t RandTarget = rand()%TargetTable.size();

		Unit * RTarget = TargetTable[RandTarget];

		if (!RTarget)
			return;

		_unit->GetAIInterface()->taunt(RTarget, true);
	}
	
	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (t > spells[0].casttime)
		{
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
			spells[0].casttime = t + spells[0].cooldown;
		}
	}
};
void SetupBossPrince_Malchezaar(ScriptMgr* pScriptMgr)
{

pScriptMgr->register_creature_script(CN_MALCHEZAAR, &MalchezaarAI::Create);
pScriptMgr->register_creature_script(CN_INFERNAL, &NetherInfernalAI::Create);
pScriptMgr->register_creature_script(CN_DUMMY, &InfernalDummyAI::Create);
pScriptMgr->register_creature_script(CN_AXES, &MAxesAI::Create);

}

