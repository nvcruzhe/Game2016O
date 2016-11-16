#pragma once
class CObjectBase;

/*
   Las aplicaciones pueden encapsular secuencias de operaciones sobre cualquier objeto de la clase CObjectBase a trav�s de 
   clases derivadas de CObjectCommand.

   Para definir cualquier secuencia de operaciones, deber� derivar sus comandos a partir de la clase CObjectCommand.

   Ejemplos : CCmdLoadResources,  CCmdApplyTransformations pueden especificar una estrategia o secuencia de acciones sobre un objeto.

   Para aplicar comandos de forma masiva o espec�fica, v�ase la clase CObjectStock y los m�todos CObjectStock::ForEach y CObjectStock::ForEachInSet

   Los comandos derivados, deber�n implementar el m�todo DoCommand quien ejecuta la secuencia de comandos espec�fica sobre el objeto. 
   Nota: La ejecuci�n no se limita a acciones aplicables sobre el objeto, si agrega variables miembro a su nuevo comando que deriva de CObjectCommand, 
   puede establecer par�metros que controlan la ejecuci�n de su implementaci�n DoCommand. Las posibilidades son incontables.
*/

class CObjectCommand
{
public:
	//Estrategia de ejecuci�n (comando) sobre el objeto.
	virtual bool DoCommand(CObjectBase* pTarget)=0;
	CObjectCommand(void);
	~CObjectCommand(void);
};

