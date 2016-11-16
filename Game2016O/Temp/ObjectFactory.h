#pragma once

class CObjectBase;

/*
F�brica base de objetos a partir del identificador �nico.

Las aplicaciones derivan de �sta clase para especificar sus f�bricas. La creaci�n de objetos se implementa
sobrecargando el m�todo CreateObject. El m�todo se limita a crear objetos �nicamente que deriven de CObjectBase.
*/

class CObjectFactory
{
public:
	//Crea un objeto a partir del identificador de clase.
	virtual CObjectBase* CreateObject(unsigned long ulClassID)=0;
	CObjectFactory(void);
	~CObjectFactory(void);
};

