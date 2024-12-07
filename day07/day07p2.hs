hasSolution :: Int -> Int -> [Int] -> Bool
hasSolution ans soFar [] = ans == soFar
hasSolution ans soFar (n:ns) =
  hasSolution ans (soFar*n) ns
  || hasSolution ans (soFar+n) ns 
  || hasSolution ans (soFar * 10^log10 + n) ns
  where log10 = 1 + floor (logBase 10 (fromIntegral n :: Float))

lineVal :: [Int] -> Int
lineVal (ans:n:ns) = if hasSolution ans n ns then ans else 0

readNums :: String -> [Int]
readNums line =
    map (read :: String->Int) (init ans:ns)
    where (ans:ns) = words line

main :: IO()
main = do
 inp <- getContents 
 print $ sum (map (lineVal . readNums) (lines inp))
