# SYSTEM CONTROLLER
import serial
import time
import CondMeterA215 as CondMeter

ARDUINO_PORT = "/dev/tty.usbserial-1110"
COND_METER_PORT = "COM15"
BAUD_RATE = 115200

PORT_ELECTROLYTE_LP30 = 1
PORT_ADDITIVE_TEP = 2
PORT_ARGON_GAS = 3

TOTAL_VOLUME = 3.00 #ml

CMD_TARGET_IDX = 0
CMD_INSTR_IDX = 1
CMD_PARAMS_IDX = 2

RESP_SOURCE_IDX = 0
RESP_TYPE_IDX = 1
RESP_ERR_IDX = 2
RESP_DATA_IDX = 3

def GetArduinoResponse(comm_port):
    response_str = "Waiting for Arduino"
    while response_str.find("RESP") == -1:
        time.sleep(0.02) #20ms
        response_str = comm_port.readline().decode('utf-8') 

    if response_str[0]!= "<" and response_str[-1] != ">":
        print("-> Incorrect Response format:" + response_str)
        return []
    print("-> ", response_str)
    response = response_str.strip('<>').split(';')
    return response

def SendArduinoCommand(comm_port, command):
  command_str = ';'.join(map(str, command))
  command_str = f"<{command_str}>"
  comm_port.write(bytes(command_str,'utf-8'))
  print("<- ", command_str)


# setup communication with equipments
arduino = serial.Serial(port = ARDUINO_PORT,  baudrate=BAUD_RATE, timeout=.5)
cond_meter = CondMeter.ConductivityMeter(COND_METER_PORT, BAUD_RATE)

# input (%additive)
# Example: 7.5 -> 7.5% of 3ml is additive, 92.5% is LP30
additive_list = ["TEP"]
print("Order to input composition: ", additive_list)
input_comp = input("Enter the composition percentage, separate by commas: ")
comp_list = [float(item) for item in input_comp.split(',')]

# convert the composition % list into volume
add_vol_list = [item / 100 * TOTAL_VOLUME for item in comp_list]
# print(add_vol_list)
port_dict = {"TEP": PORT_ADDITIVE_TEP, "ARGON": PORT_ARGON_GAS, "LP30":PORT_ELECTROLYTE_LP30}
# print(port_dict)

command_list = []
# change pump direction
command_list.append(["MIXPUMP","CLOCK"])
# add additives to the mixture
for idx, additive in enumerate(additive_list):
    command_list.append(["ROTARYVALVE","MOVEAUTO",port_dict[additive]])
    command_list.append(["MIXPUMP","DISPENSE",add_vol_list[idx]])

# add LP30
command_list.append(["ROTARYVALVE","MOVEAUTO",port_dict["LP30"]])
command_list.append(["MIXPUMP","DISPENSE",TOTAL_VOLUME - sum(add_vol_list)])

# mix the solution in 5s
command_list.append(["MIXER","START",5000])

# measure the conductivity
command_list.append(["CONDMETER","MEAS"])

# burn 1ml each time
for i in range(0,3):
    # move 1ml the solution to the test chamber
    command_list.append(["TRANSFERPUMP","DISPENSE",1000])
    # active temperature sensor to read new data every 100ms
    command_list.append(["TEMPSENSOR", "PERDIODIC",100])
    # turn the lighter on for 3s
    command_list.append(["LIGHTER","IGNITE",3000])
    # stop sending command for 90s
    command_list.append(["CONTROLLER", "DELAY", 90])

# clean out the pipes
command_list.append(["ROTARYVALVE","MOVEAUTO",port_dict["ARGON"]])
command_list.append(["MIXPUMP","START"])
command_list.append(["TRANSFERPUMP","START"])
command_list.append(["SOLENOID", "RUN", 30000]) #30s
command_list.append(["MIXPUMP","STOP"])
command_list.append(["TRANSFERPUMP","STOP"])

# process all commands
cmd_count = 0
pause_sending_cmd = False
pause_time_start = 0
pause_time_duration = 0
while cmd_count < len(command_list):
    # pause sending the next commands for a period of time
    if pause_sending_cmd == True:
        if time.time() - pause_time_start > pause_time_duration:
            pause_time_duration = False
        repsonse = GetArduinoResponse()
        continue

    current_command = command_list[cmd_count]
    cmd_count += 1
    if current_command[CMD_TARGET_IDX] == "CONTROLLER":
        if current_command[CMD_INSTR_IDX] == "DELAY":
            pause_sending_cmd = True
            pause_time_start = time.time()
            pause_time_duration = current_command[CMD_PARAMS_IDX]
    elif current_command[CMD_TARGET_IDX] == "CONDMETER":
        conductivity = cond_meter.getConductivity()
        print("Conductivity reading: {} uS/cm".format(conductivity))
    else:
        # if the command is unsuccessful => rerun 3 times => still not successful => Error, stop everything
        count = 0
        success = False
        while count < 3:
            SendArduinoCommand(arduino,current_command)
            response = GetArduinoResponse(arduino)
            count += 1
            if response[RESP_ERR_IDX] & 0xE000 == 0xE000:
                print("Error happpened at" + response[RESP_SOURCE_IDX] + "with error code ", hex(response[RESP_ERR_IDX]))
            elif response[RESP_SOURCE_IDX] == "ROTARYVALVE" and response[RESP_ERR_IDX] != 0x0000:
                print("Error happened with rotary vavle with error code" + hex(response[RESP_ERR_IDX]))
            elif response[RESP_SOURCE_IDX] == "TRANSFERPUMP" and response[RESP_ERR_IDX] != 0x0001:
                print("Error happened with EZO pump with error code" + hex(response[RESP_ERR_IDX]))
            else:
                # success operation
                success = True
                break
        
        if success == False:
            print("Error in the system. Halting operation")
            user_direction = input("enter Y if you want to override and continue the operation, otherwise enter N: ")
            if user_direction == "N":
                print("Operation stopped")
                break
            else:
                print("Operation continued")
                success = True

