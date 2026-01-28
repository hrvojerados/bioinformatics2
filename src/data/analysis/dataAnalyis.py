import numpy as np 

def splitSpace(x):
    return list(map(float, x.split(" ")))
# print("Normal")
# with open("normal.txt", "r") as file:
#     data = np.array(list(map(float, file.read().splitlines())))
#     standardizedData = (data) / 2586 
#     print(standardizedData.mean())
#     print(np.median(standardizedData))
#
# print("2X")
# with open("2X.txt", "r") as file:
#     data = np.array(list(map(float, file.read().splitlines())))
#     standardizedData = (data) / 2586 
#     print(standardizedData.mean())
#     print(np.median(standardizedData))
#
# print("halfX")
# with open("halfX.txt", "r") as file:
#     data = np.array(list(map(float, file.read().splitlines())))
#     standardizedData = (data) / 2586 
#     print(standardizedData.mean())
#     print(np.median(standardizedData))
#
# print("equal")
# with open("equal.txt", "r") as file:
#     data = np.array(list(map(float, file.read().splitlines())))
#     standardizedData = (data) / 2586 
#     print(standardizedData.mean())
#     print(np.median(standardizedData))
#
print("Normal NW")
with open("normal.txt", "r") as file:
    data = np.array(list(map(splitSpace, file.read().splitlines()[1::])))
    HMMdata = data[:, 0]
    NWdata = data[:, 1]
    print("HMM median")
    print(np.median(HMMdata))
    print("HMM mean")
    print(np.mean(HMMdata))
    print("NW median")
    print(np.median(NWdata))
    print("NW mean")
    print(np.mean(NWdata))
    print("mean - abs(HMM - NW) / NW")
    print(abs(np.mean(NWdata) - np.mean(HMMdata)) / np.mean(NWdata)) 
    print("median - abs(HMM - NW) / NW")
    print(abs(np.median(NWdata) - np.median(HMMdata)) / np.median(NWdata)) 
print()
print()
print("Equal")
with open("equal.txt", "r") as file:
    data = np.array(list(map(splitSpace, file.read().splitlines()[1::])))
    HMMdata = data[:, 0]
    NWdata = data[:, 1]
    print("HMM median")
    print(np.median(HMMdata))
    print("HMM mean")
    print(np.mean(HMMdata))
    print("NW median")
    print(np.median(NWdata))
    print("NW mean")
    print(np.mean(NWdata))
    print("mean - abs(HMM - NW) / NW")
    print(abs(np.mean(NWdata) - np.mean(HMMdata)) / np.mean(NWdata)) 
    print("median - abs(HMM - NW) / NW")
    print(abs(np.median(NWdata) - np.median(HMMdata)) / np.median(NWdata)) 
print()
print()
print("We don't like insert")
with open("weDontLikeInsert.txt", "r") as file:
    data = np.array(list(map(splitSpace, file.read().splitlines()[1::])))
    HMMdata = data[:, 0]
    NWdata = data[:, 1]
    print("HMM median")
    print(np.median(HMMdata))
    print("HMM mean")
    print(np.mean(HMMdata))
    print("NW median")
    print(np.median(NWdata))
    print("NW mean")
    print(np.mean(NWdata))
    print("mean - abs(HMM - NW) / NW")
    print(abs(np.mean(NWdata) - np.mean(HMMdata)) / np.mean(NWdata)) 
    print("median - abs(HMM - NW) / NW")
    print(abs(np.median(NWdata) - np.median(HMMdata)) / np.median(NWdata)) 
    # data = np.array(list(map(float, file.read().splitlines()[1::].split(" "))))
    # standardizedData = (data) / 2586 
    # print(standardizedData.mean())
    # print(np.median(standardizedData))
