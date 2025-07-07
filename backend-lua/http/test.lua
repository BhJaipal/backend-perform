---@param body string
---@param headers table<string, string>
---@param code integer
function Write(body, headers, code)
	if code == nil then
		code = 200
	end

	local output = "HTTP/1.1 " .. code .. status[code] .. "\r\n"
	local hasType = false
	for key, value in pairs(headers) do
		if key == "Content-Type" then
			hasType = true
		end
		output = output .. key .. ": " .. value .. "\r\n"
	end
	-- default Content-Type is text/plain
	if not hasType then
		output = output .. "Content-Type: text/plain\r\n"
	end
	output = output .. "\r\n" .. body
	write(output)
end

---@param data table
---@param headers table<string, string>
---@param code integer
function Json(data, headers, code)
	headers["Content-Type"] = "application/json"
	Write(data.__tostring(), headers, code)
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
	Write("[Lua]: Hello World from " .. req.method .. req.path .. "\n", {}, 200)
end

-- Route_some not declared so 404 Not Found

-- Start server at port specified
Serve(8080)
