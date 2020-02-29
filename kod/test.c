#include <stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>




long make_checksum(struct msg message)
{
    int checksum=0;
    int i;
    char tmp;
    for(i=0; i<sizeof(msg.data); i++){

	tmp = msg.data[i];
	checksum += (int)tmp;


    }
    return checksum;
}

int check_checksum(struct msg message, int checksum)
{
    int new_checksum = make_checksum(msg);
    if( new_checksum == checksum) return 1;
    else return 0;

}
int main()
{
    char * msg = "Alexankskskforms";
    int sum = make_checksum(msg);
    puts(msg);
    int sum2 = make_checksum(msg);

    puts(msg);

    printf("sum: %d\nsum2: %d\ncheck: %d\n",sum,sum2,check_checksum(msg,sum));
    printf("faul_check: %d\n",check_checksum(msg,222));

    puts(msg);

    return 0;
}
