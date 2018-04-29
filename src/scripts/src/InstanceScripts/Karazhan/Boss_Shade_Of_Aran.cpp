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
// The Curator + Astral Flare
#define CN_CURATOR			15691
#define CN_ASTRALFLARE		17096

#define HATEFUL_BOLT		30383
#define EVOCATION			30254
#define C_ENRAGE			28747
#define BERSERK				26662

class CuratorAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CuratorAI);
	bool m_spellcheck[4];
	SP_AI_Spell spells[4];

	CuratorAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(HATEFUL_BOLT);
		spells[0].instant = true;
		spells[0].cooldown = 12;

		spells[1].info = dbcSpell.LookupEntry(BERSERK);
		spells[1].instant = true;
		spells[1].cooldown = 720;
		spells[1].attackstoptimer = 1000;
		
		spells[2].info = dbcSpell.LookupEntry(EVOCATION);
		spells[2].instant = false;
		spells[2].cooldown = 0;
		spells[2].attackstoptimer = 19000;

		spells[3].info = dbcSpell.LookupEntry(C_ENRAGE);
		spells[3].instant = true;
		spells[3].cooldown = 0;
		spells[3].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		for(int i = 0; i < nrspells; i++)
			spells[i].casttime = spells[i].cooldown;

		evocation = false;
		enrage = false;
		uint32 t = (uint32)time(NULL);
		Timer = t + 10;
		_unit->PlaySoundToSet(9183);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Menagerie is for guests only.");
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
		_unit->PlaySoundToSet(9184);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This Curator is no longer op... er... ation... al.");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
		if(_unit->GetHealthPct() > 0)
		{
			uint32 sound = 0;
			char *text = 0;
			switch(RandomUInt(1))
			{
			case 0:
				sound = 9187;
				text = "Do not touch the displays.";
				break;
			case 1:
				sound = 9308;
				text = "You are not a guest.";
				break;
			}
			_unit->PlaySoundToSet(sound);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
		}
	}

	void AIUpdate()
	{
		if(!evocation)
		{
			if(_unit->GetManaPct() <= 10)
			{
				_unit->Root();
				_unit->setAttackTimer(spells[1].attackstoptimer, false);
				_unit->PlaySoundToSet(9186);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your request cannot be processed.");
				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
				evocation = true;
			}
			else if(!enrage && _unit->GetHealthPct() <= 16)
			{
				_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
				_unit->PlaySoundToSet(9185);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Failure to comply will result in offensive action.");
				enrage = true;
			}
			else
				Trigger();
		}
		else if(_unit->GetManaPct() > 94)
		{
			_unit->Unroot();
			evocation = false;
		}
	}

	void Trigger()
	{
		uint32 t = (uint32)time(NULL);
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget() && t > spells[0].casttime)
		{
			Unit *target = _unit->GetAIInterface()->GetSecondHated();
			_unit->CastSpell(target, spells[0].info, spells[0].instant);
			target = NULL;
			spells[0].casttime = t + spells[0].cooldown;
		}
		else if(_unit->GetAIInterface()->GetNextTarget() && !enrage && !evocation && t > Timer && 
			_unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1) != 0)
		{
			AstralSpawn();
			Timer = t + 10;
		}
	}

	void AstralSpawn()
	{
		std::vector<Player*> Target_List;
		for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); 
			itr != _unit->GetInRangePlayerSetEnd(); ++itr)
		{
			Player *RandomTarget = NULL;
			RandomTarget = static_cast< Player* >(*itr);
			if(RandomTarget && RandomTarget->isAlive() && isHostile(_unit, (*itr)))
				Target_List.push_back(RandomTarget);
			RandomTarget = NULL;
		}
		if(!Target_List.size())
			return;

		size_t Target = rand()%Target_List.size();
		Unit *RTarget = Target_List[Target];
		if(!RTarget)
			return;

		uint32 sound = 0;
		char *text = 0;
		switch(RandomUInt(1))
		{
		case 0:
			sound = 9188;
			text = "Gallery rules will be strictly enforced.";
			break;
		case 1:
			sound = 9309;
			text = "This curator is equipped for gallery protection.";
			break;
		}
		_unit->PlaySoundToSet(sound);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);

		_unit->SetUInt32Value(UNIT_FIELD_POWER1, _unit->GetUInt32Value(UNIT_FIELD_POWER1) - (_unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1) / 10));
		float dX = _unit->GetPositionX();
		float dY = _unit->GetPositionY();
		Creature *AstralFlare = NULL;
		switch(RandomUInt(3))
		{
		case 0:
			{
				AstralFlare = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASTRALFLARE, dX+3, 
					dY+3, _unit->GetPositionZ(), 0, true, false, 0, 0);
				AstralFlare->GetAIInterface()->AttackReaction(RTarget, 1, 0);
				AstralFlare = NULL;
			}break;
		case 1:
			{
				AstralFlare = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASTRALFLARE, dX+3, 
					dY-3, _unit->GetPositionZ(), 0, true, false, 0, 0);
				AstralFlare->GetAIInterface()->AttackReaction(RTarget, 1, 0);
				AstralFlare = NULL;
			}break;
		case 2:
			{
				AstralFlare = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASTRALFLARE, dX-3, 
					dY-3, _unit->GetPositionZ(), 0, true, false, 0, 0);
				AstralFlare->GetAIInterface()->AttackReaction(RTarget, 1, 0);
				AstralFlare = NULL;
			}break;
		case 3:
			{
				AstralFlare = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASTRALFLARE, dX-3, 
					dY+3, _unit->GetPositionZ(), 0, true, false, 0, 0);
				AstralFlare->GetAIInterface()->AttackReaction(RTarget, 1, 0);
				AstralFlare = NULL;
			}break;
		}
		Target_List.clear();
	}

protected:
	bool evocation;
	bool enrage;
	bool berserk;
	int nrspells;
	uint32 Timer;
};


// Astral Flare
#define ASTRAL_FLARE_PASSIVE	30234
#define ASTRAL_FLARE_VISUAL		30237
#define ARCING_SEAR				30235

class AstralFlareAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AstralFlareAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	AstralFlareAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(ASTRAL_FLARE_PASSIVE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 3;
		spells[0].perctrigger = 100.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ASTRAL_FLARE_VISUAL);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 6;
		spells[1].perctrigger = 100.0f;
		spells[1].attackstoptimer = 500;

	}

	void OnCombatStart(Unit* mTarget)
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;

		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(2500, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(2500, 0);
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
};

void SetupBoss_Curatpr(ScriptMgr* pScriptMgr)
{

pScriptMgr->register_creature_script(CN_CURATOR, &CuratorAI::Create);
pScriptMgr->register_creature_script(CN_ASTRALFLARE, &AstralFlareAI::Create);
	
}

