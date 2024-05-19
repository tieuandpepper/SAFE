keyword = "FC01;"
end_char = ">"
temp_reading = []
with open('log_170524_1.txt') as file:
    while 1:
        print(file.readline())
        line = file.readline()
        if line == "":
            break
        if keyword not in line:
            continue

        temp = (line[(line.find(keyword)+len(keyword)):-2])
        print(temp)
        temp_reading.append(int(temp)/1000)
    

with open("temp_data_5-17.txt",'w') as file:
    file.write("temperature\n")
    for item in temp_reading:
        file.write(str(item) + "\n")


