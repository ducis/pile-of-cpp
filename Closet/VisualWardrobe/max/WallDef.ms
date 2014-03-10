function CreateWallBox iAxis = undo on
(
	if (classof(selection[1]) == Editable_Poly) then 
	(
		vs = (polyop.getVertSelection  selection[1] )
		if (vs.count>0) then
		(
			BBmin = polyop.getVert selection[1] (vs as array)[1]
			BBmax = polyop.getVert selection[1] (vs as array)[1]
			--format "% %\n" BBmin BBmax
			for v in vs do
			(
				p = polyop.getVert selection[1] v
				if(p.x<BBmin.x) then BBmin.x = p.x else if(p.x>BBmax.x) then BBmax.x = p.x
				if(p.y<BBmin.y) then BBmin.y = p.y else if(p.y>BBmax.y) then BBmax.y = p.y
				if(p.z<BBmin.z) then BBmin.z = p.z else if(p.z>BBmax.z) then BBmax.z = p.z				
				--format "% % % %\n" v p BBmin BBmax
			)
			BBcenter = 0.5*(BBmin+BBmax)
			d = dummy()
			d.boxsize = BBmax-BBmin
			if(iAxis<0)then
			(
				if(d.boxsize.x<=d.boxsize.y and d.boxsize.x<=d.boxsize.z) then iAxis = 0
				if(d.boxsize.y<=d.boxsize.x and d.boxsize.y<=d.boxsize.z) then iAxis = 1
				if(d.boxsize.z<=d.boxsize.x and d.boxsize.z<=d.boxsize.y) then iAxis = 2
			)
			
			d.name = (iAxis as string) + d.name
			d.position = BBcenter
			setUserProp d "polyName" selection[1].name
			vs = vs as array
			setUserProp d "numVerts" vs.count
			for iV=1 to vs.count do
			(
				setUserProp d ("vert"+(iV as string)) vs[iV]
			)
			setUserProp d "iAxis" iAxis
		)
		--d = dummy()
		--d.boxsize = selection.max-selection.min
		--d.position = selection.center
	)
)

function CreateSpaceBox = undo on
(
	if (classof(selection[1]) == Editable_Poly) then 
	(
		vs = (polyop.getVertSelection  selection[1] )
		if (vs.count>0) then
		(
			BBmin = polyop.getVert selection[1] (vs as array)[1]
			BBmax = polyop.getVert selection[1] (vs as array)[1]
			--format "% %\n" BBmin BBmax
			for v in vs do
			(
				p = polyop.getVert selection[1] v
				if(p.x<BBmin.x) then BBmin.x = p.x else if(p.x>BBmax.x) then BBmax.x = p.x
				if(p.y<BBmin.y) then BBmin.y = p.y else if(p.y>BBmax.y) then BBmax.y = p.y
				if(p.z<BBmin.z) then BBmin.z = p.z else if(p.z>BBmax.z) then BBmax.z = p.z				
				--format "% % % %\n" v p BBmin BBmax
			)
			BBcenter = 0.5*(BBmin+BBmax)
			d = dummy()
			d.boxsize = BBmax-BBmin
			
			d.name = "Sp" + d.name
			d.position = BBcenter
			setUserProp d "polyName" selection[1].name
			vs = vs as array
			setUserProp d "numVerts" vs.count
			for iV=1 to vs.count do
			(
				setUserProp d ("vert"+(iV as string)) vs[iV]
			)
		)
		--d = dummy()
		--d.boxsize = selection.max-selection.min
		--d.position = selection.center
	)
)

function CreateInterface = undo on
(
	if (classof(selection[1]) == Editable_Poly) then
	(
		fs = (polyop.getFaceSelection  selection[1])
		if (fs.count>0) then
		(
			axisNormals = #([1,0,0],[0,1,0],[0,0,1],[-1,0,0],[0,-1,0],[0,0,-1])
			axisNames = #( "+X","+Y","+Z","-X","-Y","-Z" )
			axes = #{}
			axesUsage = #( 0, 0, 0, 0, 0, 0 )
			for iAxis = 1 to 6 do
			(
				for f in fs do
				(
					if (0.9999<dot axisNormals[iAxis] (polyop.getFaceNormal selection[1] f)) then
					(
						append axes iAxis
						axesUsage[iAxis]+=1
					)
				)
			)
			if( 1 == (axes as array).count )and (axesUsage[(axes as array)[1]] == (fs as array).count) then
			(
				vs = polyop.getVertsUsingFace selection[1] fs
				BBmin = polyop.getVert selection[1] (vs as array)[1]
				BBmax = polyop.getVert selection[1] (vs as array)[1]
				for v in vs do
				(
					p = polyop.getVert selection[1] v
					if(p.x<BBmin.x) then BBmin.x = p.x else if(p.x>BBmax.x) then BBmax.x = p.x
					if(p.y<BBmin.y) then BBmin.y = p.y else if(p.y>BBmax.y) then BBmax.y = p.y
					if(p.z<BBmin.z) then BBmin.z = p.z else if(p.z>BBmax.z) then BBmax.z = p.z
				)
				BBcenter = 0.5*(BBmin+BBmax)
				d = dummy()
				d.boxsize = BBmax-BBmin
				
				d.name = "If" + ((axes as array)[1] as string) + d.name
				d.position = BBcenter
				setUserProp d "polyName" selection[1].name
				fs = fs as array
				setUserProp d "numFaces" fs.count
				for iFace=1 to fs.count do
				(
					setUserProp d ("face"+(iFace as string)) fs[iFace]
				)
				setUserProp d "iDirection" (axes as array)[1]
			)
		)
	)
)

rollout WallDef "柜壁定义" width:180 height:425
(
	button btnX "定义顶点集为X方向柜壁" pos:[13,20] width:160 height:41

	button btnY "定义顶点集为Y方向柜壁" pos:[13,80] width:160 height:41

	button btnZ "定义顶点集为Z方向柜壁" pos:[13,140] width:160 height:41

	button btnAuto "定义顶点集为柜壁 自动方向" pos:[13,200] width:160 height:41

	button btnSpace "定义顶点集为柜内空间" pos:[13,260] width:160 height:41

	button btnInterface "定义面集为隔板边界" pos:[13,320] width:160 height:41


	on btnX pressed do
	(
		CreateWallBox(0)
	)
	on btnY pressed do
	(
		CreateWallBox(1)
	)
	on btnZ pressed do
	(
		CreateWallBox(2)
	)
	on btnAuto pressed do
	(
		CreateWallBox(-1)
	)
	on btnSpace pressed do
	(
		CreateSpaceBox()
	)
	on btnInterface pressed do
	(
		CreateInterface()
	)
)
createDialog WallDef 186 400
