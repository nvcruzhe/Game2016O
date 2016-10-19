#pragma once
#include "StateBase.h"
#include "EventBase.h"
#include "ObjectStock.h"
#include <map>
#include <list>
using namespace std;

/*

  ADMINISTRADOR DE M�QUINA DE ESTADOS FINITO JER�RQUICA

  Objetivo: Dar soporte para la implementaci�n de aplicaciones basadas en paradigma de estados.
  Ayudando principalmente en aplicaciones que hacen uso de grandes cantidades de memoria y n�mero de recursos
  como lo son las aplicaciones interactivas. Es claro que no todos los recursos son necesarios en todos los estados de la aplicaci�n.
  Tambi�n es auxiliar en el dise�o de las respuestas a eventos que la aplicaci�n deber� dar en funci�n del estado en qu� se encuentre.

  Las aplicaciones, basan su dise�o en el concepto de estado, en donde un estado inicia (CStateBase::OnEntry)
  procesa eventos (CStateBase::OnEvent) de manera periodica, y eventualmente termina (CStateBase::OnExit) para as� transitar hacia otro estado.

  Los juegos de video son un ejemplo de aplicaciones cuyo dise�o se facilita en base a �ste paradigma. 

  Los desarrolladores distribuyen su esfuerzo al definir e implementar estados. La integraci�n de los estados tambi�n resulta trivial en un aut�mata
  que conformar� un todo. Los sub estados o estados comparten y transfieren informaci�n a trav�s del contexto de sus superestados. Por lo que los desarrolladores
  pueden quedar de acuerdo al definir qu� recursos estar�n disponibles para los sub-estados. Los recursos dejan de est�r disponibles cuando el superestado ya no est� vigente.

  Los equipos de desarrolladores o un desarrollador, pueden concentrar sus esfuerzo en la implementaci�n de los m�todos OnEntry, OnExit y OnEvent 
  para definir el comportamiento de la aplicaci�n, siempre y cuando se sigan respetando los acuerdos establecidos para definir los superestados 

  El administrador de la m�quina (CStateMachineManager) controla correctamente y de manera jer�rquica las secuencias OnEntry y OnExit de cada estado y superestados
  de tal menera que la administraci�n de recursos sea autom�tica.

  Un estado puede abandonarse y trasitar hacia otro estado, solicitando al administrador a qu� estado se desea realizar la transici�n. (CStateMachineManager::Transition)

  El administrador envia los eventos (mensajes) hacia el estado vigente o activo. Si el estado opta por no procesar dicho mensaje, el administrador reenviar�
  el evento hacia su superestado para que �ste lo procese, y as� sucesivamente de ser necesario. 

  Para mayor informaci�n estudie referencias acerca de Autom�tas Jer�rquicos.
  http://www.accu-usa.org/slides/samek0311.pdf


  M. en C. Pedro Arturo Cornejo Torres

  
*/

#define UPDATE 0xff4cf5a3
class CStateMachineManager:public CObjectStock
{
private:
	//Identificador del estado inicial (Cualquier estado de la m�quina, el cu�l ser� el estado activo tras ejecutar Start()
	unsigned long m_ulIDInitialState;
	//Identificador del estado actual 
	unsigned long m_ulIDCurrentState;
	//Identificador del estado que solicita la transici�n.
	unsigned long m_ulIDSourceState; 

	unsigned long m_ulTime;  //Diezmil�simas de segundo.
	unsigned long m_uldTime; //Diezmilesimas de segundo

	map<unsigned long,unsigned long> m_mapSuperStates;
	list<CEventBase*> m_lstQueuedEvents;
protected:
	/*
		El estado nulo, muy �til como estado pozo. (La m�quina de estados se paraliza). El estado nulo siempre est� en la m�quina, y cualquier estado puede realizar
		una transici�n hacia �ste estado, en caso de excepciones no manejadas.
		CStateNull consume todos los eventos si se realiza una transici�n hacia �ste estado. 

		Debe invocarse a Reset() si se desea volver a iniciar �sta m�quina desde el comienzo. No olvde invocar a Start.
	*/
	#define CLSID_CStateNull 0xffff846f
	class CStateNull:public CStateBase
	{
	public:
		unsigned long GetClassID(){ return CLSID_CStateNull;}
		const char*   GetClassString() { return "CStateMachineManager::CStateNull";}
	protected:
		virtual void OnEntry();
		virtual void OnExit();
		virtual unsigned long OnEvent(CEventBase* pEvent);
	};
	//Se utiliza para invocar la inicializaci�n recurrente de un estado, y sus sub-estados, de manera descendente
	void InitState(unsigned long ulStateID);	
public:
	//El estado pozo
	CStateNull  m_StateNull;
	//Registra un estado e indica cual sub estado es el inicial, si es que tiene subestados. Si el estado no contiene sub-estados, ulIDInitialSubState debe ser 0
	void		RegisterState(CStateBase* pState,unsigned long ulIDState,unsigned long ulIDInitialSubState);
	//Registrados los estados, LinkToSuperState, establece la relaci�n de parentezco entre el estado y super-estado
	void		LinkToSuperState(unsigned long ulIDState,unsigned long ulIDSuperState);
	//Rompe la relaci�n de parentezco del estado indicado. As� el estado con ulIDState no pertenecer� a un super-estado
	void		UnlinkFromSuperState(unsigned long ulIDState);
	//Rompe todas las relaciones de parentezco de los estados. (No elimina los estados). Tras ejecutar �ste m�todo todos los estados son hermanos.
	void		ClearLinks(void);
	//Elimina del registro a un estado. Esto decrementar� el contador de referencias del estado posiblemente elimin�ndolo.
	CStateBase* UnregisterState(unsigned long ulIDState);
	//Consulta el identificador del super-estado que contiene al estado indicado por ulIDState
	unsigned long QuerySuperStateOf(unsigned long ulIDState);
	//Realiza una transici�n hacia cualquier estado en la HSM, realizando la liberaci�n e inicializaci�n de estados necesarios. El estado activo ser� ulIDState y todos sus ancestros
	void		Transition(unsigned long ulIDState);
	//Establece cual de los estados de la HSM ser� el estado inicial. Cualquier estado puede ser estado inicial.
	void		SetInitialState(unsigned long ulIDState);
	//Inicializa la m�quina de estados y la prepara para despachar eventos.
	void		Start();
	//Reinicia la m�quina de estados, desactivando los estados que estaban activos. No elimina estados ni relaci�n jer�rquica. Debe invocarse a Start para comenzar de nuevo en un estado activo.
	void		Reset();
	//Despacha un evento hacia los estados activos, de manera jerarquica ascendente.
	void		Dispatch(CEventBase* pEvent);
	//Coloca un evento en la cola de mensajes. Los eventos acumulados en la cola, pueden ser despachados mediante ProcessEvents, tal y como fueron colocados en la cola. Todo evento que se encole, debe ser creado din�micamente (new)
	void		PostEvent(CEventBase* pEvent);
	//Procesa todos los eventos en la cola de eventos. Los eventos son eliminados autom�ticamente. Todos los eventos encolados ser�n eliminados tras ser despachados (delete)
	void		ProcessEvents(void);
	//Obtiene el tiempo local de esta m�quina en segundos en formato flotante
	float GetLocalTime(){return m_ulTime/10000.0f;}; //Segundos....
	//Obtiene el tiempo incremental en segundos de �sta m�quina de estados en formato flotante
	float GetDeltaTime(){return m_uldTime/10000.0f;};  //Segundos
	//Tiempo incrementa uldTime diezmil�simas de segundo
	void  Time(unsigned long uldTime)
	{
		/*
			Es importante recordar, que las variables flotantes absorven los n�meros peque�os, impidiendo que incrementen, por lo que 
			es exacto usar enteros que expresen partes de un segundo, en este caso diezmil�simas de segundo.

			Al usar enteros, estos siempre incrementan, evit�ndonos serios problemas de precisi�n.
		*/
		m_ulTime+=uldTime;
		m_uldTime=uldTime;
	};                 

	CStateMachineManager(void);
	virtual ~CStateMachineManager(void);
};

