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
#include "Sethek_Halls.h"


class darkweaversythai : public MoonScriptBossAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION(darkweaversythai, MoonScriptBossAI);
	SP_AI_Spell spells[9];
	bool m_spellcheck[9];

    darkweaversythai(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		// Not sure in any way about target types
        spells[0].info = dbcSpell.LookupEntry(FROST_SHOCK);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 2000;
		spells[0].cooldown = 15;

        spells[1].info = dbcSpell.LookupEntry(FLAME_SHOCK);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 15;

        spells[2].info = dbcSpell.LookupEntry(SHADOW_SHOCK);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 15;

        spells[3].info = dbcSpell.LookupEntry(ARCANE_SHOCK);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 8.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 15;

        spells[4].info = dbcSpell.LookupEntry(CHAIN_LIGHTNING);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 10.0f;
		spells[4].attackstoptimer = 1000;
		spells[4].cooldown = 15;

        spells[5].info = dbcSpell.LookupEntry(SUMMON_SYTH_FIRE_ELEMENTAL);
		spells[5].targettype = TARGET_SELF;
		spells[5].instant = true;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;
		spells[5].cooldown = 10;

        spells[6].info = dbcSpell.LookupEntry(SUMMON_SYTH_FROST_ELEMENTAL);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = true;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;
		spells[6].cooldown = -1;

        spells[7].info = dbcSpell.LookupEntry(SUMMON_SYTH_ARCANE_ELEMENTAL);
		spells[7].targettype = TARGET_SELF;
		spells[7].instant = true;
		spells[7].perctrigger = 0.0f;
		spells[7].attackstoptimer = 1000;
		spells[7].cooldown = -1;

        spells[8].info = dbcSpell.LookupEntry(SUMMON_SYTH_SHADOW_ELEMENTAL);
		spells[8].targettype = TARGET_SELF;
		spells[8].instant = true;
		spells[8].perctrigger = 0.0f;
		spells[8].attackstoptimer = 1000;
		spells[8].cooldown = -1;

		Summons = 0;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		for (int i = 0; i < 9; i++)
			spells[i].casttime = 0;

		Summons = 0;

		int RandomSpeach = rand()%3;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time to... make my move!"); // needs corrections
			_unit->PlaySoundToSet(10503);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nice pets, yes!"); // corrections needed!
			_unit->PlaySoundToSet(10504);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nice pets have... weapons, not so... nice!");
			_unit->PlaySoundToSet(10505);
			break;
		}

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach = rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes! Fleeting life is..."); // need to add it
				_unit->PlaySoundToSet(10506);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Be free!"); // corrections needed!!
				_unit->PlaySoundToSet(10507);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		Summons = 0;
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No more life, no more pain!"); // It's talking so <censored>
		_unit->PlaySoundToSet(10508);

		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		uint32 t = (uint32)time(NULL);
		if (t > spells[4].casttime && ((_unit->GetHealthPct() <= 75 && Summons == 0) || (_unit->GetHealthPct() <= 50 && Summons == 1) || (_unit->GetHealthPct() <= 25 && Summons == 2)))
		{
			_unit->setAttackTimer(1500, false);
			_unit->GetAIInterface()->StopMovement(1000);	// really?

			SummonElementalWave();

			spells[4].casttime = t + spells[4].cooldown;
			Summons++;
			return;
		}

		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void SummonElementalWave()
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have pets... of my own"); // It's talking so <doublecensored> -.-'
		_unit->PlaySoundToSet(10502);

		_unit->CastSpell(_unit, spells[5].info, spells[5].instant);
		_unit->CastSpell(_unit, spells[6].info, spells[6].instant);
		_unit->CastSpell(_unit, spells[7].info, spells[7].instant);
		_unit->CastSpell(_unit, spells[8].info, spells[8].instant);		
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

	uint32 Summons;
	int nrspells;
};


void SetupBoss_Darkweaver_Syth(ScriptMgr* mgr)
{
 mgr->register_creature_script(CREATURE_DARKWEAVER_SYTH, &darkweaversythai::Create);
}