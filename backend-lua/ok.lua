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
				if value == true then
					str = str .. "true"
				elseif value == false then
					str = str .. "false"
				else
					str = str .. value
				end
			elseif type(value) == "nil" then
				str = str .. "null"
			else
				str = str .. JsonParse(value)
			end
			first = false
			goto continue
		end

		str = str .. ","
		if not isArray then
			str = str .. '"' .. key .. '":'
		end

		if type(value) == "string" then
			str = str .. '"' .. value .. '"'
		elseif type(value) == "number" or type(value) == "boolean" then
			if value == true then
				str = str .. "true"
			elseif value == false then
				str = str .. "false"
			else
				str = str .. value
			end
		elseif type(value) == "nil" then
			str = str .. "null"
		else
			str = str .. JsonParse(value)
		end
		::continue::
	end

	if isArray then
		str = str .. "]"
	else
		str = str .. "}"
	end

	return str
end

print(JsonParse({
	name = "Jaipal",
	age = 20,
	is_alive = true,
	hobbies = { "eating", "coding" },
	ok = {
		read = false,
	},
}))
