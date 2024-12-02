inRange :: Int -> Int -> Bool
inRange n1 n2 =
  0 < diff && diff < 4
  where
    diff = abs(n1 - n2)

isCorrect :: Int -> Int -> Bool -> Bool
isCorrect n1 n2 isAscending =
  (inRange n1 n2) && ((n1 < n2) == isAscending)

isSafe' :: [Int] -> Bool -> Bool
isSafe' [_] _ = True
isSafe' (n1:n2:ns) isAscending =
  isCorrect n1 n2 isAscending && isSafe' (n2:ns) isAscending

isSafe :: [Int] -> Bool
isSafe (n1:n2:ns) =
  isSafe' (n1:n2:ns) (n1 < n2)

main :: IO()
main = do
  inp <- getContents
  print $ length (filter isSafe (map  (map (read :: String -> Int) . words) (lines inp)))
