#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/SceneData/Scene/hkxScene.h>
//#include "d3dx
class GActor{
private:
	void GeneralUpdate_ProcessNode(hkxNode *node);
	void GeneralUpdate();
	void GeneralDraw_ProcessNode(hkxNode *node);
	void GeneralDraw();
public:
	hkxNode* m_rootNode;
	D3DXVECTOR3 position;
	D3DXQUATERNION orientation;
	GActor();
	void (GActor::*UpdateWorld)();
	void (GActor::*UpdateScene)();
};
