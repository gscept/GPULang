
import os


os.makedirs('../generated', exist_ok=True)
open_file = open('../generated/swizzle.h', 'w')

def generate_four():
    components = ['x', 'y', 'z', 'w']
    open_file.write("#define SWIZZLE4()\\\n")
    for x in components:
        open_file.write(f"X(" + x + ")\\\n")
        for y in components:
            open_file.write(f"X(" + x + y + ")\\\n")
            for z in components:
                open_file.write(f"X(" + x + y + z + ")\\\n")
                for w in components:
                    open_file.write(f"X(" + x + y + z + w + ")\\\n")
    components = ['r', 'g', 'b', 'a']
    for x in components:
        open_file.write(f"X(" + x + ")\\\n")
        for y in components:
            open_file.write(f"X(" + x + y + ")\\\n")
            for z in components:
                open_file.write(f"X(" + x + y + z + ")\\\n")
                for w in components:
                    open_file.write(f"X(" + x + y + z + w + ")\\\n")
    open_file.write("\n")

def generate_three():
    components = ['x', 'y', 'z']

    open_file.write("#define SWIZZLE3()\\\n")
    for x in components:
        open_file.write(f"X(" + x + ")\\\n")
        for y in components:
            open_file.write(f"X(" + x + y + ")\\\n")
            for z in components:
                open_file.write(f"X(" + x + y + z + ")\\\n")
    components = ['r', 'g', 'b']
    for x in components:
        open_file.write(f"X(" + x + ")\\\n")
        for y in components:
            open_file.write(f"X(" + x + y + ")\\\n")
            for z in components:
                open_file.write(f"X(" + x + y + z + ")\\\n")
    open_file.write("\n")

def generate_two():
    components = ['x', 'y']
    open_file.write("#define SWIZZLE2()\\\n")
    for x in components:
        open_file.write(f"X(" + x + ")\\\n")
        for y in components:
            open_file.write(f"X(" + x + y + ")\\\n")
    components = ['r', 'g']
    for x in components:
        open_file.write(f"X(" + x + ")\\\n")
        for y in components:
            open_file.write(f"X(" + x + y + ")\\\n")
    open_file.write("\n")

generate_four()
generate_three()
generate_two()

open_file.close()
print("Swizzle definitions generated in swizzle.h")