#pragma once

#include <string>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "vectorTraits.h"

namespace DxfFormat
{//这里dou是输出Dxf文件用的一些模板,名字起得都是和DXF文件里面一样的,请参考DXF文件的资料
	template< unsigned _GroupCode, typename T > class Value{
		template< typename _OutputStream > friend _OutputStream &operator<< ( _OutputStream &os , const Value &v ){
			return os<<L'\t'<<_GroupCode<<L'\n'<<v.m_value<<L'\n';
		}
	private:
		T m_value;
	public:
		explicit Value( const T &value ): m_value(value){}
	};
	template< unsigned _GroupCode, typename T > const Value<_GroupCode,T> MakeValue(const T &value){
		return Value<_GroupCode,T>(value);
	}



	template< unsigned _Tag, typename T > class Coord{
		template< typename _OStr > friend _OStr &operator<< ( _OStr &os , const Coord &c ){
			return os<<MakeValue<10+_Tag>(c.m_p.x)<<MakeValue<20+_Tag>(c.m_p.y)<<MakeValue<30+_Tag>(0);//<<MakeValue<30+_Tag>(c.m_z);
		}
	private:
		T m_p;
	public:
		explicit Coord( const T &p ):m_p(p){
		}
	};
	template< unsigned _Tag, typename T > const Coord<_Tag,T> MakeCoord( const T &p ){
		return Coord<_Tag,T>(p);
	}



	typedef Value< 8 , std::wstring > Layer;
	typedef Value< 0 , const wchar_t * > Label;
	typedef Value< 2 , const wchar_t * > SecName;
	typedef Value< 2 , std::wstring > BlockRef;

	template <typename T> class Line{
		template< typename _OStr > friend _OStr &operator<< ( _OStr &os , const Line &l ){
			return os<<Label(L"LINE")<<l.m_layer<<l.m_start<<l.m_end;
		}
	private:
		Coord<0,T> m_start;
		Coord<1,T> m_end;
		Layer m_layer;
	public:
		Line( const Layer &layer, const T &p0, const T &p1 ): m_start(p0), m_end(p1), m_layer(layer){}
	};



	template <typename T> class Solid{
		template< typename _OStr > friend _OStr &operator<< ( _OStr &os , const Solid &s ){
			return os<<Label(L"SOLID")<<s.m_layer<<s.m_a<<s.m_b<<s.m_c<<s.m_d;
		}
	private:
		Coord<0,T> m_a;
		Coord<1,T> m_b;
		Coord<2,T> m_c;
		Coord<3,T> m_d;
		Layer m_layer;
	public:
		Solid( 
			const Layer &layer, 
			const T &p0, 
			const T &p1, 
			const T &p2,
			const T &p3
			): m_layer(layer), m_a(p0), m_b(p1), m_c(p2), m_d(p3){}
		Solid(
			const Layer &layer,
			const T &p0,
			const T &p1,
			const T &p2
			): m_layer(layer), m_a(p0), m_b(p1), m_c(p2), m_d(p2){}
			
	};



	template <typename T> class BlockStart{
		template< typename _OStr > friend _OStr &operator<< ( _OStr &os , const BlockStart &bs ){
			return os<<Label(L"BLOCK")<<bs.m_layer<<MakeValue<2>(bs.m_name)<<bs.m_flags<<bs.m_basePoint<<MakeValue<3>(bs.m_name)<<MakeValue<1,const wchar_t *>(L"");
		}
	private:
		Layer m_layer;
		std::wstring m_name;
		Value<70, unsigned> m_flags;
		Coord<0,T> m_basePoint;
	public:
		BlockStart(
			const Layer &layer,
			const std::wstring name,
			unsigned flags,
			const T &bp
			):m_layer(layer),m_name(name),m_flags(flags),m_basePoint(bp){}
	};



	class BlockEnd{
		template< typename _OStr > friend _OStr &operator<< ( _OStr &os , const BlockEnd &be ){
			return os<<Label(L"ENDBLK")<<be.m_layer;
		}
	private:
		Layer m_layer;
	public:
		explicit BlockEnd( const Layer &layer ):m_layer(layer){}
	};



	template <typename _TextType, typename V, typename S> class Text{
		template < typename _OStr > friend _OStr &operator<< ( _OStr &os, const Text &t ){
			return os<<Label(L"TEXT")<<t.m_layer<<t.m_pos<<t.m_pos2<<t.m_height<<t.m_rot<<t.m_text<<t.m_justH<<t.m_justV;
		}
	private:
		Layer m_layer;
		Coord<0,V> m_pos;
		Coord<1,V> m_pos2;
		Value<40,S> m_height;
		Value<50,S> m_rot;
		Value<1, _TextType> m_text;
		Value<72,unsigned> m_justH;
		Value<73,unsigned> m_justV;
	public:
		Text(const Layer &layer, const _TextType &text, const V &pos, const S &h, const S &rot=0, unsigned justH=0, unsigned justV=0):m_layer(layer),m_pos(pos),m_pos2(pos),m_height(h),m_rot(rot),m_text(text),m_justH(justH),m_justV(justV){}
	};
	template <typename _TextType, typename V, typename S> Text<_TextType,V,S> MakeText(const Layer &layer, const _TextType &text, const V &pos, const S &h, const S &rot = 0,unsigned justH=0,unsigned justV=0){
		return Text<_TextType,V,S>(layer,text,pos,h,rot,justH,justV);
	}


	template <typename T> class Point{
		template < typename _OStr > friend _OStr &operator<< ( _OStr &os, const Point &p ){
			return os<<Label(L"POINT")<<p.m_layer<<p.m_coord;
		}
	private:
		Layer m_layer;
		Coord<0,T> m_coord;
	public:
		Point(const Layer &layer, const T &p ):m_layer(layer),m_coord(p){}
	};



	template <typename T> class Insert{
		template < typename _OStr > friend _OStr &operator<< ( _OStr &os, const Insert &i ){
			return os<<Label(L"INSERT")<<i.m_layer<<i.m_name<<i.m_pos;
		}
	private:
		Layer m_layer;
		Value<2, std::wstring> m_name;
		Coord<0,T> m_pos;
	public:
		Insert(const Layer &layer, const std::wstring &name, const T &p ):m_layer(layer),m_name(name),m_pos(p){}
	};



	template < typename V, typename S > class DimensionBlocks{
		template < typename _OStr > friend _OStr &operator<< ( _OStr &os, const DimensionBlocks &db ){
			return os<<db.m_mainBlockStart<<db.m_lineLeftV<<db.m_lineRightV<<db.m_lineH<<db.m_arrowLeft<<db.m_arrowRight<<db.m_textBlockRef
				<<db.m_pointLeft<<db.m_pointRight<<db.m_pointRightU<<db.m_mainBlockEnd<<db.m_textBlockStart<<db.m_text<<db.m_textBlockEnd;
		}
	private:
		BlockStart<V> m_mainBlockStart;
		Line<V> m_lineLeftV,m_lineRightV,m_lineH;
		Solid<V> m_arrowLeft, m_arrowRight;
		Insert<V> m_textBlockRef;
		Point<V> m_pointLeft,m_pointRight,m_pointRightU;
		BlockEnd m_mainBlockEnd;

		BlockStart<V> m_textBlockStart;
		Text<S,V,S> m_text;
		BlockEnd m_textBlockEnd;
	public:
		DimensionBlocks(
			const Layer &layer, const std::wstring &mainBlockName, const std::wstring &textBlockName, 
			const V &n, const V &t, const S &arrowLen, const S &halfArrowWidth, const S &dim, const S &textHeight, const S &extHLineL, const S &extHLineR,
			const V &p0, const V &p1, const S &baseDot, const S &offset0, const S &offset1, const V &textOffset)
			:m_mainBlockStart(Layer(L"0")/*layer*/,mainBlockName,1,V(0,0)),
			//m_lineLeftV(layer,p0+n*offset0,p0+n*offset1),
			m_lineLeftV( layer, p0+n*offset0, t*DotProduct(p0,t)+n*(baseDot+offset1) ),
			//m_lineRightV(layer,p1+n*offset0,p1+n*offset1),
			m_lineRightV( layer, p1+n*offset0, t*DotProduct(p1,t)+n*(baseDot+offset1) ),
			//m_lineH(layer,p0+n*offsetHLine-t*extHLineL,p1+n*offsetHLine+t*extHLineR),
			m_lineH( layer, t*(DotProduct(p0,t)-extHLineL)+n*baseDot, t*(DotProduct(p1,t)+extHLineR)+n*baseDot ),
			m_arrowLeft( layer, n*(baseDot-halfArrowWidth)+t*(arrowLen+DotProduct(p0,t)), n*(baseDot+halfArrowWidth)+t*(arrowLen+DotProduct(p0,t)), n*baseDot+t*DotProduct(p0,t) ),
			//m_arrowRight(layer,p1+n*offsetHLine-t*arrowLen-n*halfArrowWidth,p1+n*offsetHLine-t*arrowLen+n*halfArrowWidth,p1+n*offsetHLine),
			m_arrowRight( layer, n*(baseDot-halfArrowWidth)+t*(DotProduct(p1,t)-arrowLen), n*(baseDot+halfArrowWidth)+t*(DotProduct(p1,t)-arrowLen),n*baseDot+t*DotProduct(p1,t) ),
			m_textBlockRef(Layer(L"0")/*layer*/,textBlockName,V(0,0)),
			m_pointLeft(Layer(L"DEFPOINTS")/*layer*/,p0),
			m_pointRight(Layer(L"DEFPOINTS")/*layer*/,p1),
			m_pointRightU(Layer(L"DEFPOINTS")/*layer*/,t*(DotProduct(p1,t)+extHLineR)+n*baseDot),
			m_mainBlockEnd(Layer(L"0")),//layer),
			m_textBlockStart(Layer(L"0")/*layer*/,textBlockName,1,V(0,0)),
			m_text(layer, dim, t*(0.5*( DotProduct(t,p0)+DotProduct(t,p1) )+textOffset.x) + (textOffset.y+baseDot)*n + V(-2*textHeight,-0.5*textHeight) + abs(n.x)*2*textHeight*n, textHeight ),
			m_textBlockEnd(Layer(L"0"))//layer)
		{
		}
	};


	
	template < typename V > class DimensionEntity{
		template < typename _OStr > friend _OStr &operator<< ( _OStr &os, const DimensionEntity &de ){
			return os<<Label(L"DIMENSION")<<de.m_layer<<de.m_blockName<<de.m_defPoint<<de.m_textPos<<de.m_type<<de.m_a<<de.m_b;
		}
	private:
		Layer m_layer;
		Value<2, std::wstring> m_blockName;
		Coord<0,V> m_defPoint;
		Coord<1,V> m_textPos;
		Value<70, unsigned> m_type;
		Coord<3,V> m_a;
		Coord<4,V> m_b;
	public:
		DimensionEntity(
			const Layer &layer, 
			const std::wstring &blockName, 
			const V &defPoint, 
			const V &textPos, 
			const V &a, 
			const V &b
			)
			:m_layer(layer),m_blockName(blockName),m_defPoint(defPoint),m_textPos(textPos),m_type(0),m_a(a),m_b(b)
		{
		}
	};



	template < typename V > const typename VectorTraits<V>::ScalarType Length( const V &v );
	template <> const float Length( const vector2 &v ){ return v.length(); }

	template < typename V > std::pair<DimensionBlocks<V,typename VectorTraits<V>::ScalarType>,DimensionEntity<V> > MakeDimension(
		const Layer &layer, const std::wstring &mainBlockName, const std::wstring &textBlockName, const V &from, const V &to, const V &t, const V &defPoint1, const V &defPoint2)
	{
		//V t(Normalized(to-from));
		V n(-t.y,t.x);
		
		//if(Length(to-from)<60){
		//	return std::make_pair(
		//		DimensionBlocks<V,VectorTraits<V>::ScalarType>(layer,mainBlockName,textBlockName,n,t,-3,3,DotProduct(t,to-from),18,36,144,from,to,30+std::max(DotProduct(n,defPoint1),DotProduct(n,defPoint2)),6,20,V(100,19)),
		//		DimensionEntity<V>(layer,mainBlockName,to+n*20,0.5*(from+to)+t*100+n*30,from,to)
		//		);
		//}
		return std::make_pair(
			DimensionBlocks<V,VectorTraits<V>::ScalarType>(layer,mainBlockName,textBlockName,n,t,3,3,DotProduct(t,to-from),18,0,0,from,to,30+std::max(DotProduct(n,defPoint1),DotProduct(n,defPoint2)),6,20,V(0,19)),
			DimensionEntity<V>(layer,mainBlockName,to+n*20,0.5*(from+to)+n*30,from,to)
			);
	};

	
	template < typename V > std::pair<DimensionBlocks<V,typename VectorTraits<V>::ScalarType>,DimensionEntity<V> > MakeDimension(
		const Layer &layer, const std::wstring &mainBlockName, const std::wstring &textBlockName, const V &from, const V &to)
	{
		return MakeDimension(layer,mainBlockName,textBlockName,from,to,Normalized(to-from),from,to);
	}



	template < typename V , typename S > class Circle{
		template < typename _OStr > friend _OStr &operator<< ( _OStr &os, const Circle &c ){
			return os<<Label(L"CIRCLE")<<c.m_layer<<c.m_center<<c.m_radius;
		}
	private:
		Layer m_layer;
		Coord<0,V> m_center;
		Value<40,S> m_radius;
	public:
		Circle(const Layer &layer,const V &center, const S &radius):m_layer(layer),m_center(center),m_radius(radius){}
	};

	template < typename V > class CenterMark{
		template < typename _OStr > friend _OStr &operator<< ( _OStr &os, const CenterMark &cm ){
			return os<<cm.m_lineX<<cm.m_lineY;
		}
	private:
		Line<V> m_lineX;
		Line<V> m_lineY;
	public:
		CenterMark(const Layer &layer, const V &center, const typename VectorTraits<V>::ScalarType &r)
			:m_lineX(layer,V(center.x-r,center.y),V(center.x+r,center.y)),
			m_lineY(layer,V(center.x,center.y-r),V(center.x,center.y+r))
		{
		}
	};

	
	template < typename V > class CenterMarkedCircle{
		typedef typename VectorTraits<V>::ScalarType S;
		template < typename _OStr > friend _OStr &operator<< ( _OStr &os, const CenterMarkedCircle &cmc ){
			return os<<cmc.m_circle<<cmc.m_centerMark;
		}
	private:
		Circle<V, S> m_circle;
		CenterMark<V> m_centerMark;
	public:
		CenterMarkedCircle(const Layer &circleLayer, const Layer &markLayer, const V &center, const S &r)
			:m_circle(circleLayer,center,r),m_centerMark(markLayer,center,r*2)
		{
		}
	};
	//template 


	class NameIterator{
	private:
		unsigned m_currentId;
		wchar_t m_buf[16];
	public:
		NameIterator(const wchar_t wch, unsigned firstId = 0):m_currentId(firstId){
			m_buf[0]=L'*';
			m_buf[1]=wch;
			_itow_s(m_currentId,m_buf+2,12,10);
		}
		NameIterator &operator++(){
			++m_currentId;
			_itow_s(m_currentId,m_buf+2,12,10);
			return *this;
		}
		const wchar_t *operator*(){
			return m_buf;
		}
	};


	template<typename V> class Viewport{
		template < typename _OStr > friend _OStr &operator<< ( _OStr &os, const Viewport &vp ){
			return os<<Label(L"TABLE")<<SecName(L"VPORT")<<MakeValue<70>(1)<<Label(L"VPORT")<<SecName(L"*ACTIVE")<<MakeValue<70>(0)
				<<MakeValue<10>(0.0)<<MakeValue<20>(0.0)<<MakeValue<11>(1.0)<<MakeValue<21>(1.0)<<vp.m_x<<vp.m_y
				<<MakeValue<13>(0.0)<<MakeValue<23>(0.0)<<MakeValue<14>(1.0)<<MakeValue<24>(1.0)<<MakeValue<15>(0.0)<<MakeValue<25>(0.0)
				<<MakeValue<16>(0.0)<<MakeValue<26>(0.0)<<MakeValue<36>(1.0)<<MakeValue<17>(0.0)<<MakeValue<27>(0.0)<<MakeValue<37>(0.0)
				<<vp.m_scale
				<<MakeValue<41>(0.0)<<MakeValue<42>(0.0)<<MakeValue<43>(0.0)<<MakeValue<44>(0.0)<<MakeValue<50>(0.0)<<MakeValue<51>(0.0)
				<<MakeValue<71>(0)<<MakeValue<72>(1000)<<MakeValue<73>(1)<<MakeValue<74>(3)<<MakeValue<75>(0)<<MakeValue<76>(0)<<MakeValue<77>(0)<<MakeValue<78>(0)
				<<Label(L"ENDTAB");
		}
	private:
		typedef typename VectorTraits<V>::ScalarType S;
		Value<40,S> m_scale;
		Value<12,S> m_x;
		Value<22,S> m_y;
	public:
		Viewport(const V &min, const V &max)
			:m_scale((max.y-min.y)*1.5),
			m_x(min.x),
			m_y(0.5*(min.y+max.y)){
		}
	};
};
