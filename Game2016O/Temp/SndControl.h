#pragma once
#include "sndobject.h"

struct IDirectSoundBuffer8;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if((x)){ (x)->Release();(x)=NULL;}}
#endif

class CSndControl :
	public CSndObject
{
	friend class CSndManager;
protected:
	CSndControl(CSndManager* pManagerOwner);
	virtual ~CSndControl(void);
	CSndManager* m_pManagerOwner;
	IDirectSoundBuffer8* m_pIDirectSoundBuffer;
public:
	virtual CSndControl* DuplicateSound(void);
	virtual void SetVolume(float fVolume);
	virtual void SetPan(float fBalance);
	virtual void SetSpeed(float fSpeed);
	virtual void Play(bool bLooping);
	virtual void Stop(void);
	virtual bool IsPlaying(void);
	//Obtiene la posici�n normalizada actual del cursor de reproducci�n 0:Inicio 1:Final
	virtual float GetPlayPosition(void)=0;
	//Establece la posici�n normalizada del cursor de reproducci�n 0: Inicio 1:Final
	virtual void SetPlayPosition(float fPosition)=0;
	//Obtiene la duraci�n total en segundos del recurso de sonido. 
	virtual float GetPlayTime()=0;
};
