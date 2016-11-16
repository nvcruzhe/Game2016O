#pragma once
#include <vector>
#include <map>
#include "ObjectPersistance.h"

using namespace std;
class CMeshBase:public CObjectPersistance
{
public:
	struct VERTEX
	{
		VECTOR4D V;
		VECTOR4D N;
		float u,v;
	};
	struct XVERTEX:public VERTEX
	{
		VECTOR4D vTangent;
		VECTOR4D vBinormal;
		VECTOR4D vNormal;
	};
	struct MATERIAL
	{
		VECTOR4D   vDiffuse;        /* Diffuse color RGBA */
		VECTOR4D   vAmbient;        /* Ambient color RGB */
		VECTOR4D   vSpecular;       /* Specular 'shininess' */
		VECTOR4D   vEmissive;       /* Emissive color RGB */
		float      fPower;
	};
	struct XMATERIAL:public MATERIAL
	{
		vector<string>  vecTexturesFileNames;
		vector<long>	vecSRVIDs;   //Volatile
		long			lShaderID;   //Volatile
	};
	struct ATTRIBUTE_RANGE
	{
		unsigned long ulAttribID;   //Index in material
		unsigned long ulFaceStart;
		unsigned long ulFaceCount;
		unsigned long ulVertexStart;
		unsigned long ulVertexCount;
	};
	vector<XMATERIAL>			m_vecMaterials;
	vector<ATTRIBUTE_RANGE>		m_vecAttributes;
	vector<VERTEX>				m_vecVertices;
	vector<unsigned long>		m_vecIndices;
	vector<unsigned short>      m_vecIndices16;
	vector<XVERTEX>				m_vecXVertices;
	//GPU Resources
	long						m_lIBID;
	long						m_lVBID;
public:
	
	static CMeshBase* Join(MATRIX4D& XA,CMeshBase* pMeshA,MATRIX4D& XB, CMeshBase* pMeshB);

	/*
		Crea una un cubo de 12 caras a patir de los puntos m�ximo y m�nimo
	*/
	static CMeshBase* CreateBox(VECTOR4D& Min,VECTOR4D& Max);
	/*
	Calcula la caja envolvente m�s peque�a que contenga a todo v�rtice relacionado con los atributos especificados.
	
		La funci�n calcula la caja envolvente que resulta de la uni�n de las cajas envolventes
		de los conjuntos de v�rtices especificados por cada atributo en la lista vecInAttributeRanges.

		Si la lista de atributos vecInAttributeRanges est� vac�a, se considerar�n todos los atributos de �sta mesh. As�
		la caja envolvente cubrir� toda la mesh.

		La funci�n tambi�n calcula los puntos m�nimo y m�ximo que describen de forma simplificada al volumen de la caja resultante.

		El metodo retorna verdadero en caso de procesar una mesh no vac�a.

	*/

	virtual bool CalcBoundingBox(vector<ATTRIBUTE_RANGE>& vecInAttributeRages,VECTOR4D& Min,VECTOR4D& Max);
	/*
		Calcula la lista de caras que intersectan con el rayo con direcci�n vRay y origen vSrc.

		La funci�n retorna verdadero si hay al menos una intersecci�n.

		Las intersecciones se almacenan en el multimapa mmapOutIntersectedFaces de tipo 
		multimap<float,unsigned int> en donde la clave es la distancia medida desde vSrc hasta el punto en el tri�ngulo que intersecta
		el segmento vRay, y el valor asociado es el n�mero de cara (tri�ngulo) en la mesh que es intersecado.

		El algoritmo retorna todas las caras "delante del rayo" que son intersecadas.

		El m�todo retorna verdadero en caso de haber encontrado al menos una intersecci�n.
	*/
	virtual bool RayIntersect(VECTOR4D& vSrc,VECTOR4D& vRay,multimap<float,unsigned int>& mmapOutIntersectedFaces,ATTRIBUTE_RANGE& Range);
	virtual bool Serialize(CStream &io,bool bSave,CObjectFactory &Factory);	
	//Extiende esta mesh con v�rtices con espacio tangente a partir de las coordenadas de textura
	virtual void BuildTangentSpaceFromTexCoordsIndexed(void);
	//Remueve los �ndices de un tri�ngulo cuya area est� cercano a cero. Reduce la cantidad de �ndices.
	void RemoveTriangle(unsigned long ulFace);
	unsigned long GetVertexStart(unsigned long ulFace);
	unsigned long RemoveNearToZeroTriangles(float fEpsilon);
	bool BuildIndex16Buffer(void);
	CMeshBase(void);
	virtual ~CMeshBase(void);
};

