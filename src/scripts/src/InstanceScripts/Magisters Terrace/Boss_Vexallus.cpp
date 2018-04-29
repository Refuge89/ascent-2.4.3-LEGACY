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
#include "../../InstanceScripts/Base/setup.h"
#include "../../InstanceScripts/Base/base.h"

/*
	Vexallus
*/
#define BOSS_VEXALLUS 24744
#define CN_PURE_ENERGY 24745

#define VEXALLUS_CHAIN_LIGHTNING 44318
#define VEXALLUS_OVERLOAD 44353
#define VEXALLUS_ARCANE_SHOCK 44319
#define VEXALLUS_SUMMON_PURE_ENERGY 44322

class VexallusAI : public MoonScriptBossAI
{
		MOONSCRIPT_FACTORY_FUNCTION(VexallusAI, MoonScriptBossAI);
		VexallusAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			AddPhaseSpell(1, AddSpell(VEXALLUS_CHAIN_LIGHTNING, Target_Current, 19, 0, 8, 0, 0));
			AddPhaseSpell(1, AddSpell(VEXALLUS_ARCANE_SHOCK, Target_ClosestPlayer, 12, 0, 20, 0, 0, true, "Un...con...tainable.", Text_Yell, 12392));
			AddPhaseSpell(2, AddSpell(VEXALLUS_OVERLOAD, Target_Self, 85, 0, 3, 0, 0));
			mPureEnergy = AddSpell(VEXALLUS_SUMMON_PURE_ENERGY, Target_Self, 85, 0, 3);

			AddEmote(Event_OnTargetDied, "Con...sume.", Text_Yell, 12393);

			mSummon = 0;
		}

		void OnCombatStart(Unit* pTarget)
		{
			Emote("Drain... life...", Text_Yell, 12389);
			SetPhase(1);
			ParentClass::OnCombatStart(pTarget);
		}

		void AIUpdate()
		{
			if((GetHealthPercent() <= 85  && mSummon == 0) ||
			        (GetHealthPercent() <= 70 && mSummon == 1) ||
			        (GetHealthPercent() <= 55 && mSummon == 2) ||
			        (GetHealthPercent() <= 40 && mSummon == 3) ||
			        (GetHealthPercent() <= 25 && mSummon == 4))
			{
				CastSpell(mPureEnergy);
				++mSummon;
			}

			if(GetHealthPercent() <= 10 && GetPhase() == 1)
				SetPhase(2);


			ParentClass::AIUpdate();
		}

		SpellDesc*	mPureEnergy;
		uint8		mSummon;
};

void SetupVexallus(ScriptMgr* pScriptMgr) 
{
	pScriptMgr->register_creature_script(BOSS_VEXALLUS, &VexallusAI::Create);	
}
