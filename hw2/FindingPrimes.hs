findingPrimes :: Int -> Int -> [Int]
findingPrimes n m = take m (filterPrime [n..]) where
  filterPrime (p:xs) = p : filterPrime [x | x <- xs, x `mod` p /= 0]
