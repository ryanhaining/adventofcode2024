getLen' :: Int -> Int -> Int
getLen' n 0 = 1
getLen' 0 blinks = getLen' 1 (blinks-1)
getLen' n blinks
  | even log10 = getLen' (n `div` denom) (blinks-1) + getLen' (n `mod` denom) (blinks-1)
  | otherwise = getLen' (n*2024) (blinks-1)
  where log10 = ceiling (logBase 10 (fromIntegral n :: Float) + 0.00001)
        denom = 10^(log10 `div` 2)

getLen :: Int -> Int
getLen = flip getLen' 25


main :: IO()
main = do
  inp <- getContents
  print $ sum (map getLen (map (read :: String -> Int) (words inp)))
