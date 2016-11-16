#include "stdafx.h"
#include "InputProcessor.h"
#include "ActionEvent.h"

CInputProcessor::CInputProcessor(CStateMachineManager * pHSMOwner)
{
	m_pHSMOwner = pHSMOwner;
}

unsigned long CInputProcessor::OnEvent(CEventBase * pEvent)
{
	if (INPUT_EVENT == pEvent->m_ulEventType)
	{
		//1.- Construir copia del estado que ingresa
		CInputEvent* pInput = (CInputEvent*)pEvent;
		CInputEvent* pCopy = new CInputEvent(
			pInput->m_nSource,
			pInput->m_ulTime,
			pInput->m_js2);
		auto GamePad = m_mapLastEvents.find(pInput->m_nSource);
		//2.- Procesar diferencias
		if (GamePad != m_mapLastEvents.end())
		{
			auto Last = GamePad->second->m_js2;
			auto Curr = pInput->m_js2;
			if (!Last.rgbButtons[0] && Curr.rgbButtons[0])
			{
				auto Action =
					new CActionEvent(GamePad->first,
						pInput->m_ulTime, JOY_BUTTON_A_PRESSED);
				m_pHSMOwner->PostEvent(Action);
			}
			if (Last.rgbButtons[0] && !Curr.rgbButtons[0])
			{
				auto Action =
					new CActionEvent(GamePad->first,
						pInput->m_ulTime, JOY_BUTTON_A_RELEASED);
				m_pHSMOwner->PostEvent(Action);
			}
			//Reportar el estado de los ejes
			{
				auto Action = new CActionEvent(GamePad->first, pInput->m_ulTime, JOY_AXIS_LX);
				Action->m_fAxis = (Curr.lX - 0x7fff) / 32768.0f;
				m_pHSMOwner->PostEvent(Action);
			    Action = new CActionEvent(GamePad->first, pInput->m_ulTime, JOY_AXIS_LY);
				Action->m_fAxis = -(Curr.lY - 0x7fff) / 32768.0f;
				m_pHSMOwner->PostEvent(Action);
				Action = new CActionEvent(GamePad->first, pInput->m_ulTime, JOY_AXIS_RX);
				Action->m_fAxis = (Curr.lZ - 0x7fff) / 32768.0f; //lZ es X para el segundo eje
				//printf("%d\n", Curr.lRz); fflush(stdin);
				m_pHSMOwner->PostEvent(Action);
				Action = new CActionEvent(GamePad->first, pInput->m_ulTime, JOY_AXIS_RY);
				Action->m_fAxis = -(Curr.lRz - 0x7fff) / 32768.0f; //lRz es y para el segundo eje
				m_pHSMOwner->PostEvent(Action);
				//printf("%d\n", Curr.lRz); fflush(stdin);
				
			}
		}
		//3.- Almacenar el estado anterior, que es el actual.
		if (GamePad == m_mapLastEvents.end())
			m_mapLastEvents.insert(
				make_pair(pInput->m_nSource, pCopy));
		else
		{
			delete GamePad->second;
			GamePad->second = pCopy;
		}

	}
	return 0;
}


CInputProcessor::~CInputProcessor()
{
}
