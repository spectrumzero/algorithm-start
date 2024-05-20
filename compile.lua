print("helloworld!")
-- 调整 package.path 和 package.cpath 以包含 LuaFileSystem 的安装路径
package.path = package.path
	.. ";C:\\Users\\admin\\scoop\\apps\\luarocks\\current\\share\\lua\\5.4\\?.lua;C:\\Users\\admin\\scoop\\apps\\luarocks\\current\\share\\lua\\5.4\\?\\init.lua;C:\\Users\\admin\\scoop\\apps\\luarocks\\current\\rocks\\lib\\lua\\5.4\\lfs.dll"

local lfs = require("lfs")

-- 获取当前目录下所有 C 源文件
local function get_source_files()
	local source_files = {}
	for file in lfs.dir(".") do
		if file:match("%.c$") then
			table.insert(source_files, file)
		end
	end
	return source_files
end

-- 编译单个 C 源文件
local function compile_file(file)
	local output_file = file:gsub("%.c$", ".exe") -- Windows 下编译生成 .exe 文件
	local command = string.format("gcc %s -o %s", file, output_file)
	print("编译命令: " .. command)
	local result = os.execute(command)
	if result == 0 then
		print("编译成功: " .. file)
	else
		print("编译失败: " .. file)
	end
end

-- 主函数
local function main()
	local source_files = get_source_files()
	if #source_files == 0 then
		print("没有找到源文件。")
		return
	end

	for _, file in ipairs(source_files) do
		compile_file(file)
	end
end

main()

local lfs = require("lfs")

-- local function get_source_files()
-- 	local source_files = {}
-- 	for file in lfs.dir(".") do
-- 		if file:match("%.c$") or file:match("%.cpp$") then
-- 			table.insert(source_files, file)
-- 		end
-- 	end
-- 	return source_files
-- end
--
-- local function compile_file(file)
-- 	local output_file = file:gsub("%.[^.]+$", "")
-- 	local command
-- 	if file:match("%.c$") then
-- 		command = "gcc " .. file .. " -o " .. output_file
-- 	elseif file:match("%.cpp$") then
-- 		command = "g++ " .. file .. " -o " .. output_file
-- 	else
-- 		return
-- 	end
-- 	print("编译命令: " .. command)
-- 	local result = os.execute(command)
-- 	if result == 0 then
-- 		print("编译成功: " .. file)
-- 	else
-- 		print("编译失败: " .. file)
-- 	end
-- end
--
-- local function main()
-- 	local source_files = get_source_files()
-- 	if #source_files == 0 then
-- 		print("没有找到源文件。")
-- 		return
-- 	end
--
-- 	for _, file in ipairs(source_files) do
-- 		compile_file(file)
-- 	end
-- end
--
-- main()
