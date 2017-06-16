flipCoin' :: String -> Integer -> [Integer]
flipCoin' [] i = error "empty string"
flipCoin' [c] i
    | c == 'H' 	= [0]
    | c == 'T' 	= [i, 0]
    | otherwise = error "invalid character"
flipCoin' (c:xs) i
    | c == c2 	= flipCoin' xs (i + 1)
    | otherwise = [i] ++ flipCoin' xs (i + 1)
    where c2 = head xs


flipCoin :: String -> [Integer]
flipCoin s = flipCoin' s 1