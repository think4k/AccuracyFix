#include "precompiled.h"

CAccuracyFix gAccuracyFix;

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
    	if (fNoMonsters != dont_ignore_monsters || FNullEnt(pentToSkip))
        	return;

    	auto EntityIndex = ENTINDEX(pentToSkip);

    	if (EntityIndex <= 0 || EntityIndex > gpGlobals->maxClients)
        	return;

    	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

    	if (!Player || !Player->IsAlive() || !Player->m_pActiveItem)
       		return;
	
	if ((Player->m_pActiveItem->iItemSlot() != PRIMARY_WEAPON_SLOT) || (Player->m_pActiveItem->iItemSlot() != PISTOL_SLOT))	
        	return;

    	float aimDistance = this->m_af_distance[Player->m_pActiveItem->m_iId]->value;
    	if (this->m_af_distance_all->value > 0)
    	{
        	aimDistance = this->m_af_distance_all->value;
    	}

    	if (aimDistance <= 0.0f || !(Player->pev->flags & FL_ONGROUND))
        	return;

    	int TargetIndex = 0, HitBoxPlace = 0;
    	if (this->GetUserAiming(pentToSkip, &TargetIndex, &HitBoxPlace, aimDistance) > 0.0f)
    	{
        	if (TargetIndex > 0 && TargetIndex <= gpGlobals->maxClients)
        	{
            		float fwdVelocity = this->m_af_accuracy[Player->m_pActiveItem->m_iId]->value;
            		if (this->m_af_accuracy_all->value > 0.0f)
            		{
                		fwdVelocity = this->m_af_accuracy_all->value;
            		}

            		if (fwdVelocity > 0.0f)
            		{
                		g_engfuncs.pfnMakeVectors(pentToSkip->v.v_angle);
                		Vector Result = (Vector)vStart + gpGlobals->v_forward * fwdVelocity;
                		g_engfuncs.pfnTraceLine(vStart, Result, fNoMonsters, pentToSkip, ptr);
            		}
        	}
    	}
}

float CAccuracyFix::GetUserAiming(edict_t* pEdict, int* cpId, int* cpBody, float distance)
{
        float result = 0.0f;
        *cpId = 0;
        *cpBody = 0;

	if (FNullEnt(pEdict))
        	return result;

    	auto EntityIndex = ENTINDEX(pEdict);
    	if (EntityIndex <= 0 || EntityIndex > gpGlobals->maxClients)
        	return result;

    	Vector v_forward;
    	Vector v_src = pEdict->v.origin + pEdict->v.view_ofs;

    	g_engfuncs.pfnAngleVectors(pEdict->v.v_angle, v_forward, NULL, NULL);
    	Vector v_dest = v_src + v_forward * distance;

    	TraceResult trEnd;
    	g_engfuncs.pfnTraceLine(v_src, v_dest, 0, pEdict, &trEnd);

    	*cpId = FNullEnt(trEnd.pHit) ? 0 : ENTINDEX(trEnd.pHit);
    	*cpBody = trEnd.iHitgroup;

    	if (trEnd.flFraction < 1.0f)
    	{
        	result = (trEnd.vecEndPos - v_src).Length();
    	}

    	return result;
}

