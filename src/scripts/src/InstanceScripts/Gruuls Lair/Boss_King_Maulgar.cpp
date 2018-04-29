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

/* TODO: BFX Update AI update spell timers rather then having it in class structure.*/
// Move things to own functions rather then using class

#include "StdAfx.h"
#include "../../InstanceScripts/Base/setup.h"
#include "../../InstanceScripts/Base/base.h"

#define CN_HIGH_KING_MAULGAR				18831
#define HIGH_KING_MAULGAR_BERSERKER_CHARGE	26561
#define HIGH_KING_MAULGAR_INTIMIDATING_ROAR	16508
#define HIGH_KING_MAULGAR_MIGHTY_BLOW		33230
#define HIGH_KING_MAULGAR_FLURRY			33232
#define HIGH_KING_MAULGAR_ARCING_SMASH		28168
#define HIGH_KING_MAULGAR_ARCING_SMASH2		39144
#define HIGH_KING_MAULGAR_WHIRLWIND			33238
#define HIGH_KING_MAULGAR_WHIRLWIND2		33239

void SpellFunc_Maulgar_Enrage(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
// 4th unit sometimes cannot be found - blame cell system
uint32 Adds[4] = { 18832, 18834, 18836, 18835 };

class HighKingMaulgarAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HighKingMaulgarAI, MoonScriptBossAI);
	HighKingMaulgarAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddPhaseSpell(2, AddSpell(HIGH_KING_MAULGAR_BERSERKER_CHARGE, Target_RandomPlayer, 10, 0, 25, 0, 40));
		AddPhaseSpell(2, AddSpell(HIGH_KING_MAULGAR_INTIMIDATING_ROAR, Target_Current, 7, 0, 20, 0, 5));
		AddSpell(HIGH_KING_MAULGAR_ARCING_SMASH, Target_Current, 8, 0, 15, 0, 10);
		AddSpell(HIGH_KING_MAULGAR_WHIRLWIND, Target_Self, 7, 15, 25);					// SpellFunc for range check?
		AddSpell(HIGH_KING_MAULGAR_MIGHTY_BLOW, Target_Current, 7, 0, 20, 0, 5);
		mEnrage = AddSpellFunc(&SpellFunc_Maulgar_Enrage, Target_Self, 0, 0, 0);
		mEnrage->AddEmote("You will not defeat the hand of Gruul!", Text_Yell, 11368);
		AddEmote(Event_OnCombatStart, "Gronn are the real power in Outland!", Text_Yell, 11367);
		AddEmote(Event_OnTargetDied, "You not so tough after all!", Text_Yell, 11373);
		AddEmote(Event_OnTargetDied, "Maulgar is king!", Text_Yell, 11375);
		AddEmote(Event_OnTargetDied, "", Text_Emote, 11374);
		AddEmote(Event_OnDied, "Grull... will crush you!", Text_Yell, 11376);

		mLastYell = -1;
		mAliveAdds = 0;
	}

	void OnCombatStart(Unit* pTarget)
	{
		SetDisplayWeapon(true, true);
		ParentClass::OnCombatStart(pTarget);

		mAliveAdds = 0;
		mLastYell = -1;
		for (int i = 0; i < 4; ++i)
		{
			Unit* pAdd = ForceCreatureFind(Adds[i]);
			if (pAdd != NULL && pAdd->isAlive())
			{
				Unit* pTarget = GetBestPlayerTarget();
				if (pTarget != NULL)
				{
					pAdd->GetAIInterface()->AttackReaction(pTarget, 200);
				}

				++mAliveAdds;
			}
		}
		if (mAliveAdds > 1)
		{
			SetCanEnterCombat(false);
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void OnCombatStop(Unit* pTarget)
	{
		ParentClass::OnCombatStop(pTarget);
		SetCanEnterCombat(true);
	}

	void OnDied(Unit* mKiller)
	{
		ParentClass::OnDied(mKiller);

		GameObject* pDoor = mKiller->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(95.26f, 251.836f, 0.47f, 183817);
		if(pDoor != NULL)
		{
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		}
	}

	void AIUpdate()
	{
		if (mAliveAdds > 1)
			return;

		if (GetPhase() == 1 && GetHealthPercent() <= 50)
		{
			SetPhase(2, mEnrage);
		}

		ParentClass::AIUpdate();
	}

	void OnAddDied()
	{
		if (mAliveAdds > 0)
		{
			--mAliveAdds;
			if (mAliveAdds > 1)
			{
				uint32 RandomText = RandomUInt(1);
				while ((int)RandomText == mLastYell)
				{
					RandomText = RandomUInt(1);
				}

				switch (RandomText)
				{
				case 0:
					Emote("You not kill next one so easy!", Text_Yell, 11369);
					break;
				case 1:
					Emote("Does not prove anything!", Text_Yell, 11370);
					break;
				}

				mLastYell = RandomText;
			}
			else if (mAliveAdds == 1)
			{
				Emote("Good, now you fight me!", Text_Yell, 0);
				SetCanEnterCombat(true);
				SetBehavior(Behavior_Default);
				SetCanMove(true);
			}
		}
	}

	uint32		mAliveAdds;
	int32		mLastYell;
	SpellDesc*	mEnrage;
};

void SpellFunc_Maulgar_Enrage(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	HighKingMaulgarAI *pMaulgarAI = (pCreatureAI != NULL) ? (HighKingMaulgarAI*)pCreatureAI : NULL;
	if (pMaulgarAI != NULL)
	{
		pMaulgarAI->ApplyAura(HIGH_KING_MAULGAR_FLURRY);
		pMaulgarAI->SetDisplayWeapon(false, false);
	}
}

#define CN_KIGGLER_THE_CRAZED					18835
#define KIGGLER_THE_CRAZED_LIGHTNING_BOLT		36152
#define KIGGLER_THE_CRAZED_GREATER_POLYMORPH	33173
#define KIGGLER_THE_CRAZED_ARCANE_EXPLOSION		33237
#define	KIGGLER_THE_CRAZED_ARCANE_SHOCK			33175

class KigglerTheCrazedAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KigglerTheCrazedAI, MoonScriptCreatureAI);
	KigglerTheCrazedAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(KIGGLER_THE_CRAZED_LIGHTNING_BOLT, Target_Current, 70, 2, 0, 0, 40);
		AddSpell(KIGGLER_THE_CRAZED_GREATER_POLYMORPH, Target_RandomPlayer, 8, 0, 15, 0, 30);	// Additional SpellFunc for removing target from target list if there are different targets?
		AddSpell(KIGGLER_THE_CRAZED_ARCANE_EXPLOSION, Target_Self, 8, 0, 20);
		AddSpell(KIGGLER_THE_CRAZED_ARCANE_SHOCK, Target_RandomPlayer, 10, 0, 15, 0, 30);
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);

		if (GetRangeToUnit(pTarget) <= 40.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void OnDied(Unit* mKiller)
	{
		ParentClass::OnDied(mKiller);
		Creature* pMaulgar = TO_CREATURE(ForceCreatureFind(CN_HIGH_KING_MAULGAR, 143.048996f, 192.725998f, -11.114700f));
		if (pMaulgar != NULL && pMaulgar->isAlive() && pMaulgar->GetScript())
		{
			HighKingMaulgarAI *pMaulgarAI = static_cast<HighKingMaulgarAI*>(pMaulgar->GetScript());
			pMaulgarAI->OnAddDied();
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		Unit* pTarget = _unit->GetAIInterface()->GetNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 40.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}
};

#define CN_BLINDEYE_THE_SEER						18836
#define BLINDEYE_THE_SEER_PRAYER_OF_HEALING			33152
#define BLINDEYE_THE_SEER_GREAT_POWER_WORD_SHIELD	33147
#define BLINDEYE_THE_SEER_HEAL						33144

class BlindeyeTheSeerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BlindeyeTheSeerAI, MoonScriptCreatureAI);
	BlindeyeTheSeerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BLINDEYE_THE_SEER_PRAYER_OF_HEALING, Target_Self, 5, 4, 30);				// Affects players? Core bugzor?
		AddSpell(BLINDEYE_THE_SEER_GREAT_POWER_WORD_SHIELD, Target_Self, 8, 0, 30);			// Strategies don't say anything about buffing friends
		AddSpell(BLINDEYE_THE_SEER_HEAL, Target_WoundedFriendly, 8, 1.5, 25);
	}

	void OnDied(Unit* mKiller)
	{
		ParentClass::OnDied(mKiller);
		Creature* pMaulgar = TO_CREATURE(ForceCreatureFind(CN_HIGH_KING_MAULGAR, 143.048996f, 192.725998f, -11.114700f));
		if (pMaulgar != NULL && pMaulgar->isAlive() && pMaulgar->GetScript())
		{
			HighKingMaulgarAI *pMaulgarAI = static_cast<HighKingMaulgarAI*>(pMaulgar->GetScript());
			pMaulgarAI->OnAddDied();
		}
	}
};

#define CN_OLM_THE_SUMMONER							18834
#define OLM_THE_SUMMONER_DEATH_COIL					33130
#define OLM_THE_SUMMONER_SUMMON_WILD_FELHUNTER		33131
#define OLM_THE_SUMMONER_DARK_DECAY					33129

class OlmTheSummonerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(OlmTheSummonerAI, MoonScriptCreatureAI);
	OlmTheSummonerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(OLM_THE_SUMMONER_DEATH_COIL, Target_RandomPlayer, 7, 0, 10, 0, 30);
		AddSpell(OLM_THE_SUMMONER_SUMMON_WILD_FELHUNTER, Target_Self, 7, 3, 15);
		AddSpell(OLM_THE_SUMMONER_DARK_DECAY, Target_RandomPlayer, 10, 0, 6);
	}

	void OnDied(Unit* mKiller)
	{
		ParentClass::OnDied(mKiller);
		Creature* pMaulgar = TO_CREATURE(ForceCreatureFind(CN_HIGH_KING_MAULGAR, 143.048996f, 192.725998f, -11.114700f));
		if (pMaulgar != NULL && pMaulgar->isAlive() && pMaulgar->GetScript())
		{
			HighKingMaulgarAI *pMaulgarAI = static_cast<HighKingMaulgarAI*>(pMaulgar->GetScript());
			pMaulgarAI->OnAddDied();
		}
	}
};

#define CN_WILD_FEL_STALKER			18847
#define WILD_FEL_STALKER_WILD_BITE	33086

class WildFelStalkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WildFelStalkerAI, MoonScriptCreatureAI);
	WildFelStalkerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(WILD_FEL_STALKER_WILD_BITE, Target_Current, 10, 0, 10, 0, 5);
		AggroRandomPlayer(200);
	}
};

#define CN_KROSH_FIREHAND      18832

/* He will first spellshield on himself, and recast every 30 sec,
   then spam great fireball to the target, also if there is any unit
   close to him (15yr) he'll cast blast wave
*/

#define GREAT_FIREBALL         33051
#define BALST_WAVE                     33061
#define SPELLSHIELD                    33054

class KroshFirehandAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KroshFirehandAI, MoonScriptCreatureAI);
	KroshFirehandAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		//spells
		mBlastWave = AddSpell(BALST_WAVE, Target_Self, 0, 0, 0, 0, 15);
		AddSpell(GREAT_FIREBALL, Target_Current, 100, 3, 0, 0, 100);
		mSpellShield = AddSpell(SPELLSHIELD, Target_Self, 0, 0, 0);

		mEventTimer = AddTimer(30000);
		mBlastWaveTimer = -1;
		SetAIUpdateFreq(250);
	}

	void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(mSpellShield);

		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate()
	{
		if (!IsCasting())
		{
			if (mBlastWaveTimer == -1 || IsTimerFinished(mBlastWaveTimer))
			{
				Unit* unit = GetBestUnitTarget(TargetFilter_Closest);
				if (unit && GetRangeToUnit(unit) < 15.0f)
				{
					CastSpellNowNoScheduling(mBlastWave);
					if (mBlastWaveTimer == -1)
						mBlastWaveTimer = AddTimer(6000);
					else
						ResetTimer(mBlastWaveTimer, 6000);
					ParentClass::AIUpdate();
					return;
				}
			}

			if (IsTimerFinished(mEventTimer))
			{
				ResetTimer(mEventTimer, 30000);
				CastSpellNowNoScheduling(mSpellShield);
			}
		}

		ParentClass::AIUpdate();
	}

	void OnDied(Unit* mKiller)
	{
		ParentClass::OnDied(mKiller);
		Creature* pMaulgar = TO_CREATURE(ForceCreatureFind(CN_HIGH_KING_MAULGAR, 143.048996f, 192.725998f, -11.114700f));
		if (pMaulgar != NULL && pMaulgar->isAlive() && pMaulgar->GetScript())
		{
			HighKingMaulgarAI *pMaulgarAI = static_cast<HighKingMaulgarAI*>(pMaulgar->GetScript());
			pMaulgarAI->OnAddDied();
		}
	}

	SpellDesc*      mSpellShield;
	SpellDesc*      mBlastWave;
	int32           mEventTimer;
	int32           mBlastWaveTimer;
};

void SetupBoss_Maulgar(ScriptMgr * mgr)
{
	
	mgr->register_creature_script(CN_KIGGLER_THE_CRAZED, &KigglerTheCrazedAI::Create);
	mgr->register_creature_script(CN_BLINDEYE_THE_SEER, &BlindeyeTheSeerAI::Create);
	mgr->register_creature_script(CN_OLM_THE_SUMMONER, &OlmTheSummonerAI::Create);
	mgr->register_creature_script(CN_WILD_FEL_STALKER, &WildFelStalkerAI::Create);
	mgr->register_creature_script(CN_KROSH_FIREHAND, &KroshFirehandAI::Create);
	mgr->register_creature_script(CN_HIGH_KING_MAULGAR,	&HighKingMaulgarAI::Create);	
}