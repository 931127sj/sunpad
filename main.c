#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "init.h"

void main(int argc, char *argv[])
{
	int filecnt = argc;

	int i, f_mode = 0;
	FILE *fp;
	char inputBuffer[MAX_LENGTH] = {0};
	char filename[MAX_NAME] = {0};

	int cc, wi;

	node* nodeBuffer;

	int key_cnt = 0;
	char buff[255];
	enum MODES mode = COMMAND_MODE;

	if(argc == 1){
		printf(" >>>> Write fomat : ./main <filename>\n");
	}else{
		int cur_line = 2;
		int cur_col = 1;
		int key, ret;
		int i;

		fputs("\033[2J", stdout);
		fputs("\033[1;1H", stdout);

		struct termios oldt, curt, newt;

		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);

		for(i = 1; i < filecnt; i++){
			strcpy(filename, (char *)argv[i]);
			if((fp = fopen(filename, "r+")) == NULL){
				fp = fopen(filename, "w+");
				f_mode = 1;
			}
		}

        sprintf(buff, "FILE NAME : \e[1;36m%s\e[0m\n", argv[1]);
        fputs(buff, stdout);

		if(f_mode == 0){
        	while(!feof(fp)){
	        	fgets(inputBuffer, MAX_LENGTH, fp);
				for(i = 0; i < MAX_LENGTH; i++){
					if(inputBuffer[i] != '\0'){
						insertData(inputBuffer[i], cur_line-1, cur_col);
						cur_col++;
					}
				}
  	            cur_line++;
				cur_col = 1;
      		}
			deleteData(cur_line-1, 0);
			printList(head, stdout);

			sprintf(buff, "\033[%d;%dH", cur_line, cur_col);
			fputs(buff, stdout);
		}

		while(1){
			key = getchar();

			ret = feof(stdin);
			if(ret != 0){
				printf("EOF\n");
				return;
			}

			ret = ferror(stdin);
			if(ret != 0){
				printf("ERROR getchar() by %d\n", ret);
				return;
			}

			if (mode == COMMAND_MODE) {				
				switch(key){
					case 'i': // INPUT MODE
						mode = INPUT_MODE;
						tcgetattr(STDIN_FILENO, &curt);
						newt = curt;
						newt.c_lflag &= ~(ECHO);
						tcsetattr(STDIN_FILENO, TCSANOW, &newt);

						sprintf(buff, "\033[%d;%dH\033[1m-- INSERT --\033[0m", 1, 40);
						fputs(buff, stdout);

						if(head != NULL){
						}
					break;
					case 'h': // CURSOR_LEFT
					case 68:
						cur_col--;
						if(cur_col < MIN_COL) cur_col = MIN_COL;
					break;
					case 'j': // CURSOR_DOWN
					case 66:
						cur_line++;
	                    if(cur_line > MAX_LINE) cur_line = MAX_LINE;
					break;
					case 'k': // CURSOR_UP
					case 65:
						cur_line--;
						if(cur_line < MIN_LINE) cur_line = MIN_LINE;
					break;
					case 'l': // CURSOR_RIGHT
					case 67:
						cur_col++;
						if(cur_col > MAX_COL){
							cur_col = 1;
							cur_line++;
							if(cur_line > MAX_LINE) cur_line = MAX_LINE;
						}
					break;
					case 'w': // SAVE
						if(key_cnt > 0)
							insertData('\0', cur_line, cur_col+1);

						fseek(fp, 0L, SEEK_SET);
						printList(head, fp);

						fputs("\033[2J", stdout);
						fputs("\033[1;1H", stdout);	

						tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
						fclose(fp);
						return;
					break;
				}
			}else if(mode == INPUT_MODE){
				switch(key){
					case 27: // ESC -> COMMAND MODE
						mode = COMMAND_MODE;
						tcgetattr(STDIN_FILENO, &curt);
						newt = curt;
						newt.c_lflag &= ~(ECHO);
						tcsetattr(STDIN_FILENO, TCSANOW, &newt);

						sprintf(buff, "\033[%d;%dH             ", 1, 40);
						fputs(buff, stdout);
					break;
					case 10: // ENTER
						mode = INPUT_MODE;

						insertData('\n', cur_line-1, cur_col);

						cur_line++;
						if(cur_line > MAX_LINE) cur_line = MAX_LINE;
						cur_col = 1;
					break;
					case 8: // BACKSPACE
						deleteData(cur_line-1, cur_col-1);

						cur_col--;
						if(cur_col < MIN_COL) cur_col = MIN_COL;
					break; 
					default:
						mode = INPUT_MODE;
						if(key != 127){
							insertData(key, cur_line-1, cur_col);
							key_cnt++;

							cur_col++;
							if(cur_col > MAX_COL){
								cur_col = 1; cur_line++;
								if(cur_line > MAX_LINE) cur_line = MAX_LINE;
							}
						}else{
							deleteData(cur_line-1, cur_col);

							cur_col--;
							if(cur_col < MIN_COL) cur_col = MIN_COL;
						}

					break;
				}
				sprintf(buff, "\033[%d;%dH", 2, 1);
				fputs(buff, stdout);
				printList(head, stdout);
			}

			sprintf(buff, "\033[%d;%dH%3d:%3d", 1, 70, cur_line-1, cur_col);
			fputs(buff, stdout);
			sprintf(buff, "\033[%d;%dH", cur_line, cur_col);
			fputs(buff, stdout);
		}
	
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		fclose(fp);
	}
	return;
}
