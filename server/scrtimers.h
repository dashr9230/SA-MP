
#ifndef SAMPSRV_TIMERS_H
#define SAMPSRV_TIMERS_H

class CScriptTimers // size: W 16 L 25
{
private:
#ifdef _WIN32
	char _gap[16];
#else
	char _gap[25];
#endif
};

#endif
