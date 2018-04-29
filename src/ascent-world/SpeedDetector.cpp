#include "StdAfx.h"

SpeedCheatDetector::SpeedCheatDetector()
{
	bigest_hacked_speed_dif = 0.0f;
	cheat_threat = 0;
	last_used_speed = 0.0f;
	last_stamp = 0;
	EventSpeedChange(0.0f);
}

void SpeedCheatDetector::EventSpeedChange(float player_speed)
{
	float diff = fabs(last_used_speed - player_speed);
	//pokud bude rozdil rychlosti vetsi jak 100 tak mame cheatera
	if(diff > 200.0f)
		++cheat_threat;

	last_used_speed = 0;
}

void SpeedCheatDetector::SkipSamplingUntil(uint32 stamp)
{
	if(last_stamp < stamp)
		last_stamp = stamp;

	last_used_speed = 0;
}

void SpeedCheatDetector::AddSample(float x, float y, uint32 stamp, float player_speed, uint32 latency)
{
	uint32 time_dif = stamp - last_stamp;
	//don't flood with calculations. Small values might get lost in calculations
	//also takes care of the delaying of the speed detector
	if(time_dif <= SPDT_SAMPLINGRATE)
		return; 

	//we reset the system in case the monitored player changed it's speed. A small loss to avoid false triggers
	if(player_speed != last_used_speed)
	{
		EventSpeedChange(player_speed);
		last_used_speed = player_speed;
	}
	//seems like we are monitored an interval. Check if we detected any speed hack in it
	else if(last_stamp != 0)
	{
		//MIRA latency compensation
		latency = (latency > 250) ? 250 : latency;
		//get current speed
		float dif_x = x - last_x;
		float dif_y = y - last_y;
		float dist = sqrt(dif_x * dif_x + dif_y * dif_y) - (player_speed * 0.001f) * float(latency);
		float cur_speed = dist / (float)time_dif * 1000.0f;

		//check if we really got a cheater here
		if(cur_speed * SPDT_DETECTION_ERROR> player_speed)
		{
			++cheat_threat;
			float hackspeed_diff = cur_speed - player_speed;
			if(hackspeed_diff> bigest_hacked_speed_dif)
				bigest_hacked_speed_dif = hackspeed_diff;
		}
		else if(cheat_threat > 0)
			--cheat_threat;

	}
	last_stamp = stamp;
	last_x = x;
	last_y = y;
}

void SpeedCheatDetector::ReportCheater(Player *_player)
{
	if((sWorld.no_antihack_on_gm && _player->GetSession()->HasGMPermissions()))
		return; // do not check GMs speed been the config tells us not to.

	//toshik is wonderfull and i can't understand how he managed to make this happen
	if(bigest_hacked_speed_dif <= 1)
	{
		cheat_threat = 0;
		EventSpeedChange(0.0f);
		return;
	}

	float speed = (_player->flying_aura) ? _player->m_flySpeed : (_player->m_swimSpeed> _player->m_runSpeed) ? _player->m_swimSpeed : _player->m_runSpeed;
	_player->BroadcastMessage("Speedhack detected. In case server was wrong then make a report how to reproduce this case. You will be logged out in 5 seconds.");
	sCheatLog.writefromsession(_player->GetSession(), "Caught %s speed hacking last occurence with speed: %f instead of %f", _player->GetName(), speed + bigest_hacked_speed_dif, speed);
	_player->Root();
	_player->Kick(5000);

	//next check will be very far away
	last_stamp = 0x0FFFFFFF;
	cheat_threat = -100; //no more reports this session (unless flooding server :P :D)
}
