inRange :: Int -> Int -> Bool
inRange n1 n2 =
  0 < diff && diff < 4
  where
    diff = abs(n1 - n2)

isCorrect :: Int -> Int -> Int -> Bool -> Bool
isCorrect n1 n2 n3 isAscending =
  (inRange n1 n2) && (inRange n2 n3) && ((n1 < n2) == (n2 < n3) && (n1 < n2) == isAscending)

isSafe'' :: [Int] -> Bool -> Bool
isSafe'' [_,_] _ = True
isSafe'' (n1:n2:n3:ns) isAscending =
  isCorrect n1 n2 n3 isAscending && isSafe'' (n2:n3:ns) isAscending

isSafe' :: [Int] -> Bool -> Bool
isSafe' [_,_] _ = True
isSafe' (n1:n2:n3:ns) isAscending=
  (isCorrect n1 n2 n3 isAscending && isSafe' (n2:n3:ns) isAscending)
  || isSafe'' (n1:n3:ns) isAscending
  || isSafe'' (n1:n2:ns) isAscending

-- I really want to not have to special case the beginning of the list.
isSafe :: [Int] -> Bool
isSafe (n0:n1:n2:ns)
  | isCorrect n0 n1 n2 (n0 < n1) = isSafe' (n0:n1:n2:ns) (n0 < n1)
  | otherwise = isSafe'' (n1:n2:ns) (n1 < n2) || isSafe'' (n0:n2:ns) (n0 < n2)

main :: IO()
main = do
  inp <- getContents
  print $ length (filter isSafe (map  (map (read :: String -> Int) . words) (lines inp)))
