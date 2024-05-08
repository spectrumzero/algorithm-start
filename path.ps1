# 创建一个临时文件来存储 Neovim 缓冲区的文件路径
$tempFile = [System.IO.Path]::GetTempFileName()

# 使用 Neovim 执行命令并将结果写入临时文件
& nvim --headless --clean --cmd "redir! > $tempFile" --cmd "echo expand('%')" --cmd "redir END" --cmd "quitall!"

# 读取临时文件中的内容（文件路径）
$currentFilePath = Get-Content $tempFile

echo $currentFilePath

# 删除临时文件
Remove-Item $tempFile

