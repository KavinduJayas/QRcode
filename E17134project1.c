#include <stdio.h>
#include <string.h>//string compare(strcmp) and string concacatenation(strcat)
#include <stdlib.h>//for dynamic memory allocation(malloc)

#define SIZESMALL 24
#define SIZELARGE 132

void removeGarbage(char*);
char *hashString(char*,int);
int toBinary(char);
void smallQR(char*,int);
void largeQR(char*,int);
void printCharacter(int,int,int,int);
void strReverse(char*,int,int);

int main(int argc, char* argv[]){
	printf("\033[0m");//rests all atributes
	int color=40;//default color black
	if(argc==3){//setting colors from input
		if(strcmp(argv[1],"-c")==0){
			if(strcmp(argv[2],"black")==0||strcmp(argv[2],"BLACK")==0){
				color=40;
			}else if(strcmp(argv[2],"red")==0||strcmp(argv[2],"RED")==0){
				color=41;
			}else if(strcmp(argv[2],"green")==0||strcmp(argv[2],"GREEN")==0){
				color=42;
			}else if(strcmp(argv[2],"yellow")==0||strcmp(argv[2],"YELLOW")==0){
				color=43;
			}else if(strcmp(argv[2],"blue")==0||strcmp(argv[2],"BLUE")==0){
				color=44;
			}else if(strcmp(argv[2],"magenta")==0||strcmp(argv[2],"MAGENTA")==0){
				color=45;
			}else if(strcmp(argv[2],"cyan")==0||strcmp(argv[2],"CYAN")==0){
				color=46;
			}else{//displaying usage for unsupported formats
				printf("Invalid argument for color.\nusage :\n%s -c [black|red|green|yellow|blue|magenta|cyan]\n%s -h for the help about the program\n",argv[0],argv[0]);
				return 0;
			}
		}else{	
			printf("Incorrect usage of arguments.\nusage :\n%s -c [black|red|green|yellow|blue|magenta|cyan]\n%s -h for the help about the program\n",argv[0],argv[0]);
			return 0;
		}

	}else if(argc ==2){
		if(strcmp(argv[1],"-h")==0){//displaying help
			printf("usage :\n%s -c [black|red|green|yellow|blue|magenta|cyan]\n%s -h for the help about the program\n",argv[0],argv[0]);
			return 0;

		}else{
			printf("Incorrect usage of arguments.\nusage :\n%s -c [black|red|green|yellow|blue|magenta|cyan]\n%s -h for the help about the program\n",argv[0],argv[0]);	
			return 0;	
		}
		
    	}else if(argc>3){
		printf("Incorrect usage of arguments.\nusage :\n%s -c [black|red|green|yellow|blue|magenta|cyan]\n%s -h for the help about the program\n",argv[0],argv[0]);
		return 0;
	}	    

	char inputString[140];
	removeGarbage(inputString);//removing garbage values

	printf("Enter the URL: ");
	scanf("%121s[^\n]",inputString);	

	if(strlen(inputString)>120){//checking if the string contains more than 120 characters
		printf("String is too long. Not supported by available QR versions\n");
		return 0;
	}else if(strlen(inputString)<=3){//checking if the string has less than 3 charachers
		printf("String is too short. Not supported by available QR versions\n");
		return 0;
	}else if(strlen(inputString)<=20){//displaying small QR	
		printf("\033[0m\033[2J\033[H");//resets all atributes and takes curser to Home		
		smallQR(hashString(inputString,SIZESMALL),color);
		printf("\033[22;1f\033[0m");
		return 0;
	}else{//displaying large QR
		printf("\033[0m\033[2J\033[H");//resets all atributes and takes curser to Home
		largeQR(hashString(inputString,SIZELARGE),color);
		printf("\033[40;1f\033[0m");
		return 0;
	}
	
	return 0;	
}

void removeGarbage(char *string){//removes garbage values
	int i;
	for(i=0;i<sizeof(*string);i++){
		string[i]='\0';
	}
}

void strReverse(char* strng,int start,int end){//reverses a string
	char temp;
	if(start>=end){
		return;
	}
	temp=strng[start];
	strng[start]=strng[end];
	strng[end]=temp;
	strReverse(strng,++start,--end);
	}


char *hashString(char *string,int hashSize){//hashes a string to any size

	int i,j,n,m;
	m=(hashSize-1)/strlen(string);//no of times the string has to be fully iterated through
	n=(hashSize-1)%strlen(string);//remaining room to be hashed after full iterations of the string
	char tempHashedString[hashSize+1];
	static char* tempString;
	tempString=malloc(hashSize);//allocating memory to the temporary string  
	tempString[0]=strlen(string)+50;
	removeGarbage(tempHashedString);//removing garbage values

	for(i=0;i<(m-1);i++){//hashing the string with full iterations
		for(j=0;j<strlen(string);j++){
			tempHashedString[j+strlen(string)*i]=*(string + j)+1+i;
		}			
	}
	if(m==1){//if the string has to be iterated only once
		for(i=0;i<strlen(string);i++){
			tempHashedString[i]=string[i]+1;
		}
	}
	for(i=0;i<n;i++){//hashing the remaining room
		tempHashedString[(m-1)*(strlen(string))+i]=*(string + i)+m;
	}
	for(i=0;i<strlen(string)-n;i++){//removing garbage values
		tempHashedString[(m-1)*(strlen(string))+i+n]='\0';
	}

	strReverse(tempHashedString,0,strlen(tempHashedString)-1);//reversing the hashedstring
	strcat(tempString,string);//concatenating with the a string containing the first hashed character

	return strcat(tempString,tempHashedString);//returning the fully hashed string

}

int toBinary(char character){//converts a character to binary
	if(character==0){
		return 0;
	}else{
		return character%2 + 10*toBinary(character/2);
	}
}

void printCharacter(int color,int row,int col,int binary){//prints a single character given the starting position
	int i,j;
	for(i=3;i>0;i--){
		for(j=3;j>0;j--){
			if(binary%10==1){
			printf("\033[%im\033[%i;%if  ",color,row+i,(col+j)*2-1);
			}
			binary /= 10;
		}
	}
}

void smallQR(char *hashedString,int color){//given the hashed string and the color; prints the small QR version 
	int i,j,k=0;

	printf("\033[1;1f");//forcing the curser to the starting point
	for(i=0;i<20;i++){//printing the background 
		for(j=0;j<20;j++){
			printf("\033[47m  ");
		}
		printf("\033[0m\n");
	}
	//printing the default boxes
	for(i=2;i<8;i++){
		printf("\033[%im\033[2;%if  \033[%i;3f  \033[%i;13f  \033[7;%if  ",color,i*2-1,i,i,i*2-1);//upper left corner box(without the dot in the middle)
	}
	for(i=14;i<20;i++){
		printf("\033[%im\033[2;%if  \033[%i;27f  \033[%i;37f  \033[7;%if  ",color,i*2-1,i-12,i-12,i*2-1);//upper rignt corner box
	}
	for(i=2;i<8;i++){
		printf("\033[%im\033[14;%if  \033[%i;3f  \033[%i;13f  \033[19;%if  ",color,i*2-1,i+12,i+12,i*2-1);//lower left corner box
	}

	for(i=0;i<2;i++){// dots in the middle
		printf("\033[%im\033[4;%if  \033[5;%if  \033[4;%if  \033[5;%if  \033[16;%if  \033[17;%if  ",color,(i+4)*2-1,(i+4)*2-1,(i+16)*2-1,(i+16)*2-1,(i+4)*2-1,(i+4)*2-1);
	}
	
	for(i=3;i<7;i++){//printing character 1 to 16
		for(j=3;j<7;j++){
			printCharacter(color,(i-1)*3+1,(j-1)*3+1,toBinary(hashedString[k]));
			k++;
		}
	}
	for(i=1;i<3;i++){//printing character  17 to 20
		for(j=3;j<5;j++){
			printCharacter(color,(i-1)*3+1,(j-1)*3+1,toBinary(hashedString[k]));
			k++;
		}
	}
	for(i=3;i<5;i++){//printing character 21 to 24
		for(j=1;j<3;j++){
			printCharacter(color,(i-1)*3+1,(j-1)*3+1,toBinary(hashedString[k]));
			k++;
		}
	}
}

void largeQR(char *hashedString,int color){//given the hashed string and the color; prints the large QR version 
	int i,j,k=0;

	printf("\033[1;1f");//forcing the curser to the starting point 
	for(i=0;i<38;i++){//printing the background 
		for(j=0;j<38;j++){
			printf("\033[47m  ");
		}
		printf("\033[0m\n");
	}//printing the default boxes
	for(i=2;i<8;i++){
		printf("\033[%im\033[2;%if  \033[%i;3f  \033[%i;13f  \033[7;%if  ",color,i*2-1,i,i,i*2-1);//upper left corner box(without the dot in the middle)
	}
	for(i=31;i<37;i++){
		printf("\033[%im\033[2;%if  \033[%i;63f  \033[%i;73f  \033[7;%if  ",color,i*2+1,i-29,i-29,i*2+1);//upper rignt corner box
	}
	for(i=2;i<8;i++){
		printf("\033[%im\033[32;%if  \033[%i;3f  \033[%i;13f  \033[37;%if  ",color,i*2-1,i+30,i+30,i*2-1);//lower left corner box
	}

	for(i=0;i<2;i++){// dots in the middle
		printf("\033[%im\033[4;%if  \033[5;%if  \033[4;%if  \033[5;%if  \033[34;%if  \033[35;%if  ",color,(i+4)*2-1,(i+4)*2-1,(i+34)*2-1,(i+34)*2-1,(i+4)*2-1,(i+4)*2-1);
	}
	
	for(i=3;i<13;i++){//printing character 1 to 100
		for(j=3;j<13;j++){
			printCharacter(color,(i-1)*3+1,(j-1)*3+1,toBinary(hashedString[k]));
			k++;
		}
	}
	for(i=1;i<3;i++){//printing character  101 to 116
		for(j=3;j<11;j++){
			printCharacter(color,(i-1)*3+1,(j-1)*3+1,toBinary(hashedString[k]));
			k++;
		}
	}
	for(i=3;i<13;i++){//printing character 117 to 132
		for(j=1;j<3;j++){
			printCharacter(color,(i-1)*3+1,(j-1)*3+1,toBinary(hashedString[k]));
			k++;
		}
	}
}


