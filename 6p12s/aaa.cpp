// ZXC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "glut.h"
#include <windows.h>
#include "3Dmouse.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>
//#pragma comment(lib, "3dMouse.lib")
#define SetZero(_)
#define	GetMotionData(_)
typedef GLfloat F;
template<int N> struct Vec{
	F s[N];
};
template<int N> F dot(const Vec<N> &l,const Vec<N> &r){
	F ret=0;
	for(int i=0;i<N;++i){
		ret+=l.s[i]*r.s[i];
	}
	return ret;
}
#define DEF_LINEAR_BIN_OP(OP) \
	template<int N> Vec<N> operator OP (const Vec<N> &l, const Vec<N> &r){ \
		Vec<N> ret; \
		for(int i=0;i<N;++i){ \
			ret.s[i]=l.s[i] OP r.s[i]; \
		} \
		return ret;	\
	}
DEF_LINEAR_BIN_OP(+)
DEF_LINEAR_BIN_OP(-)

typedef Vec<4> V4;

struct M4{
	V4 rows[4];
};
const M4 zeroM4={
	{
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	}
};
const M4 idM4={
	{
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
	}
};
V4 operator*(const M4 &m, const V4 &v){
	V4 r;
	for(int i=0;i<4;++i){
		r.s[i]=dot(m.rows[i],v);
	}
	return r;
}
M4 operator*(const M4 &l,const M4 &r){
	M4 o;
	for(int i=0;i<4;++i){
		for(int j=0;j<4;++j){
			o.rows[i].s[j]=0;
			for(int k=0;k<4;++k){
				o.rows[i].s[j]+=l.rows[i].s[k]*r.rows[k].s[j];
			}
		}
	}
	return o;
}
M4 transpose(const M4 &m){
	M4 r;
	for(int i=0;i<4;++i){
		for(int j=0;j<4;++j){
			r.rows[i].s[j]=m.rows[j].s[i];
		}
	}
	return r;
}
M4 rMat(int axis,F angle){
	M4 m = zeroM4;
	const int z = axis;
	const int x = (z+1)%3;
	const int y = (x+1)%3;
	F s=sin(angle);
	F c=cos(angle);
	m.rows[3].s[3]=m.rows[z].s[z]=1;
	m.rows[x].s[x]=c;
	m.rows[x].s[y]=s;
	m.rows[y].s[x]=-s;
	m.rows[y].s[y]=c;
	return m;
}
M4 sMat(F x, F y, F z){
	M4 m = {{
		{x,0,0,0},
		{0,y,0,0},
		{0,0,z,0},
		{0,0,0,1}}
	};
	return m;
}

M4 tMat(F x,F y,F z){
	M4 m = idM4;
	m.rows[0].s[3]=x;
	m.rows[1].s[3]=y;
	m.rows[2].s[3]=z;
	return m;
}

void Init()
{
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void Reshape(int width, int Height)
{
	float Ration_W_H ;
	Height = (Height == 0 ? 1: Height);
	Ration_W_H = 1.0 * width / Height;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, Ration_W_H,0.1,1000);

	glViewport(0,0,width,Height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

M4 mmn=idM4;
M4 mmf=idM4;
M4 mmpn[6];
M4 mmpf[6];

void MultM4(const M4 &m){
	M4 m_ = transpose(m);
	glMultMatrixf((const GLfloat*)&m_);
}
void Print(const V4 &v){
	for(int i=0;i<4;++i){
		printf("%f\t",v.s[i]);
	}
	printf("\n");
}
void Print(const M4 &m){
	for(int i=0;i<4;++i){
		Print(m.rows[i]);
	}
}
void RenderScene()
{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 20 , 20, 0, 5, 0 , 0,1,0);
	glColor4f(1,1,1,1);
	
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,100,0);
	glEnd();

	glCullFace(GL_FRONT);
	glPushMatrix();
	MultM4(mmn);
	glScalef(1,0.3,1);
	glutSolidTeapot(5);
	glPopMatrix();
	
	glPushMatrix();
	MultM4(mmf);
	glScalef(1,0.3,1);
	glutSolidTeapot(3);
	glPopMatrix();

	
	for(int i=0;i<6;++i){
		glPushMatrix();
		MultM4(mmpn[i]);
		//glutSolidTeapot(0.2);
		glCullFace(GL_BACK);
		glutSolidCone(0.2,6,10,20);
		glPopMatrix();
		glPushMatrix();
		MultM4(mmpf[i]);//glMultMatrixf((const GLfloat*)&transpose(mmpf[i]));
		//glCullFace(GL_FRONT);
		glutSolidCone(0.2,6,10,20);
		//glutSolidTeapot(0.2);
		glPopMatrix();
	}
	glutSwapBuffers();
	printf("a|>");
}
MOTION_DATA_STRUCT fake_mds={0,0,0,0,0,0};
void NormalKeyFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'z':
			printf("zero\n");
			SetZero();
			fake_mds.Xp=fake_mds.Yp=fake_mds.Zp=fake_mds.Arfa=fake_mds.Beita=fake_mds.Gama=0;
			break;
		case 27:
			exit(0);
			break;
		case '1':
			fake_mds.Xp+=0.03;
			break;
		case '2':
			fake_mds.Yp+=0.03;
			break;
		case '3':
			fake_mds.Zp+=0.03;
			break;
		case '4':
			fake_mds.Arfa+=0.01;
			break;
		case '5':
			fake_mds.Beita+=0.01;
			break;
		case '6':
			fake_mds.Gama+=0.01;
			break;
		case 'q':
			fake_mds.Xp-=0.03;
			break;
		case 'w':
			fake_mds.Yp-=0.03;
			break;
		case 'e':
			fake_mds.Zp-=0.03;
			break;
		case 'r':
			fake_mds.Arfa-=0.01;
			break;
		case 't':
			fake_mds.Beita-=0.01;
			break;
		case 'y':
			fake_mds.Gama-=0.01;
			break;
		default:
			break;

	}
	glutPostRedisplay();
}

void SpecialKeyFunc(int key, int x, int y)
{
	printf("%d\n",key);
/*	switch(key)
	{
	default:
		break;
	}*/
	glutPostRedisplay();
}

F length3(const V4 &v){
	F r=0;
	for(int i=0;i<3;++i){
		r+=v.s[i]*v.s[i];
	}
	return sqrt(r);
}
V4 normalize3(const V4 &v){
	F scl = 1.0/length3(v);
	V4 r;
	for(int i=0;i<3;++i){
		r.s[i]=v.s[i]*scl;
	}
	r.s[3]=0;
	return r;
}
V4 zeroV4={0,0,0,0};
V4 notPar3(const V4 &v){
	int j=0;
	for(int i=1;i<3;++i){
		if(abs(v.s[i])<abs(v.s[j])) j=i;
	}
	V4 r=zeroV4;
	r.s[j]=1;
	return r;
}
V4 cross3(const V4 &l,const V4 &r){
	V4 o = zeroV4;
	for(int i=0;i<3;++i){
		int j=(i+1)%3;
		int k=(j+1)%3;
		o.s[i] = l.s[j]*r.s[k]-l.s[k]*r.s[j];
	}
	return o;
}

#define PI 3.1415926535897932384626433832795
void TimerFun(int value)
{
	glutTimerFunc(50,TimerFun,1);
	glutPostRedisplay();
	
	MOTION_DATA_STRUCT mds=fake_mds;//{0,0,0,0,0,0};
	//GetMotionData(&mds);
	//const GLfloat cap=1000.0f;
	//if(abs(mds.Xp)>cap||abs(mds.Yp)>cap||abs(mds.Zp)>cap||abs(mds.Arfa)>cap||abs(mds.Beita)>cap||abs(mds.Gama)>cap) return;
	//printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",mds.Xp,mds.Yp,mds.Zp,mds.Arfa,mds.Beita,mds.Gama);

	//mmn = tMat(0,-10,0)*rMat(0,0.5);
	const float h=10.0;
	mmf = tMat(mds.Xp,h+mds.Zp,-mds.Yp)*rMat(1,mds.Gama)*rMat(0,mds.Arfa)*rMat(2,-mds.Beita);
	mmn = idM4;
	const int n=6;
	const double rn=5.0;
	const double rf=3.0;
	for(int i=0;i<n;++i){
		const double a = i/(float)n*2*PI;
		const double c = cos(a), s = sin(a);
//		V4 pnM={c*rn,s*rn,0,1};
//		V4 pfM={c*rf,s*rf,0,1};
//		V4 pf=mmf*pnM;
		M4 mnT = mmn*tMat(c*rn,0,s*rn);
		M4 mfT = mmf*tMat(c*rf,0,s*rf);
		V4 org3V4 = {0,0,0,1};
		V4 pn = mnT*org3V4;
		V4 pf = mfT*org3V4;
		Print(pn);
		Print(pf);
		V4 front = normalize3(pf-pn);
		V4 up = normalize3(cross3(front,notPar3(front)));
		V4 right = normalize3(cross3(front,up));
		M4 t;
		t.rows[0] = up;
		t.rows[1] = right;
		t.rows[2] = front;
		t.rows[3] = org3V4;
		mmpn[i] = tMat(pn.s[0],pn.s[1],pn.s[2])*transpose(t);
		t.rows[0] = right;
		t.rows[1] = up;
		t.rows[2] = zeroV4-front;
		t.rows[3] = org3V4;
		mmpf[i] = tMat(pf.s[0],pf.s[1],pf.s[2])*transpose(t);
	}

	Print(mmn);
}

int main(void)
{
	assert(sizeof(M4)==sizeof(GLfloat)*16);
	assert(sizeof(V4)==sizeof(GLfloat)*4);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA );
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1280,720);
	glutCreateWindow("ZZXZXCZXCZXCX"); 

	Init();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(RenderScene);

	glutKeyboardFunc(NormalKeyFunc);

	glutSpecialFunc(SpecialKeyFunc);

	glutTimerFunc(50,TimerFun,1);
	
	// 开启深度检测功能；
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);  
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DITHER);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);

	glutMainLoop();
	return 0;
}

