from build import mustache

# assault_cube, version 1.3.0.2 - test

mm = mustache.Memory("ac_client.exe")

# must return address -> 0x4c73ef
print(hex(mm.find_signature("ac_client.exe", "ff 08 8d 44 24")))
