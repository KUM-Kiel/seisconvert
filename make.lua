local function filename(path)
  local i, j, m = string.find(path, "([^/]+)$")
  return m
end

local function folder(path)
  local i, j, m = string.find(path, "^(.*/)")
  return m or "./"
end

local function resolve(path, from)
  local p = {}
  for s in string.gmatch(from, "[^/]+") do
    p[#p + 1] = s
  end
  for s in string.gmatch(path, "[^/]+") do
    if s == ".." then
      p[#p] = nil
    elseif s ~= "." then
      p[#p + 1] = s
    end
  end
  return table.concat(p, "/")
end

return function()
  local _libs = {}
  local _objects = {}
  local _targets = {}
  local _programs = {}
  local _includes = ""
  local _all = ""

  local function lib(m, name, objects)
    local thelib = "build/lib" .. name .. ".a"
    _all = _all .. " " .. thelib
    local l = {
      target = thelib,
      objects = {}
    }
    for i, o in ipairs(objects) do
      local d = {}
      for j, v in ipairs(o.deps) do
        d[j] = resolve(v, "src/" .. name)
      end
      local theobject = "build/objects/" .. name .. "/" .. o.target
      _objects[#_objects + 1] = {
        target = theobject,
        source = "src/" .. name .. "/" .. o.source,
        deps = d
      }
      _targets[#_targets + 1] = theobject
      l.objects[#l.objects + 1] = theobject
    end
    _includes = _includes .. " -Isrc/" .. name
    _targets[#_targets + 1] = thelib
    _libs[#_libs + 1] = l
    _libs[name] = l
  end

  local function program(m, name, objects, libs)
    local theprogram = "build/" .. name
    _programs[name] = true
    _all = _all .. " " .. theprogram
    local p = {
      target = theprogram,
      objects = {},
      libs = libs
    }
    for i, o in ipairs(objects) do
      local d = {}
      for j, v in ipairs(o.deps) do
        d[j] = resolve(v, "src")
      end
      local theobject = "build/objects/" .. o.target
      _objects[#_objects + 1] = {
        target = theobject,
        source = "src/" .. o.source,
        deps = d
      }
      _targets[#_targets + 1] = theobject
      p.objects[#p.objects + 1] = theobject
    end
    _programs[#_programs + 1] = p
  end

  local function c(m, name, deps)
    local d = {name .. ".c"}
    for i, v in ipairs(deps) do
      d[i + 1] = v
    end
    return {
      name = name,
      target = name .. ".o",
      source = name .. ".c",
      deps = d
    }
  end

  local function gen(m, name)
    local f = io.open(name, "w")
    f:write("# This file was automatically generated. Do not edit!\n")
    f:write("COMPILE = $(CC) -c -Wall -pedantic -O2 -std=c99" .. _includes .. "\n")
    f:write("LINK = $(CC) -Lbuild -o\n")
    f:write("MAKELIB = $(AR) rcs\n\n")
    f:write("TARGETS = " .. table.concat(_targets, " ") .. "\n\n")
    f:write("all:" .. _all .. "\n")
    for i, p in ipairs(_programs) do
      local l = ""
      local d = ""
      for i, v in ipairs(p.libs) do
        if _libs[v] then
          d = d .. " " .. _libs[v].target
        end
        l = l .. " -l" .. v
      end
      f:write(p.target .. ": " .. table.concat(p.objects, " ") .. d .. " " .. name .. "\n")
      f:write("\t@echo [LD] " .. p.target .. "\n")
      f:write("\t@mkdir -p " .. folder(p.target) .. "\n")
      f:write("\t@$(LINK) " .. p.target .. " " .. table.concat(p.objects, " ") .. l .. "\n")
    end
    for i, l in ipairs(_libs) do
      f:write(l.target .. ": " .. table.concat(l.objects, " ") .. " " .. name .. "\n")
      f:write("\t@echo [AR] " .. l.target .. "\n")
      f:write("\t@mkdir -p " .. folder(l.target) .. "\n")
      f:write("\t@$(MAKELIB) " .. l.target .. " " .. table.concat(l.objects, " ") .. "\n")
    end
    for i, o in ipairs(_objects) do
      f:write(o.target .. ": " .. table.concat(o.deps, " ") .. " " .. name .. "\n")
      f:write("\t@echo [CC] " .. o.target .. "\n")
      f:write("\t@mkdir -p " .. folder(o.target) .. "\n")
      f:write("\t@$(COMPILE) -o " .. o.target .. " " .. o.source .. "\n")
    end
    f:write("clean:\n\trm -rf build\n")
  end

  return {
    lib = lib,
    program = program,
    c = c,
    gen = gen
  }
end
