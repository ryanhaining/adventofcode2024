import Data.Map (fromListWith, findWithDefault)

similarity :: [[Int]] -> Int
similarity ns =
  sum (map (\n -> n*(findWithDefault 0 n counts)) nums)
  where
    nums = map (!!0) ns
    counts = fromListWith (+)  (map (\n -> (n!!1,1)) ns)

readPairs :: String -> [Int]
readPairs = map (read :: String->Int) . words

main :: IO()
main = do
  inp <- getContents
  print $ similarity (map  readPairs (lines inp))
