#pragma once

#include <set>

class CSimpleNotifier{//��ʹ��
public:
	void RegisterNotified( class ISimplyNotified *p );
	void UnregisterNotified( class ISimplyNotified *p );
	void NotifyOfStateChange();
	void NotifyOfDeath();
private:
	std::set<class ISimplyNotified *> m_theNotified;
};
