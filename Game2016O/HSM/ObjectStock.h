#pragma once
#include <set>
#include <map>
using namespace std;
class CObjectBase;
class CObjectCommand;

/* 
	La clase CObjectStock

	Implementa agregaci�n de objetos a trav�s de un contenedor asociativo (clave, objeto)
		AttachObject
		DetachObject

	Permite aplicar comandos a cada objeto de la colecci�n (ForEach) , o bien a un subconjunto del mismo mediante una partici�n (ForEachInSet)
		Debe especificarse un comando derivado de la clase CObjectCommand, que operar� sobre cada objeto.

	Busca objetos conociendo su clave (GetObjectByID())
*/

class CObjectStock
{
public:
	map<unsigned long, CObjectBase*> m_mapObjects;
public:
	CObjectBase* AttachObject(unsigned long ulIDObject,CObjectBase* pObject);
	CObjectBase* DetachObject(unsigned long ulIDObject);
	CObjectBase* GetObjectByID(unsigned long ulIDObject);
	//No AddRef version of GetObjectByID used for indirect method calls
	CObjectBase* QueryObjectByID(unsigned long ulIDObject);
	void		 ReleaseAll(void);
	void		 ForEach(CObjectCommand* pCommand);
	void		 ForEachInSet(set<unsigned long>& setIn, CObjectCommand* pCommand);
	CObjectStock(void);
	virtual ~CObjectStock(void);
};

