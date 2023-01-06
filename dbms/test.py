
def padding(offset, align):
    return -offset & (align - 1);

while(True):
    print(padding(int(input("off:")), int(input("ali:"))))
