readable (x:y:z:"Update":s) = False
readable ("**":s) = False
readable s = True
filt_unreadable = filter (readable.words)
main = interact (unlines.filt_unreadable.lines)
--main = do 
--	b <- getContents
--	writeFile "before.txt" b
--	let a = (unlines.filt_unreadable.lines) b
--	writeFile "after.txt" a
--	putStr a


