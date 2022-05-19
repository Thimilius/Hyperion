require('vstudio')

local p = premake

p.api.register {
  name = "dynamicloading",
  scope = "config",
  kind = "boolean",
  default = false
}

premake.override(premake.vstudio.cs2005.elements, "projectProperties", function(base, cfg) 
  local calls = base(cfg)
  table.insert(calls, function(cfg)
    if cfg.dynamicloading then
        p.w('<EnableDynamicLoading>true</EnableDynamicLoading>')
    end
  end)
  return calls
end)
