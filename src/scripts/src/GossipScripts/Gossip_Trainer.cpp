/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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
#include "Setup.h"

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
    Menu->SendTo(plr);

class MasterHammersmith : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7245, plr);
        
		Menu->AddItem( 0, "Please teach me how to become a hammersmith, Lilith.", 1);
		Menu->AddItem( 0, "I wish to unlearn Hammersmithing!", 2);

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object * pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		GossipMenu * Menu;
		Creature * pCreature;
		if( pObject->IsCreature() )
			pCreature = TO_CREATURE( pObject );

		switch (IntId)	// switch and case 0 can be deleted, but I added it, because in future maybe we will have to expand script with more options.
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;

			case 1:
			{
				if (!plr->_HasSkillLine(164) || plr->_GetSkillLineCurrent(164, false) < 300)
				{
				    pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Only skilled blacksmiths can obtain this knowledge." );
					SendQuickMenu(20001);
				}
		
				else if (!plr->HasSpell(9787))
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need to know Weaponsmith first to learn anything more from me." );
					SendQuickMenu(20002);
				}
		
				else if (plr->HasSpell(17040))
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You already know that." );
					SendQuickMenu(20003);
				}
	
				else if (plr->HasSpell(17041) || plr->HasSpell(17039) || plr->HasSpell(9788))
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You already know one specialization." );
					SendQuickMenu(20004);
				}
		
				else
				{
					if ( plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 600 )
					{
						pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need 6 silver coins to learn this skill.");
						SendQuickMenu(20005);
					}
		
					else
					{
						pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Make good use of this knowledge." );
						SendQuickMenu(20006);
						Creature *Trainer = TO_CREATURE(pObject);
						Trainer->CastSpell(plr, 39099, true);
						int32 gold = plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
						plr->SetUInt32Value(PLAYER_FIELD_COINAGE, gold - 600);
					}	
				}
			}break;

			case 2:
			{
				if (!plr->HasSpell(17040))
				{
					SendQuickMenu(20007);
				}

				else if ((plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 250000 && plr->getLevel() <= 50) ||
						(plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 500000 && plr->getLevel() > 50 && plr->getLevel() <= 65) ||
						(plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 1000000 && plr->getLevel() > 65))
				{
					SendQuickMenu(20008);
				}

				else
				{
					int32 unlearnGold = 0;
					if (plr->getLevel() <= 50)
						unlearnGold = 250000;
					if (plr->getLevel() > 50 && plr->getLevel() <= 65)
						unlearnGold = 500000;
					if (plr->getLevel() > 65)
						unlearnGold = 1000000;

                    plr->SetUInt32Value(PLAYER_FIELD_COINAGE, plr->GetUInt32Value(PLAYER_FIELD_COINAGE) - unlearnGold);
					plr->removeSpell(17040, false, false, 0);
					SendQuickMenu(20009);
				}
			}break;
		}
	}

    void Destroy()
    {
        
    }
};

class MasterSwordsmith : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7247, plr);
        
		Menu->AddItem( 0, "Please teach me how to become a swordsmith, Seril.", 1);
		Menu->AddItem( 0, "I wish to unlearn Swordsmithing!", 2);

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object * pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		GossipMenu * Menu;
		Creature * pCreature;
		if( pObject->IsCreature() )
			pCreature = TO_CREATURE( pObject );
		switch (IntId)	// switch and case 0 can be deleted, but I added it, because in future maybe we will have to expand script with more options.
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;

			case 1:
			{
				if (!plr->_HasSkillLine(164) || plr->_GetSkillLineCurrent(164, false) < 300)
				{
				    pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Only skilled blacksmiths can obtain this knowledge." );
					SendQuickMenu(20001);
				}
		
				else if (!plr->HasSpell(9787))
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need to know Weaponsmith first to learn anything more from me." );
					SendQuickMenu(20002);
				}
		
				else if (plr->HasSpell(17039))
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You already know that." );
					SendQuickMenu(20003);
				}
	
				else if (plr->HasSpell(17041) || plr->HasSpell(17040) || plr->HasSpell(9788))
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You already know one specialization." );
					SendQuickMenu(20004);
				}
		
				else
				{
					if (plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 600)
					{
						//pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need 6 silver coins to learn this skill.");
						SendQuickMenu(20005);
					}
		
					else
					{
						pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Make good use of this knowledge." );
						SendQuickMenu(20006);
						Creature *Trainer = TO_CREATURE(pObject);
						Trainer->CastSpell(plr, 39097, true);
						int32 gold = plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
						plr->SetUInt32Value(PLAYER_FIELD_COINAGE, gold - 600);
					}	
				}
			}break;

			case 2:
			{
				if (!plr->HasSpell(17039))
				{
					SendQuickMenu(20007);
				}

				else if ((plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 250000 && plr->getLevel() <= 50) ||
						(plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 500000 && plr->getLevel() > 50 && plr->getLevel() <= 65) ||
						(plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 1000000 && plr->getLevel() > 65))
				{
					SendQuickMenu(20008);
				}

				else
				{
					int32 unlearnGold;
					if (plr->getLevel() <= 50)
						unlearnGold = 250000;
					if (plr->getLevel() > 50 && plr->getLevel() <= 65)
						unlearnGold = 500000;
					if (plr->getLevel() > 65)
						unlearnGold = 1000000;
										
					plr->SetUInt32Value(PLAYER_FIELD_COINAGE, plr->GetUInt32Value(PLAYER_FIELD_COINAGE) - unlearnGold);
					plr->removeSpell(17039, false, false, 0);
					SendQuickMenu(20009);
				}
			}break;
		}
	}

    void Destroy()
    {
        
    }
};

class MasterAxesmith : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7243, plr);
        
		Menu->AddItem( 0, "Please teach me how to become a axesmith, Kilram.", 1);
		Menu->AddItem( 0, "I wish to unlearn Axesmithing!", 2);

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object * pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		GossipMenu * Menu;
		Creature * pCreature;
		if( pObject->IsCreature() )
			pCreature = TO_CREATURE( pObject );
		switch (IntId)	// switch and case 0 can be deleted, but I added it, because in future maybe we will have to expand script with more options.
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;

			case 1:
			{
				if (!plr->_HasSkillLine(164) || plr->_GetSkillLineCurrent(164, false) < 300)
				{
				    pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Only skilled blacksmiths can obtain this knowledge." );
					SendQuickMenu(20001);
				}
		
				else if (!plr->HasSpell(9787))
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need to know Weaponsmith first to learn anything more from me." );
					SendQuickMenu(20002);
				}
		
				else if (plr->HasSpell(17041))
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You already know that." );
					SendQuickMenu(20003);
				}
	
				else if (plr->HasSpell(17039) || plr->HasSpell(17040) || plr->HasSpell(9788))
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You already know one specialization." );
					SendQuickMenu(20004);
				}
		
				else
				{
					if (plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 600)
					{
						pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need 6 silver coins to learn this skill.");
						SendQuickMenu(20005);
					}
		
					else
					{
						pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Make good use of this knowledge." );
						SendQuickMenu(20006);
						Creature *Trainer = TO_CREATURE(pObject);
						Trainer->CastSpell(plr, 39098, true);
						int32 gold = plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
						plr->SetUInt32Value(PLAYER_FIELD_COINAGE, gold - 600);
					}	
				}
			}break;

			case 2:
			{
				if (!plr->HasSpell(17041))
				{
					SendQuickMenu(20007);
				}

				else if ((plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 250000 && plr->getLevel() <= 50) ||
						(plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 500000 && plr->getLevel() > 50 && plr->getLevel() <= 65) ||
						(plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 1000000 && plr->getLevel() > 65))
				{
					SendQuickMenu(20008);
				}

				else
				{
					int32 unlearnGold;
					if (plr->getLevel() <= 50)
						unlearnGold = 250000;
					if (plr->getLevel() > 50 && plr->getLevel() <= 65)
						unlearnGold = 500000;
					if (plr->getLevel() > 65)
						unlearnGold = 1000000;

					plr->SetUInt32Value(PLAYER_FIELD_COINAGE, plr->GetUInt32Value(PLAYER_FIELD_COINAGE) - unlearnGold);
					plr->removeSpell(17041, false, false, 0);
					SendQuickMenu(20009);
				}
			}break;
		}
	}

    void Destroy()
    {
        
    }
};

class SCRIPT_DECL ShadoweaveTailorRespec : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);

		if(pObject->GetUInt32Value(UNIT_NPC_FLAGS) & UNIT_NPC_FLAG_VENDOR)
			Menu->AddItem(1, "I would like to browse your goods.", 10);

		if(plr->_HasSkillLine(197) && plr->_GetSkillLineCurrent(197, false)>350 && !plr->HasSpell(26801) && !plr->HasSpell(26798) && !plr->HasSpell(26797) && (plr->HasFinishedQuest(10833) || plr->HasFinishedQuest(10832) || plr->HasFinishedQuest(10831)))
		{
			Menu->AddItem(0, "Learn me Shadoweave Tailoring please.", 1);
		}

		if(plr->HasSpell(26801))
		{
			Menu->AddItem(0, "I wish to unlearn Shadoweave Tailoring.", 2);
		}

        if(AutoSend)
		{    
			Menu->SendTo(plr);
		}
	}

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* pCreature = TO_CREATURE( pObject );
		if(pObject->GetTypeId() != TYPEID_UNIT)
			return;

		if(!plr)
			return;

		switch(IntId)
		{
		case 1:
			{
				if (plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 200000 )
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need 20 gold coins to learn Shadoweave Tailoring.");
					plr->Gossip_Complete();
					return;
				}
				plr->SetUInt32Value(PLAYER_FIELD_COINAGE, plr->GetUInt32Value(PLAYER_FIELD_COINAGE) - 200000);
				pCreature->CastSpell(plr, 26800, true);
				plr->Gossip_Complete();
			}break;
		case 2:
			{
				if (plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 1500000 )
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need 150 gold coins to unlearn your specialization.");
					plr->Gossip_Complete();
					return;
				}
				plr->SetUInt32Value(PLAYER_FIELD_COINAGE, plr->GetUInt32Value(PLAYER_FIELD_COINAGE) - 1500000);
				pCreature->CastSpell(plr, 41559, true);
				plr->Gossip_Complete();
			}break;
		case 10:
			{
				plr->GetSession()->SendInventoryList(pCreature);
			}break;
		}
	}

    void Destroy()
    {
        
    }
};

class SCRIPT_DECL SpellfireTailorRespec : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);

		if(pObject->GetUInt32Value(UNIT_NPC_FLAGS) & UNIT_NPC_FLAG_VENDOR)
			Menu->AddItem(1, "I would like to browse your goods.", 10);

		if(plr->_HasSkillLine(197) && plr->_GetSkillLineCurrent(197, false)>350 && !plr->HasSpell(26801) && !plr->HasSpell(26798) && !plr->HasSpell(26797) && (plr->HasFinishedQuest(10833) || plr->HasFinishedQuest(10832) || plr->HasFinishedQuest(10831)))
		{
			Menu->AddItem(0, "Learn me Spellfire Tailoring please.", 1);
		}

		if(plr->HasSpell(26797))
		{
			Menu->AddItem(0, "I wish to unlearn Spellfire Tailoring.", 2);
		}

        if(AutoSend)
		{    
			Menu->SendTo(plr);
		}
	}

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* pCreature = TO_CREATURE( pObject );
		if(pObject->GetTypeId() != TYPEID_UNIT)
			return;

		if(!plr)
			return;

		switch(IntId)
		{
		case 1:
			{
				if (plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 200000 )
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need 20 gold coins to learn Shadoweave Tailoring.");
					plr->Gossip_Complete();
					return;
				}
				plr->SetUInt32Value(PLAYER_FIELD_COINAGE, plr->GetUInt32Value(PLAYER_FIELD_COINAGE) - 200000);
				pCreature->CastSpell(plr, 26796, true);
				plr->Gossip_Complete();
			}break;
		case 2:
			{
				if (plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 1500000 )
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need 150 gold coins to unlearn your specialization.");
					plr->Gossip_Complete();
					return;
				}
				plr->SetUInt32Value(PLAYER_FIELD_COINAGE, plr->GetUInt32Value(PLAYER_FIELD_COINAGE) - 1500000);
				pCreature->CastSpell(plr, 41299, true);
				plr->Gossip_Complete();
			}break;
		case 10:
			{
				plr->GetSession()->SendInventoryList(pCreature);
			}break;
		}
	}

    void Destroy()
    {
        
    }
};

class SCRIPT_DECL MoonclothTailorRespec : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);

		if(pObject->GetUInt32Value(UNIT_NPC_FLAGS) & UNIT_NPC_FLAG_VENDOR)
			Menu->AddItem(1, "I would like to browse your goods.", 10);

		if(plr->_HasSkillLine(197) && plr->_GetSkillLineCurrent(197, false)>350 && !plr->HasSpell(26801) && !plr->HasSpell(26798) && !plr->HasSpell(26797) && (plr->HasFinishedQuest(10833) || plr->HasFinishedQuest(10832) || plr->HasFinishedQuest(10831)))
		{
			Menu->AddItem(0, "Learn me Mooncloth Tailoring please.", 1);
		}

		if(plr->HasSpell(26798))
		{
			Menu->AddItem(0, "I wish to unlearn Mooncloth Tailoring.", 2);
		}

        if(AutoSend)
		{    
			Menu->SendTo(plr);
		}
	}

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* pCreature = TO_CREATURE( pObject );
		if(pObject->GetTypeId() != TYPEID_UNIT)
			return;

		if(!plr)
			return;

		switch(IntId)
		{
		case 1:
			{
				if (plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 200000 )
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need 20 gold coins to learn Shadoweave Tailoring.");
					plr->Gossip_Complete();
					return;
				}
				plr->SetUInt32Value(PLAYER_FIELD_COINAGE, plr->GetUInt32Value(PLAYER_FIELD_COINAGE) - 200000);
				pCreature->CastSpell(plr, 26799, true);
				plr->Gossip_Complete();
			}break;
		case 2:
			{
				if (plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < 1500000 )
				{
					pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You need 150 gold coins to unlearn your specialization.");
					plr->Gossip_Complete();
					return;
				}
				plr->SetUInt32Value(PLAYER_FIELD_COINAGE, plr->GetUInt32Value(PLAYER_FIELD_COINAGE) - 1500000);
				pCreature->CastSpell(plr, 41558, true);
				plr->Gossip_Complete();
			}break;
		case 10:
			{
				plr->GetSession()->SendInventoryList(pCreature);
			}break;
		}
	}

    void Destroy()
    {
        
    }
};

void SetupTrainerScript(ScriptMgr * mgr)
{
	GossipScript * MHammersmith = (GossipScript*) new MasterHammersmith();
	GossipScript * MSwordsmith = (GossipScript*) new MasterSwordsmith();
	GossipScript * MAxesmith = (GossipScript*) new MasterAxesmith();
	GossipScript * Shadoweave = (GossipScript*) new ShadoweaveTailorRespec();
	GossipScript * Mooncloth = (GossipScript*) new MoonclothTailorRespec();
	GossipScript * Spellfire = (GossipScript*) new SpellfireTailorRespec();

	mgr->register_gossip_script(11191, MHammersmith);		// Lilith the Lithe
	mgr->register_gossip_script(11193, MSwordsmith);		// Seril Scourgebane
	mgr->register_gossip_script(11192, MAxesmith);			// Kilram
	mgr->register_gossip_script(22212, Shadoweave);			// |
	mgr->register_gossip_script(22208, Mooncloth);			// Speciallization quest givers from Shattrath
	mgr->register_gossip_script(22213, Spellfire);			// |
}