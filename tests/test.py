from build import mustache

mm = mustache.Memory("cs2.exe")

module_data = mm.get_module_data("ac_client.dll")

print(f"ProcessId: {mm.get_process_id()}")
print(f"ModuleBase: {module_data[0]} ModuleSize: {module_data[1]}")

