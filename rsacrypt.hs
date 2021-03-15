{-# LANGUAGE ForeignFunctionInterface #-}

module Safe where

import Foreign.C.Types

rsaCryptHs :: CULong -> CULong -> CULong -> CULong
rsaCryptHs base pow m = fromInteger $ _rsaCryptHs (toInteger base) (toInteger pow) (toInteger m)

_rsaCryptHs :: Integer -> Integer -> Integer -> Integer
_rsaCryptHs base 1 m = base `mod` m
_rsaCryptHs base pow m | even pow = ((_rsaCryptHs base (div pow 2) m) ^ 2) `mod` m
                   | odd  pow = ((_rsaCryptHs base (div (pow-1) 2) m) ^ 2 * base) `mod` m

foreign export ccall rsaCryptHs :: CULong -> CULong -> CULong -> CULong
