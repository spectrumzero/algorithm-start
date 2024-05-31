local dap = require("dap")

dap.adapters.gdb = {
	type = "executable",
	command = "gdb",
	args = { "-i", "dap" },
}

dap.configurations.c = {
	{
		name = "Launch",
		type = "gdb",
		request = "launch",
		program = function()
			return vim.fn.input("Path to executable: ", vim.fn.getcwd() .. "/", "file")
		end,
		cwd = "${workspaceFolder}",
		stopAtBeginningOfMainSubprogram = false,
	},
}

-- local dap = require("dap")
--
-- dap.configurations.c = {
-- 	{
-- 		type = "lldb",
-- 		request = "launch",
-- 		name = "Launch file",
-- 		-- program = "${file}",
-- 		program = "E:/lazycoding/Algorithm/chap4_tree/1_binarytree.c",
-- 		cwd = "${workspaceFolder}",
-- 		cPath = function()
-- 			return "E:\\mingw64\\bin"
-- 		end,
-- 	},
-- }
