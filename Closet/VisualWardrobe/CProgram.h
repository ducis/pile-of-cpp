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
	int AcceptCommand(const wstring &cmd, const wstring &arg, wostream &out);//执行指令，cmd是指令名，arg是参数，out是输出
	void InsertCommand(const wstring &cmd,class ICommand *pCommand);//添加指令
	CProgram();
	void OnResize(unsigned int w, unsigned int h);
	void Clear();//清屏
	void Redraw();//绘制场景
	void Present();//绘制好场景后backbuffer和frontbuffer互换
	IScene *GetScene();//返回当前场景
	int BindDC(HDC hDC);//给OpenGL设置Device context
	void UseCurrentDC();//使用当前的Device context
	void PushNewBorn(class ISceneNode*);//压入节点到待处理队列中
	class ISceneNode* PopNewBorn();//弹出队列头的待处理节点
	class ISceneNode* PeekNewBorn();//返回队列头的待处理节点
	void SetCursorHS(float x,float y);//设置鼠标在齐次空间的位置
	void HideCursor();//隐藏鼠标(十字线)
	int PickCP(float x,float y);//选取Control Plane
	int PickWithCursor();//选取Control Plane
	void ClearPicks();//清除选取
	void RequestTransformBaking(class ITransformBaked *);//不使用
	void BakeBatch(int x_mm,int y_mm,int z_mm,int rotZ_90deg);//不使用
	void DiscardTransformBakingBatch();//不使用
	class CControlPlaneSystem *GetAxisAlignedCPSystem(int iAxis);//返回Control Plane系统,不使用
	void NextCP();//下一个Control Plane
	void PrevCP();//上一个Control Plane
	void MoveCP(int delta);//移动Control Plane 的位置 delta
	void RememberCurCP();//记下当前Control Plane
	void ClearTheOtherCP();//清除对Control Plane 的记忆
	void AnnotateCP();//标注当前的和记住的Control Plane间的距离
	void WireFrameMode(){ m_renderMode = WIREFRAMEMODE; }//设定显示模式为线框
	void ShadedMode(){ m_renderMode = SHADEDMODE; }//设定显示模式为着色
	bool IsInWireFrameMode(){ return WIREFRAMEMODE == m_renderMode; }//返回是否处于线框模式
	void RegisterFloatingSymbol(class IFloatingSymbol *);//添加显示在最上方的标记
	bool IsInOrthoView(){ 
		return m_orthoViewAxis>=0;//m_bInOrthoView; 
	}//返回是否处于正交视图模式
	int GetOrthoViewAxis(){
		return m_orthoViewAxis;
	}//返回当前正交视图模式的轴,0x 1y 2z
	//void SetInOrthoView(bool b){ m_bInOrthoView = b; }
	void SetOrthoViewAxis(int iAxis){ m_orthoViewAxis = iAxis; }//设定当前正交模式的轴 0x 1y 2z
	class CTexture *GetTexture(const std::wstring &name);//通过name查找材质
	const boost::property_tree::wptree &GetConfig(){ return m_config; }//返回配置参数树
	template<typename T> T GetConfigVar(const std::wstring &path) const;//返回配置参数,注意类型必须正确,否则程序会崩溃
	template<typename T> void SetConfigVar(const std::wstring &path,const T &val);//设置配置参数
	int SetConfigVar(std::wistream &in, std::wostream &out);//设置配置参数，从in中提取参数名和值，out是输出
	int ToggleBoolConfigVar(std::wistream &in, std::wostream &out);//切换布尔型配置参数，从in中提取参数名，out是输出

	void RegisterPickable(int group, class IPickable *p);//注册可选取对象，注册后的才可以选取，group可取任意值，但鼠标点选时一次只能从一个group中选取
	void UnregisterPickable(int group, class IPickable *p);//注销可选取对象
	void Pick(int group);//在group中用当前鼠标位置选取
	void Unpick();//取消选定

	void BeginDrag();//开始拖动
	void EndDrag();//结束拖动
	void RotatePicked();//旋转选中的物体(一次90度)

	void RegisterWardrobeCell(class IWardrobeCell *pCell);//注册衣柜的格子,注册后的才可以选取(不属于Pick(int group)所能选取的物体)
	void UnregisterWardrobeCell(class IWardrobeCell *pCell);//注销衣柜格子
	void PickWardrobeCell();//选取衣柜格子
	void UnpickWardrobeCell();//取消选定衣柜格子
	void SplitWardrobeCell(int iAxis);//iAxis方向分裂衣柜格子(0x 1y 2z)
	void DeployNewBornIntoPickedCell();//从待处理队列中弹出一个节点放入选定的衣柜格子中

	void RegisterAnnotated(class IAnnotated *p);//注册带标注的物体，注册了的标注物体才会显示标注
	void UnregisterAnnotated(class IAnnotated *p);//注销带标注的物体

	const matrix44 GetViewProjTransform()const;//获取当前的视距阵和投影矩阵相乘的结果

	void RemovePicked();//删除当前选中的节点

	void AlignPickedWithPointedAt(int group, unsigned directionMask);	
	//将当前已选定的IBoxy物体(如柜体)与当前鼠标位置(最后一次SetCursorHS输入的鼠标位置)指向的IBoxy物体在+x和+z方向上对齐
	//一个参数是取哪一组物体
	void ConnectPickedToPointedAt(int group);
	//将当前已选定的IConnectible物体(如柜体)与当前鼠标位置(最后一次SetCursorHS输入的鼠标位置)指向的IConnectible物体连接
	//一个参数是取哪一组物体
	void DisconnectPickedFromPointedAt(int group);
	//将当前已选定的IConnectible物体(如柜体)与当前鼠标位置(最后一次SetCursorHS输入的鼠标位置)指向的IConnectible物体断开连接
	//一个参数是取哪一组物体

	class CAxialAABBAnnotation *GetWallAnnotation(const vector3 &facing, int iAxis) const;//获取墙体位置的标注物体，facing是朝向，iAxis是轴(0x1y2z)
	void RegisterWallAnnotation(class CAxialAABBAnnotation * , const vector3 &facing , int iAxis);//注册墙体位置的标注物体
	void UnregisterWallAnnotation(class CAxialAABBAnnotation * , int iAxis);//注销墙体位置的标注物体

	class IMaterial *GetMaterial(const class CIdentifier &id) const;//用标识符查找材质
	class IMaterial *GetCurrentMaterial() const;//获取当前材质
	void SetCurrentMaterial(const class CIdentifier &id);//设置当前材质
	void ListMaterials(std::wostream &out) const;

	class CDxfHeader *GetCurrentDxfHeaderTablesSec() const;//获取当前的Dxf文件头
	void SetCurrentDxfHeaderTablesSec(const class CIdentifier &id);//查找标识符为id的Dxf文件头并将其设置为当前的Dxf文件头
	void FormatPickedToDXF(const std::wstring &path);//如果当前选中的物体是板材那么将其输入到路径是pathDXF中
	class IDrillingRule *GetCurrentDrillingRule(unsigned direction);//获取当前的direction方向上的打孔规则

	class IHoleStyle *GetHoleStyle(const std::wstring &name);//查找名字为name的孔式样
	
	const boost::property_tree::wptree &GetManufactureProperties(){ return m_manufactureProperties; }//返回manufactureProperties的树
	void LoadManufacturePropertiesFromFile(const std::wstring &path);//装载路径为path的文件中的所有manufactureProperties

	const boost::property_tree::wptree &GetGenerators(){ return m_generators; }//返回generators的树
	void LoadGeneratorsFromFile(const std::wstring &path);//装载路径为path的文件中的所有generators

	void RegisterBoard(const class CBoard &board){ m_boards.insert( &board ); }//注册板材,注册过的板材才会出现在材料清单中
	void UnregisterBoard(const class CBoard &board){ m_boards.erase( &board ); }//注销板材
	void OutputBoardList(std::wostream &os);//输出材料清单

	void ActivateCamera();//启用摄像机(CTargetCamera)
	void DeactivateCamera();//禁用摄像机
	void SetCameraOrtho(int iView){ if(m_pCamera.get())	m_pCamera->MakeOrthogonal(iView); }////将当前摄像机重置为正交视图,一个参数(0到4)表示是五个正交视图（前后左右俯）中的哪一个
	void CenterCameraView();//将摄像机对准场景的中心
	void FitView();//修改投影矩阵以容下整个场景
	float GetMinCellSize(){ return m_minCellSize; }//返回衣柜格子尺寸的下限

	const boost::property_tree::wptree &GetGenericComponentScheme(){ return m_generics; }//返回GenericComponentSchemes树的根节点.
	void LoadGenericComponentSchemeFromFile(const std::wstring &path);//装载路径为path的文件中的所有schemes
	ISceneNodeBoxy *CreateGenericComponentVariant(const std::wstring &scheme, const std::wstring &variant, unsigned flags);
	//生成通过scheme和variant生成一个的GenericComponent(抽屉之类)
	void ListVariants(const std::wstring &scheme, std::wostream &out) const;

	template<class T> size_t GetNumRegisteredOrderedObjects() const{ return GetOrderedObjectRegistry<T>().size(); }//返回某类型的物体已在主程序中注册了多少个
	template<class T> bool RegisterOrderedObject(T &obj){  //注册物体
		std::vector<T*> &ctr(GetOrderedObjectRegistry<T>());
		std::vector<T*>::iterator iFound(std::find(ctr.begin(),ctr.end(),&obj));
		if(iFound!=ctr.end())	return false;
		ctr.push_back(&obj);
		return true;
	}
	template<class T> void UnregisterOrderedObject(T &obj){//注销物体
		std::vector<T*> &ctr(GetOrderedObjectRegistry<T>());
		ctr.erase(std::find(ctr.begin(),ctr.end(),&obj),ctr.end());
	}
	template<class T> T &GetOrderedObject(size_t id){//找到第id个注册的物体，顺序就是注册的顺序
		return *GetOrderedObjectRegistry<T>()[id];
	}
	template<class T> size_t GetObjectId(const T &obj){//返回某个物体是第几个注册的
		return std::find(GetOrderedObjectRegistry<T>().begin(),GetOrderedObjectRegistry<T>().end(),&obj)-GetOrderedObjectRegistry<T>().begin();
	}
	template<class T> T *GetPickedAs(){//将当前选中的物体的指针转化为T*类型并返回
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
