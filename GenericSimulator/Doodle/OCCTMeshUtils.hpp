#pragma once

#include <boost/fusion/tuple.hpp>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>
//#include <boost/operators.hpp>
#include <StdPrs_ToolShadedShape.hxx>
#include <limits>
#include <boost/array.hpp>
#include "NumericRange.hpp"
#include <cmath>
#include <Poly_Triangulation.hxx>
//#include <numeric>
//#include <functional>
//#include <boost/range/iterator_range.hpp> 
//#include <boost/range/adaptor/transformed.hpp>
//#include <boost/range/numeric.hpp>


namespace OpenCASCADEUtils{
	//class ShapeFaceIterator: boost::incrementable<ShapeFaceIterator>{
	//	TopExp_Explorer m_e;
	//public:
	//	explicit ShapeFaceIterator(const TopoDS_Shape &s): m_e(s,TopAbs_FACE){}
	//	ShapeFaceIterator(){}
	//	ShapeFaceIterator &operator++(){
	//		m_e.Next();
	//	}
	//	//ShapeFaceIterator operator++( int ){
	//	//	ShapeFaceIterator r = *this;
	//	//	++(*this);
	//	//	return r;
	//	//}
	//	bool operator!=(const ShapeFaceIterator &) const{ return m_e.More(); }
	//	const TopoDS_Face &operator*() const{ return TopoDS::Face(m_e.Current()); }
	//};
	//class ShapeFaceMeshIterator: boost::incrementable<ShapeFaceMeshIterator>{
	//	ShapeFaceIterator m_sfi;
	//	TopLoc_Location m_l;
	//public:
	//	template <typename _ShapeFaceIteratorInitializer>
	//	ShapeFaceMeshIterator(const _ShapeFaceIteratorInitializer &s,const TopLoc_Location &l): m_sfi(s), m_l(l){}
	//	template <typename _ShapeFaceIteratorInitializer>
	//	explicit ShapeFaceMeshIterator(const _ShapeFaceIteratorInitializer &s): m_sfi(s){}
	//	ShapeFaceMeshIterator(){}
	//	ShapeFaceMeshIterator() &operator++(){ ++m_sfi; }
	//	bool operator!=(const ShapeFaceMeshIterator &rhs) const{ return m_sfi!=rhs.m_sfi; }
	//	const Poly_Triangulation &operator*() const{ return *StdPrs_ToolShadedShape::Triangulation(*m_sfi,m_l); }
	//};
	inline std::size_t GetMeshedShapeVertCount(const TopoDS_Shape &s){
		std::size_t result = 0;
		for(TopExp_Explorer e(s,TopAbs_FACE);e.More();e.Next()){
			result += StdPrs_ToolShadedShape::Triangulation(TopoDS::Face(e.Current()),TopLoc_Location())->NbNodes();
		}
		return result;
		//return boost::accumulate
		//(
		//	boost::make_iterator_range(ShapeFaceMeshIterator(s),ShapeFaceMeshIterator())
		//		|boost::adaptors::transformed(std::mem_fun_ref(Poly_Triangulation::NbNodes)),
		//	0
		//);
	}
	inline std::size_t GetMeshedShapeTriangleCount(const TopoDS_Shape &s){
		std::size_t result = 0;
		for(TopExp_Explorer e(s,TopAbs_FACE);e.More();e.Next()){
			result += StdPrs_ToolShadedShape::Triangulation(TopoDS::Face(e.Current()),TopLoc_Location())->NbTriangles();
		}
		return result;
		//return boost::accumulate
		//(
		//	boost::make_iterator_range(ShapeFaceMeshIterator(s),ShapeFaceMeshIterator())
		//		|boost::adaptors
		//);
	}

	//could use some iterators and vector output

	template< typename _OutputIterator >
	_OutputIterator CopyMeshedShapeIndices(_OutputIterator dest,const TopoDS_Shape &s)
	{
		Standard_Integer offset = 0;
		for(TopExp_Explorer e(s,TopAbs_FACE);e.More();e.Next()){
			Handle_Poly_Triangulation h = StdPrs_ToolShadedShape::Triangulation(TopoDS::Face(e.Current()),TopLoc_Location());
			const Poly_Array1OfTriangle &f = h->Triangles();
			const TColgp_Array1OfPnt &v = h->Nodes();
			offset-=v.Lower();
			Standard_Integer 
				stride = (e.Current().Orientation() == TopAbs_REVERSED)?-1:1,
				first = 2-stride,
				last = 2*(1+stride);//(-1,3,0) or (1,1,4)
			for(Standard_Integer i=f.Lower();i<=f.Upper();++i){
				const Poly_Triangle & tri = f(i);
				for(int j=first;j!=last;j+=stride){
					*dest++ = tri(j)+offset;
				}
			}
			offset+=v.Lower();
			offset+=v.Length();
		}
		return dest;
	}

	template< typename _ScalarOutputIterator >
	_ScalarOutputIterator CopyComponentsOfMeshedShapeVertexNormals(_ScalarOutputIterator dest,const TopoDS_Shape &s){
		for(TopExp_Explorer e(s,TopAbs_FACE);e.More();e.Next()){
			Handle_Poly_Triangulation h = StdPrs_ToolShadedShape::Triangulation(TopoDS::Face(e.Current()),TopLoc_Location());
			Poly_Connect pc(h);
			TColgp_Array1OfDir n(1,h->NbNodes());
			StdPrs_ToolShadedShape::Normal(TopoDS::Face(e.Current()),pc,n);
			for(Standard_Integer i = n.Lower();i<=n.Upper();++i){
				const gp_Dir &d = n(i);
				*dest++ = d.X();
				*dest++ = d.Y();
				*dest++ = d.Z();
			}
		}
		return dest;
	}

	template< typename _ScalarOutputIterator >
	_ScalarOutputIterator CopyComponentsOfMeshedShapeVertexPositions(_ScalarOutputIterator dest, const TopoDS_Shape &s){
		for(TopExp_Explorer e(s,TopAbs_FACE);e.More();e.Next()){
			Handle_Poly_Triangulation h = StdPrs_ToolShadedShape::Triangulation(TopoDS::Face(e.Current()),TopLoc_Location());
			const TColgp_Array1OfPnt &v = h->Nodes();
			for(Standard_Integer i=v.Lower();i<=v.Upper();++i){
				const gp_Pnt &pt = v(i);
				*dest++ = pt.X();
				*dest++ = pt.Y();
				*dest++ = pt.Z();
			}
		}
		return dest;
	}

	boost::array<Standard_Real,6> GetMeshedShapeAABB(const TopoDS_Shape &s){
		boost::array<Standard_Real,6> r=
		{ 
			std::numeric_limits<Standard_Real>::max(),std::numeric_limits<Standard_Real>::max(),std::numeric_limits<Standard_Real>::max(),
			std::numeric_limits<Standard_Real>::min(),std::numeric_limits<Standard_Real>::min(),std::numeric_limits<Standard_Real>::min()
		};
		for(TopExp_Explorer e(s,TopAbs_FACE);e.More();e.Next()){
			Handle_Poly_Triangulation h = StdPrs_ToolShadedShape::Triangulation(TopoDS::Face(e.Current()),TopLoc_Location());
			const TColgp_Array1OfPnt &v = h->Nodes();
			for(Standard_Integer i=v.Lower();i<=v.Upper();++i){
				using namespace NumericRange;
				const gp_Pnt &pt = v(i);
				Extend(pt.X(),r[0],r[3]);
				Extend(pt.Y(),r[1],r[4]);
				Extend(pt.Z(),r[2],r[5]);
			}
		}
		return r;
	}

	Standard_Real GetMeshedShapeMaxVertexDistanceSquared(const TopoDS_Shape &s){
		Standard_Real r=0;
		for(TopExp_Explorer e(s,TopAbs_FACE);e.More();e.Next()){
			Handle_Poly_Triangulation h = StdPrs_ToolShadedShape::Triangulation(TopoDS::Face(e.Current()),TopLoc_Location());
			const TColgp_Array1OfPnt &v = h->Nodes();
			for(Standard_Integer i=v.Lower();i<=v.Upper();++i){
				using namespace NumericRange;
				const gp_Pnt &pt = v(i);
				ExtendR( pow(v(i).X(),2) + pow(v(i).Y(),2) + pow(v(i).Z(),2) , r);
			}
		}
		return r;
	}
}
