import Data.List (sort)

absDiff :: (Int, Int)-> Int
absDiff (n1, n2) = abs (n1 - n2)

distance :: ([Int], [Int]) -> Int
distance (left, right) =
  sum (map absDiff (zip (sort left) (sort right)))

readPairs :: String -> [Int]
readPairs = map (read :: String->Int) . words

splitLeftRight :: [[Int]] -> ([Int], [Int])
splitLeftRight = foldl (\(left, right) [n1, n2] -> (n1:left, n2:right)) ([], [])

main :: IO()
main = do
  inp <- getContents
  print $ distance (splitLeftRight (map readPairs (lines inp)))
