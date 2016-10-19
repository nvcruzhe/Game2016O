#pragma once

/*
	Las aplicaciones derivan de CEventBase para incorporar m�s informaci�n en el evento seg�n las necesidades.

	NO SE DEBEN AGREGAR MAS DATOS MIEMBRO EN ESTA CLASE BASE. Deber� derivar para incorporar m�s par�metros en el evento.

	Ejemplos: CEventWin32, CEventGame, CEventCustom etc...
	Cada uno de estos tipos de evento deber�n tener datos muy espec�ficos.
	Una forma de resolver en especifico de qu� clase particular de evento se trata es a trav�s de m_ulEventType, 
	mediante una enumeraci�n definida por el usuario. La enumeraci�n contiene tipos (n�meros) �nicos en el contexto de la aplicaci�n.
*/
#define APP_LOOP 0x0
class CEventBase
{
public:
	//Tipo de evento definido por el usuario, ayuda a resolver el tipo de evento.
	unsigned long m_ulEventType;
	CEventBase(void);
	virtual ~CEventBase(void);
};

