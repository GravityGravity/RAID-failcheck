#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

extern int* asci; //Array storing integers then converting to characters
extern int* Wdisks; //Array of working disk numbers
extern struct disk* disks; //Array of disks

//Converts integers read from RAID disks into Characters
void numtochar (int arraysize) { 

    for (int i = 0; i < (arraysize); i++) {
        int num = asci[i];
        if((num >= 48 && num <= 57) || (num >= 65 && num <= 90) || (num >= 97 && num <= 122)) {
            char temp = num;
            printf("%c", temp);
        }
        else {
            printf("_");
        }
    }

    printf("\n");
}

void raid0 (int n, int s, int c, int w) { //RAID0 STRIPING
    int scan;

    for (int i = 0; i < s; i++) {

        for (int j = 0; j < w; j++) {
            scanf("%3d", &scan);
            disks[(Wdisks[j])].array[i] = scan;
        }
    }

    int fill = 0;
    int k = 0;
    int stripe = (s / c); //total stripes
    int LLstr = 0; //Lower limit of stripe (The floor of the current stripe)
    int ULstr = 0; //Upperlimit of stripe   (The ceiling of the current stripe)
    
    for(int i = 0; i < stripe; i++) {

        LLstr = c * i; //lower limit
        ULstr = c * (i+1);

        for(int j = 0; j < n; j++) {

            k = LLstr;

            for (k; k < ULstr; k++) {  

            asci[fill] = disks[j].array[k];  //fill array index and move to next disk
            fill++;
            }
        }
    }

    numtochar(fill);
}

void raid01 (int w, int n, int s) { //RAID01 

    int scan;

    for (int i = 0; i < s; i++) {

        for (int j = 0; j < w; j++) {

            scanf("%3d", &scan);
            disks[(Wdisks[j])].array[i] = scan;

        }
    }

    for (int i = 0; i < n/2; i++) {  //If a disk FAILED Check for backup RAID

        if (disks[i].working == -1) {

            if (disks[(n/2 + i)].working == -1) {

                printf("DATA LOST\n");
                exit(EXIT_SUCCESS);

            }
            else {

                disks[i] = disks[(n/2 + i)];

            }
        }
    }

    int fill = 0;

    for (int i = 0; i < s; i++) { //Read DISKS

        for (int j = 0; j < (n/2); j++) {
            
            asci[fill] = disks[j].array[i];
            fill++;

        }
    }

    numtochar(fill);
}

void raid1 (int n, int s, int w) { //RAID1

    int scan;

    for (int i = 0; i < s; i++) {

        for (int j = 0; j < w; j++) {
            scanf("%3d", &scan);
            disks[(Wdisks[j])].array[i] = scan;
        }
    }
    
    int fill = 0;

    for (int i = 0; i < s; i++) {
        asci[fill] = disks[(Wdisks[0])].array[i];
        fill++;
    }

    numtochar(fill);

}

void raid10 (int w, int n, int s) { //RAID10

    int scan = 0;

    for (int i = 0; i < s; i++) {

        for (int j = 0; j < w; j++) {
            scanf("%d", &scan);
            disks[(Wdisks[j])].array[i] = scan;
        }
    }

    for (int i = 0; i < n; (i += 2)) { //Check DISKS adjacent to failed disks
        if (disks[i].working == -1) {

            if (disks[(i+1)].working == -1) {

                printf("DATA LOST\n");
                exit(EXIT_SUCCESS);
            }
            else {

                disks[i] = disks[(i+1)];

            }
        }
    }

    int fill = 0;

    for (int i = 0; i < s; i++) {
        
        for (int j = 0; j < n; j += 2) {

            asci[fill] = disks[j].array[i];
            fill++;

        }
    }

    numtochar(fill);
}

void raid4 (int w, int n, int s) { //RAID4

    int scan;

    for (int i = 0; i < s; i++) {

        for (int j = 0; j < w; j++) {
            scanf("%3d", &scan);
            disks[(Wdisks[j])].array[i] = scan;
        }
    }

    int p;
    int fill = 0;

    for (int i = 0; i < s; i++) { //Check ALL RAID sectors.  IF sector is lost, calculate missing sectior using PARITY XOR

        int p = 0;

        for (int j = 0; j < (n-1); j++) {

            if (disks[j].working == -1) {

                for (int k = 0; k < w; k++) {

                    p = p ^ (disks[(Wdisks[k])].array[i]);
                    asci[fill] = p;

                }

                fill++;

            }
            else {
            
                asci[fill] = disks[j].array[i];
                fill++;

            }
        }
    }

    numtochar(fill);
}

void raid5 (int w, int n, int s) {

    int scan;
    int p = 0;  //parity bit
    int ppos = n; //parity position
    
    for(int i = 0; i < s; i++) { //Calculate Parity blocks and fill in DISKS

        p = 0;
        ppos = (ppos-1);

        if (ppos == -1) {

            ppos = (n-1);
    
        }

        for(int j = 0; j < (n); j++) {
                
            if (j == ppos) {

                if (disks[j].working == -1){
                    disks[j].array[i] = -2;

                } 
                else {

                    scanf("%d", &scan);
                    disks[j].array[i] = scan;

                }
            }
            else if ((disks[j].working == -1) && (j != ppos)){

                disks[j].array[i] = -1;  
                
            }
            else {

                scanf("%d", &scan);
                disks[j].array[i] = scan;  

            }
        }
    }

    for (int i = 0; i < s; i++) {
        
        p = 0;

        for (int j = 0; j < n; j++) {
            
            if (disks[j].array[i] == -1) {

                for (int k = 0; k < w; k++) {
                    p = p ^ (disks[(Wdisks[k])].array[i]);
                    
                }
                disks[j].array[i] = p;
            }
        
        }
    }

    p = 0;
    int fill = 0;
    int under;
    ppos = n;

    for (int i = 0; i < s; i++) { //Read each entry into Num array then mark with a discovered signal

        under = i;
        p = 0;
        ppos = (ppos-1);
        if (ppos == -1) {
            ppos = (n-1);
        }

        for (int j = 0; j < (n); j++) {

            if (disks[j].array[under] == -3) {
                break;
            } 

            if ((j == ppos) && (j != 0)) {

                disks[j].array[i] = -3;

                if(i != (s-1)) {

                    under = i + 1;

                }
            }

            if ((j == ppos) && (j == 0)) {
                break;
            }

            if (fill != ((s * (n-1)))) {

                asci[fill] = disks[j].array[under];
                fill++;
                disks[j].array[under] = -3;

            }
        }
    }

    numtochar(fill);
}
