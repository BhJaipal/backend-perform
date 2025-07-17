local codes = {
	[100] = "Continue",
	[200] = "OK",
	[201] = "Created",
	[202] = "Accepted",
	[204] = "No Content",
	-- errors
	[400] = "Bad Request",
	[401] = "Unauthorized",
	[403] = "Forbidden",
	[404] = "Not Found",
	[405] = "Method Not Allowed",
	[406] = "Not Acceptable",
	[408] = "Request Timeout",
	[500] = "Internal Server Error",
	[501] = "Not Implemented",
	[502] = "Bad Gateway",
	[503] = "Service Unavailable",
	-- 300s
	[301] = "Moved Permanently",
	[302] = "Found",
	[304] = "Not Modified",
	[307] = "Temporary Redirect",
	[308] = "Permanent Redirect",
}

---@param data table
function JsonParse(data)
	local isArray = false
	local first = true

	---@type string
	local str = ""
	for key, value in pairs(data) do
		if first then
			if type(key) == "number" then
				isArray = true
				str = "["
			else
				str = '{"' .. key .. '":'
			end
			if type(value) == "string" then
				str = str .. '"' .. value .. '"'
			elseif type(value) == "number" or type(value) == "boolean" then
				str = str .. value
			elseif type(value) == "nil" then
				str = str .. "null"
			else
				str = str .. JsonParse(value)
			end
			first = false
			print("[Lua]: " .. str)
			goto continue
		end

		str = str .. ","
		if not isArray then
			str = str .. '"' .. key .. '":'
		end

		if type(value) == "string" then
			str = str .. '"' .. value .. '"'
		elseif type(value) == "number" or type(value) == "boolean" then
			str = str .. value
		elseif type(value) == "nil" then
			str = str .. "null"
		else
			str = str .. JsonParse(value)
		end
		::continue::
	end
	print("[Lua]: " .. str)

	if isArray then
		str = str .. "]"
	else
		str = str .. "}"
	end

	return str
end

---@param code integer
---@param data table
---@param headers table<string, string>
function WriteJson(code, data, headers)
	headers["Content-Type"] = "application/json"

	-- json = JsonParse(data)
	-- print(json)

	Write(code, "" .. data, headers)
end

---@param code integer
---@param data string
---@param headers table<string, string>
function Write(code, data, headers)
	local output = "HTTP/1.1 " .. code .. codes[code] .. "\r\n"
	for key, value in pairs(headers) do
		output = output .. key .. ": " .. value .. "\r\n"
	end
	output = output .. "\r\n" .. data
	write(output)
end

if socket() < 0 then
	print("[Lua]: Cannot bind socket")
	os.exit(1)
end
--[[
if bind(8000) < 0 then
	print("[Lua]: Cannot bind server, port is probably busy")
	os.exit(1)
end
--]]
function Listen()
	---@class Request
	---@field path string
	---@field method string
	---@field body string | nil
	---@field Allow string | nil
	---@field Accept string | nil
	---@field Accept-Charset string | nil
	---@field Accept-Encoding string | nil
	---@field Accept-Language string | nil
	---@field Access-Control-Allow-Origin string | nil
	---@field Access-Control-Allow-Credentials string | nil
	---@field Access-Control-Expose-Headers string | nil
	---@field Access-Control-Max-Age string | nil
	---@field Access-Control-Allow-Methods string | nil
	---@field Access-Control-Allow-Headers string | nil
	---@field Content-Type string | nil
	---@field Content-Location string | nil
	---@field Content-Length string | nil
	---@field Content-Language string | nil
	---@field Content-Encoding string | nil
	---@field Connection string | nil
	---@field Cache-Control string | nil
	---@field Date string | nil
	---@field Proxy-Authenticate string | nil
	---@field Upgrade string | nil
	---@field Server string | nil
	---@field Set-Cookie string | nil
	---@field WWW-Authenticate string | nil

	---@type Request
	local req = read()

	print("[Lua]: ", req.method .. req.path)

	if req.path == "/" then
		-- write("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello World")
		Write(200, "Hello World", { ["Content-Type"] = "text/plain" })
	elseif req.path == "/data" then
		WriteJson(200, {
			["name"] = "Jaipal",
		}, { ["Content-Type"] = "text/plain" })
	else
		Write(404, "'" .. req.path .. "' Not Found", {})
	end
end

-- listen(5)
