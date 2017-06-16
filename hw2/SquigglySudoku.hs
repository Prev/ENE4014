import Debug.Trace

xList' :: [Integer] -> Integer -> Integer -> [Integer]
--           map         i       ypos       list
xList' [] i ypos   = []
xList' (x:xs) i ypos
    | (ypos * 9 <= i && (ypos+1) * 9 > i)  = [x] ++ xList' xs (i+1) ypos
    | otherwise                            = xList' xs (i+1) ypos

xList :: [Integer] -> Integer -> [Integer]
xList map ypos = xList' map 0 ypos



yList' :: [Integer] -> Integer -> Integer -> [Integer]
--          map           i       xpos        list 
yList' [] i xpos   = []
yList' (x:xs) i xpos
    | (mod i 9) == xpos      = [x] ++ yList' xs (i+1) xpos
    | otherwise              = yList' xs (i+1) xpos

yList :: [Integer] -> Integer -> [Integer]
yList map ypos = yList' map 0 ypos


gList' :: [Integer] -> [Integer] -> Integer -> Integer -> [Integer]
--          map          gmap          i         gid       
gList' [] gmap i gid   = []
gList' (x:xs) gmap i gid
    | (gmap !! fromInteger i) == gid     = [x] ++ gList' xs gmap (i+1) gid
    | otherwise                          = gList' xs gmap (i+1) gid

gList :: [Integer] -> [Integer] -> Integer -> [Integer]
gList map gmap gid = gList' map gmap 0 gid




canPlace :: [Integer] -> [Integer] -> (Integer, Integer) -> Integer -> Bool
--              map      group_map       (xpos, ypos)   number to place
canPlace map gmap pos n
    | elem n (xList map (fst pos))  = False  -- `n` is used in X-List
    | elem n (yList map (snd pos))  = False  -- `n` is used in Y-List
    | elem n (gList map gmap gid)   = False  -- `n` is used in Group-List
    | otherwise                     = True
    where
        index  = (9 * (fst pos)) + (snd pos)
        gid    = gmap !! fromInteger index


getCandidates :: [Integer] -> [Integer] -> (Integer, Integer) -> [Integer]
--                  map        group_map      (xpos, ypos)
getCandidates map gmap pos
    | (map !! fromInteger index) /= 0  = [ (map !! fromInteger index) ]
    | otherwise                        = [n | n <- [1..9], canPlace map gmap (fst pos, snd pos) n]
    where
        index = (9 * (fst pos)) + (snd pos)



getNewMap :: [Integer] -> Integer -> Integer -> [Integer]
getNewMap map index n = (take (fromInteger index) map) ++ [n] ++ (drop (fromInteger (index+1)) map)


squigglySudoku' :: [Integer] -> [Integer] -> [Integer] -> Integer -> (Bool, [Integer])
--                candidates       map         gmap        index     (possible, map)
squigglySudoku' [] map gmap index
    | index >= 81 = (True, map)
    | otherwise = (False, [])

squigglySudoku' (x:xs) map gmap index
    | next_index >= 81 = (True, newmap)
    | fst rst  = rst
    | otherwise = squigglySudoku' xs map gmap index

    where
        newmap = getNewMap map index x
        next_index = index + 1
        next_candidates = getCandidates newmap gmap (div next_index 9, mod next_index 9)
        rst = squigglySudoku' next_candidates newmap gmap next_index


squigglySudoku :: [Integer] -> [Integer] -> [Integer]
squigglySudoku map gmap = snd (squigglySudoku' (getCandidates map gmap (0, 0)) map gmap 0)



valList = [0,0,3,0,0,0,2,0,5,0,0,0,9,0,5,0,0,7,5,8,0,3,0,9,0,1,4,0,0,1,0,0,4,0,0,0,9,0,0,5,1,2,0,0,3,0,0,0,7,0,0,9,0,0,7,5,0,2,0,1,0,6,9,3,0,0,6,0,7,0,0,0,1,0,6,0,0,0,8,0,0]
blkList = [1,1,2,2,2,3,3,3,3,1,1,2,1,2,2,2,3,3,1,1,1,1,5,2,2,3,3,4,4,4,4,5,5,6,6,3,4,4,4,5,5,5,6,6,6,7,4,4,5,5,6,6,6,6,7,7,8,8,5,9,9,9,9,7,7,8,8,8,9,8,9,9,7,7,7,7,8,8,8,9,9]

