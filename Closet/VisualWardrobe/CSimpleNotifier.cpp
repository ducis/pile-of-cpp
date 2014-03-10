#include "CSimpleNotifier.h"
#include "ISimplyNotified.h"
#include <algorithm>
#include <functional>
//≤ª π”√
using namespace std;

void CSimpleNotifier::RegisterNotified( class ISimplyNotified *p ){
	m_theNotified.insert(p);
}



void CSimpleNotifier::UnregisterNotified( class ISimplyNotified *p ){
	m_theNotified.erase(p);
}



void CSimpleNotifier::NotifyOfStateChange(){
	for_each(m_theNotified.begin(),m_theNotified.end(),mem_fun(&ISimplyNotified::OnNotifierStateChange));
}


void CSimpleNotifier::NotifyOfDeath(){
	for_each(m_theNotified.begin(),m_theNotified.end(),mem_fun(&ISimplyNotified::OnNotifierDeath));
}

