#include "autodefuse.h"

bool Settings::AutoDefuse::enabled = false;
float Settings::AutoDefuse::time = 0.5f;

void AutoDefuse::CreateMove(CUserCmd *cmd)
{
	if (!Settings::AutoDefuse::enabled)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer || !localplayer->GetAlive())
		return;

	if (localplayer->GetTeam() != TeamID::TEAM_COUNTER_TERRORIST)
		return;



	C_PlantedC4* bomb = nullptr;

	for (int i = 1; i < entityList->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* entity = entityList->GetClientEntity(i);
		if (!entity)
			continue;

		if (entity->GetClientClass()->m_ClassID == EClassIds::CPlantedC4)
		{
			bomb = (C_PlantedC4*) entity;
			break;
		}
	}

	if (!bomb || bomb->IsBombDefused())
		return;

	float bombTimer = bomb->GetBombTime() - globalVars->curtime;


	if (Settings::AutoDefuse::enabled)
	{

		if (localplayer->HasDefuser() && bombTimer > (5.0f + Settings::AutoDefuse::time))
			return;


		if (!localplayer->HasDefuser() && bombTimer > (10.0f + Settings::AutoDefuse::time))
			return;

		float distance = localplayer->GetVecOrigin().DistTo(bomb->GetVecOrigin());
		if (distance <= 75.0f)
			cmd->buttons |= IN_USE;
	}
}
