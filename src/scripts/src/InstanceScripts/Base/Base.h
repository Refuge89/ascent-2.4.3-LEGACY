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

#ifndef _INSTANCE_SCRIPTS_BASE_H_
#define _INSTANCE_SCRIPTS_BASE_H_

#include "StdAfx.h"
#include "Setup.h"

#include <string>
#include <vector>
#include <map>

#define urand( i, y) i + rand()%(y-i)

#define INVALIDATE_TIMER			-1
#define DEFAULT_UPDATE_FREQUENCY	1000	//milliseconds
#define DEFAULT_DESPAWN_TIMER		2000	//milliseconds

#define MOONSCRIPT_FACTORY_FUNCTION(ClassName, ParentClassName)\
public:\
	ADD_CREATURE_FACTORY_FUNCTION(ClassName);\
	typedef ParentClassName ParentClass;

enum TargetType
{
	Target_Self,						//Target self (Note: doesn't always mean self, also means the spell can choose various target)
	Target_Current,						//Current highest aggro (attacking target)
	Target_SecondMostHated,				//Second highest aggro
	Target_Destination,					//Target is a destination coordinates (x, y, z)
	Target_Predefined,					//Pre-defined target unit
	Target_RandomPlayer,				//Random target player
	Target_RandomPlayerNotCurrent,		//Random target player, but not the current highest aggro
	Target_RandomPlayerDestination,		//Random player destination coordinates (x, y, z)
	Target_RandomUnit,					//Random target unit (players, totems, pets, etc.)
	Target_RandomUnitNotCurrent,		//Random target unit (players, totems, pets, etc.), but not the current highest aggro
	Target_RandomDestination,			//Random destination coordinates (x, y, z)
	Target_RandomFriendly,				//Random friendly target unit
	Target_WoundedPlayer,				//Random wounded player
	Target_WoundedUnit,					//Random wounded unit (players, totems, pets, etc.)
	Target_WoundedFriendly,				//Random wounded friendly target
	Target_ClosestPlayer,				//Closest target player
	Target_ClosestPlayerNotCurrent,		//Closest target player, but not the current highest aggro
	Target_ClosestUnit,					//Closest target unit (players, totems, pets, etc.)
	Target_ClosestUnitNotCurrent,		//Closest target unit (players, totems, pets, etc.), but not the current highest aggro
	Target_ClosestFriendly				//Closest friendly target unit
};

enum TextType
{
	Text_Say,
	Text_Yell,
	Text_Emote
};

enum EventType
{
	Event_OnCombatStart,
	Event_OnTargetDied,
	Event_OnDied,
    Event_OnCombatStop,
	Event_OnTaunt
};

enum BehaviorType
{
	Behavior_Default,
	Behavior_Melee,
	Behavior_Ranged,
	Behavior_Spell,
	Behavior_Flee,
	Behavior_CallForHelp
};

struct Coords
{
	float	mX;
	float	mY;
	float	mZ;
	float	mO;
	uint32	mAddition;
	float	mx;
	float	my;
	float	mz;
	float	mo;
	
};


// DotA Stuff
enum Side
{
	Side_Default,
	Side_Alliance,
	Side_Horde
};


enum UnitPosition
{
	Position_Default,
	Position_Melee,
	Position_Caster,
	Position_Special
};

enum MoveType
{
    Move_None,
    Move_RandomWP,
    Move_CircleWP,
    Move_WantedWP,
    Move_DontMoveWP,
    Move_Quest,
    Move_ForwardThenStop
};

struct EmoteDesc
{
	EmoteDesc(const char* pText, TextType pType, uint32 pSoundId)
	{
		mText = ( pText && strlen(pText) > 0 ) ? pText : "";
		mType = pType;
		mSoundId = pSoundId;
	}

	std::string	mText;
	TextType	mType;
	uint32		mSoundId;
};

class SpellDesc;
class MoonScriptCreatureAI;
class MoonScriptBossAI;

typedef void(*SpellFunc)(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
typedef std::vector<EmoteDesc*> EmoteArray;
typedef std::vector<Player*> PlayerArray;
typedef std::vector<Unit*> UnitArray;
typedef std::vector<SpellDesc*> SpellDescArray;
typedef std::list<SpellDesc*> SpellDescList;
typedef std::pair<int32, SpellDesc*> PhaseSpellPair;
typedef std::vector<PhaseSpellPair> PhaseSpellArray;
typedef std::pair<int32, int32> TimerPair;
typedef std::vector<TimerPair> TimerArray;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class SpellDesc
class SpellDesc
{
public:
	SpellDesc(SpellEntry* pInfo, SpellFunc pFnc, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange, float pMaxRange, bool pStrictRange, char* pText, TextType pTextType, uint32 pSoundId);
	virtual ~SpellDesc();

	EmoteDesc*	AddEmote(const char* pText, TextType pType=Text_Yell, uint32 pSoundId=0);
	void		TriggerCooldown(uint32 pCurrentTime=0);

	SpellEntry*	mInfo;				//Spell Entry information (generally you either want a SpellEntry OR a SpellFunc, not both)
	SpellFunc	mSpellFunc;			//Spell Function to be executed (generally you either want a SpellEntry OR a SpellFunc, not both)
	TargetType	mTargetType;		//Target type (see enum above)
	float		mChance;			//Percent of the cast of this spell in a total of 100% of the attacks
	float		mCastTime;			//Duration of the spell cast (seconds). Zero means the spell is instant cast
	int32		mCooldown;			//Spell cooldown (seconds)
	float		mMinRange;			//Minimum range for spell to be cast
	float		mMaxRange;			//Maximum range for spell to be cast
	bool		mStrictRange;		//If true, creature won't run to (or away of) target, it will instead skip that attack
	EmoteArray	mEmotes;			//Emotes (random) shouted on spell cast
	bool		mEnabled;			//True if the spell is enabled for casting, otherwise it will never be scheduled (useful for bosses with phases, etc.)
	Unit*		mPredefinedTarget;	//Pre-defined Target Unit (Only valid with target type Target_Predefined);

	//Those are not properties, they are data member used by the evaluation system
	uint32		mLastCastTime;		//Last time at which the spell casted (used to implement cooldown), set to 0
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class MoonScriptCreatureAI
class MoonScriptCreatureAI : public CreatureAIScript
{
public:
	MoonScriptCreatureAI(Creature* pCreature);
	virtual ~MoonScriptCreatureAI();

	//Movement
	bool					GetCanMove();
	void					SetFlyMode(bool pValue);
	void					SetCanMove(bool pCanMove);
	void					MoveTo(MoonScriptCreatureAI* pCreature);
	void					MoveTo(Unit* pUnit);
	void					MoveTo(float pX, float pY, float pZ);
	void					MoveToSpawnOrigin();
	void					StopMovement();

	//Attack and Combat State
	bool					GetCanEnterCombat();
	void					SetCanEnterCombat(bool pCanEnterCombat);
	bool					IsInCombat();
	void					DelayNextAttack(int32 pMilliseconds);
	void					SetDespawnWhenInactive(bool pValue);

	//Behavior
	void					SetBehavior(BehaviorType pBehavior);
	BehaviorType			GetBehavior();
	void					SetAllowMelee(bool pAllow);
	bool					GetAllowMelee();
	void					SetAllowRanged(bool pAllow);
	bool					GetAllowRanged();
	void					SetAllowSpell(bool pAllow);
	bool					GetAllowSpell();
	void					SetAllowTargeting(bool pAllow);
	bool					GetAllowTargeting();
	void					AggroNearestUnit();
	void					AggroNearestPlayer(int pInitialThreat=1);
	void					AggroRandomUnit(int pInitialThreat=1);
	void					AggroRandomPlayer(int pInitialThreat=1);
	
	// Bfx Custom named functions for cleaner coding/scripting, they return the old value of the function.
	Unit* GetRandomPlayer() { return GetBestPlayerTarget(); };
	Unit* GetRandomPlayerNotCurrent() { return GetBestPlayerTarget(TargetFilter_NotCurrent); };
	Unit* GetRandomUnit() {  return GetBestUnitTarget(); };
	Unit* GetCurrentTarget() { return _unit->GetAIInterface()->GetNextTarget(); };


	//Status
	void					ClearHateList();
	void					WipeHateList();
	int32					GetHealthPercent();
	int32					GetManaPercent();
	void					Regenerate();
	bool					IsAlive();
	void					SetScale(float pScale);
	float					GetScale();
	void					SetDisplayId(uint32 pDisplayId);
	void					SetWieldWeapon(bool pValue);
	void					SetDisplayWeapon(bool pMainHand, bool pOffHand);
	void					SetDisplayWeaponIds(uint32 pItem1Id, uint32 pItem1Info, uint32 pItem1Slot, uint32 pItem2Id, uint32 pItem2Info, uint32 pItem2Slot);

	//Environment
	float					GetRange(MoonScriptCreatureAI* pCreature);
	float					GetRangeToUnit(Unit* pUnit);
	GameObject*				GetNearestGameObject(uint32 pGameObjectId=0);
	MoonScriptCreatureAI*	GetNearestCreature(uint32 pCreatureId=0);
	MoonScriptCreatureAI*	SpawnCreature(uint32 pCreatureId, bool pForceSameFaction=false);
	MoonScriptCreatureAI*	SpawnCreature(uint32 pCreatureId, float pX, float pY, float pZ, float pO=0, bool pForceSameFaction=false);
	Unit*					ForceCreatureFind(uint32 pCreatureId, float pX=0, float pY=0, float pZ=0, bool pCurrentPosition=true);
	void					Despawn(uint32 pDelay=0, uint32 pRespawnTime=0);
	
	//Instances 
	bool					IsHeroic();

	//Spells
	SpellDesc*				AddSpell(uint32 pSpellId, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange=0, float pMaxRange=0, bool pStrictRange=false, char* pText=NULL, TextType pTextType=Text_Yell, uint32 pSoundId=0);
	SpellDesc*				AddSpellFunc(SpellFunc pFnc, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange=0, float pMaxRange=0, bool pStrictRange=false, char* pText=NULL, TextType pTextType=Text_Yell, uint32 pSoundId=0);
	void					CastSpell(SpellDesc* pSpell);
	void					CastSpellNowNoScheduling(SpellDesc* pSpell);
	SpellDesc*				FindSpellById(uint32 pSpellId);
	SpellDesc*				FindSpellByFunc(SpellFunc pFnc);
	bool					IsCasting();
	void					ApplyAura(uint32 pSpellId);
	void					RemoveAura(uint32 pSpellId);
	void					RemoveAuraOnPlayers(uint32 pSpellId);
	void					RemoveAllAuras();
	void					CancelAllCooldowns();

	//Emotes
	EmoteDesc*				AddEmote(EventType pEventType, const char* pText, TextType pType, uint32 pSoundId=0);
	void					RemoveEmote(EventType pEventType, EmoteDesc* pEmote);
	void					RemoveAllEmotes(EventType pEventType);
	void					Emote(EmoteDesc* pEmote);
	void					Emote(const char* pText, TextType pType=Text_Yell, uint32 pSoundId=0);

    //Others
    //Others
	void					SetTargetToChannel(Unit* pTarget, uint32 pSpellId);
	Unit*					GetTargetToChannel();

	//Timers
	uint32					AddTimer(int32 pDurationMillisec);
	int32					GetTimer(int32 pTimerId);
	void					RemoveTimer(int32& pTimerId);
	void					ResetTimer(int32 pTimerId, int32 pDurationMillisec);
	bool					IsTimerFinished(int32 pTimerId);
	void					CancelAllTimers();
	
	//waypoints
	void					SetMoveType(MoveType pMoveType);

	//Options
	void					SetAIUpdateFreq(uint32 pUpdateFreq);
	uint32					GetAIUpdateFreq();

	//Reimplemented Events
    //virtual void            Reset();
	virtual void			OnCombatStart(Unit* pTarget);
	virtual void			OnCombatStop(Unit* pTarget);
	virtual void			OnTargetDied(Unit* pTarget);
	virtual void			OnDied(Unit* pKiller);
	virtual void			AIUpdate();

	// DotA Stuff
	void					SetWaveUnitPreferences();
	void					GenerateWavePath();
	void					SetSummonPreferences(bool Despawn, uint32 Delay);
	WayPoint*				CreateWP(int id, uint32 waittime, uint32 flags);
	uint32					IsWavePart();
	void					SetSide(uint32 pSide);
	void					SetPosition(uint32 pPosition);

	int32					mWay;
	
	//waypoints
	void					ForceWaypointMove(uint32 pWaypointId);
	void					SetWaypointToMove(uint32 pWaypointId);
	MoveType				GetMoveType();

protected:
	enum					TargetFilter {TargetFilter_None=0, TargetFilter_Closest=1<<0, TargetFilter_Friendly=1<<1, TargetFilter_NotCurrent=1<<2, TargetFilter_Wounded=1<<3, TargetFilter_SecondMostHated=1<<4};

	bool					IsSpellScheduled(SpellDesc* pSpell);
	bool					CastSpellInternal(SpellDesc* pSpell, uint32 pCurrentTime=0);
	void					CastSpellOnTarget(Unit* pTarget, TargetType pType, SpellEntry* pEntry, bool pInstant);
	int32					CalcSpellAttackTime(SpellDesc* pSpell);
	void					CancelAllSpells();

	bool					IsUnitInRange(Unit* pTarget, SpellDesc* pSpell);
	Unit*					GetTargetForSpell(SpellDesc* pSpell);
	Unit*					GetBestPlayerTarget(TargetFilter pFilter=TargetFilter_None);
	Unit*					GetBestUnitTarget(TargetFilter pFilter=TargetFilter_None);
	Unit*					ChooseBestTargetInArray(UnitArray& pTargetArray, TargetFilter pFilter);
	Unit*					GetNearestTargetInArray(UnitArray& pTargetArray);
	bool					IsValidUnitTarget(Object* pObject, TargetFilter pFilter);
	void					PushRunToTargetCache(Unit* pTarget, SpellDesc* pSpell);
	void					PopRunToTargetCache();

	void					RandomEmote(EmoteArray& pEmoteArray);

	SpellDescArray			mSpells;
	SpellDescList			mQueuedSpells;
	SpellDescList			mScheduledSpells;

	Unit*					mRunToTargetCache;
	SpellDesc*				mRunToTargetSpellCache;


	EmoteArray				mOnCombatStartEmotes;
    EmoteArray              mOnCombatStopEmotes;
	EmoteArray				mOnTargetDiedEmotes;
	EmoteArray				mOnDiedEmotes;
	EmoteArray				mOnTauntEmotes;

	bool					mDespawnWhenInactive;

	TimerArray				mTimers;
	int32					mTimerIdCounter;
	uint32					mAIUpdateFrequency;
	uint32					mBaseAttackTime;
	uint32					mSide;
	uint32					mPosition;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class MoonScriptBossAI
class MoonScriptBossAI : public MoonScriptCreatureAI
{
public:
	MoonScriptBossAI(Creature* pCreature);
	virtual ~MoonScriptBossAI();

	//Basic Interface
	SpellDesc*		AddPhaseSpell(int32 pPhase, SpellDesc* pSpell);
	int32			GetPhase();
	void			SetPhase(int32 pPhase, SpellDesc* pPhaseChangeSpell=NULL);
	void			SetEnrageInfo(int32 pPhase, SpellDesc* pSpell, int32 pTriggerMilliseconds);

	//Reimplemented Events
	virtual void	OnCombatStart(Unit* pTarget);
	virtual void	OnCombatStop(Unit* pTarget);
	virtual void	AIUpdate();

protected:
	int32			mPhaseIndex;
	PhaseSpellArray	mPhaseSpells;
	int32			mEnragePhase;
	SpellDesc*		mEnrageSpell;
	int32			mEnrageTimerDuration;
	int32			mEnrageTimer;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Premade Spell Functions
void SpellFunc_ClearHateList(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_Disappear(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_Reappear(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STL Utilities
template <class Type> inline void DeleteArray(std::vector<Type> pVector)
{
	typename std::vector<Type>::iterator Iter = pVector.begin();
	for( ; Iter != pVector.end(); ++Iter )
	{
		delete (*Iter);
	}
	pVector.clear();
}

//Warning: do not use if item can reside more than once in same vector
template <class Type> inline void DeleteItem(std::vector<Type> pVector, Type pItem)
{
	typename std::vector<Type>::iterator Iter = std::find(pVector.begin(), pVector.end(), pItem);
	if( Iter != pVector.end() )
	{
		delete (*Iter);
		pVector.erase(Iter);
	}
}

#endif /* _INSTANCE_SCRIPTS_BASE_H_ */