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
#include "Raid_MoltenCore.h" 



#define CN_FIRELORD 11668
class Firelord : public MoonScriptCreatureAI
{
public:
        MOONSCRIPT_FACTORY_FUNCTION(Firelord, MoonScriptCreatureAI);
    Firelord(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                AddSpell(19392, Target_Self, 90, 0, 10);
        }
};

#define CN_FIREWALKER 11666
class Firewalker : public MoonScriptCreatureAI
{
public:
        MOONSCRIPT_FACTORY_FUNCTION(Firewalker, MoonScriptCreatureAI);
    Firewalker(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                AddSpell(19636, Target_Current, 50, 0, 30);
                AddSpell(19635, Target_Self, 20, 0, 60);
        }
};

#define CN_FLAME_IMP 11669
class FlameImp : public MoonScriptCreatureAI
{
public:
        MOONSCRIPT_FACTORY_FUNCTION(FlameImp, MoonScriptCreatureAI);
    FlameImp(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                AddSpell(20602, Target_Current, 50, 0, 10);
        }
};

#define CN_FLAMEWAKER_ELITE 11664
class FlamewakerElite : public MoonScriptCreatureAI
{
public:
        MOONSCRIPT_FACTORY_FUNCTION(FlamewakerElite, MoonScriptCreatureAI);
    FlamewakerElite(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                AddSpell(20229, Target_Current, 50, 0, 20);
                AddSpell(20623, Target_Current, 40, 0, 10);
        }
};

#define CN_LAVA_ELEMENTAL 12076
class LavaElemental : public MoonScriptCreatureAI
{
public:
        MOONSCRIPT_FACTORY_FUNCTION(LavaElemental, MoonScriptCreatureAI);
    LavaElemental(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                AddSpell(19641, Target_Current, 50, 0, 10);
        }
};

#define CN_LAVA_REAVER 12100
class LavaReaver : public MoonScriptCreatureAI
{
public:
        MOONSCRIPT_FACTORY_FUNCTION(LavaReaver, MoonScriptCreatureAI);
    LavaReaver(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                AddSpell(19642, Target_Current, 40, 0, 20);
                AddSpell(19644, Target_Current, 50, 0, 10);
        }
};

#define CN_LAVA_SPAWN 12265
class LavaSpawn : public MoonScriptCreatureAI
{
public:
        MOONSCRIPT_FACTORY_FUNCTION(LavaSpawn, MoonScriptCreatureAI);
    LavaSpawn(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                AddSpell(19569, Target_Self, 40, 0, 15);
                AddSpell(19391, Target_Current, 50, 2, 10);
        }
};

#define CN_LAVA_SURGER 12101
class LavaSurger : public MoonScriptCreatureAI
{
public:
        MOONSCRIPT_FACTORY_FUNCTION(LavaSurger, MoonScriptCreatureAI);
    LavaSurger(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                AddSpell(19196, Target_RandomPlayer, 50, 0, 8);
        }
};

#define CN_MOLTEN_DESTROYER 11659
class MoltenDestroyer : public MoonScriptCreatureAI
{
public:
        MOONSCRIPT_FACTORY_FUNCTION(MoltenDestroyer, MoonScriptCreatureAI);
    MoltenDestroyer(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                AddSpell(20276, Target_Current, 40, 0, 15);
                AddSpell(19129, Target_Current, 50, 0, 20);
        }
};

#define CN_MOLTEN_GIANT 11658
class MoltenGiant : public MoonScriptCreatureAI
{
public:
        MOONSCRIPT_FACTORY_FUNCTION(MoltenGiant, MoonScriptCreatureAI);
    MoltenGiant(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                AddSpell(18944, Target_Self, 40, 0, 60);
                AddSpell(18945, Target_Current, 50, 0, 15);
        }
};

void SetupMoltenCoreTrash(ScriptMgr * mgr)
{
        mgr->register_creature_script(CN_FIRELORD, &Firelord::Create);
        mgr->register_creature_script(CN_FIREWALKER, &Firewalker::Create);
        mgr->register_creature_script(CN_FLAME_IMP, &FlameImp::Create);
        mgr->register_creature_script(CN_FLAMEWAKER_ELITE, &FlamewakerElite::Create);
        mgr->register_creature_script(CN_LAVA_ELEMENTAL, &LavaElemental::Create);
        mgr->register_creature_script(CN_LAVA_REAVER, &LavaReaver::Create);
        mgr->register_creature_script(CN_LAVA_SPAWN, &LavaSpawn::Create);
        mgr->register_creature_script(CN_LAVA_SURGER, &LavaSurger::Create);
        mgr->register_creature_script(CN_MOLTEN_DESTROYER, &MoltenDestroyer::Create);
        mgr->register_creature_script(CN_MOLTEN_GIANT, &MoltenGiant::Create);
}