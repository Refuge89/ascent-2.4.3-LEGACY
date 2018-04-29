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


// Terestian Illhoof
#define CN_ILLHOOF					15688

#define SHADOW_BOLT					19729
// #define S_DEMONCHAINS				30120
#define S_KILREK						30066
// #define F_PORTAL1					30179
// #define F_PORTAL2					30171
#define I_ENRAGE					32964
#define SACRIFICE					30115

// Kil'Rek
#define CN_KILREK					17229
#define AMPLIFY_FLAMES				30053
#define BROKEN_PACT					30065

// Imps
#define CN_FIENDISH_IMP				17267
#define FIREBALL_IMP				31620

// Demon Chains
#define	CN_DEMONCHAINS				17248
#define	CHAINS_VISUAL				30206

#define CN_FPORTAL					17265

class IllhoofAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(IllhoofAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	IllhoofAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
		for(int i = 0; i < nrspells; i++)
			m_spellcheck[i] = false;

		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 5;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(I_ENRAGE);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 600;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
/*
		spells[2].info = dbcSpell.LookupEntry(S_DEMONCHAINS);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].cooldown = 45;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
*/
	}

	void OnCombatStart(Unit* mTarget)
	{
		uint32 t = (uint32)time(NULL);
		for(int i = 0; i < nrspells; i++)
			spells[i].casttime = spells[i].cooldown;

		DemonChain = t + 45;
		ImpTimer = t + 10;
		ReSummon = false;
		
		_unit->PlaySoundToSet(9260);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah, you're just in time. The rituals are about to begin.");
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		clean();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		clean();
		_unit->PlaySoundToSet(9262);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "My life, is yours. Oh great one.");
		RemoveAIUpdateEvent();
	}

	void clean()
	{
		Creature* portal = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-11249.51f, -1702.182f, 179.237f, CN_FPORTAL);
		Creature* portal2 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-11239.534f, -1715.338f, 179.237f, CN_FPORTAL);

		if(portal != NULL && portal2 != NULL)
		{
			portal->SafeDelete();
			portal2->SafeDelete();
		}
	}

	void OnTargetDied(Unit* mTarget)
	{
		uint32 sound = 0;
		char *text = 0;
		switch(RandomUInt(1))
		{
			case 0:
				sound = 9264;
				text = "Your blood will anoint my circle.";
				break;
			case 1:
				sound = 9329;
				text = "The great one will be pleased.";
				break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);

		uint32 t = (uint32)time(NULL);
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			if(t > ImpTimer)
			{
				spawnSummoningPortals();
				ImpTimer = -1;
			}
			if(t > DemonChain)
			{
				DemonChain = t + 45;
				PlrSacrifice();
			}
			if(_unit->HasAura(BROKEN_PACT) && !ReSummon)
			{
				ReSummon = true;
				ReKilrek = t + 45;
			}
			else if(ReSummon && t > ReKilrek)
			{
				ReSummon = false;
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(S_KILREK), true);
				_unit->RemoveAura(BROKEN_PACT);
			}
		}
	}

	void spawnSummoningPortals()
	{
		switch(rand()%2)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Come, you dwellers in the dark. Rally to my call!");
			_unit->PlaySoundToSet(9265);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Gather, my pets. There is plenty for all.");
			_unit->PlaySoundToSet(9331);
			break;
		}
		
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FPORTAL, -11249.51f, -1702.182f, 179.237f, 0, false, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FPORTAL, -11239.534f, -1715.338f, 179.237f, 0, false, false, 0, 0);
	}

	void PlrSacrifice()
	{
		switch(rand()%2)
		{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Please, accept this humble offering, oh great one.");
				_unit->PlaySoundToSet(9263);
				break;

			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Let the sacrifice serve his testament to my fealty.");
				_unit->PlaySoundToSet(9330);
				break;
		}

		std::vector<Player *> TargetTable;
		std::set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin();
		std::advance(itr, 1);
		for(; itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
		{
			if(isHostile(_unit, (*itr)) && (*itr)->GetInstanceID() == _unit->GetInstanceID())
			{
				Player* RandomTarget = NULL;
				RandomTarget = static_cast< Player* >(*itr);
				if (RandomTarget && RandomTarget->isAlive() && isHostile(_unit, RandomTarget))
					TargetTable.push_back(RandomTarget);
				RandomTarget = NULL;
			}
		}
		if (!TargetTable.size())
			return;

		size_t RandTarget = rand()%TargetTable.size();
		Unit * RTarget = TargetTable[RandTarget];

		if (!RTarget) return;

		_unit->CastSpell(RTarget, dbcSpell.LookupEntry(SACRIFICE), false);

		TargetTable.clear();

		float dcX = -11234.7f;
		float dcY = -1698.73f;
		float dcZ = 179.24f;
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_DEMONCHAINS, dcX, dcY, dcZ, 0, true, false, 0, 0);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i = 0; i < nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if (m_spellcheck[i])
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
	bool ReSummon;
	uint32 ImpTimer;
	uint32 ReKilrek;
	uint32 DemonChain;
};

// Kil'Rek
class KilrekAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KilrekAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	KilrekAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
		 
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(AMPLIFY_FLAMES);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 5;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(BROKEN_PACT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
	}

	void OnCombatStart(Unit* mTarget)
	{
		spells[0].casttime = (uint32)time(NULL) + spells[0].cooldown;

		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();

		Unit *Illhoof = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 15688);
		if(Illhoof != NULL && Illhoof->isAlive())
			Illhoof->CastSpell(Illhoof, spells[1].info, spells[1].instant);
	}

	void AIUpdate()
	{
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

				if (m_spellcheck[i])
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
			std::vector<Unit *> TargetTable;
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if((*itr) != _unit && isHostile(_unit, (*itr)) && (*itr)->GetInstanceID() == _unit->GetInstanceID() && ((*itr)->GetTypeId() == TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER))
				{
					Unit* RandomTarget = NULL;
					RandomTarget = static_cast< Unit* >(*itr);
					
					if (RandomTarget && RandomTarget->isAlive() && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0)
						TargetTable.push_back(RandomTarget);
				}
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
	int nrspells;
};

// Fiendish Imp
class FiendishImpAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FiendishImpAI);
	SP_AI_Spell spells[1]; 
	bool m_spellcheck[1]; 

	FiendishImpAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
	
		nrspells = 1; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FIREBALL_IMP);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 0;
		spells[0].perctrigger = 100.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].casttime = 0;

		Unit *target = NULL;
		target = FindTargetForSpell();
		if (target != NULL)
		{
			_unit->GetAIInterface()->AttackReaction(target, 2500, 0);
		}
	}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);

		if (_unit->GetDistance2dSq(mTarget) <= 1225.0f)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
		}

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->SafeDelete();
	}
		
	void AIUpdate()
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 1225.0f)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
			if (_unit->GetCurrentSpell() == NULL && RandomUInt(10) > 2)
			{
				_unit->setAttackTimer(spells[2].attackstoptimer, false);

				Unit *target = NULL;
				target = _unit->GetAIInterface()->GetNextTarget();

				_unit->CastSpell(target, spells[0].info, spells[0].instant);
				return;
			}
		}

		float val = RandomFloat(100.0f);
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

	Unit* FindTargetForSpell()
	{
		Unit* target = NULL;
		float distance = 90.0f;

		Unit *pUnit;
		float dist;

		for (std::set<Object*>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
				continue;

			pUnit = static_cast<Unit*>((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			if(pUnit->m_invisible)
				continue;
			
			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist > distance*distance)
				continue;

			target = pUnit;
			break;
		}

		return target;
	}

protected:	

	int nrspells;
};

class DemonChains : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DemonChains);
	DemonChains(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(CHAINS_VISUAL), true);
		_unit->Root();
		_unit->DisableAI();
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		if(_unit->GetHealthPct() > 0)
			_unit->Despawn(10000, 0);
	}

	void OnDied(Unit * mKiller)
	{
		Unit *uIllhoof = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), 
			_unit->GetPositionZ(), CN_ILLHOOF);
		if(uIllhoof != NULL && uIllhoof->isAlive())
			uIllhoof->RemoveAura(SACRIFICE);

		_unit->Despawn(10000, 0);
	}
};

class FiendPortal : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FiendPortal);
	FiendPortal(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->Root();

		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(10000);
	}

	void AIUpdate()
	{
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FIENDISH_IMP, _unit->GetPositionX(), _unit->GetPositionY(), 
			_unit->GetPositionZ(), 0, true, false, 0, 0);
		
	}
};
void SetupBoss_Illhoof(ScriptMgr* pScriptMgr)
{

pScriptMgr->register_creature_script(CN_ILLHOOF, &IllhoofAI::Create);
pScriptMgr->register_creature_script(CN_KILREK, &KilrekAI::Create);
pScriptMgr->register_creature_script(CN_FIENDISH_IMP, &FiendishImpAI::Create);
pScriptMgr->register_creature_script(CN_DEMONCHAINS, &DemonChains::Create);
pScriptMgr->register_creature_script(CN_FPORTAL, &FiendPortal::Create);

}

