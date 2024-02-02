
#pragma once

typedef void (__cdecl *CMDPROC)(PCHAR);

class CCmdWindow // size: 6908
{
private:
	char _gap0[6908];

public:

	CMDPROC		m_pDefaultCmd;	 // used when no command specifier was
								 // used (ie. a normal chat message)


	CCmdWindow();

	void AddDefaultCmdProc(CMDPROC cmdDefault);
};
