
#pragma once

//----------------------------------------------------

// 27 bytes, the block the create-actor RPC reads off the wire
typedef struct _ACTOR_INFO
{
	ACTORID	ActorID;
	int		iModel;
	VECTOR	vecPos;
	float	fRotation;
	float	fHealth;
	BYTE	byteInvulnerable;
} ACTOR_INFO;

//----------------------------------------------------

class CActorPool
{
public:
	int field_0;
	int field_4[MAX_ACTORS];
	int field_FA4[MAX_ACTORS];
	int field_1F44[MAX_ACTORS];

	char _gap2EE4[8000];

public:

	CActorPool();
	~CActorPool(); // .text:100018E0

	void sub_100016F0(ACTORID ActorID); // .text:100016F0
	void sub_10001900(ACTOR_INFO *pActorInfo); // .text:10001900

	int GetAt(ACTORID ActorID) {
		if(ActorID >= MAX_ACTORS) return 0;
		if(!field_FA4[ActorID]) return 0;
		return field_4[ActorID];
	}
	ACTORID FUNC_100018B0(DWORD dwPtr); // .text:100018B0
};

//----------------------------------------------------
