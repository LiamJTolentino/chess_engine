$RootDir = Resolve-Path "$PSScriptRoot\.."
$BuildDir = Join-Path $RootDir "build\windows"

cmake -S $RootDir -B $BuildDir `
    -DCMAKE_BUILD_TYPE=Release

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

cmake --build $BuildDir --config Release

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}