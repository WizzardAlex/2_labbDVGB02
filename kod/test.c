#include <stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

long convert_bin(int dec)
{

    int bin =0;
    int rem,
	i=1,
	step=1;
    while(dec!=0){
	rem = dec%2;
	dec/=2;
	bin+=rem*i;
	i*=10;
    }
    return bin;

}


long make_checksum(char* msg)
{
    int checksum=0;
    int i;
    char tmp;
    for(i=0; i<sizeof(msg); i++){

	tmp = msg[i];
	checksum += (int)tmp;


    }
    return checksum;
}

int check_checksum(char* msg, int checksum)
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
