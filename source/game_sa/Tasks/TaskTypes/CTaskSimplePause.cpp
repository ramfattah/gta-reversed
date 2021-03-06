#include "StdInc.h"

void CTaskSimplePause::InjectHooks()
{
    HookInstall(0x48E750, &CTaskSimplePause::Constructor);
    HookInstall(0x48E830, &CTaskSimplePause::ProcessPed_Reversed);
    HookInstall(0x48E810, &CTaskSimplePause::MakeAbortable_Reversed);
}

CTaskSimplePause::CTaskSimplePause(int time)
{
    m_timer.m_nStartTime = 0;
    m_timer.m_nInterval = 0;
    m_timer.m_bStarted = false;
    m_timer.m_bStopped = false;
    m_nTime = time;
}

CTaskSimplePause::~CTaskSimplePause()
{
    // nothing here
}

CTaskSimplePause* CTaskSimplePause::Constructor(int time)
{
#ifdef USE_DEFAULT_FUNCTIONS 
    return plugin::CallMethodAndReturn<CTaskSimplePause*, 0x48E750, CTask*, int>(this, time);
#else
    this->CTaskSimplePause::CTaskSimplePause(time);
    return this;
#endif
}

CTask* CTaskSimplePause::Clone()
{
    return plugin::CallMethodAndReturn<CTask*, 0x48E780, CTask*>(this);
}

bool CTaskSimplePause::MakeAbortable_Reversed(CPed* ped, eAbortPriority priority, CEvent* _event)
{
    m_timer.m_nStartTime = CTimer::m_snTimeInMilliseconds;
    m_timer.m_nInterval = -1;
    m_timer.m_bStarted = true;
    return true;
}

bool CTaskSimplePause::MakeAbortable(CPed* ped, eAbortPriority priority, CEvent* _event)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return plugin::CallMethodAndReturn<bool, 0x48E810, CTask*, CPed*, int, CEvent*>(this, ped, priority, _event);
#else
    return CTaskSimplePause::MakeAbortable_Reversed(ped, priority, _event);
#endif
}

bool CTaskSimplePause::ProcessPed_Reversed(CPed* ped)
{
    if (!m_timer.m_bStarted && !m_timer.Start(m_nTime)) {
        return false;
    }
    m_timer.Reset();
    return m_timer.IsOutOfTime();
}

bool CTaskSimplePause::ProcessPed(CPed* ped)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return plugin::CallMethodAndReturn<bool, 0x48E830, CTaskSimplePause*, CPed*>(this, ped);
#else
    return CTaskSimplePause::ProcessPed_Reversed(ped);
#endif
}
