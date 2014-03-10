#pragma once

#include "IBoxy.h"
#include <vector>
#include <utility>

class IDrillingRule;

class CBoard{//储存板材信息的类,只保存板材本身的信息,不保存板材在柜体中的位置和方向
public:
	struct Hole{//板面上的孔(不包括板边缘的孔和板边缘的孔的附加孔)
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
	void ClearHoles();//清除所有的孔
	void PunchZHole(const vector2 &pos, const Hole &hole, int iSide);//在板面上加一个孔
	void PunchXYHoles();//根据绑定的打孔规则在板边缘上加孔
	unsigned GetXYHoleCount(unsigned dir) const;//dir是方向,只能取0(-x),1(-y),3(+x),4(+y)
	float GetXYHolePos(unsigned dir,unsigned id) const;
	class IHoleStyle * GetXYHole(unsigned dir, unsigned id) const;
	void ApplyDrillingRule(class IDrillingRule *p, unsigned axis, unsigned positive);//设置打孔规则
	//void CheckDrillingRules();
	void ClearCuts();//清除切除线
	void Cut(const vector2 rectMin, const vector2 rectMax);//在板面上切去一块(比如遇到墙角时)
	const std::wstring &GetName() const{  return m_name; }
	void SetName(const std::wstring &s){  m_name = s; }//设置板的名字
	const std::wstring &GetComments() const;//获取板的说明文字
	void SetEnabled(bool enabled){	m_bEnabled = enabled; }
	bool IsEnabled() const { return m_bEnabled; }
	void SetCapping(bool y, bool positive, unsigned thickness);//设置边缘封条的厚度,y是是否在y轴方向上(false x,true y),positive是是否正向(false - , true + )
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
