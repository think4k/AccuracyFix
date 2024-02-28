#pragma once

class CAccuracyFix
{

	public:
		void ServerActivate();

		void TraceLine(const float* vStart, const float* vEnd, int fNoMonsters, edict_t* pentToSkip, TraceResult* ptr);
		float GetUserAiming(edict_t* edict, int* cpId, int* cpBody, float distance);
		cvar_t* CvarRegister(const char* Name, const char* Value);

	private:
		std::map<std::string, cvar_t> m_Cvar;

		cvar_t* m_af_accuracy_all;
		std::array<cvar_t*, MAX_WEAPONS + 1> m_af_accuracy;

		cvar_t* m_af_distance_all;
		std::array<cvar_t*, MAX_WEAPONS + 1> m_af_distance;
};

extern CAccuracyFix gAccuracyFix;
