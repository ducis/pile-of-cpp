#pragma once

#include "CBoard.h"
#include "DxfFormat.h"
#include "DxfBasics.h"
#include "stdext.h"
#include "MyUtilities.h"
#include "singleton.h"
#include "CProgram.h"
#include "CDxfHeader.h"
#include <sstream>

template< class _OutputStream > 
void CBoard::WriteToDXF( _OutputStream &os ) const{
	using namespace DxfFormat;

	os<<Label(L"SECTION")<<SecName(L"HEADER");
	os<<Label(L"ENDSEC");


	vector2 center(0,0);
	const float spacing(300.0f);
	vector2 mainViewCenter( center.x-0.5*m_size.x,center.y+0.5*m_size.y );
	vector2 rightViewCenter( center.x+spacing+0.5*m_thickness, mainViewCenter.y );
	vector2 topViewCenter( mainViewCenter.x, center.y-spacing-0.5*m_thickness );

	os<<Label(L"SECTION")<<SecName(L"TABLES");
	{
		CDxfHeader *p(GetSingleton<CProgram>()->GetCurrentDxfHeaderTablesSec());
		if(p) p->WriteToDXF(os);
	}
	vector2 vpMin( -m_size.x, -spacing-m_thickness ), vpMax( spacing+m_thickness, m_size.y );
	os<<Viewport<vector2>( vpMin*2.0f+center, vpMax*2.0f+center );
	os<<Label(L"ENDSEC");

	typedef std::pair<DimensionBlocks<vector2,float>,DimensionEntity<vector2> > dim_type;

	NameIterator niD(L'D') , niU(L'U');

	dim_type dim0(
		MakeDimension( Layer(L"标注"), *niD, *niU, vector2(center.x-m_size.x,center.y+m_size.y), vector2(center.x,center.y+m_size.y) )
		);
	dim_type dim1(
		MakeDimension( Layer(L"标注"), *(++niD), *(++niU), vector2(center.x,center.y+m_size.y), vector2(center.x,center.y))
		);
	dim_type dim2(
		MakeDimension( Layer(L"标注"), *(++niD), *(++niU), vector2(center.x+spacing,center.y+m_size.y), vector2(center.x+spacing+m_thickness,center.y+m_size.y) )
		);
	std::vector<dim_type> holeDims;
	if( (!m_holesZ[0].empty()) || (!m_holesZ[1].empty()) ){
		std::vector<vector2> holePos;

		vector2 lr(LowerRight());
		holePos.assign(1,vector2(lr));
		for(int positive(0); positive<2; ++positive){
			std::transform( m_holesZ[positive].begin(), m_holesZ[positive].end(), std::back_inserter(holePos), stdext::get1st<hole_pos_2> );
		}
		std::sort(holePos.begin(),holePos.end(),MyUtilities::OperateOnYs<vector2>(std::greater<float>()));
		std::stable_sort(holePos.begin(),holePos.end(),MyUtilities::OperateOnXes<vector2>(std::greater<float>()));
		holePos.erase( std::unique(holePos.begin(),holePos.end(),MyUtilities::OperateOnXes<vector2>(std::equal_to<float>())), holePos.end() );

		std::transform(holePos.begin(),holePos.end(),holePos.begin(),std::bind2nd(std::plus<vector2>(),mainViewCenter));
		holePos.push_back(LowerLeft()+mainViewCenter);
		for(std::vector<vector2>::iterator i(holePos.begin()+1); i!=holePos.end(); ++i){
			holeDims.push_back(MakeDimension( Layer(L"标注"), *(++niD), *(++niU), *(i-1), *i, vector2(-1,0),holePos.front(),holePos.front()));
		}

		vector2 ll(LowerLeft());
		holePos.assign(1,vector2(ll));
		for(int positive(0); positive<2; ++positive){
			std::transform( m_holesZ[positive].begin(), m_holesZ[positive].end(), std::back_inserter(holePos), stdext::get1st<hole_pos_2> );
		}
		std::sort(holePos.begin(),holePos.end(),MyUtilities::OperateOnXes<vector2>(std::greater<float>()));
		std::stable_sort(holePos.begin(),holePos.end(),MyUtilities::OperateOnYs<vector2>(std::less<float>()));
		holePos.erase( std::unique(holePos.begin(),holePos.end(),MyUtilities::OperateOnYs<vector2>(std::equal_to<float>())), holePos.end() );

		std::transform(holePos.begin(),holePos.end(),holePos.begin(),std::bind2nd(std::plus<vector2>(),mainViewCenter));
		holePos.push_back(UpperLeft()+mainViewCenter);
		for(std::vector<vector2>::iterator i(holePos.begin()+1); i!=holePos.end(); ++i){
			holeDims.push_back(MakeDimension( Layer(L"标注"), *(++niD), *(++niU), *(i-1), *i, vector2(0,1),holePos.front(),holePos.front()));
		}
	}
	
	for( unsigned axis(0); axis<2; ++axis ) 	if( (!m_holesXY[axis][0].empty()) || (!m_holesXY[axis][1].empty()) )
	{
		std::vector<float> holePos;
		unsigned theOtherAxis((axis+1)%2);
		holePos.assign(1,-0.5*m_size[theOtherAxis]);
		for(int positive(0); positive<2;++positive){
			std::transform( m_holesXY[axis][positive].begin() , m_holesXY[axis][positive].end() , std::back_inserter(holePos) , stdext::get1st<hole_pos_1> );
		}
		std::sort(holePos.begin(),holePos.end());
		holePos.erase( std::unique(holePos.begin(),holePos.end()), holePos.end() );
		holePos.push_back(0.5*m_size[theOtherAxis]);
		vector2 t(0,0);
		t[theOtherAxis] = 1;
		vector2 b( axis?topViewCenter:rightViewCenter );
		vector2 def( b );
		for(std::vector<float>::iterator i(holePos.begin()+1); i!=holePos.end(); ++i){
			vector2 prev(b), cur(b);
			prev[theOtherAxis]+=*(i-1);
			cur[theOtherAxis]+=*i;
			holeDims.push_back(MakeDimension( Layer(L"标注"), *(++niD), *(++niU), prev, cur, t, def, def));
		}
	}


	os<<Label(L"SECTION")<<SecName(L"BLOCKS");
	os<<dim0.first;
	os<<dim1.first;
	os<<dim2.first;
	for(std::vector<dim_type>::iterator i( holeDims.begin() ); i!=holeDims.end(); ++i){
		os<<i->first;
	}
	os<<Label(L"ENDSEC");
	os<<Label(L"SECTION")<<SecName(L"ENTITIES");
	PutRect( os,L"实线", center.x-m_size.x,		center.y+0,						center.x+0,						center.y+m_size.y		);
	PutRect( os,L"实线", center.x+spacing,		center.y+0,						center.x+spacing+m_thickness,	center.y+m_size.y		);
	PutRect( os,L"实线", center.x-m_size.x,		center.y-spacing-m_thickness,	center.x+0,						center.y-spacing		);
	os<<dim0.second;
	os<<dim1.second;
	os<<dim2.second;
	for(std::vector<dim_type>::iterator i( holeDims.begin() ); i!=holeDims.end(); ++i){
		os<<i->second;
	}
	Layer layers[2] = { Layer(L"虚线"), Layer(L"实线") };
	
	std::vector<Hole> holesSeq;
	for(int positive(0); positive<2; ++positive){
		std::transform( m_holesZ[positive].begin(), m_holesZ[positive].end(), std::back_inserter(holesSeq), stdext::get2nd<hole_pos_2>);
		for(int axis(0); axis<2; ++axis){
			std::transform( m_holesXY[axis][positive].begin(), m_holesXY[axis][positive].end(), std::back_inserter(holesSeq), stdext::get2nd<hole_pos_1> );
		}
	}
	std::sort(holesSeq.begin(),holesSeq.end());
	holesSeq.erase(std::unique(holesSeq.begin(),holesSeq.end()),holesSeq.end());

	vector2 textOffsetScale(1,1);
	for(int positive(1); positive>=0; --positive){
		for(std::vector<hole_pos_2>::const_iterator i( m_holesZ[positive].begin() ); i!=m_holesZ[positive].end(); ++i){
			vector2 pos(mainViewCenter+i->first);
			os<<CenterMarkedCircle<vector2>( layers[positive], Layer(L"中心线"), pos, 0.5*i->second.diameter );
			os<<MakeText(Layer(L"标注"),std::find(holesSeq.begin(),holesSeq.end(),i->second)-holesSeq.begin(),pos+textOffsetScale*0.5*i->second.diameter,12,0,positive?0:2,positive?0:3);
		}
		for(std::vector<hole_pos_1>::const_iterator i( m_holesXY[0][positive].begin() ); i!=m_holesXY[0][positive].end(); ++i){
			vector2 pos(rightViewCenter+vector2(0,i->first));
			os<<CenterMarkedCircle<vector2>( layers[positive], Layer(L"中心线"), pos, 0.5*i->second.diameter );
			os<<MakeText(Layer(L"标注"),std::find(holesSeq.begin(),holesSeq.end(),i->second)-holesSeq.begin(),pos+textOffsetScale*0.5*i->second.diameter,12,0,positive?0:2,positive?0:3);
		}
		for(std::vector<hole_pos_1>::const_iterator i( m_holesXY[1][positive].begin() ); i!=m_holesXY[1][positive].end(); ++i){
			vector2 pos(topViewCenter+vector2(i->first,0));
			os<<CenterMarkedCircle<vector2>( layers[positive], Layer(L"中心线"), pos, 0.5*i->second.diameter );
			os<<MakeText(Layer(L"标注"),std::find(holesSeq.begin(),holesSeq.end(),i->second)-holesSeq.begin(),pos+textOffsetScale*0.5*i->second.diameter,12,0,positive?0:2,positive?0:3);
		}
		textOffsetScale = -textOffsetScale;
	}

	{
		vector2 p0(100,-124),p1(180,-124),p2(260,-124);
		os<<MakeText(Layer(L"文字"),(wchar_t*)(L"Hole"),p0,18)<<MakeText(Layer(L"文字"),(wchar_t*)(L"DIA"),p1,18)<<MakeText(Layer(L"文字"),(wchar_t*)(L"DEP"),p2,18);
		for(std::vector<Hole>::const_iterator i(holesSeq.begin()); i!=holesSeq.end(); ++i){
			p0.y-=24;
			p1.y-=24;
			p2.y-=24;
			os<<MakeText(Layer(L"文字"),i-holesSeq.begin(),p0,18)<<MakeText(Layer(L"文字"),i->diameter,p1,18)<<MakeText(Layer(L"文字"),i->depth,p2,18);
		}
	}
	//os<<Circle<vector2,float>(Layer(L"实线"),center,100.0f);

	for(std::vector<line_seg>::const_iterator i(m_cuts.begin()); i!=m_cuts.end(); ++i){
		os<<Line<vector2>(Layer(L"切割线"),i->first+mainViewCenter,i->second+mainViewCenter);
	}
	os<<Label(L"ENDSEC");
	os<<Label(L"EOF");
}
