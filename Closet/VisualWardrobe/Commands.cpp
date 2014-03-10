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


// BEGINCMD..ENDCMD�������һ��ָ��õ���CNamedCommand�࣬
//SIMPLECALL�����һ���޲���ָ�������ֱ�ӵ���CProgram�е�ͬ������


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

	BEGINCMD(ReturnThirteen);//����13,��������
	INVOKE{
		return 13;
	}
	ENDCMD(ReturnThirteen);
	

	BEGINCMD(WireCube);//�ڵ�ǰ�����в���һ���߿�������,��������
	INVOKE{
		pProgram->GetScene()->Insert( new CSceneNodeGLUTWireCube );
		return 0;
	}
	ENDCMD(WireCube);


	BEGINCMD(SolidCube);//�ڵ�ǰ�����в���һ����Ƭ������,��������
	INVOKE{
		pProgram->GetScene()->Insert( new CSceneNodeGLUTSolidCube );
		return 0;
	}
	ENDCMD(SolidCube);


	BEGINCMD(SetBackgroundColor);//���ñ�����ɫ,����������,�ֱ�����ɫ��r,g,b����,ȡֵ��Χ��Ϊ[0.0,1.0]
	INVOKE{
		float r = 1.0f,g = 1.0f,b = 1.0f;
		arg>>r>>g>>b;
		glClearColor(r,g,b,1.0f);
		return 0;
	}
	ENDCMD(SetBackgroundColor);

	
	BEGINCMD(ReturnError);//������������� ��������
	INVOKE{
		int e;
		arg>>e;
		return e;
	}
	ENDCMD(ReturnError);


	BEGINCMD(Camera);//��ʹ��
	INVOKE{
		return 0;
	}
	ENDCMD(Camera);

	
	BEGINCMD(LookAt);
	//�趨�Ӿ���. �Ÿ�����,�������ӵ�,Ŀ�����Ϸ����x,y,z����,Ĭ��ֵ��(0,0,10000),(0,0,0),(0,1,0)
	//ע�����������ֱ�ӵ���OpenGL�ĺ���,���ᱣ��״̬,��CTargetCamera������
	//���Զ��ж��Ƿ���������ͼ
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


	BEGINCMD(TopView);//����ͼ,����(x,y,10000)��(x,y,0)��.����Ϊx��y.���ᱣ��״̬,��CTargetCamera������
	INVOKE{
		float x = 0, y = 0;
		arg>>x>>y;
		wostringstream wss;
		wss<<x<<L' '<<y<<L' '<<10000<<L' '<<x<<L' '<<y<<L" 0 0 1 0";
		return pProgram->AcceptCommand(L"LookAt",wss.str(),log);
	}
	ENDCMD(TopView);

	BEGINCMD(FrontView);//����ͼ,����(x,-30000,z)��(x,0,z)��.����Ϊx��z.���ᱣ��״̬,��CTargetCamera������
	INVOKE{
		float x = 0, z = 0;
		arg>>x>>z;
		wostringstream wss;
		wss<<x<<L' '<<-30000<<L' '<<z<<L' '<<x<<L' '<<0<<L' '<<z<<L" 0 0 1";
		return pProgram->AcceptCommand(L"LookAt",wss.str(),log);
	}
	ENDCMD(FrontView);


	BEGINCMD(LeftView);//����ͼ,����(30000,y,z)��(0,y,z)��.����Ϊy��z.���ᱣ��״̬,��CTargetCamera������
	INVOKE{
		float y = 0, z = 0;
		arg>>y>>z;
		wostringstream wss;
		wss<<30000<<L' '<<y<<L' '<<z<<L' '<<0<<L' '<<y<<L' '<<z<<L" 0 0 1";
		return pProgram->AcceptCommand(L"LookAt",wss.str(),log);
	}
	ENDCMD(LeftView);



	BEGINCMD(Ortho);//�趨ͶӰ����Ϊ����ͶӰ����,�ĸ������ֱ�Ϊ��,��,��ƽ��,Զƽ��
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


	BEGINCMD(Perspective);//�趨ͶӰ����Ϊ͸��ͶӰ����,���������ֱ�Ϊ�ӽ�(��λΪ��,y����),��ƽ��,Զƽ��
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


	BEGINCMD(Zoom);//����ͶӰ����,ֻ������ͶӰ��Ч,һ������Ϊ���ŵı���(1.0Ϊ����)
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

	BEGINCMD(LightsOn);//ȫ�����õƹ�
	INVOKE{
		glEnable(GL_LIGHTING);
		return 0;
	}
	ENDCMD(LightsOn);

	BEGINCMD(LightsOff);//ȫ�ֽ��õƹ�
	INVOKE{
		glDisable(GL_LIGHTING);
		return 0;
	}
	ENDCMD(LightsOff);

	BEGINCMD(LightDir);//����һ�������,�ĸ���������һ�������ǵƹ��(0��7),�������ǵƹⷽ������
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

	BEGINCMD(KillAllLights);//�������еƹ�
	INVOKE{
		for(int i=0;i<8;++i){
			glDisable(GL_LIGHT0+i);
		}
		return 0;
	}
	ENDCMD(KillAllLights);

	BEGINCMD(CoordSys);//�ڳ���������һ������ϵ����ʾ����һ����������������ʾ�ĳ���
	INVOKE{
		float sz = 1000;
		arg>>sz;
		pProgram->GetScene()->Insert( new CSceneNodeCoordSys(sz) );
		return 0;
	}
	ENDCMD(CoordSys);

	BEGINCMD(LoadFile);//��ȡһ��xml�ļ��������еĳ������ѹ���������еĴ�������У���һ���������ļ�·��
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
	//��ָ����Generator��ManufactureProperty����һ���µ�(�ǹ����)�����ڵ�ѹ��������Ĵ�������У�
	//�����������ֱ���Generator��ManufactureProperty������(�����ļ���,���ļ�����Ӧxml��������)
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
	//��������Ĵ���������е���һ���ڵ㲢���뵽��ǰ������
	INVOKE{
		pProgram->GetScene()->Insert(GetSingleton<CProgram>()->PopNewBorn());
		return 0;
	}
	ENDCMD(DeployNewBorn);

	
	BEGINCMD(DeployNewBornAsComponent);
	//��������Ĵ���������е���һ���ڵ�,��װΪһ��CSceneNodeComponent,�ٲ��뵽��ǰ������
	INVOKE{
		pProgram->GetScene()->Insert( new CSceneNodeComponent(pProgram->PopNewBorn(),arg) );
		return 0;
	}
	ENDCMD(DeployNewBornAsComponent);

	BEGINCMD(CreateWardrobe);
	//��ָ����Generator��ManufactureProperty����һ���µĹ���ѹ��������Ĵ�������У�
	//�����������ֱ���Generator��ManufactureProperty������(�����ļ���,���ļ�����Ӧxml��������)
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
	//��������Ĵ���������е���һ���ڵ�,��֤�Ƿ���һ��ObjectSet�ڵ㣬
	//����ǵĻ��ͽ�CSceneNodeObjectSet���ӽڵ�ȡ�����ŵ�һ���µ�BoxyDraggableContainer��,�ٽ��½ڵ���뵽��ǰ������
	//5������ ��һ������z�����ת,�ڶ��������ĸ���λ��,������ǲ������Ƿ񴴽�Լ��
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

	BEGINCMD(DeployNewBornIntoInterior);//��ʹ��
	INVOKE{
		CSceneNodeInteriorDef *pInterior = GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent();
		pInterior->AddSubNode( pProgram->PopNewBorn() );
		pInterior->SetBoardNum(pInterior->GetSubNodeNum());
		pInterior->RepositionSubNodes();
		return 0;
	}
	ENDCMD(DeployNewBornIntoInterior);

	BEGINCMD(PlaceRecentlyDeployed);//�趨�������Ľڵ��λ��,����z��Ƕ�,������
	INVOKE{
		int x = 0, y = 0, z = 0, r = 0;
		arg>>r>>x>>y>>z;
		pProgram->BakeBatch(x,y,z,r);
		return 0;
	}
	ENDCMD(PlaceRecentlyDeployed);

	BEGINCMD(SetCursorHS);//�趨������οռ�����,���������ֱ�Ϊx,y
	INVOKE{
		float x = 0.0f,y = 0.0f;
		arg>>x>>y;
		pProgram->SetCursorHS(x,y);
		return 0;
	}
	ENDCMD(SetCursorHS);

	BEGINCMD(PickCPHS);//ʹ���������οռ������ControlPlane����ʰȡ,���������ֱ�Ϊx,y.�������������x,y��Ϊ-100,�൱��ȡ��ѡȡ
	INVOKE{
		float x = -100.0f,y = -100.0f;
		arg>>x>>y;
		log<<pProgram->PickCP(x,y)<<endl;
		return 0;
	}
	ENDCMD(PickCPHS);

	BEGINCMD(MoveCP);//�ƶ�ControlPlane,һ���������ƶ���
	INVOKE{
		int d = 0;
		arg>>d;
		pProgram->MoveCP(d);
		return 0;
	}
	ENDCMD(MoveCP);

	BEGINCMD(Set);//�趨һ�����ò���
	INVOKE{
		return pProgram->SetConfigVar(arg,log);
	}
	ENDCMD(Set);

	BEGINCMD(Toggle);//�л�һ�����������ò�����ֵ
	INVOKE{
		return pProgram->ToggleBoolConfigVar(arg,log);
	}
	ENDCMD(Toggle);

	BEGINCMD(NextInteriorDef);//ѡȡ��һ��InteriorDef
	INVOKE{
		GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->Next();
		return 0;
	}
	ENDCMD(NextInteriorDef);

	BEGINCMD(PrevInteriorDef);//ѡȡǰһ��InteriorDef
	INVOKE{
		GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->Prev();
		return 0;
	}
	ENDCMD(PrevInteriorDef);

	BEGINCMD(SetInteriorDefAxis);//�趨InteriorDef�ķ���,һ�������Ƿ���(0:x,1:y,2:z)
	INVOKE{
		unsigned int iAxis = 2;
		arg>>iAxis;
		GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent()->SetAxis(iAxis%3);
		return 0;
	}
	ENDCMD(SetInteriorDefAxis);

	BEGINCMD(CycleInteriorDefAxis);//ʹ����һ��InteriorDef�ķ���
	INVOKE{
		GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent()->SetAxis(
			(GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent()->GetAxis()+1)%3
			);
		return 0;
	}
	ENDCMD(CycleInteriorDefAxis);

	BEGINCMD(IncInteriorDefBoardNum);//�ڵ�ǰInteriorDef�����Ӱ���,һ������������(��ֵΪ����)
	INVOKE{
		int delta = 1;
		arg>>delta;
		GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent()->SetBoardNum(
			std::max(GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent()->GetBoardNum()+delta,0)
			);
		return 0;
	}
	ENDCMD(IncInteriorDefBoardNum);

	
	BEGINCMD(IncInteriorDefBoardThickness);//���ӵ�ǰInteriorDef�а��ӵĺ��,һ��������������(��ֵΪ����)
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

	
	BEGINCMD(ClearInterior);//����ǰInteriorDefs
	INVOKE{
		CSceneNodeInteriorDef *pInterior = GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent();
		pInterior->ClearSubNodes();
		pInterior->SetBoardNum(pInterior->GetSubNodeNum());
		return 0;
	}
	ENDCMD(ClearInterior);


	BEGINCMD(RemoveInteriorNthBoard);//ɾ������ǰInteriorDef�еĵ�n����,һ������Ϊn
	INVOKE{
		CSceneNodeInteriorDef *pInterior = GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent();
		int n = 1;
		arg>>n;
		pInterior->RemoveNthSubNode(--n);
		return 0;
	}
	ENDCMD(RemoveInteriorNthBoard);


	BEGINCMD(SetInteriorOffsetBegin);//���õ�ǰInteriorDef�е�һ�����ƫ����,һ��������ƫ����
	INVOKE{
		CSceneNodeInteriorDef *pInterior = GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent();
		int d = 0;
		arg>>d;
		pInterior->SetOffsetBegin(d);
		pInterior->RepositionSubNodes();
		return 0;
	}
	ENDCMD(SetInteriorOffsetBegin);

	
	BEGINCMD(SetInteriorOffsetEnd);//���õ�ǰInteriorDef�����һ�����ƫ����,һ��������ƫ����
	INVOKE{
		CSceneNodeInteriorDef *pInterior = GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->GetCurrent();
		int d = 0;
		arg>>d;
		pInterior->SetOffsetEnd(d);
		pInterior->RepositionSubNodes();
		return 0;
	}
	ENDCMD(SetInteriorOffsetEnd);

	
	BEGINCMD(Pick);//ʹ�õ�ǰ�����λ��(�����һ��SetCursorHS��������λ��)�Գ����еĵ�n���������ʰȡ,һ��������n
	INVOKE{
		int group(0);
		arg>>group;
		pProgram->Pick(group);
		return 0;
	}
	ENDCMD(Pick);


	BEGINCMD(DivideCell);//��axis�����Ϸ��ѵ�ǰѡ����cell(�¹��еĲ����и����һ��),һ��������axis(0:x,1:y,2:z)
	INVOKE{
		int axis(0);
		arg>>axis;
		pProgram->SplitWardrobeCell(axis);
		return 0;
	}
	ENDCMD(DivideCell);


	BEGINCMD(AlignPickedWithPointedAt);
	//����ǰ��ѡ����IBoxy����(�����)�뵱ǰ���λ��(���һ��SetCursorHS��������λ��)ָ���IBoxy������+x��+z�����϶���
	//һ��������ȡ��һ������
	INVOKE{
		int group(0);
		arg>>group;
		pProgram->AlignPickedWithPointedAt(group,(1<<3) | (1<<5));
		return 0;
	}
	ENDCMD(AlignPickedWithPointedAt);

	
	BEGINCMD(ConnectPickedToPointedAt);
	//����ǰ��ѡ����IConnectible����(�����)�뵱ǰ���λ��(���һ��SetCursorHS��������λ��)ָ���IConnectible��������
	//һ��������ȡ��һ������
	INVOKE{
		int group(0);
		arg>>group;
		pProgram->ConnectPickedToPointedAt(group);
		return 0;
	}
	ENDCMD(ConnectPickedToPointedAt);


	BEGINCMD(DisconnectPickedFromPointedAt);
	//����ǰ��ѡ����IConnectible����(�����)�뵱ǰ���λ��(���һ��SetCursorHS��������λ��)ָ���IConnectible����Ͽ�����
	//һ��������ȡ��һ������
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
	//�����ǰѡ�е�������һ��CBoard�Ļ������dxf,һ��������pdf��·��
	INVOKE{
		std::wstring path;
		arg>>path;
		pProgram->FormatPickedToDXF(path);
		return 0;
	}
	ENDCMD(FormatPickedBoardToDXF);


	BEGINCMD(FormatBoardList);
	//�����ǰ���еĸ�����б�
	INVOKE{
		std::wstring path;
		arg>>path;
		pProgram->OutputBoardList(std::wofstream(path.c_str()));
		return 0;
	}
	ENDCMD(FormatBoardList);

	BEGINCMD(SetCameraOrtho);//����ǰ���������Ϊ������ͼ,һ������(0��4)��ʾ�����������ͼ��ǰ�����Ҹ����е���һ��
	INVOKE{
		int i=0;
		arg>>i;
		pProgram->SetCameraOrtho(i);
		return 0;
	}
	ENDCMD(SetCameraOrtho);

	BEGINCMD(CreateComponent);
	//����һ��GenericComponent(�����,���֮��)������������Ĵ��������.
	//һ��������GenericComponent������(������"����A�������.�����1"������·��),�����ļ�����xml�н���·��
	INVOKE{
		std::wstring path;
		arg>>path;
		pProgram->PushNewBorn(new CSceneNodeBoxyGenericComponent(pProgram->GetGenericComponentScheme().get_child(path)));
		return 0;
	}
	ENDCMD(CreateComponent);

	BEGINCMD(CreateComponentVariant);
	//����һ��GenericComponent(�����,���֮��)�ı��ֲ�����������Ĵ��������.
	//���������ֱ���GenericComponent��·���������ļ�·����xml�н���·�����ͱ��ֵ�·��
	INVOKE{
		std::wstring scheme,variant;
		unsigned flags;
		arg>>scheme>>variant>>flags;
		pProgram->PushNewBorn( pProgram->CreateGenericComponentVariant(scheme,variant,flags) );
		return 0;
	}
	ENDCMD(CreateComponentVariant);

	BEGINCMD(SetRoomMaterial);
	//���÷���Ĳ��ʣ�������������һ����"Floor","Walls","Ceiling"ָ���Ǹ���һ���֣��ڶ����ǲ�����
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
	//���õ�ǰѡ��������(�����IMaterialUser�ӿ�)�Ĳ��ʣ�һ�������ǲ�����
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
	//�����������ǰѡ���İ��λ��
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
