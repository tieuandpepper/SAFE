# A test volume is 3ml --> 3000 ul
TOTAL_VOL = 3000

# Relative ratio of adjustment of additives 
ADDITIVE_INTERVAL = 0.1
# Maximum additive concentration in the electrolyte mixture
ADDITIVE_MAX_CON = 0.2
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

def VolumeCal(final_comp, current_comp, curr_add):
    if curr_add == ADDITIVE_COUNT_TOTAL:
        return final_comp
    

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



def main():
    ScreenTest()
    print(test_comp)
    total_vol = GetTotalVol(test_comp)
    print(total_vol)
    

if __name__ == '__main__':
    main()