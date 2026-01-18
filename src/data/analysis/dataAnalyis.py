import numpy as np 

print("Normal")
with open("normal.txt", "r") as file:
    data = np.array(list(map(float, file.read().splitlines())))
    standardizedData = (data) / 2586 
    print(standardizedData.mean())
    print(np.median(standardizedData))

print("2X")
with open("2X.txt", "r") as file:
    data = np.array(list(map(float, file.read().splitlines())))
    standardizedData = (data) / 2586 
    print(standardizedData.mean())
    print(np.median(standardizedData))

print("halfX")
with open("halfX.txt", "r") as file:
    data = np.array(list(map(float, file.read().splitlines())))
    standardizedData = (data) / 2586 
    print(standardizedData.mean())
    print(np.median(standardizedData))

print("equal")
with open("equal.txt", "r") as file:
    data = np.array(list(map(float, file.read().splitlines())))
    standardizedData = (data) / 2586 
    print(standardizedData.mean())
    print(np.median(standardizedData))

print("We don't like insert")
with open("WeDontLikeInsert.txt", "r") as file:
    data = np.array(list(map(float, file.read().splitlines())))
    standardizedData = (data) / 2586 
    print(standardizedData.mean())
    print(np.median(standardizedData))
