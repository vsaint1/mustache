from build import mustache

mouse = mustache.Mouse()

while True:
    print(f"Mouse position {mouse.get_pos_x()}, {mouse.get_pos_y()}\n")
    if mustache.key_pressed(0x05):
        mouse.move(100,200)
