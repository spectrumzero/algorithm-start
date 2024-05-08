#打印 helloworld
write-host "Hello world"


# 设置源文件和输出文件的路径
$sourceFile = ".\chap1_arrayandlinkedlist\array.c"
$outputFile = ".\chap1_arrayandlinkedlist\array.exe"
$anotherexefile =".\output\hashmap_array.exe"



# 调用 GCC 编译器编译源文件
clang $sourceFile -o $outputFile

# 检查编译是否成功
if ($LASTEXITCODE -eq 0)
{
  Write-Host "Compilation successful. Output file: $outputFile"
  # 编译成功后就运行源文件
  #
  # 在外部控制台输出会闪退，直接在终端输出
  # Start-Process $outputFile 
  & $outputFile
    


} else
{
  Write-Host "Compilation failed."
}

#运行编译后的源文件
