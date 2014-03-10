#pragma once

#include "IBoxy.h"
#include <vector>
#include <utility>

class IDrillingRule;

class CBoard{//��������Ϣ����,ֻ�����ı������Ϣ,���������ڹ����е�λ�úͷ���
public:
	struct Hole{//�����ϵĿ�(���������Ե�Ŀ׺Ͱ��Ե�Ŀ׵ĸ��ӿ�)
		float diameter;
		float depth;
		Hole(float _diameter,float _depth):diameter(_diameter),depth(_depth){}
		bool operator<(const Hole &rhs) const{
			return diameter<rhs.diameter||(diameter==rhs.diameter&&depth<rhs.depth);
		}
		bool operator==(const Hole &rhs) const{
			return diameter==rhs.diameter && depth==rhs.depth;
		}
	};
	const vector2 GetSize() const;
	void SetSize(const vector2 &size);
	float GetThickness() const;
	void SetThickness(float thickness);

	class IMaterial *GetMaterial() const;
	void SetMaterial(class IMaterial *p);
	template< class _OutputStream > void WriteToDXF( _OutputStream &os ) const;

	CBoard(const vector2 &size, float thickness, IMaterial *pMaterial, bool flip);
	~CBoard();
	void ClearHoles();//������еĿ�
	void PunchZHole(const vector2 &pos, const Hole &hole, int iSide);//�ڰ����ϼ�һ����
	void PunchXYHoles();//���ݰ󶨵Ĵ�׹����ڰ��Ե�ϼӿ�
	unsigned GetXYHoleCount(unsigned dir) const;//dir�Ƿ���,ֻ��ȡ0(-x),1(-y),3(+x),4(+y)
	float GetXYHolePos(unsigned dir,unsigned id) const;
	class IHoleStyle * GetXYHole(unsigned dir, unsigned id) const;
	void ApplyDrillingRule(class IDrillingRule *p, unsigned axis, unsigned positive);//���ô�׹���
	//void CheckDrillingRules();
	void ClearCuts();//����г���
	void Cut(const vector2 rectMin, const vector2 rectMax);//�ڰ�������ȥһ��(��������ǽ��ʱ)
	const std::wstring &GetName() const{  return m_name; }
	void SetName(const std::wstring &s){  m_name = s; }//���ð������
	const std::wstring &GetComments() const;//��ȡ���˵������
	void SetEnabled(bool enabled){	m_bEnabled = enabled; }
	bool IsEnabled() const { return m_bEnabled; }
	void SetCapping(bool y, bool positive, unsigned thickness);//���ñ�Ե�����ĺ��,y���Ƿ���y�᷽����(false x,true y),positive���Ƿ�����(false - , true + )
	unsigned GetCapping(bool y, bool positive);
private:
	unsigned m_capping[2][2];
	bool m_bEnabled;
	mutable std::wstring m_comments;
	//class IDrillingRule *m_pDrillingRules[2][2];
	std::vector<std::pair<float,IHoleStyle *> > m_holeStyles[2][2];
	bool m_flip;
	typedef std::pair<vector2,Hole> hole_pos_2;
	typedef std::pair<float,Hole> hole_pos_1;
	std::vector<hole_pos_2> m_holesZ[2];
	std::vector<hole_pos_1> m_holesXY[2][2];
	vector2 m_size;
	float m_thickness;
	class IMaterial *m_pMaterial;
	const vector2 LowerLeft() const { return -0.5*m_size; }
	const vector2 LowerRight() const { return vector2(0.5*m_size.x,-0.5*m_size.y); }
	const vector2 UpperLeft() const { return vector2(-0.5*m_size.x,0.5*m_size.y); }
	const vector2 UpperRight() const { return 0.5*m_size; }
	typedef std::pair<vector2,vector2> line_seg;
	std::vector<line_seg> m_cuts;
	void Cut(const line_seg &seg);
	std::wstring m_name;
};
