from build import mustache

# consts

VK_END = 0x23
GLOW_ON = 86400.0
GLOW_OFF = 0.0
B_SPOTTED = 0x1630 + 0xC

# cs2 - test

mm = mustache.Memory("cs2.exe")

module_data = mm.get_module_info("client.dll")

print(f"ProcessId: {mm.get_process_id()}")
print(f"ModuleBase: {module_data[0]} ModuleSize: {module_data[1]}")

local_player = mm.read_ptr(mm.get_module_base() + 0x16D4F48)
health = mm.read_int(local_player + 0x32C)
print(f"Health: {health}")



while not mustache.key_pressed(VK_END):
    local_player = mm.read_ptr(mm.get_module_base() + 0x16D4F48)

    if not local_player:
        continue

    local_team = mm.read_int(local_player + 0x3bf)

    entity_list = mm.read_ptr(mm.get_module_base() + 0x17CE6A0)

    for i in range(64):
        list_entry = mm.read_ptr(entity_list + 0x10)

        if not list_entry:
            continue

        player = mm.read_ptr(list_entry + (i * 0x78))

        if not player:
            continue

        pawn_handle = mm.read_uint(player + 0x7EC)

        controller = mm.read_ptr(entity_list + 0x8 * ((pawn_handle & 0x7FFF) >> 9) + 16)

        if not controller:
            continue

        cs_pawn = mm.read_ptr(controller + 0x78 * (pawn_handle & 0x1FF))

        if cs_pawn == local_player:
            continue

        mm.write_float(cs_pawn + 0x13E4, GLOW_ON)
        mm.write_bool(cs_pawn + B_SPOTTED, True)



# if mustache.key_pressed(0x05):
#     mustache.mouse_move(0x0001,100,200,0,0)

