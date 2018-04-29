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
#include "Setup.h"

// Exarch Nasuun
#define GOSSIP_EXARCH_NASUUN_1    "Nasuun, do you know how long until we have an alchemy lab at the Sun's Reach Harbor?"
#define GOSSIP_EXARCH_NASUUN_2    "What do you know about the magical gates at the Sunwell Plateau being brought down?"
#define GOSSIP_EXARCH_NASUUN_3    "I have something else to ask you about."

class ExarchNasuun_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 12227, plr);
        Menu->AddItem( 0, GOSSIP_EXARCH_NASUUN_1, 1);  // this is the status
		Menu->AddItem( 0, GOSSIP_EXARCH_NASUUN_2, 2);
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;
		
		GossipMenu * Menu;
        switch(IntId)
        {
		case 0:
			GossipHello(pObject, plr, true);
			break;

        case 1:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr); // Text of status [The Alchemy lab is not quite yet ready, $N. Mar'nah says she is (percentual) percent done with its assembly.$Bhowever.$B$BIf you wold like to help her with that, you will find her inside the inn at the Sun's Reach Harbor.]
				Menu->AddItem( 0, GOSSIP_EXARCH_NASUUN_3, 0);
				Menu->SendTo(plr);
            }break;
		case 2:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 12305, plr);
				Menu->AddItem( 0, GOSSIP_EXARCH_NASUUN_3, 0);
				Menu->SendTo(plr);
            }break;
		}
    }

    void Destroy()
    {
        
    }
};

class ZephyrGossipScript : public GossipScript
{
public:
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->AddItem(0, "Bring me to Caverns of Time!", 1); 
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		Creature *Zephyr = TO_CREATURE(pObject);
		if (Zephyr == NULL)
			return;

		switch (IntId)
		{
		case 0:
			GossipHello(pObject, plr, true);
			break;
		case 1:
			if(plr->GetStanding(989) >=21000)
			{
				//plr->SafeTeleport( 1, 0, -8170.441406f, -4751.321777f, 33.457771f, 5.136f);
				plr->Gossip_Complete();
				Zephyr->CastSpell(plr, dbcSpell.LookupEntryForced(37778), true);
			}else{
				plr->Gossip_Complete();
				plr->BroadcastMessage("You need to be Revered with the faction Keepers of Time!"); // Dunno what the correct text is ^^
			}
			break;
		}
	}
 
    void Destroy()
    {
        
	}

};

void SetupShattrathGossip(ScriptMgr * mgr)
{
	GossipScript * ZephyrGossip = (GossipScript*) new ZephyrGossipScript;
	mgr->register_gossip_script(25967, ZephyrGossip);		// Zephyr
	GossipScript * ExarchNasuunGossip = (GossipScript*) new ExarchNasuun_Gossip;
	mgr->register_gossip_script(24932, ExarchNasuunGossip); // Exarch Nasuun

}
