from build import mustache

# assault_cube, version 1.3.0.2 - test

mm = mustache.Memory("ac_client.exe")

# must return address -> 0x4c73ef
print(hex(mm.find_signature("ac_client.exe", "ff 08 8d 44 24")))

module_data = mm.get_module_info("ac_client.exe")

lp = mm.read_ptr(mm.get_module_base() + 0x18AC00)
print(lp)
vm = mm.read_view_matrix(lp + 0x017DFFC)
print(vm[0])