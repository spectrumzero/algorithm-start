module = {}

module.cons = "这是一个常量"

function module.func1()
	io.write("这是一个公有函数！\n")
end

local function func2()
	print("这是一个私有函数！\n")
end

local function func3()
	print("helloworld!")
end

function module.func4()
	module.func2() -- 调用模块内的私有函数
	module.func3() -- 调用模块内的私有函数
end

return module
-- local module = {}
--
-- module.cons = "这是一个常量"
--
-- function module.func1()
-- 	io.write("这是一个公有函数！\n")
-- end
--
-- local function module.func2()
-- print("这是一个私有函数！\n")
-- end
--
-- local function module.func3()
-- print("helloworld!")
-- end
--
-- function module.func4()
-- func2()
-- func3()
-- end
--
-- return module
--
