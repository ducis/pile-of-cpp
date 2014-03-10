
 out_name = GetSaveFileName()

 out_file = createfile out_name
 
 format "<ObjectSet>\n" to:out_file
 /*format "	<Min>% % %</Min>\n"   geometry.min.x geometry.min.y geometry.min.z  to:out_file
 format "	<Max>% % %</Max>\n"   geometry.max.x geometry.max.y geometry.max.z  to:out_file*/
 /*format "	<Count>%</Count>\n"   geometry.count  to:out_file*/
 
 
 /*
 cellMarkers = #()
for h in helpers do
(
	if(isKindOf h point) then
	(
		if(matchPattern h.name pattern:"CellMarker*") then
		(
			append cellMarkers h.position
		)
		else
		(
			format "	<AutoLabel>% % % %</AutoLabel>\n" h.name h.position.x h.position.y h.position.z to:out_file
		)
	)
	if(isKindOf h dummy) then
	(
		if(matchPattern h.name pattern:"Sp*") then
		(
			format "	<InteriorDef>\n" to:out_file
			format "		<PolyName>%</PolyName>\n" (getUserProp h "polyName") to:out_file
			nVerts = (getUserProp h "numVerts") as integer
			format "		<Verts>" to:out_file
			for iV = 1 to nVerts do
			(
				format "% " (((getUserProp h ("vert"+(iV as string)) ) as integer)-1) to:out_file
			)
			format "</Verts>\n" to:out_file
			format "	</InteriorDef>\n" to:out_file
		)
		else if(matchPattern h.name pattern:"If*") then
		(
			format "	<InterfaceDef>\n" to:out_file
			format "		<PolyName>%</PolyName>\n" (getUserProp h "polyName") to:out_file
			nFaces = (getUserProp h "numFaces") as integer
			format "		<Faces>" to:out_file
			for iFace = 1 to nFaces do
			(
				format "% " (((getUserProp h ("face"+(iFace as string))) as integer)-1) to:out_file
			)
			format "</Faces>\n" to:out_file
			format "		<Direction>%</Direction>\n" 	((getUserProp h "iDirection")-1) to:out_file
			format "	</InterfaceDef>\n" to:out_file
		)
		else
		(
			format "	<WallDef>\n" to:out_file
			format "		<PolyName>%</PolyName>\n" (getUserProp h "polyName") to:out_file
			nVerts = (getUserProp h "numVerts") as integer
			format "		<Verts>" to:out_file
			for iV = 1 to nVerts do
			(
				format "% " (((getUserProp h ("vert"+(iV as string)) ) as integer)-1) to:out_file
			)
			format "</Verts>\n" to:out_file
			format "		<Axis>%</Axis>\n"	(getUserProp h "iAxis") to:out_file
			format "	</WallDef>\n" to:out_file
		)
	)
)
*/


/*
format "	<BWTFromBoxes>\n" to:out_file
for pcm in cellMarkers do
(
	format "		<CellMarker>% % %</CellMarker>\n" pcm.x pcm.y pcm.z to:out_file
)
for iGeometry = 1 to geometry.count do
(
	select geometry[iGeometry]
	format "		<AABB> % % %  % % % </AABB>\n" selection.min.x selection.min.y selection.min.z selection.max.x selection.max.y selection.max.z to:out_file
)
format "	</BWTFromBoxes>\n" to:out_file
*/
--for iGeometry = 1 to 0 do --geometry.count do
for iGeometry = 1 to geometry.count do
(
	t = copy(geometry[iGeometry]);
	convertto t editable_poly;
	polyop.CollapseDeadStructs t;
	
	bakeChannel_Obj = 50
	flattenAngle = 45
	flattenSpacing = 0.02
	flattenRotate = true
	flattenFillHoles = true
	RTT_UnwrapMod_AppData_Index = 1000
	max modify mode

   modPanel.setCurrentObject t
/*
	--(
		unwrapMod = Unwrap_UVW()
		unwrapMod.setAlwaysEdit false
		unwrapMod.setMapChannel bakeChannel_Obj
		unwrapMod.setFlattenAngle flattenAngle 
		unwrapMod.setFlattenSpacing flattenSpacing 
		unwrapMod.setFlattenNormalize true
		unwrapMod.setFlattenRotate flattenRotate 
		unwrapMod.setFlattenFillHoles flattenFillHoles 
		unwrapMod.setApplyToWholeObject true
		unwrapMod.name = "Automatic Flatten UVs"
		unwrapMod.setTVSubObjectMode 3
		--unwrapMod.setDebugLevel 0

		setAppData unwrapMod 0x41dd73d5 #object

		-- add it to the object
		-- add directly to the object to avoid groupness
		addModifier t unwrapMod
		
		-- & flatten things
		unwrapMod.flattenMapByMatID \
			flattenAngle  \
			flattenSpacing  \
			true \
			2 \
			flattenRotate  \
			flattenFillHoles 
	--)
			
		t.iNodeBakeProperties.removeAllBakeElements()
		be1 = diffusemap()
		size = 256
		be1.outputSzX = be1.outputSzY = size
		--be1.fileType = (getDir out_name+"_diffuse.tga") 
		be1.fileType = be1.fileName = (out_name+"."+((iGeometry-1) as string)+".png")
		be1.filterOn = true --enable filtering 
		be1.shadowsOn = false --disable shadows
		be1.lightingOn = false --disable lighting
		be1.enabled = true --enable baking
		t.INodeBakeProperties.addBakeElement be1 
		t.INodeBakeProperties.bakeEnabled = true 
		t.INodeBakeProperties.bakeChannel = bakeChannel_Obj
		t.INodeBakeProperties.nDilations = 1 
		select t
		render rendertype:#bakeSelected vfb:off progressBar:true outputSize:[size,size]
	*/
		
	/* format "%\n" (polyop.getDeadFaces t) */
	format "	<Poly>\n" to:out_file
	--format "		<Name>%</Name>\n" (geometry[iGeometry].name) to:out_file
	--format "		<DiffuseMap>%</DiffuseMap>\n" (pathconfig.convertPathToRelativeTo be1.fileName (pathconfig.removePathLeaf out_name)) to:out_file
	
	nVerts = polyop.getNumVerts(t);
	format "		<Verts num = \"%\">\n" nVerts  to:out_file
	--format "%"(polyop.getNumMaps t);
	for i = 1 to nVerts do
	(
		v = polyop.getVert t i node:scene
		format "			<Vertex>% % %</Vertex>\n" (v.x ) (v.y ) (v.z ) to:out_file
		--format "			<Vertex>% % %</Vertex>\n" (v.x as integer) (v.y as integer) (v.z as integer) to:out_file
	)
	format "		</Verts>\n" to:out_file
	
	/*
	nMapVerts = polyop.getNumMapVerts t bakeChannel_Obj
	format "		<MapVerts num = \"%\">\n" nMapVerts to:out_file
	for i = 1 to nMapVerts do
	(
		mv = polyop.getMapVert t bakeChannel_Obj i
		format "			<MapVert> % % </MapVert>\n" mv.x -mv.y to:out_file
	)
	format "		</MapVerts>\n" to:out_file
	*/
	
	--nMapFaces = polyop.getNumMapFaces t bakeChannel_Obj
	--format "		<MapFaces num = \"%\">\n" nMapFaces to:out_file
	--format "		</MapFaces>\n" to:out_file
	
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
		
		/*
		format "				<MapVerts>" to:out_file
		mv = polyop.getMapFace t bakeChannel_Obj i
		for mvi in mv do
		(
			format "% " (mvi-1) to:out_file
		)
		format "</MapVerts>\n" to:out_file
		*/
		
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
	
	/*axisNormals = #([1,0,0],[0,1,0],[0,0,1])
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
	*/
	--(	

	--	deleteModifier geometry[iGeometry] 1
	--)
	--delete unwrapMod
	format "	</Poly>\n" to:out_file
	delete t;
)

 format "</ObjectSet>\n" to:out_file
	
  close out_file

--   edit out_name 



