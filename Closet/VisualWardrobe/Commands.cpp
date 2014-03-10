#pragma once
#include "CProgram.h"
#include "CNamedCommand.h"
#include "3d.h"
#include <sstream>
#include "ISceneNodeFactory.h"
#include "ptree.h"
#include "CSceneNodeGLUT-.h"
#include "CSceneNodeCoordSys.h"
#include "CSceneNodeComponent.h"
#include "CSceneNodeFactoryRegistry.h"
#include <fstream>
#include "IScene.h"
#include "CTypedSceneNodeManager.h"
#include "CSceneNodeInteriorDef.h"
#include "CSceneNodeObjectSet.h"
#include "CSceneNodeBoxyDraggableContainer.h"
#include "CSceneNodeBinaryWardrobeTreeNode.h"
#include "CSceneNodeWardrobe.h"
#include "CSceneNodeBoxyGenericComponent.h"
#include "CSceneNodeRoom.h"
#include "CSceneNodeBoard.h"
#include "IMaterial.h"
#include "IMaterialUser.h"
#include <algorithm>


// BEGINCMD..ENDCMD宏是添加一条指令，用的是CNamedCommand类，
//SIMPLECALL是添加一条无参数指令，作用是直接调用CProgram中的同名函数


#define BEGINCMD(name) \
class _Cmd##name: public CNamedCommand{ \
private: \
	static _Cmd##name s_instance; \
	_Cmd##name(){	GetSingleton<CProgram>()->InsertCommand(wstring(L#name),this);	} \


#define ENDCMD(name) }; \
	_Cmd##name _Cmd##name::s_instance;

#define INVOKE int Invoke(class CProgram *pProgram, std::wistream &arg, std::wostream &log) const
#define SIMPLECALL(name)		\
	BEGINCMD(name);				\
	INVOKE{						\
		pProgram->name();		\
		return 0;				\
	}							\
	ENDCMD(name);

namespace Commands{
	SIMPLECALL(RememberCurCP);
	SIMPLECALL(ClearTheOtherCP);
	SIMPLECALL(AnnotateCP);
	SIMPLECALL(WireFrameMode);
	SIMPLECALL(ShadedMode);
	SIMPLECALL(PickWithCursor);
	SIMPLECALL(NextCP);
	SIMPLECALL(PrevCP);
	SIMPLECALL(HideCursor);
	SIMPLECALL(Unpick);
	SIMPLECALL(BeginDrag);
	SIMPLECALL(EndDrag);
	SIMPLECALL(RotatePicked);
	SIMPLECALL(PickWardrobeCell);
	SIMPLECALL(UnpickWardrobeCell);
	SIMPLECALL(DeployNewBornIntoPickedCell);
	SIMPLECALL(RemovePicked);
	SIMPLECALL(ActivateCamera);
	SIMPLECALL(DeactivateCamera);
	SIMPLECALL(CenterCameraView);
	SIMPLECALL(FitView);
	BEGINCMD(OnResize);
	INVOKE{
		unsigned int w(512),h(512);
		arg>>w>>h;
		pProgram->OnResize(w,h);
		return 0;
	}
	ENDCMD(OnResize);

	BEGINCMD(ReturnThirteen);//返回13,用作测试
	INVOKE{
		return 13;
	}
	ENDCMD(ReturnThirteen);
	

	BEGINCMD(WireCube);//在当前场景中插入一个线框正方体,用作测试
	INVOKE{
		pProgram->GetScene()->Insert( new CSceneNodeGLUTWireCube );
		return 0;
	}
	ENDCMD(WireCube);


	BEGINCMD(SolidCube);//在当前场景中插入一个面片正方体,用作测试
	INVOKE{
		pProgram->GetScene()->Insert( new CSceneNodeGLUTSolidCube );
		return 0;
	}
	ENDCMD(SolidCube);


	BEGINCMD(SetBackgroundColor);//设置背景颜色,有三个参数,分别是颜色的r,g,b分量,取值范围均为[0.0,1.0]
	INVOKE{
		float r = 1.0f,g = 1.0f,b = 1.0f;
		arg>>r>>g>>b;
		glClearColor(r,g,b,1.0f);
		return 0;
	}
	ENDCMD(SetBackgroundColor);

	
	BEGINCMD(ReturnError);//返回输入的整数 用作测试
	INVOKE{
		int e;
		arg>>e;
		return e;
	}
	ENDCMD(ReturnError);


	BEGINCMD(Camera);//不使用
	INVOKE{
		return 0;
	}
	ENDCMD(Camera);

	
	BEGINCMD(LookAt);
	//设定视矩阵. 九个参数,依次是视点,目标点和上方向的x,y,z坐标,默认值是(0,0,10000),(0,0,0),(0,1,0)
	//注意这个设置是直接调用OpenGL的函数,不会保存状态,和CTargetCamera不相容
	//会自动判断是否是正交视图
	INVOKE{
		float eyex = 0,eyey = 0,eyez = 10000,centerx = 0,centery = 0,centerz = 0,upx = 0,upy = 1,upz = 0;
		arg>>eyex>>eyey>>eyez>>centerx>>centery>>centerz>>upx>>upy>>upz;
		glPushAttrib(GL_TRANSFORM_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyex,eyey,eyez,centerx,centery,centerz,upx,upy,upz);
		glPopAttrib();
		//pProgram->SetInOrthoView((eyex==centerx?1:0)+(eyey==centery?1:0)+(eyez==centerz?1:0) == 2);
		if((eyex==centerx?1:0)+(eyey==centery?1:0)+(eyez==centerz?1:0) == 2){
			if(eyex != centerx){
				pProgram->SetOrthoViewAxis(0);
			}else if(eyey != centery){
				pProgram->SetOrthoViewAxis(1);
			}else if(eyez != centerz){
				pProgram->SetOrthoViewAxis(2);
			}
		}else{
			pProgram->SetOrthoViewAxis(-1);
		}
		return 0;
	}
	ENDCMD(LookAt);


	BEGINCMD(TopView);//俯视图,即从(x,y,10000)向(x,y,0)看.参数为x和y.不会保存状态,和CTargetCamera不相容
	INVOKE{
		float x = 0, y = 0;
		arg>>x>>y;
		wostringstream wss;
		wss<<x<<L' '<<y<<L' '<<10000<<L' '<<x<<L' '<<y<<L" 0 0 1 0";
		return pProgram->AcceptCommand(L"LookAt",wss.str(),log);
	}
	ENDCMD(TopView);

	BEGINCMD(FrontView);//正视图,即从(x,-30000,z)向(x,0,z)看.参数为x和z.不会保存状态,和CTargetCamera不相容
	INVOKE{
		float x = 0, z = 0;
		arg>>x>>z;
		wostringstream wss;
		wss<<x<<L' '<<-30000<<L' '<<z<<L' '<<x<<L' '<<0<<L' '<<z<<L" 0 0 1";
		return pProgram->AcceptCommand(L"LookAt",wss.str(),log);
	}
	ENDCMD(FrontView);


	BEGINCMD(LeftView);//正视图,即从(30000,y,z)向(0,y,z)看.参数为y和z.不会保存状态,和CTargetCamera不相容
	INVOKE{
		float y = 0, z = 0;
		arg>>y>>z;
		wostringstream wss;
		wss<<30000<<L' '<<y<<L' '<<z<<L' '<<0<<L' '<<y<<L' '<<z<<L" 0 0 1";
		return pProgram->AcceptCommand(L"LookAt",wss.str(),log);
	}
	ENDCMD(LeftView);



	BEGINCMD(Ortho);//设定投影矩阵为正交投影矩阵,四个参数分别为宽,高,近平面,远平面
	INVOKE{
		float w = 10000,h = 10000,zNear = 1,zFar = 50000;
		arg>>h>>zNear>>zFar;
		{
			float vp[4];
			glGetFloatv(GL_VIEWPORT,vp);
			w = h * vp[2]/vp[3];
		}
		glPushAttrib(GL_TRANSFORM_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-w*0.5,w*0.5,-h*0.5,h*0.5,zNear,zFar);
		glPopAttrib();
		return 0;
	}
	ENDCMD(Ortho);


	BEGINCMD(Perspective);//设定投影矩阵为透视投影矩阵,三个参数分别为视角(单位为度,y方向),近平面,远平面
	INVOKE{
		double fovy(45.0),zNear(10),zFar(50000);
		arg>>fovy>>zNear>>zFar;
		float vp[4];
		glGetFloatv(GL_VIEWPORT,vp);
		double a(vp[2]/vp[3]);
		glPushAttrib(GL_TRANSFORM_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fovy,a,zNear,zFar);
		glPopAttrib();
		return 0;
	};
	ENDCMD(Perspective);


	BEGINCMD(Zoom);//缩放投影矩阵,只对正交投影有效,一个参数为缩放的倍数(1.0为不变)
	INVOKE{
		float d(1.0f);
		arg>>d;
		
		glPushAttrib(GL_TRANSFORM_BIT);
		glMatrixMode(GL_PROJECTION);
		glScalef(d,d,1);
		glPopAttrib();
		return 0;
	}
	ENDCMD(Zoom);

	BEGINCMD(LightsOn);//全局启用灯光
	INVOKE{
		glEnable(GL_LIGHTING);
		return 0;
	}
	ENDCMD(LightsOn);

	BEGINCMD(LightsOff);//全局禁用灯光
	INVOKE{
		glDisable(GL_LIGHTING);
		return 0;
	}
	ENDCMD(LightsOff);

	BEGINCMD(LightDir);//启用一个方向光,四个参数，第一个参数是灯光号(0到7),后三个是灯光方向向量
	INVOKE{
		unsigned int id;
		arg>>id;
		if(id>=8)
			return 1;
		float lv[4] = {0,0,1,0};
		arg>>lv[0]>>lv[1]>>lv[2];
		glPushMatrix();
		glLoadIdentity();
		glLightfv(GL_LIGHT0+id,GL_POSITION,lv);
		glPopMatrix();
		glEnable(GL_LIGHT0+id);
		return 0;
	}
	ENDCMD(LightDir);

	BEGINCMD(KillAllLights);//禁用所有灯光
	INVOKE{
		for(int i=0;i<8;++i){
			glDisable(GL_LIGHT0+i);
		}
		return 0;
	}
	ENDCMD(KillAllLights);

	BEGINCMD(CoordSys);//在场景中增添一个坐标系的显示，有一个参数是坐标轴显示的长度
	INVOKE{
		float sz = 1000;
		arg>>sz;
		pProgram->GetScene()->Insert( new CSceneNodeCoordSys(sz) );
		return 0;
	}
	ENDCMD(CoordSys);

	BEGINCMD(LoadFile);//读取一个xml文件并将其中的场景结点压入主程序中的待处理队列，有一个参数是文件路径
	INVOKE{
		std::wstring fname;
		arg>>fname;
		using namespace boost::property_tree;
		wptree tr;
		wifstream f( fname.c_str() );
		if( !f.is_open() ){
			log<<L"Can't open file "<<fname<<L'\n';
			return -1;
		}
		read_xml(f,tr);
		if(tr.empty()){
			log<<L"Empty\n";
			return 0;
		}
		for(wptree::iterator itr = tr.begin(); itr != tr.end(); ++itr){
			ISceneNodeFactory *pFac = GetSingleton<CSceneNodeFactoryRegistry>()->GetFactory(itr->first);
			if(!pFac){
				log<<L"Undefined node: "<<itr->first<<L" when parsing "<<fname<<L'\n';
				//return -1;
			}else{
				ISceneNode *p = pFac->Create(itr->second);
				pProgram->PushNewBorn(p);
				log<<L"Node type: "<<itr->first<<'\n';
			}
		}
		return 0;
	}
	ENDCMD(LoadFile);

	BEGINCMD(Generate);
	//用指定的Generator和ManufactureProperty生成一个新的(非柜体的)场景节点压入主程序的待处理队列，
	//有两个参数分别是Generator和ManufactureProperty的名字(不是文件名,是文件中相应xml结点的名字)
	INVOKE{
		using namespace boost::property_tree;
		std::wstring generatorName;
		std::wstring manufactureParamSetName;
		arg>>generatorName>>manufactureParamSetName;
		const wptree &trG( pProgram->GetGenerators().get_child(generatorName) );
		const wptree &trP( pProgram->GetManufactureProperties().get_child(manufactureParamSetName) );
		ISceneNode *p( CSceneNodeBinaryWardrobeTreeNode::BuildBWT(trG,trP).first );
		pProgram->PushNewBorn(p);
		return 0;
	}
	ENDCMD(Generate);

	BEGINCMD(DeployNewBorn);
	//从主程序的待处理队列中弹出一个节点并插入到当前场景中
	INVOKE{
		pProgram->GetScene()->Insert(GetSingleton<CProgram>()->PopNewBorn());
		return 0;
	}
	ENDCMD(DeployNewBorn);

	
	BEGINCMD(DeployNewBornAsComponent);
	//从主程序的待处理队列中弹出一个节点,包装为一个CSceneNodeComponent,再插入到当前场景中
	INVOKE{
		pProgram->GetScene()->Insert( new CSceneNodeComponent(pProgram->PopNewBorn(),arg) );
		return 0;
	}
	ENDCMD(DeployNewBornAsComponent);

	BEGINCMD(CreateWardrobe);
	//用指定的Generator和ManufactureProperty生成一个新的柜体压入主程序的待处理队列，
	//有两个参数分别是Generator和ManufactureProperty的名字(不是文件名,是文件中相应xml结点的名字)
	INVOKE{
		using namespace boost::property_tree;
		std::wstring generatorGroupName;
		std::wstring manufactureParamSetName;
		arg>>generatorGroupName>>manufactureParamSetName;
		const wptree &trG( pProgram->GetGenerators().get_child(generatorGroupName) );
		pProgram->PushNewBorn( new CSceneNodeWardrobe(*new CSceneNodeWardrobeSelector( 
			trG.get_child(L"None"),
			trG.get_child(L"Left"),
			trG.get_child(L"Right"),
			trG.get_child(L"Both"),
			pProgram->GetManufactureProperties().get_child(manufactureParamSetName)
			)) );
		return 0;
	}
	ENDCMD(CreateWardrobe);

	BEGINCMD(DeployNewBornAsBoxyDraggableContainer);
	//从主程序的待处理队列中弹出一个节点,验证是否是一个ObjectSet节点，
	//如果是的话就将CSceneNodeObjectSet的子节点取出来放到一个新的BoxyDraggableContainer中,再将新节点插入到当前场景中
	//5个参数 第一个是绕z轴的旋转,第二个到第四个是位置,第五个是布尔型是否创建约束
	INVOKE{
		//if(!dynamic_cast<CSceneNodeObjectSet*>(pProgram->PeekNewBorn())){
		//	log<<L"Root is not an ObjectSet.\n";
		//	return 1;
		//}
		//assert(pObjSet);
		ISceneNode *pSN(pProgram->PopNewBorn());
		CSceneNodeObjectSet *pObjSet = dynamic_cast<CSceneNodeObjectSet*>(pSN);
		ISceneNodeBoxy *pSNB(NULL);
		if(!pObjSet){
			pSNB = dynamic_cast<ISceneNodeBoxy*>(pSN);
			if(!pSNB){
				return 1;
			}
		}
		float rotInDeg(0);
		vector3 translation(0,0,0);
		bool bCreateConstraint(true);
		arg>>rotInDeg>>translation>>bCreateConstraint;
		CSceneNodeBoxyDraggableContainer *pBDC = new CSceneNodeBoxyDraggableContainer(rotInDeg,translation,bCreateConstraint);
		if(pObjSet){
			pBDC->RipObjectSet(pObjSet);
		}else{
			assert(pSNB);
			pBDC->Insert(pSNB);
		}
		pBDC->EnableAutoAnnotation();
		if(pObjSet) delete pObjSet;
		pProgram->GetScene()->Insert(pBDC);
		pBDC->SetSurfaceDraggable(
			(1<<CSceneNodeBoxyDraggableContainer::DraggableSurfaceType::X_R) |
			(1<<CSceneNodeBoxyDraggableContainer::DraggableSurfaceType::Y_L) |
			(1<<CSceneNodeBoxyDraggableContainer::DraggableSurfaceType::Y_R) |
			(1<<CSceneNodeBoxyDraggableContainer::DraggableSurfaceType::Z_R) );
		return 0;
	}
	ENDCMD(DeployNewBornAsBoxyDraggableContainer);

	BEGINCMD(DeployNewBornIntoInterior);//不使用
	INVOKE{
		CSceneNodeInteriorDef *pInterior = GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent();
		pInterior->AddSubNode( pProgram->PopNewBorn() );
		pInterior->SetBoardNum(pInterior->GetSubNodeNum());
		pInterior->RepositionSubNodes();
		return 0;
	}
	ENDCMD(DeployNewBornIntoInterior);

	BEGINCMD(PlaceRecentlyDeployed);//设定最近插入的节点的位置,和绕z轴角度,不常用
	INVOKE{
		int x = 0, y = 0, z = 0, r = 0;
		arg>>r>>x>>y>>z;
		pProgram->BakeBatch(x,y,z,r);
		return 0;
	}
	ENDCMD(PlaceRecentlyDeployed);

	BEGINCMD(SetCursorHS);//设定鼠标的齐次空间坐标,两个参数分别为x,y
	INVOKE{
		float x = 0.0f,y = 0.0f;
		arg>>x>>y;
		pProgram->SetCursorHS(x,y);
		return 0;
	}
	ENDCMD(SetCursorHS);

	BEGINCMD(PickCPHS);//使用输入的齐次空间坐标对ControlPlane进行拾取,两个参数分别为x,y.若不输入参数则x,y均为-100,相当于取消选取
	INVOKE{
		float x = -100.0f,y = -100.0f;
		arg>>x>>y;
		log<<pProgram->PickCP(x,y)<<endl;
		return 0;
	}
	ENDCMD(PickCPHS);

	BEGINCMD(MoveCP);//移动ControlPlane,一个参数是移动量
	INVOKE{
		int d = 0;
		arg>>d;
		pProgram->MoveCP(d);
		return 0;
	}
	ENDCMD(MoveCP);

	BEGINCMD(Set);//设定一个配置参数
	INVOKE{
		return pProgram->SetConfigVar(arg,log);
	}
	ENDCMD(Set);

	BEGINCMD(Toggle);//切换一个布尔型配置参数的值
	INVOKE{
		return pProgram->ToggleBoolConfigVar(arg,log);
	}
	ENDCMD(Toggle);

	BEGINCMD(NextInteriorDef);//选取下一个InteriorDef
	INVOKE{
		GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->Next();
		return 0;
	}
	ENDCMD(NextInteriorDef);

	BEGINCMD(PrevInteriorDef);//选取前一个InteriorDef
	INVOKE{
		GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->Prev();
		return 0;
	}
	ENDCMD(PrevInteriorDef);

	BEGINCMD(SetInteriorDefAxis);//设定InteriorDef的方向,一个参数是方向(0:x,1:y,2:z)
	INVOKE{
		unsigned int iAxis = 2;
		arg>>iAxis;
		GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent()->SetAxis(iAxis%3);
		return 0;
	}
	ENDCMD(SetInteriorDefAxis);

	BEGINCMD(CycleInteriorDefAxis);//使用下一个InteriorDef的方向
	INVOKE{
		GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent()->SetAxis(
			(GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent()->GetAxis()+1)%3
			);
		return 0;
	}
	ENDCMD(CycleInteriorDefAxis);

	BEGINCMD(IncInteriorDefBoardNum);//在当前InteriorDef中增加板子,一个参数是数量(负值为减少)
	INVOKE{
		int delta = 1;
		arg>>delta;
		GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent()->SetBoardNum(
			std::max(GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent()->GetBoardNum()+delta,0)
			);
		return 0;
	}
	ENDCMD(IncInteriorDefBoardNum);

	
	BEGINCMD(IncInteriorDefBoardThickness);//增加当前InteriorDef中板子的厚度,一个参数是增加量(负值为减少)
	INVOKE{
		int delta = 0;
		arg>>delta;
		CSceneNodeInteriorDef *pInterior = GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent();
		pInterior->SetBoardThickness(
			std::max(GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent()->GetBoardThickness()+delta,0)
			);
		pInterior->RepositionSubNodes();
		return 0;
	}
	ENDCMD(IncInteriorDefBoardThickness);

	
	BEGINCMD(ClearInterior);//清理当前InteriorDefs
	INVOKE{
		CSceneNodeInteriorDef *pInterior = GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent();
		pInterior->ClearSubNodes();
		pInterior->SetBoardNum(pInterior->GetSubNodeNum());
		return 0;
	}
	ENDCMD(ClearInterior);


	BEGINCMD(RemoveInteriorNthBoard);//删除掉当前InteriorDef中的第n个板,一个参数为n
	INVOKE{
		CSceneNodeInteriorDef *pInterior = GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent();
		int n = 1;
		arg>>n;
		pInterior->RemoveNthSubNode(--n);
		return 0;
	}
	ENDCMD(RemoveInteriorNthBoard);


	BEGINCMD(SetInteriorOffsetBegin);//设置当前InteriorDef中第一个板的偏移量,一个参数是偏移量
	INVOKE{
		CSceneNodeInteriorDef *pInterior = GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent();
		int d = 0;
		arg>>d;
		pInterior->SetOffsetBegin(d);
		pInterior->RepositionSubNodes();
		return 0;
	}
	ENDCMD(SetInteriorOffsetBegin);

	
	BEGINCMD(SetInteriorOffsetEnd);//设置当前InteriorDef中最后一个板的偏移量,一个参数是偏移量
	INVOKE{
		CSceneNodeInteriorDef *pInterior = GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent();
		int d = 0;
		arg>>d;
		pInterior->SetOffsetEnd(d);
		pInterior->RepositionSubNodes();
		return 0;
	}
	ENDCMD(SetInteriorOffsetEnd);

	
	BEGINCMD(Pick);//使用当前的鼠标位置(即最后一次SetCursorHS输入的鼠标位置)对场景中的第n组物体进行拾取,一个参数是n
	INVOKE{
		int group(0);
		arg>>group;
		pProgram->Pick(group);
		return 0;
	}
	ENDCMD(Pick);


	BEGINCMD(DivideCell);//在axis方向上分裂当前选定的cell(衣柜中的不再有隔板的一格),一个参数是axis(0:x,1:y,2:z)
	INVOKE{
		int axis(0);
		arg>>axis;
		pProgram->SplitWardrobeCell(axis);
		return 0;
	}
	ENDCMD(DivideCell);


	BEGINCMD(AlignPickedWithPointedAt);
	//将当前已选定的IBoxy物体(如柜体)与当前鼠标位置(最后一次SetCursorHS输入的鼠标位置)指向的IBoxy物体在+x和+z方向上对齐
	//一个参数是取哪一组物体
	INVOKE{
		int group(0);
		arg>>group;
		pProgram->AlignPickedWithPointedAt(group,(1<<3) | (1<<5));
		return 0;
	}
	ENDCMD(AlignPickedWithPointedAt);

	
	BEGINCMD(ConnectPickedToPointedAt);
	//将当前已选定的IConnectible物体(如柜体)与当前鼠标位置(最后一次SetCursorHS输入的鼠标位置)指向的IConnectible物体连接
	//一个参数是取哪一组物体
	INVOKE{
		int group(0);
		arg>>group;
		pProgram->ConnectPickedToPointedAt(group);
		return 0;
	}
	ENDCMD(ConnectPickedToPointedAt);


	BEGINCMD(DisconnectPickedFromPointedAt);
	//将当前已选定的IConnectible物体(如柜体)与当前鼠标位置(最后一次SetCursorHS输入的鼠标位置)指向的IConnectible物体断开连接
	//一个参数是取哪一组物体
	INVOKE{
		int group(0);
		arg>>group;
		pProgram->DisconnectPickedFromPointedAt(group);
		return 0;
	}
	ENDCMD(DisconnectPickedFromPointedAt);
	//BEGINCMD(SplitCell);
	//INVOKE{
	//	return 0;
	//}
	//ENDCMD(SplitCell);


	BEGINCMD(FormatPickedBoardToDXF);
	//如果当前选中的物体是一个CBoard的话就输出dxf,一个参数是pdf的路径
	INVOKE{
		std::wstring path;
		arg>>path;
		pProgram->FormatPickedToDXF(path);
		return 0;
	}
	ENDCMD(FormatPickedBoardToDXF);


	BEGINCMD(FormatBoardList);
	//输出当前所有的隔板的列表
	INVOKE{
		std::wstring path;
		arg>>path;
		pProgram->OutputBoardList(std::wofstream(path.c_str()));
		return 0;
	}
	ENDCMD(FormatBoardList);

	BEGINCMD(SetCameraOrtho);//将当前摄像机重置为正交视图,一个参数(0到4)表示是五个正交视图（前后左右俯）中的哪一个
	INVOKE{
		int i=0;
		arg>>i;
		pProgram->SetCameraOrtho(i);
		return 0;
	}
	ENDCMD(SetCameraOrtho);

	BEGINCMD(CreateComponent);
	//创建一个GenericComponent(领带盒,裤架之类)并插入主程序的待处理队列.
	//一个参数是GenericComponent的名字(可以用"厂家A的领带盒.领带盒1"这样的路径),不是文件名是xml中结点的路径
	INVOKE{
		std::wstring path;
		arg>>path;
		pProgram->PushNewBorn(new CSceneNodeBoxyGenericComponent(pProgram->GetGenericComponentScheme().get_child(path)));
		return 0;
	}
	ENDCMD(CreateComponent);

	BEGINCMD(CreateComponentVariant);
	//创建一个GenericComponent(领带盒,裤架之类)的变种并插入主程序的待处理队列.
	//两个参数分别是GenericComponent的路径（不是文件路径是xml中结点的路径）和变种的路径
	INVOKE{
		std::wstring scheme,variant;
		unsigned flags;
		arg>>scheme>>variant>>flags;
		pProgram->PushNewBorn( pProgram->CreateGenericComponentVariant(scheme,variant,flags) );
		return 0;
	}
	ENDCMD(CreateComponentVariant);

	BEGINCMD(SetRoomMaterial);
	//设置房间的材质，两个参数，第一个是"Floor","Walls","Ceiling"指定是改哪一部分，第二个是材质名
	INVOKE{
		std::wstring part,mtl;
		arg>>part>>mtl;
		IMaterial *pMtl(pProgram->GetMaterial(mtl));
		if(!pProgram->GetNumRegisteredOrderedObjects<CSceneNodeRoom>()) return -2;
		CSceneNodeRoom &room(pProgram->GetOrderedObject<CSceneNodeRoom>(0));
		if(part==L"Floor"){
			room.SetFloorMaterial(pMtl);
		}else if(part==L"Walls"){
			room.SetWallMaterial(pMtl);
		}else if(part==L"Ceiling"){
			room.SetCeilingMaterial(pMtl);
		}else{
			return -1;
		}
		return 0;
	}
	ENDCMD(SetRoomMaterial);

	BEGINCMD(SetPickedMaterial);
	//设置当前选定的物体(如果有IMaterialUser接口)的材质，一个参数是材质名
	INVOKE{
		std::wstring mtl;
		arg>>mtl;
		IMaterial *pMtl(pProgram->GetMaterial(mtl));
		IMaterialUser *pUser(pProgram->GetPickedAs<IMaterialUser>());
		if(!pUser){
			return -1;
		}
		pUser->SetMaterial(pMtl);
		return 0;
	}
	ENDCMD(SetPickedMaterial);

	BEGINCMD(ToggleBoardRemovable);
	//锁定或解锁当前选定的板的位置
	INVOKE{
		CSceneNodeBoard *pBoard(pProgram->GetPickedAs<CSceneNodeBoard>());
		if(pBoard){
			pBoard->SetBoardRemovable(!pBoard->GetBoardRemovable());
		}
		return 0;
	}
	ENDCMD(ToggleBoardRemovable);

	BEGINCMD(QueryManufactureProperties);
	INVOKE{
		std::wstring path;
		arg>>path;
		const boost::property_tree::wptree &wpt( pProgram->GetManufactureProperties().get_child(path) );
		for(boost::property_tree::wptree::const_iterator i(wpt.begin());i!=wpt.end();++i){
			log<<i->first<<std::endl;
		}
		return 0;
	}
	ENDCMD(QueryManufactureProperties);

	BEGINCMD(QueryGenerators);
	INVOKE{
		std::wstring path;
		arg>>path;
		const boost::property_tree::wptree &wpt( pProgram->GetGenerators().get_child(path) );
		for(boost::property_tree::wptree::const_iterator i(wpt.begin());i!=wpt.end();++i){
			log<<i->first<<std::endl;
		}
		return 0;
	}
	ENDCMD(QueryGenerators);

	BEGINCMD(QueryMaterials);
	INVOKE{
		pProgram->ListMaterials(log);
		return 0;
	}
	ENDCMD(QueryMaterials);

	BEGINCMD(QueryComponents);
	INVOKE{
		std::wstring path;
		arg>>path;
		const boost::property_tree::wptree &wpt( pProgram->GetGenericComponentScheme().get_child(path) );
		for(boost::property_tree::wptree::const_iterator i(wpt.begin());i!=wpt.end();++i){
			log<<i->first;
		}
		return 0;
	}
	ENDCMD(QueryComponents);

	
	BEGINCMD(QueryVariants);
	INVOKE{
		std::wstring cmp;
		arg>>cmp;
		pProgram->ListVariants(cmp,log);
		return 0;
	}
	ENDCMD(QueryVariants);
}
