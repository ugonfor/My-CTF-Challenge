from pwn import *

data = [0x1,0xc,0x90,0xd8,0x6c,0x54,0x60,0x5f,0xf0,0x80,0x92,0xee,0xa0,0x8,0x80,0xc0,0xd8,0xa0,0x79,0x82,0x80,0xdc,0x90,0xd0,0x80,0x2e,0x36,0x60,0x88,0xc0,0xc4,0xf0,0x9a,0xd0,0x40,0x9c,0x78,0xbf,0xee,0x94,0x60,0x98,0xb0,0x96,0xb0,0x80,0x80,0xe0,0x80,0xf6,0x18,0x5c,0xed,0x40,0x80,0xe4,0x80,0xba,0x40,0xea,0x88,0xfc,0xc0,0x20,0x40,0x60,0x7,0x80,0x80,0x60,0x80,0xbe,0x70,0xbc,0xe0,0xb8,0xd8,0x40,0xf8,0x8e,0x40,0x38,0x84,0x52,0x40,0x9e,0x8b,0xbe,0xe6,0x60,0xd0,0xc2,0x40,0x80,0x29,0xdc,0x72,0xc0,0xe0,0x60,0x80,0xf0,0x38,0x96,0xc0,0x80,0x80,0xd4,0x60,0x96,0xb4,0x16,0xe0,0x40,0xf0,0x10,0xa8,0xa8,0xc0,0xc0,0xf8,0x85,0x8,0x80,0xc0,0x30,0x77,0x80,0x86,0x2d,0xe0,0x10,0x80,0x40,0xe1,0x40,0xc0,0x4,0x60,0x40,0xc0,0xee,0x1e,0xd0,0x30]
flag = "CYKOR{I'm_SORRY_About_This_Dirty_Challenge_haha_But_isn't_it_Funny?_haha}_haha_here_is_outside_of_flag!!_only_for_padding"

print(len(data))
print(len(flag))

data = data[-121:]
print(len(data))
print(data)

xor = []
for i in range(121):
    xor.append(ord(flag[i]) ^ data[i])

print(xor)