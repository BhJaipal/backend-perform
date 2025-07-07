---@param data table
---@param headers table<string, string>
---@param code integer
function Json(data, headers, code)
	headers["Content-Type"] = "application/json"
	output = ""
	for key, value in pairs(headers) do
		output = output .. key .. ": " .. value
	end
	output = output .. "\r\n"
	write(data.__tostring(), output, code)
end

---@param path string
function Add(path)
	add_route(path)
end

---@param port integer
function Serve(port)
	start(port)
end

-- NOTE:Type Alias for callback fn

---@class Request
---@field method string
---@field path string

-- Routes
Add("/")
-- _ is replaced for - and / in callback function name

Add("/some")
-- NOTE:
-- for function, replace each - and / with _ and add 'Route' at start of function declaration
-- So, for /: Route_
-- for /some: Route_some

-- NOTE: Callback functions

---@param req Request
function Route_(req)
	write("[Lua]: Hello World from " .. req.method .. req.path .. "\n", "Content-Type: text/plain", 200)
end

function Route_some(req)
	write('{"some": true}', "Content-Type: application/json", 200)
end

-- Route_some not declared so 404 Not Found

-- Start server at port specified
Serve(8080)
