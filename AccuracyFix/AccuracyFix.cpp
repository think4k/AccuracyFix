#include "precompiled.h"

CAccuracyFix gAccuracyFix;

void CAccuracyFix::ServerActivate()
{
	this->m_af_accuracy_all = this->CvarRegister("af_accuracy_all", "-1.0");

	this->m_af_distance_all = this->CvarRegister("af_distance_all", "-1.0");

	for (int WeaponID = WEAPON_P228; WeaponID <= WEAPON_P90; WeaponID++)
	{
		auto SlotInfo = g_ReGameApi->GetWeaponSlot((WeaponIdType)WeaponID);

		if (SlotInfo)
		{
			if ((SlotInfo->slot == PRIMARY_WEAPON_SLOT) || (SlotInfo->slot == PISTOL_SLOT))
			{
				if (SlotInfo->weaponName)
				{
					char cvarName[32] = { 0 };

					Q_snprintf(cvarName, sizeof(cvarName), "af_accuracy_%s", SlotInfo->weaponName);

					this->m_af_accuracy[WeaponID] = this->CvarRegister(cvarName, "9999.0");

					Q_snprintf(cvarName, sizeof(cvarName), "af_distance_%s", SlotInfo->weaponName);

					this->m_af_distance[WeaponID] = this->CvarRegister(cvarName, "2048.0");
				}
			}
		}
	}

	g_engfuncs.pfnServerCommand("exec addons/accuracyfix/accuracyfix.cfg\n");
	g_engfuncs.pfnServerExecute();
}

void CAccuracyFix::TraceLine(const float* vStart, const float* vEnd, int fNoMonsters, edict_t* pentToSkip, TraceResult* ptr)
{
	auto EntityIndex = ENTINDEX(pentToSkip);
	
	if (UTIL_IsValidPlayerIndex(entityIndex))	
	{
		auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

		if (Player)
		{
			if (Player->IsAlive())
			{
				if (fNoMonsters == dont_ignore_monsters)
				{
					if (Player->m_pActiveItem)
					{
						if (!((BIT(WEAPON_NONE) | BIT(WEAPON_HEGRENADE) | BIT(WEAPON_C4) | BIT(WEAPON_SMOKEGRENADE) | BIT(WEAPON_FLASHBANG) | BIT(WEAPON_KNIFE)) & BIT(Player->m_pActiveItem->m_iId)))
						{
							auto aimDistance = this->m_af_distance[Player->m_pActiveItem->m_iId]->value;

							if (this->m_af_distance_all->value > 0)
							{
								aimDistance = this->m_af_distance_all->value;
							}

							int TargetIndex = 0, HitBoxPlace = 0;
							
							if (this->GetUserAiming(pentToSkip, &TargetIndex, &HitBoxPlace, aimDistance) > 0.0f)
							{
								if (UTIL_IsValidPlayerIndex(TargetIndex))	
								{
									auto fwdVelocity = this->m_af_accuracy[Player->m_pActiveItem->m_iId]->value;
									
									if (this->m_af_accuracy_all->value > 0.0f)
									{
										fwdVelocity = this->m_af_accuracy_all->value;
									}
									
									if (fwdVelocity > 0.0f)
									{
										g_engfuncs.pfnMakeVectors(pentToSkip->v.v_angle);

										Vector Result = Vector(0.0f, 0.0f, 0.0f);

										Result[0] = (vStart[0] + (gpGlobals->v_forward[0] * fwdVelocity));
										Result[1] = (vStart[1] + (gpGlobals->v_forward[1] * fwdVelocity));
										Result[2] = (vStart[2] + (gpGlobals->v_forward[2] * fwdVelocity));

										g_engfuncs.pfnTraceLine(vStart, Result, fNoMonsters, pentToSkip, ptr);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

float CAccuracyFix::GetUserAiming(edict_t* pEdict, int* cpId, int* cpBody, float distance)
{
	float Result = 0.0f;

	if (!FNullEnt(pEdict))
	{
		auto Entityindex = ENTINDEX(pEdict);
		
		if (UTIL_IsValidPlayerIndex(Entityindex))	
		{
			Vector v_forward;

			Vector v_src = pEdict->v.origin + pEdict->v.view_ofs;

			g_engfuncs.pfnAngleVectors(pEdict->v.v_angle, v_forward, NULL, NULL);

			TraceResult trEnd;

			Vector v_dest = v_src + v_forward * distance;

			g_engfuncs.pfnTraceLine(v_src, v_dest, 0, pEdict, &trEnd);

			*cpId = FNullEnt(trEnd.pHit) ? 0 : ENTINDEX(trEnd.pHit);

			*cpBody = trEnd.iHitgroup;

			if (trEnd.flFraction < 1.0f)
			{
				Result = (trEnd.vecEndPos - v_src).Length();
			}

			return Result;
		}
	}

	*cpId = 0;

	*cpBody = 0;

	return Result;
}

cvar_t* CAccuracyFix::CvarRegister(const char* Name, const char* Value)
{
	cvar_t* Pointer = g_engfuncs.pfnCVarGetPointer(Name);

	if (!Pointer)
	{
		this->m_Cvar[Name].name = Name;

		this->m_Cvar[Name].string = (char*)(Value);

		g_engfuncs.pfnCVarRegister(&this->m_Cvar[Name]);

		Pointer = g_engfuncs.pfnCVarGetPointer(this->m_Cvar[Name].name);

		if (Pointer)
		{
			g_engfuncs.pfnCvar_DirectSet(Pointer, Value);
		}
	}

	return Pointer;
}
