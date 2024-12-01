import Data.List (sort)

absDiff :: (Int, Int)-> Int
absDiff (n1, n2) = abs (n1 - n2)

distance :: [[Int]] -> Int
distance ns =
  sum (map absDiff (zip left right))
  where
    left = sort (map (!!0) ns)
    right = sort (map (!!1) ns)

main :: IO()
main = do
  inp <- getContents
  print $ distance (map  (map (read :: String->Int) . words) (lines inp))
