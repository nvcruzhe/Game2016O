#pragma once

#include "Camera3D.h"
#include "LowPassFilter.h"

class CCameraControl
{
public:
	struct CAMERA_TRANSITION
	{
		unsigned long t;
		VECTOR4D vPosition;
		VECTOR4D vLookAt;
		VECTOR4D vUpDir;
		float fFinalPositionCutOffFreq;
		float fLookAtPositionCutOffFreq;
	};
protected:
	CCamera3D* m_pCamera;
	VECTOR4D m_vFinalPosition;
	VECTOR4D m_vLookAtPosition;
	VECTOR4D m_vUpDirection;
	float m_fFinalPositionCutOffFreq;
	float m_fLookAtPositionCutOffFreq;
	CAMERA_TRANSITION* m_pCameraTransitions;
	int m_nCameraTransitionCount;
	int m_nTransitionIndex;	
	CLowPassFilter m_aFiltersLookAtPosition[4];
	CLowPassFilter m_aFiltersFinalPosition[4];
	CLowPassFilter m_aFiltersUpDirection[4];
public:

	CCameraControl(void);
	virtual ~CCameraControl(void);
	void SetCamera(CCamera3D* pCamera);
	CCamera3D* GetCamera(void);
	VECTOR4D& LookAtPosition(void){return m_vLookAtPosition;}
	VECTOR4D& UpDirection(void){return m_vUpDirection;}
	VECTOR4D& FinalPosition(void){return m_vFinalPosition;}
	float &LookAtCutOffFreq(void){return m_fLookAtPositionCutOffFreq;};
	float &FinalPositionCutOffFreq(void){return m_fFinalPositionCutOffFreq;};
	//Transfiere los valores finales a los filtros haciendo instant�neo el cambio
	virtual void ResetFilters(void);
	//Calcula la siguiente muestra de salida de los filtros (flotante medido en segundos)
	virtual void Time(float dts);
	//Transfiere la salida de los filtros a los valores de c�mara
	virtual void Transfer(void);
	//Establece el conjunto de transiciones de c�mara, se renicia la secuencia
	void SetCameraTransitions(CCameraControl::CAMERA_TRANSITION* pTransitions,int nTransitionCount);
	//Establece los par�metros de c�mara seg�n el tiempo expira en la transici�n de c�mara actual. (El par�metro Reset, Reinicia la secuencia)
	//La funci�n retorna el indice de transici�n actual (Tiempo en diezmil�simas de segundo)
	int UpdateCameraSequence(unsigned long t,bool bReset=false);
};

