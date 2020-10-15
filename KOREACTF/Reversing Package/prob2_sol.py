
#step1
#First Input
#Function Epilogue is norally "Leave Ret" -> \xC9\xC3
Key1 = "".join([ chr(a ^ b) for (a,b) in zip(b'\xa6\x84', b'\xc9\xc3') ])
