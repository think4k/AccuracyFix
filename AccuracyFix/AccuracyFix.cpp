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
					char cvarName[64] = { 0 };

					Q_snprintf(cvarName, sizeof(cvarName), "af_accuracy_%s", SlotInfo->weaponName);

					this->m_af_accuracy[WeaponID] = this->CvarRegister(cvarName, "9999.0");

					Q_snprintf(cvarName, sizeof(cvarName), "af_distance_%s", SlotInfo->weaponName);

					this->m_af_distance[WeaponID] = this->CvarRegister(cvarName, "4096.0");
				}
			}
		}
	}

	g_engfuncs.pfnServerCommand("exec addons/accuracyfix/accuracyfix.cfg\n");
	g_engfuncs.pfnServerExecute();
}

void CAccuracyFix::TraceLine(const float* vStart, const float* vEnd, int fNoMonsters, edict_t* pentToSkip, TraceResult* ptr)
{
	if (fNoMonsters == dont_ignore_monsters && vStart && vEnd && pentToSkip && ptr)
	{
		if (!FNullEnt(pentToSkip))
		{
			auto EntityIndex = g_engfuncs.pfnIndexOfEdict(pentToSkip);

			if (EntityIndex > 0 && EntityIndex <= gpGlobals->maxClients)
			{
				auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

				if (Player && Player->IsPlayer() && Player->IsAlive() && Player->m_pActiveItem)
				{
					if ((Player->m_pActiveItem->iItemSlot() == PRIMARY_WEAPON_SLOT) || (Player->m_pActiveItem->iItemSlot() == PISTOL_SLOT))
					{
						auto aimDistance = this->m_af_distance[Player->m_pActiveItem->m_iId]->value;

						if (this->m_af_distance_all->value > 0)
						{
							aimDistance = this->m_af_distance_all->value;
						}

						int TargetIndex = 0, HitBoxPlace = 0;

						if (this->GetUserAiming(pentToSkip, &TargetIndex, &HitBoxPlace, aimDistance) > 0.0f)
						{
							if (TargetIndex > 0 && TargetIndex <= gpGlobals->maxClients)
							{
								auto fwdVelocity = this->m_af_accuracy[Player->m_pActiveItem->m_iId]->value;

								if (this->m_af_accuracy_all->value > 0.0f)
								{
									fwdVelocity = this->m_af_accuracy_all->value;
								}

								if (fwdVelocity > 0.0f)
								{
									g_engfuncs.pfnMakeVectors(pentToSkip->v.v_angle);

									auto vEndRes = (Vector)vStart + gpGlobals->v_forward * fwdVelocity;

									g_engfuncs.pfnTraceLine(vStart, vEndRes, fNoMonsters, pentToSkip, ptr);
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
    	if (FNullEnt(pEdict) || cpId == nullptr || cpBody == nullptr)
    	{
        	*cpId = 0;
        	*cpBody = 0;
        	return 0.0f;
    	}

    	int Entityindex = ENTINDEX(pEdict);
    	if (Entityindex <= 0 || Entityindex > gpGlobals->maxClients)
    	{
        	*cpId = 0;
        	*cpBody = 0;
        	return 0.0f;
    	}

    	Vector v_src = pEdict->v.origin + pEdict->v.view_ofs;
    	Vector v_forward;
    	g_engfuncs.pfnAngleVectors(pEdict->v.v_angle, v_forward, nullptr, nullptr);
    	Vector v_dest = v_src + v_forward * distance;

    	TraceResult trEnd;
    	g_engfuncs.pfnTraceLine(v_src, v_dest, 0, pEdict, &trEnd);

    	*cpId = FNullEnt(trEnd.pHit) ? 0 : ENTINDEX(trEnd.pHit);
    	*cpBody = trEnd.iHitgroup;

    	if (trEnd.flFraction < 1.0f)
    	{
       	 return (trEnd.vecEndPos - v_src).Length();
   	}

    	return 0.0f;
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
