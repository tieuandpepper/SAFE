# A test volume is 3ml --> 3000 ul
ELECTROLYTE_VOL = 3000

# Relative ratio of adjustment of additives 
ADDITIVE_INTERVAL = 0.1
# Maximum additive concentration in the electrolyte mixture
ADDITIVE_MAX_CON = 0.2
ADDITIVE_LIST = ['TMP','TEP','DMMP','TTFEP','FEMC','FDEC','BTFE','TTE','HFPN','PFPN','TFPN','FPPN']
ADDITIVE_COUNT_TOTAL = len(ADDITIVE_LIST)
ADDITIVE_RATIO_TOTAL = [0.05, 0.1, 0.15]

# helper function

def VolumeCal(final_comp, current_comp, curr_add):
    if curr_add == ADDITIVE_COUNT_TOTAL:
        return final_comp
    






def main():
    additive_volume = VolumeCal([],[],0)
    for i in range(0,ADDITIVE_COUNT_TOTAL):
        print("Additive {}: {} mL".format(ADDITIVE_LIST[i],sum(additive_volume[:][i])))

if __name__ == '__main__':
    main()