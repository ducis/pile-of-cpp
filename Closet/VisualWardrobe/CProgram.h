#pragma once
#include "windowsnominmax.h"
#include <algorithm>
#include <extrie.h>
#include <algorithm>
#include <vector>
#include <string>
#include <ostream>
#include "IScene.h"
#include <queue>
#include <set>
#include "mtxlib.h"
#include <map>
#include "ptree.h"
#include "CViewPanner.h"
#include "CIdentifier.h"
#include "CTargetCamera.h"
#include "CTypedSceneNodeManager.h"

using namespace std;
class CProgram{
public:
	//friend class ICommand;
	int AcceptCommand(const wstring &cmd, const wstring &arg, wostream &out);//ִ��ָ�cmd��ָ������arg�ǲ�����out�����
	void InsertCommand(const wstring &cmd,class ICommand *pCommand);//���ָ��
	CProgram();
	void OnResize(unsigned int w, unsigned int h);
	void Clear();//����
	void Redraw();//���Ƴ���
	void Present();//���ƺó�����backbuffer��frontbuffer����
	IScene *GetScene();//���ص�ǰ����
	int BindDC(HDC hDC);//��OpenGL����Device context
	void UseCurrentDC();//ʹ�õ�ǰ��Device context
	void PushNewBorn(class ISceneNode*);//ѹ��ڵ㵽�����������
	class ISceneNode* PopNewBorn();//��������ͷ�Ĵ�����ڵ�
	class ISceneNode* PeekNewBorn();//���ض���ͷ�Ĵ�����ڵ�
	void SetCursorHS(float x,float y);//�����������οռ��λ��
	void HideCursor();//�������(ʮ����)
	int PickCP(float x,float y);//ѡȡControl Plane
	int PickWithCursor();//ѡȡControl Plane
	void ClearPicks();//���ѡȡ
	void RequestTransformBaking(class ITransformBaked *);//��ʹ��
	void BakeBatch(int x_mm,int y_mm,int z_mm,int rotZ_90deg);//��ʹ��
	void DiscardTransformBakingBatch();//��ʹ��
	class CControlPlaneSystem *GetAxisAlignedCPSystem(int iAxis);//����Control Planeϵͳ,��ʹ��
	void NextCP();//��һ��Control Plane
	void PrevCP();//��һ��Control Plane
	void MoveCP(int delta);//�ƶ�Control Plane ��λ�� delta
	void RememberCurCP();//���µ�ǰControl Plane
	void ClearTheOtherCP();//�����Control Plane �ļ���
	void AnnotateCP();//��ע��ǰ�ĺͼ�ס��Control Plane��ľ���
	void WireFrameMode(){ m_renderMode = WIREFRAMEMODE; }//�趨��ʾģʽΪ�߿�
	void ShadedMode(){ m_renderMode = SHADEDMODE; }//�趨��ʾģʽΪ��ɫ
	bool IsInWireFrameMode(){ return WIREFRAMEMODE == m_renderMode; }//�����Ƿ����߿�ģʽ
	void RegisterFloatingSymbol(class IFloatingSymbol *);//�����ʾ�����Ϸ��ı��
	bool IsInOrthoView(){ 
		return m_orthoViewAxis>=0;//m_bInOrthoView; 
	}//�����Ƿ���������ͼģʽ
	int GetOrthoViewAxis(){
		return m_orthoViewAxis;
	}//���ص�ǰ������ͼģʽ����,0x 1y 2z
	//void SetInOrthoView(bool b){ m_bInOrthoView = b; }
	void SetOrthoViewAxis(int iAxis){ m_orthoViewAxis = iAxis; }//�趨��ǰ����ģʽ���� 0x 1y 2z
	class CTexture *GetTexture(const std::wstring &name);//ͨ��name���Ҳ���
	const boost::property_tree::wptree &GetConfig(){ return m_config; }//�������ò�����
	template<typename T> T GetConfigVar(const std::wstring &path) const;//�������ò���,ע�����ͱ�����ȷ,�����������
	template<typename T> void SetConfigVar(const std::wstring &path,const T &val);//�������ò���
	int SetConfigVar(std::wistream &in, std::wostream &out);//�������ò�������in����ȡ��������ֵ��out�����
	int ToggleBoolConfigVar(std::wistream &in, std::wostream &out);//�л����������ò�������in����ȡ��������out�����

	void RegisterPickable(int group, class IPickable *p);//ע���ѡȡ����ע���Ĳſ���ѡȡ��group��ȡ����ֵ��������ѡʱһ��ֻ�ܴ�һ��group��ѡȡ
	void UnregisterPickable(int group, class IPickable *p);//ע����ѡȡ����
	void Pick(int group);//��group���õ�ǰ���λ��ѡȡ
	void Unpick();//ȡ��ѡ��

	void BeginDrag();//��ʼ�϶�
	void EndDrag();//�����϶�
	void RotatePicked();//��תѡ�е�����(һ��90��)

	void RegisterWardrobeCell(class IWardrobeCell *pCell);//ע���¹�ĸ���,ע���Ĳſ���ѡȡ(������Pick(int group)����ѡȡ������)
	void UnregisterWardrobeCell(class IWardrobeCell *pCell);//ע���¹����
	void PickWardrobeCell();//ѡȡ�¹����
	void UnpickWardrobeCell();//ȡ��ѡ���¹����
	void SplitWardrobeCell(int iAxis);//iAxis��������¹����(0x 1y 2z)
	void DeployNewBornIntoPickedCell();//�Ӵ���������е���һ���ڵ����ѡ�����¹������

	void RegisterAnnotated(class IAnnotated *p);//ע�����ע�����壬ע���˵ı�ע����Ż���ʾ��ע
	void UnregisterAnnotated(class IAnnotated *p);//ע������ע������

	const matrix44 GetViewProjTransform()const;//��ȡ��ǰ���Ӿ����ͶӰ������˵Ľ��

	void RemovePicked();//ɾ����ǰѡ�еĽڵ�

	void AlignPickedWithPointedAt(int group, unsigned directionMask);	
	//����ǰ��ѡ����IBoxy����(�����)�뵱ǰ���λ��(���һ��SetCursorHS��������λ��)ָ���IBoxy������+x��+z�����϶���
	//һ��������ȡ��һ������
	void ConnectPickedToPointedAt(int group);
	//����ǰ��ѡ����IConnectible����(�����)�뵱ǰ���λ��(���һ��SetCursorHS��������λ��)ָ���IConnectible��������
	//һ��������ȡ��һ������
	void DisconnectPickedFromPointedAt(int group);
	//����ǰ��ѡ����IConnectible����(�����)�뵱ǰ���λ��(���һ��SetCursorHS��������λ��)ָ���IConnectible����Ͽ�����
	//һ��������ȡ��һ������

	class CAxialAABBAnnotation *GetWallAnnotation(const vector3 &facing, int iAxis) const;//��ȡǽ��λ�õı�ע���壬facing�ǳ���iAxis����(0x1y2z)
	void RegisterWallAnnotation(class CAxialAABBAnnotation * , const vector3 &facing , int iAxis);//ע��ǽ��λ�õı�ע����
	void UnregisterWallAnnotation(class CAxialAABBAnnotation * , int iAxis);//ע��ǽ��λ�õı�ע����

	class IMaterial *GetMaterial(const class CIdentifier &id) const;//�ñ�ʶ�����Ҳ���
	class IMaterial *GetCurrentMaterial() const;//��ȡ��ǰ����
	void SetCurrentMaterial(const class CIdentifier &id);//���õ�ǰ����
	void ListMaterials(std::wostream &out) const;

	class CDxfHeader *GetCurrentDxfHeaderTablesSec() const;//��ȡ��ǰ��Dxf�ļ�ͷ
	void SetCurrentDxfHeaderTablesSec(const class CIdentifier &id);//���ұ�ʶ��Ϊid��Dxf�ļ�ͷ����������Ϊ��ǰ��Dxf�ļ�ͷ
	void FormatPickedToDXF(const std::wstring &path);//�����ǰѡ�е������ǰ����ô�������뵽·����pathDXF��
	class IDrillingRule *GetCurrentDrillingRule(unsigned direction);//��ȡ��ǰ��direction�����ϵĴ�׹���

	class IHoleStyle *GetHoleStyle(const std::wstring &name);//��������Ϊname�Ŀ�ʽ��
	
	const boost::property_tree::wptree &GetManufactureProperties(){ return m_manufactureProperties; }//����manufactureProperties����
	void LoadManufacturePropertiesFromFile(const std::wstring &path);//װ��·��Ϊpath���ļ��е�����manufactureProperties

	const boost::property_tree::wptree &GetGenerators(){ return m_generators; }//����generators����
	void LoadGeneratorsFromFile(const std::wstring &path);//װ��·��Ϊpath���ļ��е�����generators

	void RegisterBoard(const class CBoard &board){ m_boards.insert( &board ); }//ע����,ע����İ�ĲŻ�����ڲ����嵥��
	void UnregisterBoard(const class CBoard &board){ m_boards.erase( &board ); }//ע�����
	void OutputBoardList(std::wostream &os);//��������嵥

	void ActivateCamera();//���������(CTargetCamera)
	void DeactivateCamera();//���������
	void SetCameraOrtho(int iView){ if(m_pCamera.get())	m_pCamera->MakeOrthogonal(iView); }////����ǰ���������Ϊ������ͼ,һ������(0��4)��ʾ�����������ͼ��ǰ�����Ҹ����е���һ��
	void CenterCameraView();//���������׼����������
	void FitView();//�޸�ͶӰ������������������
	float GetMinCellSize(){ return m_minCellSize; }//�����¹���ӳߴ������

	const boost::property_tree::wptree &GetGenericComponentScheme(){ return m_generics; }//����GenericComponentSchemes���ĸ��ڵ�.
	void LoadGenericComponentSchemeFromFile(const std::wstring &path);//װ��·��Ϊpath���ļ��е�����schemes
	ISceneNodeBoxy *CreateGenericComponentVariant(const std::wstring &scheme, const std::wstring &variant, unsigned flags);
	//����ͨ��scheme��variant����һ����GenericComponent(����֮��)
	void ListVariants(const std::wstring &scheme, std::wostream &out) const;

	template<class T> size_t GetNumRegisteredOrderedObjects() const{ return GetOrderedObjectRegistry<T>().size(); }//����ĳ���͵�����������������ע���˶��ٸ�
	template<class T> bool RegisterOrderedObject(T &obj){  //ע������
		std::vector<T*> &ctr(GetOrderedObjectRegistry<T>());
		std::vector<T*>::iterator iFound(std::find(ctr.begin(),ctr.end(),&obj));
		if(iFound!=ctr.end())	return false;
		ctr.push_back(&obj);
		return true;
	}
	template<class T> void UnregisterOrderedObject(T &obj){//ע������
		std::vector<T*> &ctr(GetOrderedObjectRegistry<T>());
		ctr.erase(std::find(ctr.begin(),ctr.end(),&obj),ctr.end());
	}
	template<class T> T &GetOrderedObject(size_t id){//�ҵ���id��ע������壬˳�����ע���˳��
		return *GetOrderedObjectRegistry<T>()[id];
	}
	template<class T> size_t GetObjectId(const T &obj){//����ĳ�������ǵڼ���ע���
		return std::find(GetOrderedObjectRegistry<T>().begin(),GetOrderedObjectRegistry<T>().end(),&obj)-GetOrderedObjectRegistry<T>().begin();
	}
	template<class T> T *GetPickedAs(){//����ǰѡ�е������ָ��ת��ΪT*���Ͳ�����
		return dynamic_cast<T*>(m_pCurrentlyPicked);
	}
private:
	//bool m_bInOrthoView;
	int m_orthoViewAxis;
	enum{
		WIREFRAMEMODE,
		SHADEDMODE
	}m_renderMode;

	HDC m_hDC;
	Trie<wchar_t,class ICommand*> m_trieCommands;
	IScene *m_pScene;
	std::queue<class ISceneNode*> m_newBorn;
	vector2 m_cursorHS;
	std::vector<class IControlPlane*> m_pickedCPs;
	int m_selectedCP;
	IControlPlane *m_pTheOtherCP;
	class CControlPlaneSystem *m_axisAlignedCPSys[3];
	std::vector<class ITransformBaked*> m_transformBakingRequested;
	std::set<class IFloatingSymbol*> m_pFloatingSymbols;
	std::map<std::wstring,class CTexture*> m_textures;
	boost::property_tree::wptree m_config;

	std::map< int,std::set<class IPickable *> > m_pRegisteredPickables;
	class IPickable *m_pCurrentlyPicked;
	class IDraggable *m_pCurrentlyDragged;
	vector2 m_dragDelta;

	std::set<class IWardrobeCell*> m_pWardrobeCells;
	class IWardrobeCell *m_pCurrentWardrobeCell;

	std::set<class IAnnotated*> m_pAnnotated;
	std::map< class CAxialAABBAnnotation *,vector3 > m_pWallAnnotations[3];

	std::map< class CIdentifier , class CSimpleMaterial * > m_pMaterials;
	IMaterial *m_pCurrentMaterial;

	CViewPanner m_viewPanner;

	//std::map< class CIdentifier , class CDxfHeader * > m_pDxfHeaders;
	std::map< class CIdentifier , class CDxfHeader * > m_pTablesSecDxfHeaders;
	CDxfHeader *m_pCurrentTablesSecDxfHeader;

	std::map< std::wstring , class IHoleStyle * > m_pHoleStyles;

	boost::property_tree::wptree m_manufactureProperties;
	boost::property_tree::wptree m_generators;
	boost::property_tree::wptree m_generics;

	std::set< const class CBoard * > m_boards;
	std::auto_ptr< class CTargetCamera > m_pCamera;
	std::vector< class CSceneNodeRoom* > m_roomList;
	std::vector< class CSceneNodeWardrobe* > m_wardrobeList;

	boost::property_tree::wptree::const_assoc_iterator m_propNodeMinCellSize;
	float m_minCellSize;

	typedef vector3 GenericComponentVariantType;
	typedef std::map<std::wstring, GenericComponentVariantType> GenericComponentVariantTableType;
	typedef std::map<std::wstring, GenericComponentVariantTableType> GenericComponentVariantTableRegistryType;
	GenericComponentVariantTableRegistryType m_genericComponentVariantTables;


	
	void LoadGenericComponentVariantTableFromFile(std::wstring);


	void RenderCursor();
	void RenderCPSelection();
	void RenderAnnotations();
	void RenderFloatingSymbols();
	const matrix44 GetViewTransform();
	void LoadMaterials();
	void LoadDxfHeaders();
	void LoadHoleStyles();
	void LoadManufactureProperties();
	void LoadGenerators();
	void LoadGenericComponentSchemes();
	void LoadGenericComponentVariantTables();
	template<class T> const std::vector<T*> &GetOrderedObjectRegistry() const;
	template<class T> std::vector<T*> &GetOrderedObjectRegistry(){
		return const_cast<std::vector<T*> &>(const_cast<const CProgram *>(this)->GetOrderedObjectRegistry<T>());
	}
};
