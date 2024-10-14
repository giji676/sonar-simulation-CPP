import numpy as np

arr = []

for i in range(7):
    js = []
    for j in range(8):
        ks = []
        for k in range(9):
            ks.append((str(i) + str(j) + str(k)))
        js.append(ks)
    arr.append(js)

F = np.array(arr)

print(F[:3])
print("=" * 60)

cxs = np.array([0, 0, 1, 1, 1, 0, -1, -1, -1])
zipped = zip(range(9), cxs)

for i, cs in zipped:
    F[:, :, i] = np.roll(F[:, :, i], cs, axis = 1)

print(F[:3])
