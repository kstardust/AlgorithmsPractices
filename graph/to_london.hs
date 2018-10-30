import System.IO

data Section = Section {getA :: Int, getB :: Int, getC :: Int} deriving (Show)
type RoadSystem = [Section]

data Label = A | B | C deriving (Show)
type Path = [(Label, Int)]

groupOf :: Int -> [a] -> [[a]]
groupOf _ [] = []
groupOf n xs = take n xs: groupOf n (drop n xs)

roadStep :: (Path, Path) -> Section -> (Path, Path)
roadStep (pathA, pathB) (Section a b c) =
  let sumA = sum $ map snd pathA
      sumB = sum $ map snd pathB
      newCrossToA = sumB + b + c
      newForwardToA = sumA + a
      newCrossToB = sumA + a + c
      newForwardToB = sumB + b
      newPathToA = if newCrossToA < newForwardToA
                   then
                     (C, c):(B, b):pathB
                   else
                     (A, a):pathA
      newPathToB = if newCrossToB < newForwardToB
                   then
                     (C, c):(A, a):pathA
                   else
                     (B, b):pathB
  in (newPathToA, newPathToB)

optimalPath :: RoadSystem -> Path
optimalPath roadSystem =
  let (pathA, pathB) = foldl roadStep ([], []) healthrowToLondon
  in if sum (map snd pathA) <= sum (map snd pathB)
     then
       reverse pathA
     else
       reverse pathB

main = do
  content <- getContents
  let threes = groupOf 3 $ map read $ lines content
      optimal = optimalPath $ map (\[x, y, z] -> Section x y z) threes
      bestPath = concat $ map (show . fst) optimal
      bestPrice = sum $ map snd optimal
  putStrLn $ "Best Path: " ++ show bestPath
  putStrLn $ "Best Price: " ++ show bestPrice
