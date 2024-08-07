keyword = "FC01;"
end_char = ">"
temp_reading = []
count = 0
with open('log_240524_0.6.txt') as file:
    while 1:
        # print(file.readline())
        line = file.readline()
        if line == "":
            break
        if keyword not in line:
            continue
        count+= 1
        temp = (line[(line.find(keyword)+len(keyword)):-2])
        # print(temp)
        temp_reading.append(int(temp)/1000)
    
print("Total data points:",count)
with open("temp_data_5-24-0.60ml.txt",'w') as file:
    file.write("temperature\n")
    for item in temp_reading:
        file.write(str(item) + "\n")


