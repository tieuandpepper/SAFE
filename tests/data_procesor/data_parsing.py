keyword = "Temp Sensor: "
unit = " deg C"
temp_reading = []
with open('log_temp_3-4.txt') as file:
    while 1:
        # print(file.readline())
        line = file.readline()
        if line == "":
            break
        if keyword not in line:
            continue

        temp = line[(line.find(keyword)+len(keyword)):line.find(unit)]
        temp_reading.append(temp)
    

with open("temp_data_3-4.txt",'w') as file:
    file.write("temperature\n")
    for item in temp_reading:
        file.write(str(item) + "\n")


