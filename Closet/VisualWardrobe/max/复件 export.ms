
 out_name = GetSaveFileName()

 out_file = createfile out_name
 
 format "<ObjectSet>\n" to:out_file
 /*format "	<Min>% % %</Min>\n"   geometry.min.x geometry.min.y geometry.min.z  to:out_file
 format "	<Max>% % %</Max>\n"   geometry.max.x geometry.max.y geometry.max.z  to:out_file*/
 format "	<Count>%</Count>\n"   geometry.count  to:out_file
for iGeometry = 1 to geometry.count do
(
	t = copy(geometry[iGeometry]);
	convertto t editable_poly;
	polyop.CollapseDeadStructs t;
	/* format "%\n" (polyop.getDeadFaces t) */
	format "	<Poly>\n" to:out_file
	nVerts = polyop.getNumVerts(t);
	format "		<Verts num = \"%\">\n" nVerts  to:out_file
	for i = 1 to nVerts do
	(
		v = polyop.getVert t i node:scene
		format "			<Vertex>% % %</Vertex>\n" v.x v.y v.z to:out_file
	)
	format "		</Verts>\n" to:out_file
	
	
	nFaces = polyop.getNumFaces(t);
	format "		<Faces num = \"%\">\n" nFaces to:out_file
	for i = 1 to nFaces do
	(
		format "			<Face degree = \"%\">\n" (polyop.getFaceDeg t i) to:out_file
 		v = polyop.getFaceVerts t i
		format "				<Verts>" to:out_file
		for vi in v do
		(
			format "% " (vi-1) to:out_file
		)
		format "</Verts>\n" to:out_file 
		ed = polyop.getFaceEdges t i
		format "				<Edges>" to:out_file
		for edi in ed do
		(
			format "% " (edi-1) to:out_file
		)
		format "</Edges>\n" to:out_file 
		n = polyop.getFaceNormal t i node:scene
		format "				<Normal>% % %</Normal>\n" n.x n.y n.z to:out_file
		format "			</Face>\n"  to:out_file		
	)
	format "		</Faces>\n" to:out_file
	
	
	nEdges = polyop.getNumEdges(t);
	format "		<Edges num = \"%\">\n" nEdges to:out_file
	for i = 1 to nEdges do
	(
		format "			<Edge>\n" to:out_file
		v = polyop.getEdgeVerts t i
		format "				<Verts>" to:out_file
		for vi in v do
		(
			format "% " (vi-1) to:out_file
		)
		format "</Verts>\n" to:out_file
		f = polyop.getEdgeFaces t i
		format "				<Faces>" to:out_file
		for fi in f do
		(
			format "% " (fi-1) to:out_file
		)
		format "</Faces>\n" to:out_file
		format "			</Edge>\n" to:out_file
	)
	format "		</Edges>\n" to:out_file
	
	axisNormals = #([1,0,0],[0,1,0],[0,0,1])
	axisNames = #( "X","Y","Z" )
	for iAxis = 1 to 3 do
	(
		format "		<ControlPlane%Set>\n" axisNames[iAxis] to:out_file
		for iFace = 1 to nFaces do
		(
			if (0.9999<abs(dot axisNormals[iAxis] (polyop.getFaceNormal t iFace))) and not (bit.get (polyop.getFaceFlags t iFace) 30) then
			(
				format "			<ControlPlane>\n" to:out_file
				polyop.setFaceFlags t #(iFace) (bit.set (polyop.getFaceFlags t iFace) 30 true) undoable:false
				fQ = #(iFace)
				vs = #()
				format "				<Faces>" to:out_file
				for f in fQ do
				(
					es = polyop.getFaceEdges t f
					join vs (polyop.getFaceVerts t f)
					for ed in es do
					(
						fs = polyop.getEdgeFaces t ed
						for ff in fs do
						(
							bf = (polyop.getFaceFlags t ff) 
							if (0.9999<abs(dot axisNormals[iAxis] (polyop.getFaceNormal t ff)))and not (bit.get bf 30) then
							(
								polyop.setFaceFlags t #(ff) (bit.set bf 30 true) undoable:false
								append fQ ff;
							)
						)
					)
					format "% " (f-1) to:out_file
				)
				format "</Faces>\n" to:out_file
				vs = makeUniqueArray vs
				format "				<Verts>" to:out_file
				for v in vs do
				(
					format "% " (v-1) to:out_file
				)
				format "</Verts>\n" to:out_file
				format "			</ControlPlane>\n" to:out_file
			)
		)
		format "		</ControlPlane%Set>\n" axisNames[iAxis] to:out_file
	)
	format "	</Poly>\n" to:out_file
	delete t;
)

 format "</ObjectSet>\n" to:out_file
	
  close out_file

   edit out_name 


