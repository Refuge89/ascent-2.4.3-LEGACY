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

// Netherspite
#define CN_NETHERSPITE		15689
#define CN_VOIDZONE			17470

// #define NETHERBURN		30523 not aura
// #define VOIDZONE			28863
#define CONSUMPTION			32251 // used by void zone
#define NETHERBREATH		38524 // old 36631
#define N_BERSERK			38688
#define NETHERBURN			30522

class NetherspiteAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NetherspiteAI);
	bool m_spellcheck[3];
	SP_AI_Spell spells[3];

	NetherspiteAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
	
	   
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}	

		spells[0].info = dbcSpell.LookupEntry(NETHERBREATH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = RandomUInt(5)+30;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(N_BERSERK);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 540;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
		
		spells[2].info = dbcSpell.LookupEntry(NETHERBURN);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;

		NDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-11186.2f, -1665.14f, 281.398f, 185521);
	}

	void OnCombatStart(Unit* mTarget)
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;

		uint32 t = (uint32)time(NULL);
		VoidTimer = t + 25;
		_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

		RegisterAIUpdateEvent(1000);

		if(NDoor)
		{
			NDoor->SetUInt32Value(GAMEOBJECT_STATE, 1);
			NDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
		}
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->RemoveAura(NETHERBURN);

		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		if(NDoor)
			NDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();

		if(NDoor)
			NDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}

	void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if(t > VoidTimer && _unit->GetAIInterface()->GetNextTarget())
		{
			VoidTimer = t + 20;
			std::vector<Unit *> TargetTable;
			for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
			{ 
				Unit* RandomTarget = NULL;
				RandomTarget = static_cast< Unit* >(*itr);

				if (RandomTarget && RandomTarget->isAlive() && isHostile(_unit, (*itr)))
					TargetTable.push_back(RandomTarget);
			}

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;
			float vzX = 5 * cos(RandomFloat(6.28f))+RTarget->GetPositionX();
			float vzY = 5 * cos(RandomFloat(6.28f))+RTarget->GetPositionY();
			float vzZ = RTarget->GetPositionZ();
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_VOIDZONE, vzX, vzY, vzZ, 0, true, false, 0, 0);
			TargetTable.clear();
		}

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
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
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
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

protected:
	int nrspells;
	uint32 VoidTimer;
	GameObject *NDoor;
};

class VoidZoneAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(VoidZoneAI);
	SP_AI_Spell spells[1];

	VoidZoneAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->Root();
		_unit->DisableAI();
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;
		_unit->Despawn(30000, 0);

		RegisterAIUpdateEvent(2000);

		spells[0].info = dbcSpell.LookupEntry(CONSUMPTION);
		spells[0].instant = true;
		spells[0].cooldown = 2;
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
};

void SetupBossNetherSpite(ScriptMgr* pScriptMgr)
{

pScriptMgr->register_creature_script(CN_NETHERSPITE, &NetherspiteAI::Create);
pScriptMgr->register_creature_script(CN_VOIDZONE, &VoidZoneAI::Create);

}