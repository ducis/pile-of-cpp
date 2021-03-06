
#include <Ogre.h>
#include <RenderSystems/Direct3D9/OgreD3D9Plugin.h>
#include <OgrePanelOverlayElement.h>
#include <RenderSystems/Direct3D9/OgreD3D9Texture.h>
#include <RenderSystems/Direct3D9/OgreD3D9HardwarePixelBuffer.h>
#include <opencv2/opencv.hpp>

//#include <OGRE/RenderSystems/Direct3D9/OgreD3D9RenderSystem.h>

#include <memory>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <functional>
#include <cassert>
#include <windows.h>
#include <ctime>
#include <utility>
#include <boost/qvm/all.hpp>
#include <conio.h>
#include <windows.h>
#include <fstream>


struct PlanarConfig{
	float x,y,r;
	PlanarConfig(float _x,float _y,float _r):x(_x),y(_y),r(_r){}
	PlanarConfig(){}
};
PlanarConfig operator+(const PlanarConfig &l, const PlanarConfig &r){
	return PlanarConfig(l.x+r.x,l.y+r.y,l.r+r.r);
}


struct PlanarConfigToCameraConfig: std::unary_function< const PlanarConfig &, std::pair<Ogre::Vector3,Ogre::Quaternion> >
{	
	float m_pitch;
	float m_height;
	PlanarConfigToCameraConfig(float h,float p):m_pitch(p),m_height(h){}
	void SetPitch(float p){ m_pitch = p; }
	void SetHeight(float h){ m_height = h; }
	result_type operator()(argument_type pc) const
	{
		Ogre::Matrix3 m;
		m.FromEulerAnglesYXZ( Ogre::Radian(pc.r), Ogre::Radian(m_pitch), Ogre::Radian(0) );
		return make_pair
			(
				Ogre::Vector3( pc.x, m_height, pc.y ),
				Ogre::Quaternion( m )
			);
	}
};

template< typename _StateVector, typename _StateVectorToCameraConfigurationTransform >
void SetOgreCamera( Ogre::Camera *pCam, const _StateVector &s, const _StateVectorToCameraConfigurationTransform &f){
	std::pair<Ogre::Vector3,Ogre::Quaternion> cc = f(s);
	pCam->setPosition(cc.first);
	pCam->setOrientation(cc.second);
};

//double Eval( Ogre::RenderTexture(

int main()
//int WINAPI WinMain(          HINSTANCE hInstance,
//    HINSTANCE hPrevInstance,
//    LPSTR lpCmdLine,
//    int nCmdShow
//)
{
	std::auto_ptr<Ogre::Root> pOgreRoot(new Ogre::Root("plugins.cfg"));
	pOgreRoot->installPlugin(new Ogre::D3D9Plugin());
	const Ogre::RenderSystemList &list = pOgreRoot->getAvailableRenderers();
	std::transform(list.begin(),list.end(),std::ostream_iterator<Ogre::String>(std::ofstream("renderers.txt")),std::mem_fun(&Ogre::RenderSystem::getName));
	if(!pOgreRoot->showConfigDialog()){
		return 0;
	}
	Ogre::RenderWindow *pWindow(pOgreRoot->initialise(true));
	assert(pWindow->isActive());
	assert(pWindow->isPrimary());
	//Ogre::WindowEventUtilities::addWindowEventListener(pWindow,
	Ogre::SceneManager *pSceneManager(pOgreRoot->createSceneManager(Ogre::ST_GENERIC));
	Ogre::Camera *pCam = pSceneManager->createCamera("zzz");
	pCam->setNearClipDistance(10);
	pCam->setFarClipDistance(20000);
	//pCam->setPolygonMode(Ogre::PM_WIREFRAME);
	//Ogre::Viewport *pViewport = pWindow->addViewport(pCam,);
	//pViewport->setBackgroundColour(Ogre::ColourValue(0,0,1));
	//pCam->setAspectRatio(Ogre::Real(pViewport->getActualWidth())/Ogre::Real(pViewport->getActualHeight()));
	pCam->setAspectRatio(Ogre::Real(4.0/3.0));
	float fov = 0.67334964;
	std::ifstream("fov.txt")>>fov;
	pCam->setFOVy (Ogre::Radian(fov));

	Ogre::ResourceGroupManager& lRgMgr = Ogre::ResourceGroupManager::getSingleton();
	pOgreRoot->addResourceLocation("","FileSystem");
	lRgMgr.initialiseAllResourceGroups();
	std::string floorMeshName = "floor.mesh";
	std::ifstream("load.txt")>>floorMeshName;
 	Ogre::Entity *pFloor = pSceneManager->createEntity("zzz",floorMeshName);
	Ogre::SceneNode *pNode = pSceneManager->getRootSceneNode()->createChildSceneNode();
	pNode->attachObject(pFloor);
	Ogre::TextureManager *pTextureManager = pOgreRoot->getTextureManager();
	Ogre::TexturePtr pTex = pTextureManager->createManual("FirstRTT","General",Ogre::TEX_TYPE_2D,512,512,1,Ogre::MIP_UNLIMITED,Ogre::  /*PF_A8R8G8B8*/  PF_FLOAT32_RGBA,      Ogre::TU_AUTOMIPMAP|Ogre::TU_RENDERTARGET  /*|Ogre::TU_DYNAMIC*/   /*|Ogre::TU_STATIC*/  );
	Ogre::RenderTexture *pRTT = pTex->getBuffer()->getRenderTarget();
	pOgreRoot->getRenderSystem()->attachRenderTarget(*pRTT);
	
	pRTT->addViewport(pCam)->setBackgroundColour(Ogre::ColourValue(0,0,0,0));

	Ogre::PanelOverlayElement *pPanelShow = new Ogre::PanelOverlayElement("fff");
	pPanelShow->setMetricsMode(Ogre::GMM_PIXELS);
	pPanelShow->setDimensions(512,512);
	pPanelShow->setPosition(0,0);
	assert(pPanelShow->isVisible());
	//assert(pRTT->getViewport(0)->getOverlaysEnabled());
	pPanelShow->setTiling(1,1);
	pPanelShow->setTransparent(false);
	pPanelShow->setUV(0,0,1,1);
	//pPanelShow->setMaterialName("01-Default");
	pRTT->getViewport(0)->setOverlaysEnabled(false);

	Ogre::Overlay *pOverlay = Ogre::OverlayManager::getSingleton().create("sss");
	//pOverlay->add2D(pPanelShow);
	pOverlay->show();
	assert(pOverlay->isVisible());

	pWindow->addViewport(pOgreRoot->createSceneManager(Ogre::ST_GENERIC)->createCamera("jjj"));
	
	Ogre::MaterialPtr pPanelMat = Ogre::MaterialManager::getSingleton().create("PanelMaterial","General");
	pPanelMat->setAmbient(Ogre::ColourValue(1,1,1,1));
	pPanelMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setTextureName("FirstRTT");
	pPanelMat->getTechnique(0)->getPass(0)->setTextureFiltering(Ogre::TFO_TRILINEAR);
	pPanelShow->setMaterialName("PanelMaterial");
	//Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","ff"

	//Ogre::TexturePtr pCapturedTex = pTextureManager->load("zzzzzz.bmp","General",Ogre::TEX_TYPE_2D,Ogre::MIP_UNLIMITED);
	Ogre::TexturePtr pCapturedTex = pTextureManager->createManual("CamIn","General",Ogre::TEX_TYPE_2D,512,512,1,1,Ogre::PF_R8G8B8,Ogre::TU_DYNAMIC_WRITE_ONLY);
	Ogre::HardwarePixelBufferSharedPtr pCapTexBuf = pCapturedTex->getBuffer();
	Ogre::PanelOverlayElement *pCapturedTexShow = new Ogre::PanelOverlayElement("rrr");
	pOverlay->add2D(pCapturedTexShow);
	Ogre::MaterialPtr pCapturedMat = Ogre::MaterialManager::getSingleton().getByName("CapturedMaterial");//.create("CapturedMaterial","General");
	pCapturedMat->setAmbient(Ogre::ColourValue(1,1,1,1));
	pCapturedMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setTextureName("CamIn");
	pCapturedMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setTextureName("FirstRTT");
	//pCapturedMat->getTechnique(0)->getPass(0)->setFragmentProgram("Average2");
	pCapturedTexShow->setMaterialName("CapturedMaterial");
	pCapturedTexShow->setPosition(512,0);
	pCapturedTexShow->setDimensions(512,512);
	pCapturedTexShow->setMetricsMode(Ogre::GMM_PIXELS);
	pCapturedTexShow->setTiling(1,1);
	pCapturedTexShow->setTransparent(false);
	pCapturedTexShow->setUV(0,0,1,1);

	Ogre::TexturePtr pCorrelationResultTex = pTextureManager->createManual("CorrelationRTT","General",Ogre::TEX_TYPE_2D,512,512,1,Ogre::MIP_UNLIMITED,Ogre::  /*PF_A8R8G8B8*/  PF_FLOAT32_RGBA,      Ogre::TU_AUTOMIPMAP|Ogre::TU_RENDERTARGET  /*|Ogre::TU_DYNAMIC*/   /*|Ogre::TU_STATIC*/  );
	Ogre::RenderTexture *pCorrRTT = pCorrelationResultTex->getBuffer()->getRenderTarget();
	pOgreRoot->getRenderSystem()->attachRenderTarget(*pCorrRTT);
	{
		Ogre::Viewport *pViewport = pCorrRTT->addViewport( pOgreRoot->createSceneManager(Ogre::ST_GENERIC)->createCamera("ggg") );
		Ogre::CompositorManager::getSingleton().addCompositor(pViewport, "Correlate");
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(pViewport, "Correlate", true);
		pViewport->setOverlaysEnabled(false);
	}
	{
		Ogre::MaterialPtr p = Ogre::MaterialManager::getSingleton().getByName("CorrelateMtl");
		p->getTechnique(0)->getPass(0)->createTextureUnitState();
		p->getTechnique(0)->getPass(0)->createTextureUnitState();
		p->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName("FirstRTT");
		p->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName("CamIn");
	}
	{
		Ogre::PanelOverlayElement *pCorrTexShow = new Ogre::PanelOverlayElement("nnn");
		pOverlay->add2D(pCorrTexShow);
		Ogre::MaterialPtr pCorrMat = Ogre::MaterialManager::getSingleton().create("CorrResultMaterial","General");
		pCorrMat->setAmbient(Ogre::ColourValue(1,1,1,1));
		pCorrMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setTextureName("CorrelationRTT");
		pCorrTexShow->setMaterialName("CorrResultMaterial");
		pCorrTexShow->setPosition(0,512);
		pCorrTexShow->setDimensions(128,128);
		pCorrTexShow->setMetricsMode(Ogre::GMM_PIXELS);
		pCorrTexShow->setTiling(1,1);
		pCorrTexShow->setTransparent(false);
		pCorrTexShow->setUV(0,0,1,1);
	}
	//pOgreRoot->startRendering();


	//for(int i=0;i<8;++i)		pOgreRoot->getRenderSystem()->_setTextureUnitFiltering(i,Ogre::FO_LINEAR,Ogre::FO_LINEAR,Ogre::FO_LINEAR);
	bool go = true;

	//Ogre::CompositorManager::getSingleton().addCompositor(pViewport,"Correlate");
	Ogre::TexturePtr pLastTex = pTextureManager->createManual("LastRTT","General",Ogre::TEX_TYPE_2D,32,32,1,1,Ogre::PF_FLOAT32_RGBA, Ogre::TU_RENDERTARGET);
	Ogre::HardwarePixelBufferSharedPtr pLastRTTSurface = pLastTex->getBuffer();
	Ogre::RenderTexture *pLastRTT = pLastRTTSurface->getRenderTarget();
	//{
	//	Ogre::Camera *c = pOgreRoot->createSceneManager(Ogre::ST_GENERIC)->createCamera("1");
	//	//float x,y,w,h;
	//	//std::cin>>x>>y>>w>>h;
	//	Ogre::Viewport *p = pLastRTT->addViewport( c,0,0,0,1.0/32.0,1.0/32.0 );
	//	Ogre::CompositorManager::getSingleton().addCompositor(p, "TexFill");
	//	Ogre::CompositorManager::getSingleton().setCompositorEnabled(p, "TexFill", true);
	//	p->setOverlaysEnabled(false);
	//}
	//Ogre::Viewport *pLastRTTViewports[27];
	//Ogre::Camera *pLastRTTBlankCam = pOgreRoot->createSceneManager(Ogre::ST_GENERIC)->createCamera("1");
	for (int i=0;i<27;++i){
		char name[2] = {'0'+i/10,'0'+i%10};
		Ogre::Viewport *p = pLastRTT->addViewport( pOgreRoot->createSceneManager(Ogre::ST_GENERIC)->createCamera(name),i,i/32.0,0,1.0/32.0,1.0/32.0 );
		Ogre::CompositorManager::getSingleton().addCompositor(p, "TexFill");
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(p, "TexFill", true);
		p->setOverlaysEnabled(false);
		//pLastRTTViewports[i] = p;
	}
	{
		Ogre::MaterialPtr p = Ogre::MaterialManager::getSingleton().getByName("TexFillMtl");
		p->getTechnique(0)->getPass(0)->createTextureUnitState()->setTextureName("CorrelationRTT");
		p->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureFiltering(Ogre::TFO_TRILINEAR);
	}
	cv::VideoCapture videoCap;
	{
		Ogre::Quaternion q = pCam->getOrientation();
		std::cout<<q.getYaw()<<' '<<q.getPitch()<<' '<<q.getRoll();
	}
	{
		//int i;
		//std::cin>>i;
		//videoCap.open(i);
		int i=0;
		std::ifstream("cam.txt")>>i;
		videoCap.open(i);
	}
	assert(videoCap.isOpened());
	cv::Mat rawVideoFrame;
	cv::Mat squareVideoFrame;

	PlanarConfigToCameraConfig pc2cc(200,-0.4);
	PlanarConfig currentState(300,300,0.785);	
	float dl = 5;
	float da = 0.02;
	std::ifstream("conf.txt")>>pc2cc.m_height>>pc2cc.m_pitch>>currentState.x>>currentState.y>>currentState.r>>dl>>da;
	float dx[3] = {-dl,0,dl}, dy[3] = {-dl,0,dl}, dr[3] = {-da,0,da};
	PlanarConfig deltas[27];
	for(int ix = 0;ix<3;++ix){
		for(int iy = 0;iy<3;++iy){
			for(int ir = 0;ir<3;++ir){
				deltas[(ix*3+iy)*3+ir ] = PlanarConfig(dx[ix],dy[iy],dr[ir]);
			}
		}
	}

	bool track = false;
	while(go){
		videoCap>>rawVideoFrame;
		cv::resize(rawVideoFrame,squareVideoFrame,cv::Size(512,512));
		assert(squareVideoFrame.depth()==CV_8U);
		assert(squareVideoFrame.channels()==3);
		assert(squareVideoFrame.isContinuous());
		pCapTexBuf->blitFromMemory(Ogre::PixelBox(512,512,1,Ogre::PF_R8G8B8,squareVideoFrame.ptr()));
		Ogre::WindowEventUtilities::messagePump();
		//{
		//	SetOgreCamera(pCam,currentState,pc2cc);
		//}
		//pRTT->update();
		//pCorrRTT->update();
		//pLastRTT->update();
		for(int i=0;i<27;++i){
			SetOgreCamera(pCam,currentState+deltas[i],pc2cc);
			pRTT->update();
			pCorrRTT->update();
			//pLastRTTViewports[i]->update();
			//pLastRTT->update();
			pLastRTT->_beginUpdate();
			pLastRTT->_updateViewport(i);
			pLastRTT->_endUpdate();
		}
		
		{
			//Ogre::D3D9Texture *pD3DTex = dynamic_cast<Ogre::D3D9Texture*>(pTex.get());
			//assert(pD3DTex);
			//IDirect3DBaseTexture9 *p = pD3DTex->getTexture();
			//assert(p);
			//int c = p->GetLevelCount();
			//p->GenerateMipSubLevels();
			//Sleep(100);
			pWindow->update();
		}
		
		//Ogre::D3D9HardwarePixelBuffer *pD3Dpb = dynamic_cast<Ogre::D3D9HardwarePixelBuffer*>(pPixelBuffer.get());
		//assert(pD3Dpb);
		//pD3Dpb->_genMipmaps(pD3DTex->getTexture());

		//pTex->getBuffer(0,1)->blit(pPixelBuffer);
		//pTex->getBuffer(0,pTex->getNumMipmaps()-1)->blit(pPixelBuffer);
		//for( unsigned int i=1; i<pTex->getNumMipmaps(); ++i ){
		//	pTex->getBuffer(0,i)->blit(pTex->getBuffer(0,i-1));
		//}
		//std::cout<<"|.";
		float f[32][4];
		//pLastRTTSurface->readData( 0, 0, f );
		pLastRTTSurface->blitToMemory(Ogre::Box(0,0,32,1),Ogre::PixelBox(32,1,1,Ogre::PF_FLOAT32_RGBA,&f[0][0]));
		//std::cout<<f[0][0]/f[0][3]<<' '<<f[0][1]/f[0][3]<<' '<<f[0][2]/f[0][3]<<'\n';
		//std::cout<<f[0][0]<<' '<<f[0][1]<<' '<<f[0][2]<<' '<<f[0][3]<<'\n';
		if(track){
			int iNext = (1*3+1)*3+1;
			for(int i=0;i<27;++i){
				if( (f[i][0]+f[i][1]+f[i][2])/f[i][3] > (f[iNext][0]+f[iNext][1]+f[iNext][2])/f[iNext][3] ){
					iNext = i;
				}
			}
			currentState=currentState+deltas[iNext];
		}
		if(_kbhit()){
			_getch();
			track = !track;
		//	std::cout<<"saving...\n";
		//	//pRTT->writeContentsToFile("xcvx.png");
		//	Ogre::Image img;
		//	pTex->convertToImage(img,false);
		//	img.save("zzzz.png");
		//	std::cout<<"saved.\n";
			//float x=100,y=100,z=100;
			//std::cin>>x>>y>>z;
			//pCam->setPosition(Ogre::Vector3(x,y,z));
			//pCam->lookAt(Ogre::Vector3(0,0,0));
			//go = false;
		}
	}
	return 0;
}
