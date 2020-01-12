call .\StampMe.cmd .\build\steps.pyd
call g++ -Wall -O3 -mavx2 -ffast-math -shared -fPIC ^
-DXTENSOR_USE_XSIMD ^
-IC:\Users\Ashot\Miniconda3\include ^
-IC:\Users\Ashot\Miniconda3\Lib\site-packages\numpy\core\include ^
-IC:\mingw-w64\xsimd\include ^
-IC:\mingw-w64\xtensor\include ^
-IC:\mingw-w64\xtensor-blas\include ^
-IC:\mingw-w64\xtensor-python\include ^
-IC:\mingw-w64\xtl\include ^
-IC:\mingw-w64\pybind11\include ^
-LC:\Users\Ashot\Miniconda3\libs ^
make_module.cpp ^
-lpython37 ^
-o build/steps.pyd

