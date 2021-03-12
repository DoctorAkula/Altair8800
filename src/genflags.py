#!/bin/python
def parity(x):
    res = 0;
    while x:
        res ^= x & 1;
        x >>= 1;
    return res;

print("#include\"8080.h\"\n");
#increment flags
print("const uint8_t INR_FLAGS[256] = {");
for i in range(0, 256):
    flag = 0;
    if((i & 0xF) == 0):
        flag |= 0x10; #Aux carry set
    if(i == 0):
        flag |= 0x40; #Zero flag set
    if((i & 0x80) > 0):
        flag |= 0x80; #Sign flag set
    if(parity(i) == 0):
        flag |= 0x04; #Even parity set
    print('\t0x%X,' % flag);
print("};\n");

#decrement flags
print("const uint8_t DCR_FLAGS[256] = {");
for i in range(0, 256):
    flag = 0;
    if((i & 0xF) == 0xF):
        flag |= 0x10; #Aux carry set
    if(i == 0):
        flag |= 0x40; #Zero flag set
    if((i & 0x80) > 0):
        flag |= 0x80; #Sign flag set
    if(parity(i) == 0):
        flag |= 0x04; #Even parity set
    print('\t0x%X,' % flag);
print("};\n");

#DAA flags
print("const uint8_t DAA_FLAGS[256] = {");
for i in range(0, 256):
    flag = 0;
    if(i == 0):
        flag |= 0x40; #Zero flag set
    if((i & 0x80) > 0):
        flag |= 0x80; #Sign flag set
    if(parity(i) == 0):
        flag |= 0x04; #Even parity set
    print('\t0x%X,' % flag);
print("};");
