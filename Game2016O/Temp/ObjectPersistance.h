#pragma once
#include "Stream.h"
class CObjectFactory;

/*
	Persistencia.
	Las clases que requieran de persistencia deben derivar de la clase CObjectPersistance para que implementen el m�todo Serialize.

	El m�todo serialize requiere de una f�brica definida por la aplicaci�n para poder completar la funcionalidad de carga.

	Los objetos que se pueden serializar son �nicamente aquellos cuya clase derive de CObjectBase
*/

class CObjectPersistance
{
public:
	//Realiza las operaciones de almacenamiento o carga
	virtual bool Serialize(CStream& stream,bool bSave,CObjectFactory& Factory)=0;
	CObjectPersistance(void);
	~CObjectPersistance(void);
};

