
#pragma once

class CPlayerPed;

class CTask
{
protected:
	BOOL m_bSelfCreated;

public:
	CPlayerPed *m_pPlayerPed;
	BYTE *m_pTaskType;

	CTask();
	CTask(DWORD dwSize);
	CTask(BYTE *pTaskType);
	virtual ~CTask();

	void Create(DWORD dwSize);
	void Create(BYTE *pTaskType);
	virtual CTask* CreateCopy();
	virtual void Destroy();

	virtual void ApplyToPed(CPlayerPed *pPed);
	//virtual void ApplyToPed(CActorPed *pPed);

	virtual DWORD GetID();

	virtual BOOL IsDestroyed();
	virtual BOOL IsSimple();

};

class CTaskJetpack :
	public CTask
{
public:
	CTaskJetpack();
	CTaskJetpack(BYTE *pTaskType);
	~CTaskJetpack();
};

