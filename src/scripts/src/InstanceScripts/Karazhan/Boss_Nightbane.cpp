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


/* ToDo:
 - Rain of Bones on one random player/pet
 - Summons five Restless Skeletons.
*/

#define CN_NIGHTBANE 17225
#define CN_RESTLESS_SKELETON 17261 // not needed if spell works

#define WALK 0
#define RUN 256
#define FLY 768

// ground spells
#define BELLOWING_ROAR 36922
#define CHARRED_EARTH 30129 //Also 30209 (Target Charred Earth) triggers this
#undef CLEAVE
#define CLEAVE 31043 // fixme: correct spell?!
#define SMOLDERING_BREATH 39385
#define TAIL_SWEEP 25653 // TODO: how to use this spell???
#define DISTRACTING_ASH 30280

// air spells
#define DISTRACTING_ASH_FLY 30130 // all guides say ground spell but animation is flying?!
#define RAIN_OF_BONES 37091 // Spell bugged: should debuff with 37098
#define SMOKING_BLAST 37057
#define FIREBALL_BARRAGE 30282
#define SUMMON_BONE_SKELETONS 30170

static Coords coords[] =
{
	{ 0, 0, 0, 0 },
	{ -11173.719727f, -1863.993164f, 130.390396f, 5.343079f }, // casting point
	{ -11125.542969f, -1926.884644f, 139.349365f, 3.982360f },
	{ -11166.404297f, -1950.729736f, 114.714726f, 1.537812f },
	{ -11167.497070f, -1922.315918f, 91.473755f, 1.390549f } // landing point
};

class NightbaneAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NightbaneAI);
	bool m_spellcheck[5];
	SP_AI_Spell spells[5];

	NightbaneAI(Creature* pCreature) : CreatureAIScript(pCreature)
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
	   
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		//ground phase spells
		spells[0].info =  dbcSpell.LookupEntry(BELLOWING_ROAR);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = 30; //confirmed
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1500;

		spells[1].info = dbcSpell.LookupEntry(CHARRED_EARTH);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = false;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(CLEAVE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 7;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(DISTRACTING_ASH);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = false;
		spells[3].cooldown = 60;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(SMOLDERING_BREATH);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = false;
		spells[4].cooldown = 20;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		
		for (int i = 1; i < 5; i++)
		{
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(i, 0, FLY));
		}
	}

	void OnCombatStart(Unit* mTarget)
	{
		ResetCastTime();
		m_phase = 0;
		m_currentWP = 4;
		mTailSweepTimer = 25;
		//not sure about this: _unit->PlaySoundToSet(9456);
		//TODO emote: "An ancient being awakens in the distance..."
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->m_moveFly = false;
		_unit->GetAIInterface()->m_canMove = true;
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		switch(m_phase)
		{
		case 1:
		case 3:
		case 5:
			FlyPhase();
			break;

		case 0:
		case 2:
		case 4:
			GroundPhase();
			break;
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		switch(iWaypointId)
		{
		case 1: //casting point
			{
				_unit->GetAIInterface()->m_canMove = false;
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				m_currentWP = 1;
			}break;
		case 4: //ground point
			{
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				Land();
				m_currentWP = 4;
			}break;
		default:
			{
				//move to the next waypoint
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(iWaypointId+1);
			}break;
		};
	}

	void FlyPhase()
	{
		if(m_currentWP != 1)
			return;

		m_FlyPhaseTimer--;
		if(!m_FlyPhaseTimer)
		{
			if(_unit->GetCurrentSpell() != NULL)
				_unit->GetCurrentSpell()->cancel();

			_unit->GetAIInterface()->m_canMove = true;
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->StopMovement(0);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(2);
			m_phase++;
			return;
		}

		if(m_FlyPhaseTimer > 15)
			return;

		Unit *target = NULL;

		//first cast
		if(m_FlyPhaseTimer == 15)
		{
			//Casts Rain of Bones on one random player/pet
			//CastSpellOnRandomTarget(5, 0, 40);
			//summon 3 skeletons
			//_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), dbcSpell.LookupEntry(SUMMON_BONE_SKELETONS), true);
			return;
		}

		//Shoots powerful Smoking Blast every second for approximately 15 seconds.
		if (_unit->GetAIInterface()->GetNextTarget() != NULL)
		{
			target = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpell(target, dbcSpell.LookupEntry(SMOKING_BLAST), true);
		}

		target = NULL;
		//fireball barrage check
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{
			if ((*itr)->GetTypeId() == TYPEID_PLAYER && (*itr)->GetInstanceID() == _unit->GetInstanceID())
			{
				target = static_cast< Unit* >(*itr);

				if(_unit->GetDistance2dSq(target) > 2025) //45 yards
				{
					_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), dbcSpell.LookupEntry(FIREBALL_BARRAGE), true);
					break; //stop
				}
			}
		}
	}

	void GroundPhase()
	{
		if(m_currentWP != 4)
			return;

		//Switch if needed
		if((m_phase == 0 && _unit->GetHealthPct() <= 75) 
		|| (m_phase == 2 && _unit->GetHealthPct() <= 50) 
		|| (m_phase == 4 && _unit->GetHealthPct() <= 25))
		{
			if(_unit->GetCurrentSpell() != NULL)
				_unit->GetCurrentSpell()->cancel();

			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);		
			_unit->GetAIInterface()->StopMovement(0);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);
			Fly();
			m_FlyPhaseTimer = 17;
			m_phase++;
			return;
		}

		//Tail Sweep
		mTailSweepTimer--;
		if(!mTailSweepTimer)
		{
			Unit *target = NULL;
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{
				if ((*itr)->GetTypeId() == TYPEID_PLAYER && (*itr)->GetInstanceID() == _unit->GetInstanceID())
				{
					target = static_cast< Unit* >(*itr);

					//cone behind the boss
					if(target->isAlive() && target->isInBack(_unit))
						_unit->CastSpell(target, dbcSpell.LookupEntry(TAIL_SWEEP), true);
				}
			}
			mTailSweepTimer = 25;
		}

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = coords[id].mX;
		wp->y = coords[id].mY;
		wp->z = coords[id].mZ;
		wp->o = coords[id].mO;
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

	void Fly()
	{
		_unit->Emote(EMOTE_ONESHOT_LIFTOFF);

		WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);

		_unit->GetAIInterface()->m_moveFly = true;
	}

	void Land()
	{
		_unit->Emote(EMOTE_ONESHOT_LAND);

		WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);

		_unit->GetAIInterface()->m_moveFly = false;
	}

	void ResetCastTime()
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
						case TARGET_RANDOM_SINGLE:
							CastSpellOnRandomTarget(i, 0, 0); break;
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

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{
				if (((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID())
				{
					Unit* RandomTarget = NULL;
					RandomTarget = static_cast< Unit* >(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (!TargetTable.size())
				return;

			Unit * RTarget = *(TargetTable.begin()+rand()%TargetTable.size());

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
	uint32 m_phase;
	uint32 m_FlyPhaseTimer;
	uint32 m_currentWP;
	uint32 mTailSweepTimer;
};


void SetupNightbane(ScriptMgr* pScriptMgr)
{

pScriptMgr->register_creature_script(CN_NIGHTBANE, &NightbaneAI::Create);

}