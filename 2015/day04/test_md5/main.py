
from hashlib import md5
init = 'yzbqklnj'.encode()
for i in range(10_000_000):
    h = md5(init)
    # if h[:6] == '000000':
    #     print(i)
    #     break