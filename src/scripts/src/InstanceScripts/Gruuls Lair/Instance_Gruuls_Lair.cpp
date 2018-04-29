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



#define CN_LAIR_BRUTE				19389
#define LAIR_BRUTE_MORTALSTRIKE		39171
#define LAIR_BRUTE_CLEAVE			39174
#define LAIR_BRUTE_CHARGE			24193

void SpellFunc_LairBrute_Charge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class LairBruteAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LairBruteAI, MoonScriptCreatureAI);
	LairBruteAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(LAIR_BRUTE_CLEAVE, Target_Current, 20, 0, 15, 0, 7);
		AddSpell(LAIR_BRUTE_MORTALSTRIKE, Target_Current, 8, 0, 20, 0, 7);
		mCharge = AddSpell(LAIR_BRUTE_CHARGE, Target_Current, 0, 0, 0, 0, 40);
		AddSpellFunc(&SpellFunc_LairBrute_Charge, Target_Current, 7, 0, 35, 0, 40);
	}

	SpellDesc*	mCharge;
};

void SpellFunc_LairBrute_Charge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	LairBruteAI *pBruteAI = (pCreatureAI != NULL) ? (LairBruteAI*)pCreatureAI : NULL;
	if (pBruteAI != NULL)
	{
		Unit* pCurrentTarget = pBruteAI->GetUnit()->GetAIInterface()->GetNextTarget();
		if (pCurrentTarget != NULL && pCurrentTarget != pTarget)
		{
			pBruteAI->GetUnit()->GetAIInterface()->AttackReaction(pTarget, 500);
			pBruteAI->GetUnit()->GetAIInterface()->SetNextTarget(pTarget);
			pBruteAI->GetUnit()->GetAIInterface()->RemoveThreatByPtr(pCurrentTarget);
		}

		pBruteAI->CastSpell(pBruteAI->mCharge);
	}
}

#define CN_GRONN_PRIEST					21350
#define GRONN_PRIEST_PSYCHICSCREAM		22884
#define GRONN_PRIEST_RENEW				36679
#define GRONN_PRIEST_HEAL				36678

class GronnPriestAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GronnPriestAI, MoonScriptCreatureAI);
	GronnPriestAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(GRONN_PRIEST_PSYCHICSCREAM, Target_Self, 8, 0, 20);
		AddSpell(GRONN_PRIEST_RENEW, Target_WoundedFriendly, 6, 0, 25, 0, 100);
		AddSpell(GRONN_PRIEST_HEAL, Target_WoundedFriendly, 5, 2, 30, 0, 100);
	}
};




void SetupInstanceGruuls_Lair(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_LAIR_BRUTE, &LairBruteAI::Create);
	mgr->register_creature_script(CN_GRONN_PRIEST, &GronnPriestAI::Create);
}

