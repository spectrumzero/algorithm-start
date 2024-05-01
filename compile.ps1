# 获取当前脚本所在的目录
$currentDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# 构建可执行文件输出目录路径
$outputDir = Join-Path -Path $currentDir -ChildPath "..\executable"

# 创建输出目录（如果不存在）
if (-not (Test-Path -Path $outputDir)) {
    New-Item -Path $outputDir -ItemType Directory | Out-Null
}

# 获取当前缓冲区文件的路径
$currentBufferFile = $ExecutionContext.SessionState.Path.CurrentFileSystemLocation.Path

# 检查当前文件是否为C源文件
if ($currentBufferFile -match "\.c$") {
    # 提取源文件的文件名（不带扩展名）
    $sourceFileName = [System.IO.Path]::GetFileNameWithoutExtension($currentBufferFile)
    
    # 设置可执行文件路径
    $outputFile = Join-Path -Path $outputDir -ChildPath "$sourceFileName.exe"
    
    # 编译C源文件
    gcc $currentBufferFile -o $outputFile
    
    # 输出提示信息
    Write-Host "已生成可执行文件: $outputFile"
} else {
    Write-Host "当前文件不是C源文件。"
}
