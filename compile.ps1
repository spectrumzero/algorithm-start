# generate compile to exe files
$currentDirectory = Get-Location
$cmakeListsPath = Join-Path -Path $currentDirectory -ChildPath "CmakeLists.txt"

if (-not (Test-Path $cmakeListsPath))
{
  Write-Host("No CmakeLists.txt in current directory, please check.")
  return
}

Write-Host "Start generating and compiling..."

$buildFolderPath = ".\build"

if (-not (Test-Path $buildFolderPath))
{
  New-Item -ItemType Directory -Path $buildFolderPath | Out-Null
  Write-Host "build folder created."
}

# 指定生成visual studio项目
cmake -G "Visual Studio 17 2022" -A x64 -S . -B ./build/

if ($LASTEXITCODE -eq 0)
{
  cmake --build ./build/ --config DEBUG
}
