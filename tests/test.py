from build import mustache

# cs2 - test

mm = mustache.Memory("cs2.exe",None)

module_data = mm.get_module_info("client.dll")

print(f"ProcessId: {mm.get_process_id()}")
print(f"ModuleBase: {module_data[0]} ModuleSize: {module_data[1]}")

local_player = mm.read_ptr(mm.get_module_base() + 0x16C8F58)
health = mm.read_int(local_player + 0x32C)
print(f"Health: {health}")


