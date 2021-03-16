{-# LANGUAGE ForeignFunctionInterface #-}

module Safe where

import Foreign.C.Types
import System.Exit

--- "Wrapper" funktioner som konverterar mellans haskells Integer datatyper
--- som kan vara oändligt stora till Cs unsigned long, som max är 64 bitar.
rsaCryptHs :: CULong -> CULong -> CULong -> CULong
rsaCryptHs base pow m = fromInteger $ _rsaCryptHs (toInteger base) (toInteger pow) (toInteger m)
findDHs :: CULong -> CULong -> CULong
findDHs e phi = fromInteger $ modInv (toInteger e) (toInteger phi)

--- Givet a och m, returnera x såpass att (a * x) % m  = 1 och x > 0
modInv :: Integer -> Integer -> Integer
modInv a m = mkPos i
  where
    (i, _, _) = gcdExt a m
    mkPos x --- Lägg till en till m om x är negativt, behövs då euklides algorithm har negativa resultat.
      | x < 0 = x + m
      | otherwise = x
 
-- Utökad euklides algorithm
-- Givet positiva a and b, returnera x, y och g
-- såpass att a * x + b * y = g, där g = gcd(a,b).
-- Både x och y kan vara negativa.
gcdExt :: Integer -> Integer -> (Integer, Integer, Integer)
gcdExt a 0 = (1, 0, a)
gcdExt a b =
  let (q, r) = a `quotRem` b --- quotient och remainer = a / b, a % b
      (s, t, g) = gcdExt b r --- recursera, s = förra resten, t = resten, g = gcd
  in (t, s - q * t, g)

-- En snabb metod för att räkna ut potenser med modulo.
-- Detta funkar genom att dela in varje potens i en
-- serie av potenser av två, vilket gör att man kan
-- skippa uträkningar. För mer info kring hur detta funkar
-- kolla in https://dev-notes.eu/2019/12/Fast-Modular-Exponentiation/
_rsaCryptHs :: Integer -> Integer -> Integer -> Integer
_rsaCryptHs base 1 m = base `mod` m
_rsaCryptHs base pow m | even pow = ((_rsaCryptHs base (div pow 2) m) ^ 2) `mod` m
                       | odd  pow = ((_rsaCryptHs base (div (pow-1) 2) m) ^ 2 * base) `mod` m


-- Exportera funktioner till min C kod
foreign export ccall rsaCryptHs :: CULong -> CULong -> CULong -> CULong
foreign export ccall findDHs :: CULong -> CULong -> CULong
