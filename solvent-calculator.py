import csv
# A test volume is 3ml --> 3000 ul
TOTAL_VOL = 3000

# Relative ratio of adjustment of additives 
ADDITIVE_INTERVAL = 0.1
# Maximum additive concentration in the electrolyte mixture
ADDITIVE_MAX_CON = 0.15
ADDITIVE_LIST = ['TMP','TEP','DMMP','TTFEP','FEMC','FDEC','BTFE','TTE','HFPN','PFPN','TFPN','FPPN']
ADDITIVE_COUNT_TOTAL = len(ADDITIVE_LIST)
ADDITIVE_RATIO_TOTAL = [0.05, 0.1, 0.15]

# SCREEN TEST MACRO
ADDITIVE_RATIO = 0.1
LP30_RATIO = 0.9

comp_header = ['LP30'] + ADDITIVE_LIST
final_comp = []
test_comp = []
# helper function

def PermutationFinder(permutation_arr, arr, idx, target, balance):
    # if combination is found
    if (balance == 0):
        permutation_arr.append(arr[:])
        for i in range(idx-1,len(arr)):
            if (idx < 0): break
            arr[i] = 0
        # print(result[-1])
        # print(arr)
        return
    
    # base condition
    if (balance < 0):
        # print('<0')
        return
    if (idx == len(arr)):
        # print('out of bound')
        return

    # loop start from prev number at array idx-1
    for k in range(0,balance+1):
        # next element of array is k
        arr[idx] = k
        # print("current condition: {}, idx {}, balance {}, k = {}".format(arr,idx,balance-k,k))

        # call recursively with balance in mind
        PermutationFinder(permutation_arr, arr, idx+1, target, balance - k)



def ScreenTest():
    for idx, additive in enumerate(ADDITIVE_LIST):
        volume_dist = [0]*(ADDITIVE_COUNT_TOTAL + 1)
        volume_dist[0] = LP30_RATIO * TOTAL_VOL
        volume_dist[idx + 1] = ADDITIVE_RATIO * TOTAL_VOL
        test_comp.append(volume_dist)
        # print(test_comp)
        

def GetTotalVol(comp_list, have_LP30 = True):
    if (have_LP30):
        chemicals = comp_header
    else:
        chemicals = ADDITIVE_LIST
    
    # only one composition
    if (type(comp_list[0]) is int):
        return comp_list
    
    total_vol = [0] * len(chemicals)
    for idx, additive in enumerate(chemicals):
        for col in range(0,len(comp_list)):
            total_vol[idx] += comp_list[col][idx]
    
    return total_vol

# test for duplicates
def Validation(arr):
    arr.sort()
    for i in range(1,len(arr)):
        if (arr[i] == arr[i-1]):
            return False
    return True


def main():
    arr = [0]*ADDITIVE_COUNT_TOTAL
    permutation_arr = []
    PermutationFinder(permutation_arr,arr, 0, 10, 10)
    permutation_arr.sort()
    print("Total permuation:", len(permutation_arr))
    print("Validation: {}".format(Validation(permutation_arr)))
    total_vol = [0]*(ADDITIVE_COUNT_TOTAL)
    for i in range(len(permutation_arr)):
        for j in range(ADDITIVE_COUNT_TOTAL):
            total_vol[j] += permutation_arr[i][j]
    # print(permutation_arr)
    
    # The data is written as ratio 0-10 (0=0%, 10=100%)
    file_name = "SAFE/additive_composition.csv"
    with open(file_name,'w',newline='') as file:
        csv_writer = csv.writer(file)
        csv_writer.writerow(ADDITIVE_LIST)
        csv_writer.writerows(permutation_arr)
    
    # the data is written with [ul] as the unit (1ul = 10^-6 l)
    file_name = "SAFE/additve_volume.csv"
    with open(file_name,'w',newline='') as file:
        csv_writer = csv.writer(file,lineterminator='\n')
        csv_writer.writerow(ADDITIVE_LIST)
        csv_writer.writerow(total_vol)
    
    txt_file = "SAFE/chemicals_report.txt"
    with open(txt_file,'w') as file:
        line = "Total combinations: {}\n".format(len(permutation_arr))
        file.write(line)
        line = "Total formulations: {}\n".format(len(permutation_arr)*len(ADDITIVE_RATIO_TOTAL))
        file.write(line)
        line = "LP30 = {:>16,.2f} mL\n".format(TOTAL_VOL*len(permutation_arr)*(len(ADDITIVE_RATIO_TOTAL)-sum(ADDITIVE_RATIO_TOTAL))/1000)
        file.write(line)
        for i in range(ADDITIVE_COUNT_TOTAL):
            # additive volume percentage: additive_total_volume / 10
            # total volume for each additive = 3[electrolyte mL] * [additive volume percentage] * (0.05 + 0.1 + 0.15)
            line = "{:<5} = {:>16,.2f} mL\n".format(ADDITIVE_LIST[i],TOTAL_VOL*total_vol[i]*(sum(ADDITIVE_RATIO_TOTAL))/10/1000)
            file.write(line)
    print('complete')

if __name__ == '__main__':
    main()