import idc

count = 0
arr = []
for i in XrefsTo(0x112011):
    if count % 2 == 0:
        arr.append(i.frm)
    count += 1

char = []
for i in arr:
    char.append('0x'+print_operand(i,1)[:-1])

for i in range(len(char)):
    print(chr(int(char[i],16)^i),end='')