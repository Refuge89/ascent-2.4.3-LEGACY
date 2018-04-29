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


//Opera Event
//Wizzard of Oz
#define CN_DOROTHEE 17535

#define SP_AOE_FEAR 29321
#define SP_WATER_BOLT 31012
#define SP_SUMMON_TITO 31014

#define TEXT_AGGRO_DOROTHEE "Oh dear, we simply must find a way home! The old wizard could be our only hope! Strawman, Roar, Tinhead, will you... wait! Oh golly, look! We have visitors!"
#define SOUND_AGGRO_DOROTHEE 9195

#define TEXT_DEATH_DOROTHEE "Oh at last, at last. I can go home."
#define SOUND_DEATH_DOROTHEE 9190

#define TEXT_DOROTHEE_TITO_SUMMON "Don't let them hurt us, Tito! Oh, you won't, will you?"
#define SOUND_DOROTHEE_TITO_SUMMON 9191 

#define TEXT_DOROTHEE_TITO_DEATH "Tito, oh Tito, no!"
#define SOUND_DOROTHEE_TITO_DEATH 9192

class DorotheeAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DorotheeAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];
	uint32 summontito;
	

	DorotheeAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		_unit->MechanicsDispels[ DISPEL_MECHANIC_CHARM ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_FEAR ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_ROOT ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_SLEEP ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_SNARE ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_STUN ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_KNOCKOUT ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_POLYMORPH ] = 1;
		_unit->MechanicsDispels[ DISPEL_MECHANIC_BANISH ] = 1;

		spells[0].info = dbcSpell.LookupEntry(SP_AOE_FEAR);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 30;  //correct cooldown?
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SP_WATER_BOLT);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = false;
		spells[1].perctrigger = 100.0f;
		spells[1].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_DOROTHEE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_DOROTHEE);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		//Check to see if we can spawn The Crone now
		Creature* Dorothee	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10897.650f, -1755.8311f, 90.476f, 17535); //Dorothee
		Creature* Strawman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10904.088f, -1754.8988f, 90.476f, 17543); //Strawman
		Creature* Roar		= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10891.115f, -1756.4898f, 90.476f, 17546);//Roar
		Creature* Tinman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10884.501f, -1757.3249f, 90.476f, 17547); //Tinman

		if ((Dorothee == NULL || Dorothee->isDead()) && (Strawman == NULL || Strawman->isDead()) && (Roar == NULL || Roar->isDead()) && (Tinman == NULL || Tinman->isDead()))
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(18168, -10884.501f, -1757.3249f, 90.476f, 0.0f, false, true, 0, 0);
		}
		//END
		
		_unit->PlaySoundToSet(SOUND_DEATH_DOROTHEE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_DOROTHEE);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
	}
    void SpawnTito()	// Lacking in collision checks!
	{
		float xchange  = (float)RandomFloat(15.0f);
		float distance = 15.0f;

		float ychange = sqrt(distance*distance - xchange*xchange);

		if (rand()%2 == 1)
			xchange *= -1;
		if (rand()%2 == 1)
			ychange *= -1;

		float newposx = _unit->GetPositionX() + xchange;
		float newposy = _unit->GetPositionY() + ychange;

		tito = _unit->GetMapMgr()->GetInterface()->SpawnCreature(17548, newposx, newposy, _unit->GetPositionZ() + 0.5f, 2.177125f, true, false, 0, 0);
	}
	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);

		if(titoSpawned && !tito && titoDeadSpeech)
		{
			_unit->PlaySoundToSet(SOUND_DOROTHEE_TITO_DEATH);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DOROTHEE_TITO_DEATH);
			titoDeadSpeech = false;
		}

		if(summontito > 20 && !titoSpawned)
		{
			_unit->PlaySoundToSet(SOUND_DOROTHEE_TITO_SUMMON);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DOROTHEE_TITO_SUMMON);
			SpawnTito();
			titoSpawned = true;
			titoDeadSpeech = true;
			return;
		}
        summontito++;
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = static_cast< Unit* >(*itr);

					if (RandomTarget && RandomTarget == _unit->GetAIInterface()->GetMostHated() && i == 3)
						continue;

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			TargetTable.clear();
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
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
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

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
	Unit* tito;
	bool titoSpawned;
	bool titoDeadSpeech;
};	

#define CN_TITO	17548

#define SP_ANNOYING_YIPPING	31015 

//No kill sound

class TitoAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TitoAI);
	bool m_spellcheck[1];
	SP_AI_Spell spells[1];
	

	TitoAI(Creature* pCreature) : CreatureAIScript(pCreature)
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

		spells[0].info = dbcSpell.LookupEntry(SP_ANNOYING_YIPPING);  
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true; 
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;

		_unit->m_noRespawn = true;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		CastTime();
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
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

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
};

#define CN_STRAWMAN	17543

#define SP_BURNING_STRAW	31075
#define SP_BRAIN_BASH	31046


#define TEXT_AGGRO_STRAWMAN	"Now what should I do with you? I simply can't make up my mind."
#define SOUND_AGGRO_STRAWMAN	9254

#define TEXT_DEATH_STRAWMAN	"Don't let them make a mattress... out of me."
#define SOUND_DEATH_STRAWMAN	9256

#define TEXT_KILL_STRAWMAN	"I guess I'm not a failure after all. "
#define SOUND_KILL_STRAWMAN	9257

class StrawmanAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(StrawmanAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	StrawmanAI(Creature* pCreature) : CreatureAIScript(pCreature)
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

		spells[0].info = dbcSpell.LookupEntry(SP_BURNING_STRAW);//  NEEDS TO BE SO IT ONLY AFFECTS HIM WHEN HE IS HIT BY FIRE DMG!
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
	
		spells[1].info = dbcSpell.LookupEntry(SP_BRAIN_BASH);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].cooldown = 8; //not sure about this
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
 
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_STRAWMAN);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_STRAWMAN);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		//Check to see if we can spawn The Crone now
		Creature* Dorothee	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10897.650f, -1755.8311f, 90.476f, 17535);	//Dorothee
		Creature* Strawman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10904.088f, -1754.8988f, 90.476f, 17543);	//Strawman
		Creature* Roar		= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10891.115f, -1756.4898f, 90.476f, 17546);	//Roar
		Creature* Tinman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10884.501f, -1757.3249f, 90.476f, 17547);	//Tinman
		
		if ((Dorothee == NULL || Dorothee->isDead()) && (Strawman == NULL || Strawman->isDead()) && (Roar == NULL || Roar->isDead()) && (Tinman == NULL || Tinman->isDead()))
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(18168, -10884.501f, -1757.3249f, 90.476f, 0.0f, false, true, 0, 0);
		}
		//END
		
		_unit->PlaySoundToSet(SOUND_DEATH_STRAWMAN);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_STRAWMAN);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
		_unit->PlaySoundToSet(SOUND_KILL_STRAWMAN);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_KILL_STRAWMAN);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
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

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
};

#define CN_TINHEAD	17547

#define SP_CLEAVE		15284
//#define SP_RUST		31086
//dont bother.. spell does not work.. needs fix

#define TEXT_AGGRO_TINHEAD	"I could really use a heart. Say, can I have yours?"
#define SOUND_AGGRO_TINHEAD	9268

#define TEXT_DEATH_TINHEAD	"Back to being an old rust bucket.."
#define SOUND_DEATH_TINHEAD	9270

#define TEXT_KILL_TINHEAD	"Guess I'm not so rusty, after all."
#define SOUND_KILL_TINHEAD	9271

class TinheadAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TinheadAI);
	bool m_spellcheck[1];
	SP_AI_Spell spells[1];

	TinheadAI(Creature* pCreature) : CreatureAIScript(pCreature)
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

		spells[0].info = dbcSpell.LookupEntry(SP_CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
	
		/*spells[1].info = dbcSpell.LookupEntry(RUST);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 60;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
		*/
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_TINHEAD);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_TINHEAD);
	}

	void OnCombatStop(Unit *mTarget)
	{
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		//Check to see if we can spawn The Crone now
		Creature* Dorothee	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10897.650f, -1755.8311f, 90.476f, 17535);	//Dorothee
		Creature* Strawman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10904.088f, -1754.8988f, 90.476f, 17543);	//Strawman
		Creature* Roar		= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10891.115f, -1756.4898f, 90.476f, 17546);	//Roar
		Creature* Tinman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10884.501f, -1757.3249f, 90.476f, 17547);	//Tinman
		
		if ((Dorothee == NULL || Dorothee->isDead()) && (Strawman == NULL || Strawman->isDead()) && (Roar == NULL || Roar->isDead()) && (Tinman == NULL || Tinman->isDead()))
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(18168, -10884.501f, -1757.3249f, 90.476f, 0.0f, false, true, 0, 0);
		}
		//END
		
		_unit->PlaySoundToSet(SOUND_DEATH_TINHEAD);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_TINHEAD);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
		_unit->PlaySoundToSet(SOUND_KILL_TINHEAD);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_KILL_TINHEAD);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
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

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
};

#define CN_ROAR	17546

#define TEXT_AGGRO_ROAR	"Wanna fight? Huh? Do ya? C'mon, I'll fight you with both claws behind my back!"
#define SOUND_AGGRO_ROAR	9227

#define TEXT_DEATH_ROAR "You didn't have to go and do that!"
#define SOUND_DEATH_ROAR	9229

class RoarAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(RoarAI);

	RoarAI(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->PlaySoundToSet(SOUND_AGGRO_ROAR);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_ROAR);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		//Check to see if we can spawn The Crone now
		Creature* Dorothee	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10897.650f, -1755.8311f, 90.476f, 17535); //Dorothee
		Creature* Strawman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10904.088f, -1754.8988f, 90.476f, 17543); //Strawman
		Creature* Roar		= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10891.115f, -1756.4898f, 90.476f, 17546);//Roar
		Creature* Tinman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10884.501f, -1757.3249f, 90.476f, 17547); //Tinman
		
		if((Dorothee == NULL || Dorothee->isDead()) && (Strawman == NULL || Strawman->isDead()) && (Roar == NULL || Roar->isDead()) && (Tinman == NULL || Tinman->isDead()))
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(18168, -10884.501f, -1757.3249f, 90.476f, 0.0f, false, true, 0, 0);
		}
		//END
		
		_unit->PlaySoundToSet(SOUND_DEATH_ROAR);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_ROAR);
	}
};

#define CN_CRONE 18168

#define SP_SUMMON_CYCLONE 38337
#define SP_CHAIN_LIGHTNING 32337

#define TEXT_AGGRO_CRONE	"Woe to each and every one of you my pretties!"
#define SOUND_AGGRO_CRONE	9179 //needs correct sound id

#define TEXT_KILL_CRONE "Fixed you, didn't I?"
#define SOUND_KILL_CRONE 9280 //needs correct sound id

#define TEXT_DEATH_CRONE "How could you? What a cruel, cruel world!"
#define SOUND_DEATH_CRONE	9178 //needs correct sound id


class CroneAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CroneAI);
	bool m_spellcheck[2];
	SP_AI_Spell spells[2];

	CroneAI(Creature* pCreature) : CreatureAIScript(pCreature)
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

		spells[0].info = dbcSpell.LookupEntry(SP_SUMMON_CYCLONE); 
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SP_CHAIN_LIGHTNING); 
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_CRONE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_CRONE);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if(_unit->GetHealthPct() > 0)
		{
			GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
			GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
			GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

			if(DoorLeft)
				DoorLeft->SetUInt32Value(GAMEOBJECT_STATE, 1);

			if(DoorRight)
				DoorRight->SetUInt32Value(GAMEOBJECT_STATE, 0);

			if(Curtain)
				Curtain->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}

		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();

		_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetUInt32Value(GAMEOBJECT_STATE, 0);

		if(DoorRight)
			DoorRight->SetUInt32Value(GAMEOBJECT_STATE, 0);

		// Make sure the curtain stays up
		if(Curtain)
			Curtain->SetUInt32Value(GAMEOBJECT_STATE, 0);

		_unit->PlaySoundToSet(SOUND_DEATH_CRONE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_CRONE);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
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

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
};

#define CN_CYCLONEOZ			22104
#define CYCLONE_VISUAL			32332
#define CYCLONE_KNOCK			38517

class CycloneOZ : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CycloneOZ);
	CycloneOZ(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(CYCLONE_VISUAL), true);
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(CYCLONE_KNOCK), true);
	}

protected:
	int nrspells;
};

/*
 *
 *
 *
 *
 *
 */


//Romulo & Julianne
#define CN_ROMULO 17533

#define SP_BACKWARD_LUNGE 30815
#define SP_DEADLY_SWATHE 30817
#define SP_POISONED_THRUST 30822
#define SP_DARING 30841

#define TEXT_AGGRO_ROMULO "Wilt thou provoke me? Then have at thee, boy!"
#define SOUND_AGGRO_ROMULO 9233

#define TEXT_DEATH_ROMULO "Thou smilest... upon the stroke that... murders me."
#define SOUND_DEATH_ROMULO 9235

#define TEXT_DEATH2_ROMULO "This day's black fate on more days doth depend. This but begins the woe. Others must end."
#define SOUND_DEATH2_ROMULO 9236

#define TEXT_KILL_ROMULO "How well my comfort is revived by this! "
#define SOUND_KILL_ROMULO 9238

#define TEXT_RES_ROMULO "Thou detestable maw, thou womb of death; I enforce thy rotten jaws to open!"
#define SOUND_RES_ROMULO 9237

class RomuloAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(RomuloAI);
	bool m_spellcheck[4];
	SP_AI_Spell spells[4];

	RomuloAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
	 
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SP_BACKWARD_LUNGE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 12;
		spells[0].perctrigger = 20.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SP_DEADLY_SWATHE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = 0;
		spells[1].perctrigger = 20.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SP_POISONED_THRUST);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 0;
		spells[2].perctrigger = 20.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(SP_DARING);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = false;
		spells[3].cooldown = 0;
		spells[3].perctrigger = 20.0f;
		spells[3].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_ROMULO);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_ROMULO);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if(_unit->GetHealthPct() > 0)
		{
			GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
			GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
			GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

			if(DoorLeft)
				DoorLeft->SetUInt32Value(GAMEOBJECT_STATE, 1);

			if(DoorRight)
				DoorRight->SetUInt32Value(GAMEOBJECT_STATE, 0);

			if(Curtain)
				Curtain->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}
		
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		if(_unit)
			_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetUInt32Value(GAMEOBJECT_STATE, 0);

		if(DoorRight)
			DoorRight->SetUInt32Value(GAMEOBJECT_STATE, 0);

		// Make sure the curtain stays up
		if(Curtain)
			Curtain->SetUInt32Value(GAMEOBJECT_STATE, 0);

		_unit->PlaySoundToSet(SOUND_DEATH_ROMULO);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_ROMULO);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
		_unit->PlaySoundToSet(SOUND_KILL_ROMULO);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_KILL_ROMULO);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
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

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
};

#define CN_JULIANNE 17534

#define SP_ETERNAL_AFFECTION 30878
#define SP_POWERFUL_ATTRACTION 30889
#define SP_BINDING_PASSION 30890
#define SP_DEVOTION 30887

#define TEXT_AGGRO_JULIANNE "What devil art thou, that dost torment me thus?"
#define SOUND_AGGRO_JULIANNE 9196

#define TEXT_DEATH_JULIANNE "Romulo, I come! Oh... this do I drink to thee! "
#define SOUND_DEATH_JULIANNE 9198

#define TEXT_DEATH2_JULIANNE "Where is my Lord? Where is my Romulo? Ohh, happy dagger! This is thy sheath! There rust, and let me die!"
#define SOUND_DEATH2_JULIANNE 9199

#define TEXT_KILL_JULIANNE "Parting is such sweet sorrow."
#define SOUND_KILL_JULIANNE 9201

#define TEXT_RES_ROMULO_JULIANNE "Come, gentle night; and give me back my Romulo!"
#define SOUND_RES_ROMULO_JULIANNE 9200

class JulianneAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(JulianneAI);
	bool m_spellcheck[4];
	SP_AI_Spell spells[4];

	JulianneAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
	
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SP_ETERNAL_AFFECTION);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].cooldown = 12;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SP_POWERFUL_ATTRACTION);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = 0;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SP_BINDING_PASSION);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 0;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(SP_DEVOTION);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = false;
		spells[3].cooldown = 0;
		spells[3].perctrigger = 5.0f;
		spells[3].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);

		_unit->PlaySoundToSet(SOUND_AGGRO_JULIANNE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_AGGRO_JULIANNE);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if(_unit->GetHealthPct() > 0)
		{
			GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
			GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
			GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

			if(DoorLeft)
				DoorLeft->SetUInt32Value(GAMEOBJECT_STATE, 1);

			if(DoorRight)
				DoorRight->SetUInt32Value(GAMEOBJECT_STATE, 0);

			if(Curtain)
				Curtain->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}
		
		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		if(_unit)
			_unit->SafeDelete();
	}

	void OnDied(Unit * mKiller)
	{
		//_unit->RemoveAllAuras();
		//_unit->setEmoteState(EMOTE_ONESHOT_EAT);
		//_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17533, -10891.582f, -1755.5177f, 90.476f, 4.61f, false, true, 0, 0);
		_unit->PlaySoundToSet(SOUND_DEATH_JULIANNE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH_JULIANNE);
		//_unit->setEmoteState(EMOTE_STATE_DEAD);

		//_unit->PlaySoundToSet(SOUND_DEATH2_JULIANNE);
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_DEATH2_JULIANNE);
		CastTime();
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit * mTarget)
	{
		_unit->PlaySoundToSet(SOUND_KILL_JULIANNE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, TEXT_KILL_JULIANNE);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
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

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:
	int nrspells;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The Big Bad Wolf
#define CN_BIGBADWOLF					17521

#define TERRIFYING_HOWL					30752
#define WIDE_SWIPE						6749
// Combines display visual + buff
#define REDRIDINGHOOD_DEBUFF			30768
#define PBS_TAUNT						30755	// Picnic Basket Smell (taunt)

class BigBadWolfAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BigBadWolfAI);
	bool m_spellcheck[4];
	SP_AI_Spell spells[4];

	BigBadWolfAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
		
		nrspells = 3;
		for(int i = 0; i < nrspells; i++)
			m_spellcheck[i] = false;

		spells[0].info = dbcSpell.LookupEntry(TERRIFYING_HOWL);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 50.0f;
		spells[0].cooldown = 30;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(WIDE_SWIPE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].perctrigger = 50.0f;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(REDRIDINGHOOD_DEBUFF);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].perctrigger = 50.0f;
		spells[2].instant = true;
		spells[2].cooldown = 45;
		spells[2].attackstoptimer = 1000;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 60.0f;
		spells[2].soundid = 9278;
		spells[2].speech = "Run away little girl, run away!";

		spells[3].info = dbcSpell.LookupEntry(PBS_TAUNT);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].perctrigger = 0.0f;
		spells[3].instant = true;
		spells[3].cooldown = 0;
		spells[3].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		for(int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		ThreatAdd = false;
		m_threattimer = 20;
		
		_unit->PlaySoundToSet(9276);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The better to own you with!");
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
		_unit->PlaySoundToSet(9275);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Aarrhhh.");
		RemoveAIUpdateEvent();

		GameObject* DoorLeftO = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRightO = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);

		if(DoorLeftO)
			DoorLeftO->SetUInt32Value(GAMEOBJECT_STATE, 0);

		if(DoorRightO)
			DoorRightO->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{
		_unit->PlaySoundToSet(9277);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mmmm... delicious.");
	}

	void AIUpdate()
	{
		if(ThreatAdd == true)
		{
			m_threattimer--;
		}
		else if(!m_threattimer)
		{
			_unit->GetAIInterface()->taunt(RTarget, false);
			ThreatAdd = false;
			m_threattimer = 19;
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
					case TARGET_RANDOM_SINGLE:
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Red Riding Hood cast");
							std::vector<Player *> TargetTable;
							for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); 
								itr != _unit->GetInRangePlayerSetEnd(); ++itr)
							{
								Player *RandomTarget = NULL;
								RandomTarget = static_cast< Player* >(*itr);
								if(RandomTarget && RandomTarget->isAlive())
									TargetTable.push_back(RandomTarget);
								RandomTarget = NULL;
							}
							
							if (!TargetTable.size())
								return;
							
							size_t RandTarget = rand()%TargetTable.size();

							RTarget = TargetTable[RandTarget];

							if (!RTarget)
								return;
							
							_unit->CastSpell(RTarget, spells[2].info, spells[2].instant);
							_unit->GetAIInterface()->taunt(RTarget, true);
							ThreatAdd = true;
							m_threattimer = 19;
						}break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
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
	int m_threattimer;
	bool ThreatAdd;
	Unit *RTarget;
};


#define TERRIFYING_HOWL 30752
#define MORPH_LITTLE_RED_RIDING_HOOD 30768
#define DEBUFF_LITTLE_RED_RIDING_HOOD 30756

class THEBIGBADWOLFAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(THEBIGBADWOLFAI);
		bool m_spellcheck[3];
		SP_AI_Spell spells[3];

	THEBIGBADWOLFAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
		
		//SpellEntry *infoImmunity;
		nrspells = 3;
	for(int i=0;i<nrspells;i++)
	{
		m_spellcheck[i] = false;
	}

	spells[0].info = dbcSpell.LookupEntry(TERRIFYING_HOWL);
	spells[0].targettype = TARGET_VARIOUS;
	spells[0].instant = true;
	spells[0].cooldown = 10;
	spells[0].perctrigger = 0.0f;
	spells[0].attackstoptimer = 1000;

	spells[1].info = dbcSpell.LookupEntry(MORPH_LITTLE_RED_RIDING_HOOD);
	spells[1].targettype = TARGET_ATTACKING;
	spells[1].instant = true;
	spells[1].cooldown = 30;
	spells[1].perctrigger = 0.0f;
	spells[1].attackstoptimer = 1000;

	spells[2].info = dbcSpell.LookupEntry(DEBUFF_LITTLE_RED_RIDING_HOOD);
	spells[2].targettype = TARGET_ATTACKING;
	spells[2].instant = true;
	spells[2].cooldown = 30;
	spells[2].perctrigger = 0.0f;
	spells[2].attackstoptimer = 1000;
	spells[2].soundid = 9278;
	spells[2].speech = "Run away little girl, run away!";
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		_unit->PlaySoundToSet(9276);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The better to own you with!");
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetUInt32Value(GAMEOBJECT_STATE, 1);

		if(DoorRight)
			DoorRight->SetUInt32Value(GAMEOBJECT_STATE, 0);

		if(Curtain)
			Curtain->SetUInt32Value(GAMEOBJECT_STATE, 1);

		CastTime();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetUInt32Value(GAMEOBJECT_STATE, 0);

		if(DoorRight)
			DoorRight->SetUInt32Value(GAMEOBJECT_STATE, 0);

		// Make sure the curtain stays up
		if(Curtain)
			Curtain->SetUInt32Value(GAMEOBJECT_STATE, 0);

		CastTime();
		_unit->PlaySoundToSet(9275);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "AArrhhh.");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		_unit->PlaySoundToSet(9277);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mmmm... delicious.");
	}

	void AIUpdate()
	{
	float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
	for(int i=0;i<nrspells;i++)
		spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
		float comulativeperc = 0;
		Unit *target = NULL;
		for(int i=0;i<nrspells;i++)
		{
			spells[i].casttime--;
		
		if (m_spellcheck[i])
		{
			spells[i].casttime = spells[i].cooldown;
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

			if (spells[i].speech != "")
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
			_unit->PlaySoundToSet(spells[i].soundid); 
			}

			m_spellcheck[i] = false;
			return;
		}

		if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
		{
			_unit->setAttackTimer(spells[i].attackstoptimer, false);
			m_spellcheck[i] = true;
		}
		comulativeperc += spells[i].perctrigger;
			}
	}
	}

protected:
	int nrspells;
};


uint32 WayStartBBW[1000000];

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
	Menu->SendTo(Plr);

class BarnesGS : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(WayStartBBW[pObject->GetInstanceID()] == 5)
		{
			//Splendid! Marvelous! An excellent performance!    (when opera event is over)
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8975, Plr);
		}
		else
		{	
			//Finally, everything is in place. Are you ready for your big stage debut?
 			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8970, Plr);
			Menu->AddItem( 0, "I'm not an actor.", 1 );
 			if(AutoSend)
 				Menu->SendTo(Plr);

		}
	}

	void GossipSelectOption(Object* pObject, Player * Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch (IntId)
		{
			case 0:
				GossipHello(pObject, Plr, true);
				break;

			case 1:
				{
					//Don't worry, you'll be fine. You look like a natural!
					GossipMenu * Menu;
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8971, Plr);
					Menu->AddItem( 0, "Ok, I'll give it a try, then.", 2 );
					Menu->SendTo(Plr);
				}break;
			case 2:
				{
					Creature *pCreature = static_cast< Creature* >(pObject);
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Splendid. I'm going to get the audience ready. Break a leg!");
					pCreature->CastSpell(pObject->GetGUID(), 32616, false);
					pCreature->GetAIInterface()->StopMovement(0);
					pCreature->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
					pCreature->GetAIInterface()->setMoveType(11);
					pCreature->GetAIInterface()->setWaypointToMove(0);
					pCreature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
					pCreature->PlaySoundToSet(9357);
					WayStartBBW[pCreature->GetInstanceID()] = 2;
				}break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class GrandMother : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7245, Plr);
		
		Menu->AddItem( 0, "What phat lewts you have Grandmother!", 1);

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

			case 1:
			{
				static_cast< Creature* >(pObject)->Despawn(100, 0);
				Creature *pop = pObject->GetMapMgr()->GetInterface()->SpawnCreature(17521, pObject->GetPositionX(), pObject->GetPositionY(), 
					pObject->GetPositionZ(), 0, true, true, 0, 0);
				pop->GetAIInterface()->AttackReaction(Plr, 1, 0);
				break;
			}
		}
	}

	void Destroy()
	{
		delete this;
	}
};

/*	Alot of the code for this script was taken from M4ksiu and his Black Temple script, 
	who I'd like to thank for his contributions to the scripting scene.	*/
static Coords Barnes[] = 
{
	{ },
	{ -10873.91f, -1780.177f, 90.50f, 3.3f },
	{ -10895.299805f, -1783.349976f, 90.50f, 4.5f },
	{ -10873.91f, -1780.177f, 90.50f, 3.3f },
	{ -10868.4f, -1781.63f, 90.50f, 1.24f }
};

class BarnesAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BarnesAI);

	BarnesAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, 0));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 43000, 0));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, 0));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, 0));

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);

		WayStartBBW[_unit->GetInstanceID()] = 1;

		eventRand = 0;
		switch(RandomUInt(2))
		{
		case 0:
			eventRand = 0;
			break;
		case 1:
			eventRand = 1;
			break;
		case 2:
			eventRand = 2;
			break;
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		switch (iWaypointId)
		{
			case 0:
				_unit->GetAIInterface()->setWaypointToMove(1);
				WayStartBBW[_unit->GetInstanceID()] = 3;
				break;
			case 1:
				break;
			case 2:
				cleanStage();
				WayStartBBW[_unit->GetInstanceID()] = 4;
				_unit->GetMapMgr()->GetInterface()->SpawnCreature(19525, Barnes[2].mX, Barnes[2].mY, Barnes[2].mZ, Barnes[2].mO, true, false, 0, 0)->Despawn(43000, 0);
				_unit->SetFacing(4.5f);
				switch(eventRand)
				{
				case 0:
					BarnesSpeakRed();
					break;
				case 1:
					BarnesSpeakRJ();
					break;
				case 2:
					BarnesSpeakWOZ();
					break;
				}
				break;
			case 3:
				switch(eventRand)
				{
				case 0:
					EventRed();
					break;
				case 1:
					EventRJ();
					break;
				case 2:
					EventWOZ();
					break;
				}
				_unit->SetUInt32Value(UNIT_FIELD_FLAGS, 1);
				WayStartBBW[_unit->GetInstanceID()] = 5;
				break;
		}
	}

	void cleanStage()
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetUInt32Value(GAMEOBJECT_STATE, 1);

		if(DoorRight)
			DoorRight->SetUInt32Value(GAMEOBJECT_STATE, 0);

		if(Curtain)
			Curtain->SetUInt32Value(GAMEOBJECT_STATE, 1);
				
		Creature* Julianne	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10883.0f, -1751.81f, 90.4765f, 17534);
		Creature* Romulo	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10883.0f, -1751.81f, 90.4765f, 17533);
		Creature* BigBadWolf= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10883.0f, -1751.81f, 90.4765f, 17521);
		Creature* Grandma	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10883.0f, -1751.81f, 90.4765f, 17603);

		Creature* Dorothee	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10897.650f, -1755.8311f, 90.476f, 17535); //Dorothee
		Creature* Strawman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10904.088f, -1754.8988f, 90.476f, 17543); //Strawman
		Creature* Roar		= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10891.115f, -1756.4898f, 90.476f, 17546);//Roar
		Creature* Tinman	= _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-10884.501f, -1757.3249f, 90.476f, 17547); //Tinman

		GameObject* House = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10883.0f, -1751.81f, 90.4765f, 183493);
		GameObject* Tree  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10877.7f, -1763.18f, 90.4771f, 183492);
		GameObject* Tree2 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10906.7f, -1750.01f, 90.4765f, 183492);
		GameObject* Tree3 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10909.5f, -1761.79f, 90.4773f, 183492);
		//GameObject* BackDrop = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10890.9f, -1744.06f, 90.4765f, 183491);

		if(Julianne)
			Julianne->SafeDelete();
		if(Romulo)
			Romulo->SafeDelete();
			
		if(BigBadWolf)
			BigBadWolf->SafeDelete();
		if(Grandma)
			Grandma->SafeDelete();
			
		if(Dorothee)
			Dorothee->SafeDelete();
		if(Strawman)
			Strawman->SafeDelete();
		if(Roar)
			Roar->SafeDelete();
		if(Tinman)
			Tinman->SafeDelete();

		if(House)
			House->GetMapMgr()->GetInterface()->DeleteGameObject(House);
		if(Tree)
			Tree->GetMapMgr()->GetInterface()->DeleteGameObject(Tree);
		if(Tree2)
			Tree2->GetMapMgr()->GetInterface()->DeleteGameObject(Tree2);
		if(Tree3)
			Tree3->GetMapMgr()->GetInterface()->DeleteGameObject(Tree3);
		//if(BackDrop)
		//	BackDrop->GetMapMgr()->GetInterface()->DeleteGameObject(BackDrop);
	}

	void BarnesSpeakWOZ()
	{
		// Start text
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Good evening, ladies and gentleman. Welcome to this evening's presentation!");
		_unit->PlaySoundToSet(9175);
		// Timed text 1
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"Tonight, we plumb the depths of the human soul as we join a lost, lonely girl trying desperately, with the help of her loyal companions, to find her way home.", 
			EVENT_UNIT_CHAT_MSG, 7000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9338, EVENT_UNK, 7000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Timed text 2
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL,
			"But she is pursued by a wicked, malevolent crone!", 
			EVENT_UNIT_CHAT_MSG, 23000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9339, EVENT_UNK, 23000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Timed text 3
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"Will she survive? Will she prevail? Only time will tell. And now: On with the show!", 
			EVENT_UNIT_CHAT_MSG, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9340, EVENT_UNK, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Applause
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9332, EVENT_UNK, 41000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

	void EventWOZ()
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetUInt32Value(GAMEOBJECT_STATE, 1);

		if(DoorRight)
			DoorRight->SetUInt32Value(GAMEOBJECT_STATE, 1);

		if(Curtain)
			Curtain->SetUInt32Value(GAMEOBJECT_STATE, 0);

		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 16616);

		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17535, -10897.650f, -1755.8311f, 90.476f, 4.61f, false, true, 0, 0); //Dorothee
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17543, -10904.088f, -1754.8988f, 90.476f, 4.61f, false, true, 0, 0); //Strawman
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17546, -10891.115f, -1756.4898f, 90.476f, 4.61f, false, true, 0, 0);//Roar
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17547, -10884.501f, -1757.3249f, 90.476f, 4.61f, false, true, 0, 0); //Tinman
	}

	void BarnesSpeakRJ()
	{
		// Start text
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Good evening, ladies and gentleman. Welcome to this evening's presentation!");
		_unit->PlaySoundToSet(9175);
		// Timed text 1
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"Tonight we explore a tale of forbidden love!", EVENT_UNIT_CHAT_MSG, 6000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9341, EVENT_UNK, 6000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		// Timed text 2
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL,
			"But beware, for not all love stories end happily. As you may find out, sometimes love pricks like a thorn.", 
			EVENT_UNIT_CHAT_MSG, 19300, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9342, EVENT_UNK, 19300, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		// Timed text 3

		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"But don't take it from me. See for yourself what tragedy lies ahead when the paths of star-crossed lovers meet. And now: On with the show!", 
			EVENT_UNIT_CHAT_MSG, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9343, EVENT_UNK, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		// Applause
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9332, EVENT_UNK, 41000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

	void EventRJ()
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);

		if(DoorLeft)
			DoorLeft->SetUInt32Value(GAMEOBJECT_STATE, 1);

		if(DoorRight)
			DoorRight->SetUInt32Value(GAMEOBJECT_STATE, 1);

		if(Curtain)
			Curtain->SetUInt32Value(GAMEOBJECT_STATE, 0);

		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 16616);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17534, -10891.582f, -1755.5177f, 90.476f, 4.61f, false, true, 0, 0); //Spawn Julianne
	}

	void BarnesSpeakRed()
	{
		// Start text
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Good evening, ladies and gentleman. Welcome to this evening's presentation!");
		_unit->PlaySoundToSet(9175);
		// Timed text 1
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"Tonight things are not what they seems for tonight your eyes may not be trusted.", 
			EVENT_UNIT_CHAT_MSG, 7000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9335, EVENT_UNK, 7000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Timed text 2
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL,
			"Take for instance this quiet elderly woman waiting for a visit from her granddaughter. Surely there is nothing to fear from this sweet gray-haired old lady.", 
			EVENT_UNIT_CHAT_MSG, 17000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9336, EVENT_UNK, 17000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Timed text 3
		sEventMgr.AddEvent(static_cast< Unit* >(_unit), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, 
			"But don't let me pull the wool over your eyes. See for yourself what lies beneath those covers. And now: On with the show!", 
			EVENT_UNIT_CHAT_MSG, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9337, EVENT_UNK, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Applause
		sEventMgr.AddEvent(static_cast< Object* >(_unit), &Object::PlaySoundToSet, (uint32)9332, EVENT_UNK, 41000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

	void EventRed()
	{
		GameObject* DoorLeft = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10917.1445f, -1774.05f, 90.478f, 184279);
		GameObject* DoorRight = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10872.195f, -1779.42f, 90.45f, 184278);
		GameObject* Curtain = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-10894.17f, -1774.218f, 90.477f, 183932);
		GameObject* House = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(183493, -10883.0f, -1751.81f, 90.4765f, -1.72788f, false, 0, 0);
		GameObject* Tree = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(183492, -10877.7f, -1763.18f, 90.4771f, -1.6297f, false, 0, 0);
		GameObject* Tree2 = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(183492, -10906.7f, -1750.01f, 90.4765f, -1.69297f, false, 0, 0);
		GameObject* Tree3 = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(183492, -10909.5f, -1761.79f, 90.4773f, -1.65806f, false, 0, 0);
		GameObject* BackDrop = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(183491, -10890.9f, -1744.06f, 90.4765f, -1.67552f, false, 0, 0);
		
		if(DoorLeft)
			DoorLeft->SetUInt32Value(GAMEOBJECT_STATE, 1);

		if(DoorRight)
			DoorRight->SetUInt32Value(GAMEOBJECT_STATE, 1);

		if(Curtain)
			Curtain->SetUInt32Value(GAMEOBJECT_STATE, 0);

		// Back Right - House
		if(House)
		{
			House->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.760406f);
			House->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.649448f);
			House->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
		}

		// Front Right - Tree
		if(Tree)
		{
			Tree->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.748956f);
			Tree->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.66262f);
		}

		// Back Left - Tree 2
		if(Tree2)
		{
			Tree2->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.648956f);
			Tree2->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.66262f);
		}

		// Front Left - Tree 3
		if(Tree3)
		{
			Tree3->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.737277f);
			Tree3->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.67559f);
		}

		// Back - Back Drop
		if(BackDrop)
		{
			BackDrop->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.743145f);
			BackDrop->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.669131f);
		}

		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 16616);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(17603, -10891.582f, -1755.5177f, 90.476f, 4.61f, false, true, 0, 0);

	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = Barnes[id].mX;
		wp->y = Barnes[id].mY;
		wp->z = Barnes[id].mZ;
		wp->o = Barnes[id].mO;
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

protected:
	int eventRand;
};


class StageLight : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(StageLight);
	StageLight(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;
		_unit->CastSpell(_unit, 34126, true);
	}
};


void SetupOpera(ScriptMgr* pScriptMgr)
{
	// Opera event related
	pScriptMgr->register_creature_script(CN_BIGBADWOLF, &BigBadWolfAI::Create);
	pScriptMgr->register_creature_script(CN_ROMULO, &RomuloAI::Create);
	pScriptMgr->register_creature_script(CN_JULIANNE, &JulianneAI::Create);
	pScriptMgr->register_creature_script(19525, &StageLight::Create);
	GossipScript * KGrandMother = (GossipScript*) new GrandMother;
	GossipScript * KBarnes = (GossipScript*) new BarnesGS;
	pScriptMgr->register_gossip_script(16812, KBarnes);
	pScriptMgr->register_gossip_script(17603, KGrandMother);
	pScriptMgr->register_creature_script(16812, &BarnesAI::Create);

	//WoOz here... commented yet to be implemented - kamyn
	pScriptMgr->register_creature_script(CN_DOROTHEE, &DorotheeAI::Create);
	pScriptMgr->register_creature_script(CN_STRAWMAN, &StrawmanAI::Create);
	pScriptMgr->register_creature_script(CN_TINHEAD, &TinheadAI::Create);
	pScriptMgr->register_creature_script(CN_ROAR, &RoarAI::Create);
	pScriptMgr->register_creature_script(CN_TITO, &TitoAI::Create);
	pScriptMgr->register_creature_script(CN_CRONE, &CroneAI::Create);
	pScriptMgr->register_creature_script(CN_CYCLONEOZ, &CycloneOZ::Create);
	
}