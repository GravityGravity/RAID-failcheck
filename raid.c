#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "raidtype.h"

/*
===INPUT EXAMPLE===
./a.out T N C S W

"T" - Raid level (0, 1, 01, 10, 4, 5)
"N" - # of disks
"C" - size of chunks
"S" - # of sectors in each disk
"W" - # of failed disks
*/

int* asci; //Array storing integers then converting to characters
int* Wdisks; //Array of working disk numbers
disk* disks; //Array of disks

char T[3];
int N, C, S, W;
int lines; //Total number of entries into disks
int raidnum; //RAID level
int raid01check; //Checks if backups for RAID01 isnt failed

void createdisk (int n, int s, int w) { //Create a disk and identify working versus non-working disks
    disks = (disk*)malloc(n * sizeof(disk));

    for (int i = 0; i < n; i++) {
        disks[i].disknum = i;
        disks[i].array = malloc(s * sizeof(int));
        disks[i].working = -1;
    }

    for (int i = 0; i < w; i++){
        disks[(Wdisks[i])].working = 1;
    }
}

//Check RAID LEVEL and CALL IMPLEMENTATION
void raid(char* T, int n, int s, int c, int w) { 

    int raidnum;

    if(strcmp("01", T) == 0) {
        raidnum = 2;
    } 
    else {
        raidnum = atoi(T);
    }

    switch (raidnum) {

        case 0:
            if (W != N) {
                printf("DATA LOST\n");
                break;
            }
            createdisk(n, s, w);
            raid0(n, s, c, w);
            break;

        case 1:
            createdisk(n, s, w);
            raid1(n, s, w);
            break;

        case 2: //RAID01
            if (W < (N/2)){
                printf("DATA LOST\n");
                break;
            }

            createdisk(n, s, w);

            if (W > 1) {
                
                raid01check = 0;

                 for (int i = 0; i < n/2; i++) {

                    if (disks[i].working == -1) {
                        raid01check++;
                        break;
                    }
                 }

                 for (int i = n/2; i < n; i++) {

                    if (disks[i].working == -1) {
                        raid01check++;
                        break;
                    }
                 }
            }
            
            if (raid01check == 2) {
                printf("DATA LOST\n");
                break;
            }

            raid01(w, n, s);
            break;

        case 4: //RAID4
            if (W < (N-1)){
                printf("DATA LOST\n");
                break;
            }
            createdisk(n, s, w);
            raid4(w, n, s);
            break;

        case 5:
            if (W < (N-1)){
                printf("DATA LOST\n");
                break;
            }
            createdisk(n, s, w);
            raid5(w, n, s);
            break;

        case 10:
            if (W < (N/2)){
                printf("DATA LOST\n");
                break;
            }
            createdisk(n, s, w);
            raid10(w, n, s);
            break;
    }

}

int main () {

    scanf("%3s", T);
    scanf("%2d", &N);
    scanf("%2d", &C);
    scanf("%12d", &S);
    scanf("%2d", &W);


    if (W == 0) {
        printf("DATA LOST\n");
        return 0;
    } 
    else {
    lines = (W * S);
    asci = malloc(sizeof(int) * (lines)); //ALLOCATE ARRAYS
    Wdisks = malloc(sizeof(int) * W);

    for (int i = 0; i < (W); i++) { //Scan specified working disks into array
        scanf("%d", &Wdisks[i]);
    }

    raid(T, N, S, C, W);

    return 0;
    }
}
