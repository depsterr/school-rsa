{-# LANGUAGE ForeignFunctionInterface #-}

module Safe where

import Foreign.C.Types
import System.Exit

--- C wrapper functions needed to convert to C types
--- From haskell Integers.
rsaCryptHs :: CULong -> CULong -> CULong -> CULong
rsaCryptHs base pow m = fromInteger $ _rsaCryptHs (toInteger base) (toInteger pow) (toInteger m)
findDHs :: CULong -> CULong -> CULong
findDHs e phi = fromInteger $ modInv (toInteger e) (toInteger phi)

-- Given a and m, return x such that (a * x) % m  = 1 and x > 0
modInv :: Integer -> Integer -> Integer
modInv a m = mkPos i
  where
    (i, _, _) = gcdExt a m
    mkPos x
      | x < 0 = x + m
      | otherwise = x
 
-- Extended Euclidean algorithm.
-- Given non-negative a and b, return x, y and g
-- such that ax + by = g, where g = gcd(a,b).
-- Note that x or y may be negative.
gcdExt :: Integer -> Integer -> (Integer, Integer, Integer)
gcdExt a 0 = (1, 0, a)
gcdExt a b =
  let (q, r) = a `quotRem` b
      (s, t, g) = gcdExt b r
  in (t, s - q * t, g)

-- Fast power with modulo. Returns (a ^ b) % c
_rsaCryptHs :: Integer -> Integer -> Integer -> Integer
_rsaCryptHs base 1 m = base `mod` m
_rsaCryptHs base pow m | even pow = ((_rsaCryptHs base (div pow 2) m) ^ 2) `mod` m
                       | odd  pow = ((_rsaCryptHs base (div (pow-1) 2) m) ^ 2 * base) `mod` m


-- Export functions to C code
foreign export ccall rsaCryptHs :: CULong -> CULong -> CULong -> CULong
foreign export ccall findDHs :: CULong -> CULong -> CULong
