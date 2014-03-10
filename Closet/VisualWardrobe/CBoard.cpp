#include "CBoard.impl.h"
#include "IMaterial.h"
#include <cassert>
#include <iosfwd>
#include <fstream>
#include <vector>
#include <iterator>
#include "IDrillingRule.h"
#include <algorithm>
#include "IHoleStyle.h"
#include "singleton.h"
#include "CProgram.h"
#include <sstream>

const vector2 CBoard::GetSize() const{
	return vector2( m_size.x-m_capping[0][0]-m_capping[0][1], m_size.y-m_capping[1][0]-m_capping[1][1] );
}


void CBoard::SetSize(const vector2 &size){
	assert(size.x>0);
	assert(size.y>0);
	m_size.x = floor(size.x+0.5f);
	m_size.y = floor(size.y+0.5f);
}


float CBoard::GetThickness() const{
	return m_thickness;
}


void CBoard::SetThickness(float thickness){
	assert(thickness>0.5f);
	m_thickness = floor(thickness+0.5f);
}


IMaterial *CBoard::GetMaterial() const{
	return m_pMaterial;
}


void CBoard::SetMaterial(IMaterial *p){
	m_pMaterial = p;
}


CBoard::CBoard(const vector2 &size, float thickness, IMaterial *pMaterial, bool flip)
:m_size(size),m_thickness(thickness),m_pMaterial(pMaterial),m_flip(flip),m_bEnabled(true)
{
	m_capping[0][0] = 0;
	m_capping[0][1] = 0;
	m_capping[1][0] = 0;
	m_capping[1][1] = 0;
	GetSingleton<CProgram>()->RegisterBoard(*this);
}

CBoard::~CBoard()
{
	GetSingleton<CProgram>()->UnregisterBoard(*this);
}

template void CBoard::WriteToDXF<std::wostream>(std::wostream &os) const;


void CBoard::ClearHoles(){
	for(int i(0); i<2; ++i){
		m_holesZ[i].clear();
		for(int j(0); j<2; ++j){
			m_holesXY[j][i].clear();
		}
	}
}


void CBoard::PunchZHole(const vector2 &pos, const Hole &hole, int iSide){
	m_holesZ[iSide].push_back(std::make_pair(pos,hole));
}



unsigned CBoard::GetXYHoleCount(unsigned dir) const{
	assert(dir<6);
	assert(dir%3<2);
	return m_holeStyles[dir%3][dir/3].size();
}



float CBoard::GetXYHolePos(unsigned dir,unsigned id) const{
	return m_holeStyles[dir%3][dir/3][id].first;
}



IHoleStyle *CBoard::GetXYHole(unsigned int dir, unsigned int id) const{
	return m_holeStyles[dir%3][dir/3][id].second;
}


void CBoard::PunchXYHoles(){
	for(int axis(0); axis<2; ++axis){
		for(int positive(0); positive<2; ++positive){
			for(std::vector<std::pair<float,IHoleStyle *> >::iterator i( m_holeStyles[axis][positive].begin() ); 
				i!=m_holeStyles[axis][positive].end();
				++i
				)
			{
				m_holesXY[axis][positive].push_back(std::make_pair( i->first,Hole(i->second->GetDiameter(),i->second->GetDepth()) ) );
				std::pair<float,IHoleStyle*> sec(i->second->GetSecondaryHole());
				if(sec.second){
					vector2 pos(0,0);
					pos[axis] = (m_size[axis]*0.5-sec.first) * (positive?1:-1);
					pos[(axis+1)%2] = i->first;
					m_holesZ[m_flip?1:0].push_back(std::make_pair(pos , Hole(sec.second->GetDiameter(),sec.second->GetDepth()) ));
				}
			}
		}
	}
}


void CBoard::ApplyDrillingRule(class IDrillingRule *p, unsigned axis, unsigned positive){
	assert(axis<2);
	assert(positive<2);
	m_holeStyles[axis][positive].clear();
	if(!p) return;
	p->Generate(m_size[(axis+1)%2]);
	int n( p->GetHoleCount() );
	m_holeStyles[axis][positive].reserve(n);
	for(int i(0); i<n; ++i){
		m_holeStyles[axis][positive].push_back(std::make_pair(p->GetHolePos(i),p->GetHoleStyle(i)));
	}
}



void CBoard::ClearCuts(){
	m_cuts.clear();
}



void CBoard::Cut(const vector2 rectMin, const vector2 rectMax){
	Cut(line_seg(vector2(rectMin.x,rectMin.y),vector2(rectMax.x,rectMin.y)));
	Cut(line_seg(vector2(rectMin.x,rectMin.y),vector2(rectMin.x,rectMax.y)));
	Cut(line_seg(vector2(rectMax.x,rectMax.y),vector2(rectMax.x,rectMin.y)));
	Cut(line_seg(vector2(rectMax.x,rectMax.y),vector2(rectMin.x,rectMax.y)));
}


void CBoard::Cut(const line_seg &seg){
	if(Intersect(MakeRect(seg.first,seg.second),std::make_pair(-0.5*m_size,0.5*m_size))){
		line_seg s(seg);
		for(int iAxis(0); iAxis<2; ++iAxis){
			MyUtilities::Clamp(s.first[iAxis],-0.5f*m_size[iAxis],0.5f*m_size[iAxis]);
			MyUtilities::Clamp(s.second[iAxis],-0.5f*m_size[iAxis],0.5f*m_size[iAxis]);
		}
		m_cuts.push_back(s);
	}
}



void CBoard::SetCapping(bool y, bool positive, unsigned thickness){
	m_capping[y][positive] = thickness;
}



unsigned CBoard::GetCapping(bool y, bool positive){
	return m_capping[y][positive];
}



const std::wstring &CBoard::GetComments() const{  
	std::wostringstream buf;
	bool anything(false);
	for(unsigned iAxis(0);iAxis<2;++iAxis){
		wchar_t wchAxis(iAxis?L'³¤':L'¿í');
		if(m_capping[iAxis][0] == m_capping[iAxis][1]){
			if(m_capping[iAxis][0]){
				if(anything) buf<<L'£¬'; anything = true;
				buf<<L'Á½'<<wchAxis<<L'·â'<<m_capping[iAxis][0]<<L".0";
			}
		}else{
			for(unsigned positive(0); positive<2; ++positive){
				if(m_capping[iAxis][positive]){
					if(anything) buf<<L'£¬'; anything = true;
					buf<<L'Ò»'<<wchAxis<<L'·â'<<m_capping[iAxis][0]<<L".0";
				}
			}
		}
	}
	if(anything){
		buf<<L'¡£';
	}
	m_comments = buf.str();
	return m_comments; 
}
