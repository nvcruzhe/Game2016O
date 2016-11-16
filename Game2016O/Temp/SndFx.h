#pragma once
#include "SndControl.h"

class CSndFx :
	public CSndControl
{
	friend class CSndFactory;
	friend class CSndManager;
protected:
	CSndFx(CSndManager* pManagerOwner);
	virtual ~CSndFx(void);
	//Obtiene la posici�n normalizada actual del cursor de reproducci�n 0:Inicio 1:Final
	virtual float GetPlayPosition(void);
	//Establece la posici�n normalizada del cursor de reproducci�n 0: Inicio 1:Final
	virtual void SetPlayPosition(float fPosition);
	//Obtiene la duraci�n total en segundos del recurso de sonido. 
	virtual float GetPlayTime();
};
