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


//Trash mobs

//Coilskar Witch
#define CN_COILSKAR_WITCH 24696

#define COILSKAR_WITCH_FORKED_LIGHTNING 46150
#define COILSKAR_WITCH_FROST_ARROW 44639
#define COILSKAR_WITCH_MANA_SHIELD 46151
#define COILSKAR_WITCH_SHOOT 35946

class CoilskarWitchAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CoilskarWitchAI, MoonScriptBossAI);
	CoilskarWitchAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(COILSKAR_WITCH_FORKED_LIGHTNING, Target_Current, 60, 2, 12, 0, 30);
		AddSpell(COILSKAR_WITCH_FROST_ARROW, Target_RandomPlayer, 15, 0, 16, 0, 40);
		AddSpell(COILSKAR_WITCH_MANA_SHIELD, Target_Self, 6, 0, 40, 0, 0);
		AddSpell(COILSKAR_WITCH_SHOOT, Target_RandomPlayer, 75, 1.5, 4, 5, 30);
	}
	
};

//Sister of Torment
#define CN_SISTER_OF_TORMENT 24697

#define SISTER_OF_TORMENT_DEADLY_EMBRACE 44547
#define SISTER_OF_TORMENT_LASH_OF_PAIN 44640

class SisterOfTormentAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SisterOfTormentAI, MoonScriptBossAI);
	SisterOfTormentAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(SISTER_OF_TORMENT_LASH_OF_PAIN, Target_Current, 60, 0, 8, 0, 5);
		AddSpell(SISTER_OF_TORMENT_DEADLY_EMBRACE, Target_RandomPlayer, 20, 1.5, 16, 0, 20);
	}
	
};

//Sunblade Blood Knight
#define CN_SB_BLOOD_KNIGHT 24684

#define BLOOD_KNIGHT_HOLY_LIGHT 46029
#define BLOOD_KNIGHT_JUDGEMENT_OF_WRATH 44482 
#define BLOOD_KNIGHT_SEAL_OF_WRATH 46030

class SunbladeBloodKnightAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SunbladeBloodKnightAI, MoonScriptBossAI);
	SunbladeBloodKnightAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(BLOOD_KNIGHT_JUDGEMENT_OF_WRATH, Target_Current, 20, 0, 30, 0, 5);
		AddSpell(BLOOD_KNIGHT_SEAL_OF_WRATH, Target_Self, 99, 0, 30, 0, 0);
		AddSpell(BLOOD_KNIGHT_HOLY_LIGHT, Target_Self, 10, 2, 30, 0, 40);		
	}
	
};

//Sunblade Imp
#define CN_SB_IMP 24815

#define IMP_FIREBOLT 44577

class SunbladeImpAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SunbladeImpAI, MoonScriptBossAI);
	SunbladeImpAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(IMP_FIREBOLT, Target_Current, 100, 2, (int32)2.5, 0, 30);
	}

};

//Sunblade Mage Guard 
#define CN_SB_MAGE_GUARD 24683

#define MAGE_GUARD_GLAVE_THROW 46028
#define MAGE_GUARD_MAGIC_DAMPENING_FIELD 44475

class SunbladeMageGuardAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SunbladeMageGuardAI, MoonScriptBossAI);
	SunbladeMageGuardAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(MAGE_GUARD_GLAVE_THROW, Target_Current, 60, 0, 25, 0, 5);
		AddSpell(MAGE_GUARD_MAGIC_DAMPENING_FIELD, Target_Self, 20, 1, 35, 0, 20);
	}

};

//Sunblade Magister
#define CN_SB_MAGISTER 24685 

#define MAGISTER_ARCANE_NOVA 46036
#define MAGISTER_FROSTBOLT 46035

class SunbladeMagisterAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SunbladeMagisterAI, MoonScriptBossAI);
	SunbladeMagisterAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(MAGISTER_FROSTBOLT, Target_Current, 65, 2, 4, 0, 30);
		AddSpell(MAGISTER_ARCANE_NOVA, Target_Self, 12, 1.5, 40, 0, 0);
	}

};

#define CREATURE_BRIGHTSCALE_WYRM 24761

class mob_Brightscale_wyrmAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(mob_Brightscale_wyrmAI, MoonScriptCreatureAI);
	mob_Brightscale_wyrmAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{

	}
	
	void OnLoad()
	{
	  SetFlyMode( true );
	  
	  RegisterAIUpdateEvent( 100 );
	  ParentClass::OnLoad();
	}
	
	
	void OnCombatStart(Unit* mTarget)
	{
	
	  ParentClass::OnCombatStart(mTarget);
	  
	}
	
	void OnCombatStop(Unit* mTarget)
	{
	
	  ParentClass::OnCombatStop(mTarget);
	  
	}
	
	void AIUpdate()
	{
		ParentClass::AIUpdate();
	}
	
	

};

void SetupMagistersTerrace(ScriptMgr* pScriptMgr) 
{
	pScriptMgr->register_creature_script(CN_COILSKAR_WITCH, &CoilskarWitchAI::Create);
	pScriptMgr->register_creature_script(CN_SISTER_OF_TORMENT, &SisterOfTormentAI::Create);
	pScriptMgr->register_creature_script(CN_SB_IMP, &SunbladeImpAI::Create);
	pScriptMgr->register_creature_script(CN_SB_MAGE_GUARD, &SunbladeMageGuardAI::Create);
	pScriptMgr->register_creature_script(CN_SB_MAGISTER, &SunbladeMagisterAI::Create);
	pScriptMgr->register_creature_script(CREATURE_BRIGHTSCALE_WYRM, &mob_Brightscale_wyrmAI::Create);
}