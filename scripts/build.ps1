param(
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Debug",  # param $Config is Debug by default
    [int]$Jobs = 2,
    [string]$Generator = "MinGW Makefiles",
    [switch]$Clean,
    [switch]$Run
)

$BuildDir = "build"
$ExeTest  = "slist_test.exe"

Write-Host "Configuration: $Config" -ForegroundColor Magenta

if ($Clean -and (Test-Path $BuildDir)) {
    Write-Host "Removing build directory..." -ForegroundColor Magenta
    Remove-Item $BuildDir -Recurse -Force
}

Write-Host "Configuring project..." -ForegroundColor Magenta
cmake -S . -B $BuildDir -G $Generator 
"-DCMAKE_BUILD_TYPE=$Config"
# -DCMAKE_C_COMPILER=gcc 
# -DCMAKE_CXX_COMPILER=g++

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configure failed." -ForegroundColor Red
    exit $LASTEXITCODE
}

cmake --build $BuildDir --verbose --parallel $Jobs

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed." -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host "Build completed successfully." -ForegroundColor Green

if ($Run) {
    $ExePath = Join-Path $BuildDir $ExeTest
    if (Test-Path $ExePath) {
        Write-Host "Running $ExePath..." -ForegroundColor Magenta
        & $ExePath
        exit 0
    }
    Write-Host "Executable not found: $ExePath." -ForegroundColor Red
    exit 1
}