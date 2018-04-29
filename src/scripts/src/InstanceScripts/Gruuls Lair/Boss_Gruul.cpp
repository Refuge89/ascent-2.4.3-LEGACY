/*
 * Sun++ for Ascent MMORPG Server
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

/* TODO: BFX Update some scripts with AI update spell timers rather then having it in class structure.*/

#include "StdAfx.h"
#include "../../InstanceScripts/Base/setup.h"
#include "../../InstanceScripts/Base/base.h"

#define CN_GRUUL_THE_DRAGONKILLER					19044
#define GRUUL_THE_DRAGONKILLER_GROWTH				36300	// +
#define GRUUL_THE_DRAGONKILLER_CAVE_IN				36240	// + 
#define GRUUL_THE_DRAGONKILLER_GROUND_SLAM			33525	// +
#define GRUUL_THE_DRAGONKILLER_GROUND_SLAM2			39187	// +
#define GRUUL_THE_DRAGONKILLER_SHATTER				33671	// does not make dmg - to script
#define GRUUL_THE_DRAGONKILLER_HURTFUL_STRIKE		33813	// +
#define GRUUL_THE_DRAGONKILLER_REVERBERATION		36297	// +
#define GRUUL_THE_DRAGONKILLER_STONED				33652	// +
#define GRUUL_THE_DRAGONKILLER_GRONN_LORDS_GRASP	33572	// Should be used only after Ground Slam

void SpellFunc_Gruul_GroundSlam(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_Gruul_Stoned(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_Gruul_Shatter(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class GruulTheDragonkillerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GruulTheDragonkillerAI, MoonScriptCreatureAI);
	GruulTheDragonkillerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		mHurtfulStrike = AddSpell(GRUUL_THE_DRAGONKILLER_HURTFUL_STRIKE, Target_Current, 0, 0, 0, 0, 8);
		mGroundSlam = AddSpell(GRUUL_THE_DRAGONKILLER_GROUND_SLAM, Target_Self, 0, 1, 0);
		mGroundSlam->AddEmote("Scurry.", Text_Yell, 11356);
		mGroundSlam->AddEmote("No escape.", Text_Yell, 11357);
		mGroundSlam2 = AddSpell(GRUUL_THE_DRAGONKILLER_GROUND_SLAM2, Target_Self, 0, 1, 0);
		mStoned = AddSpellFunc(&SpellFunc_Gruul_Stoned, Target_Self, 0, 2, 0);
		mShatter = AddSpellFunc(&SpellFunc_Gruul_Shatter, Target_Self, 0, 3, 0);
		mShatter2 = AddSpell(GRUUL_THE_DRAGONKILLER_SHATTER, Target_Self, 0, 1, 0);
		mShatter2->AddEmote("Stay...", Text_Yell, 11358);
		mShatter2->AddEmote("Beg for life.", Text_Yell, 11359);
		AddSpell(GRUUL_THE_DRAGONKILLER_REVERBERATION, Target_Self, 4, 0, 30);
		AddSpell(GRUUL_THE_DRAGONKILLER_CAVE_IN, Target_RandomPlayerDestination, 7, 0, 25);
		AddSpellFunc(&SpellFunc_Gruul_GroundSlam, Target_Self, 6, 1, 35);
		AddEmote(Event_OnCombatStart, "Come and die.", Text_Yell, 11355);
		AddEmote(Event_OnTargetDied, "No more.", Text_Yell, 11360);
		AddEmote(Event_OnTargetDied, "Unworthy.", Text_Yell, 11361);
		AddEmote(Event_OnTargetDied, "Die.", Text_Emote, 11362);
		AddEmote(Event_OnDied, "", Text_Yell, 11363);

		mGrowthTimer = mHurtfulTimer = -1;
		mGrowthStacks = 0;
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);

		mGrowthTimer = AddTimer(30000);
		mHurtfulTimer = AddTimer(8000);
		mGrowthStacks = 0;

		GameObject* pGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(166.897f, 368.226f, 16.9209f, 184662);
		if (pGate != NULL)
			pGate->SetUInt32Value(GAMEOBJECT_STATE, 1);
	}

	void OnCombatStop(Unit* pTarget)
	{
		ParentClass::OnCombatStop(pTarget);

		GameObject* pGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(166.897f, 368.226f, 16.9209f, 184662);
		if (pGate != NULL)
			pGate->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}

	void OnDied(Unit* mKiller)
	{
		ParentClass::OnDied(mKiller);

		GameObject* pGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(166.897f, 368.226f, 16.9209f, 184662);
		if (pGate != NULL)
			pGate->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}

	void AIUpdate()
	{
		if (!IsCasting())
		{
			if (IsTimerFinished(mGrowthTimer))
			{
				if (mGrowthStacks == 30)
				{
					RemoveAura(GRUUL_THE_DRAGONKILLER_GROWTH);
					mGrowthStacks = 0;
				}
				if (mGrowthStacks != 29)
				{
					ResetTimer(mGrowthTimer, 30000);
				}
				else if (mGrowthStacks == 29)
				{
					ResetTimer(mGrowthTimer, 300000);
				}

				ApplyAura(GRUUL_THE_DRAGONKILLER_GROWTH);
				++mGrowthStacks;
			}
			else if (IsTimerFinished(mHurtfulTimer))
			{
				Unit* pCurrentTarget = _unit->GetAIInterface()->GetNextTarget();
				if (pCurrentTarget != NULL)
				{
					Unit* pTarget = pCurrentTarget;
					for (set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr)  
					{
						Player *pPlayer = static_cast< Player* >(*itr);
						if (!pPlayer->isAlive())
							continue;
						if (pPlayer->m_invisible)
							continue;
						if (pPlayer->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
							continue;
						if (GetRangeToUnit(pPlayer) > 8.0f)
							continue;
						if (_unit->GetAIInterface()->getThreatByPtr(pPlayer) >= _unit->GetAIInterface()->getThreatByPtr(pCurrentTarget))
							continue;

						pTarget = TO_UNIT(pPlayer);
					}
					
					if (pTarget == pCurrentTarget)
						CastSpellNowNoScheduling(mHurtfulStrike);
					else
						_unit->CastSpell(pTarget, GRUUL_THE_DRAGONKILLER_HURTFUL_STRIKE, true);
				}

				ResetTimer(mHurtfulTimer, 8000);
			}
		}

		ParentClass::AIUpdate();
	}

	UnitArray GetInRangePlayers()
	{
		UnitArray TargetArray;
		for (set< Player* >::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
		{ 
			if (IsValidUnitTarget(*itr, TargetFilter_None))
			{
				TargetArray.push_back(static_cast< Unit* >(*itr));
			}
		}

		return TargetArray;
	}

	uint32		mGrowthStacks;		// temporary way to store it
	int32		mHurtfulTimer;
	int32		mGrowthTimer;

	SpellDesc*	mHurtfulStrike;
	SpellDesc*	mGroundSlam;
	SpellDesc*	mGroundSlam2;
	SpellDesc*	mStoned;
	SpellDesc*	mShatter;
	SpellDesc*	mShatter2;
};

void SpellFunc_Gruul_GroundSlam(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	GruulTheDragonkillerAI *pGruul = (pCreatureAI != NULL) ? (GruulTheDragonkillerAI*)pCreatureAI : NULL;
	if (pGruul != NULL)
	{
		pGruul->CastSpellNowNoScheduling(pGruul->mGroundSlam);
		pGruul->CastSpell(pGruul->mGroundSlam2);
		pGruul->CastSpell(pGruul->mStoned);
	}
}

void SpellFunc_Gruul_Stoned(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	GruulTheDragonkillerAI *pGruul = (pCreatureAI != NULL) ? (GruulTheDragonkillerAI*)pCreatureAI : NULL;
	if (pGruul != NULL)
	{
		UnitArray TargetArray = pGruul->GetInRangePlayers();
		if (TargetArray.size() > 0)
		{
			for (size_t i = 0; i < TargetArray.size(); ++i)
			{
				Unit* pTarget = TargetArray[i];
				pTarget->CastSpell(pTarget, GRUUL_THE_DRAGONKILLER_STONED, true);
			}
		}

		pGruul->CastSpell(pGruul->mShatter);
	}
}

void SpellFunc_Gruul_Shatter(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	GruulTheDragonkillerAI *pGruul = (pCreatureAI != NULL) ? (GruulTheDragonkillerAI*)pCreatureAI : NULL;
	if (pGruul != NULL)
	{
		pGruul->CastSpell(pGruul->mShatter2);	// Spell to script
		UnitArray TargetArray = pGruul->GetInRangePlayers();
		if (TargetArray.size() > 0)
		{
			for (size_t i = 0; i < TargetArray.size(); ++i)
			{
				Unit* pTarget = TargetArray[i];
				pTarget->RemoveAura(GRUUL_THE_DRAGONKILLER_STONED);
			}
		}
	}
}

void SetupBoss_Gruul(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_GRUUL_THE_DRAGONKILLER, &GruulTheDragonkillerAI::Create);
}
