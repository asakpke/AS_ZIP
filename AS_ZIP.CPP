#include <stdio.h>
#include <conio.h>
#include <string.h>
//start-tree
struct node
{
	char data;
	int is_data;
	int no_bits;
	char bits[9];
	//int ref;
	node *top;
	node *left;
	node *right;
};
node *rt, *cu, *nw;
node *ptr_node[256];
int ptr_counter=0;

void md_tree(int, node*, int);// ==> make tree
node* md_new_node();
void sort_nodes();
void count_node_bits( node * );
void show_sorted_nodes();
//end-tree

void show_byte( char );

//start-tree
char tre_byte=0;
int tre_bit=8;
//end-tree
//char my_byte=0;
//int my_bit;
//unsigned long total_bytes_stored=0;
unsigned long tre_total_bytes_stored=0;
FILE /**fpw,*/ *fpr;
//start-tree
FILE *tre_fpw, *tre_fpr;
//end-tree

int main(int argc, char *argl[])
{
/// *
	int lv_bit;
	clrscr();
	char chars[256];
	int no_chars[256];
	int code_bits[256];
	long total_chars=-1;
	//int bit_code=1;
	int v;
	int bit;

	char ch, file_temp[30];
	char file_path[50]="c:\\tc\\bin\\", file_name[9]="file", file_ext[5]=".txt";

	if( argc == 2 )
	{
		char *ptr_bs, *ptr_dot;
		printf("\nargl[0] = %s", argl[0]);
		printf("\nargl[1] = %s", argl[1]);

		int str_len = strlen(argl[1]);
		ptr_bs = strrchr(argl[1], '\\');
		ptr_dot = strstr(argl[1], ".");
		if( ptr_dot == NULL )
			strcpy(file_ext, "");
		else
			strcpy( file_ext, ptr_dot);

		if( ptr_bs != NULL )
		{
			ptr_bs++;
			printf("\nptr = %s", ptr_bs);
			strncpy(file_name, ptr_bs, strlen(ptr_bs)-strlen(file_ext));
			file_name[strlen(ptr_bs)-strlen(file_ext)] = '\0';
			strncpy(file_path, argl[1], str_len-strlen(file_name)-strlen(file_ext));
			file_path[str_len-strlen(file_name)-strlen(file_ext)] = '\0';
		}
		else
		{
			strncpy(file_name, argl[1], str_len-strlen(file_ext));
			file_name[str_len-strlen(file_ext)] = '\0';
			strcpy(file_path, "");
		}
		printf("\n\nFile extention = %s", file_ext);
		printf("\n\nFile name = %s", file_name);
		printf("\n\nFile path = %s", file_path);
		getch();
	}

	unsigned long total=0;

	strcpy( file_temp, file_path );
	strcat( file_temp, file_name );
	strcat( file_temp, file_ext );
	//fpr = fopen( file_temp, "r" );
	fpr = fopen( file_temp, "r" );
	if( fpr == NULL)
	{
		fprintf(stderr, "Cannot open input file.\n");
		return 1;
	}
	int is_same;
	printf( "%s = \n", file_temp );
	//printf(", File Data = ");
	//size_t tre_sz;
	//tre_sz = fread( &ch, sizeof(char), 1, tre_fpr);
	while( (ch = fgetc(fpr)) != EOF	)
	//while( tre_sz != 0 )
	{
		printf("%c", ch); //it is for to display file data
		is_same = 0;
		for( v=0; v <= total_chars; v++ )
		{
			if( ch == chars[v] )
			{
				no_chars[v]++;
				is_same = 1;
				break;
			}
		}
		if( is_same == 0 )
		{
			total_chars++;
			chars[total_chars] = ch;
			no_chars[total_chars] = 1;
		}
		total++;
		//tre_sz = fread( &ch, sizeof(char), 1, tre_fpr);
	}//while( (ch = fgetc(fpr)) != EOF	)
	getch();//to see file data
	int lv_in, temp;
	char temp_ch;
	//bit_code = 1;
	for( v=0; v <= total_chars; v++ )
	{
		for( lv_in=v+1; lv_in <= total_chars; lv_in++ )
			if( no_chars[v] < no_chars[lv_in] )
				{ //s
					temp_ch =	chars[v];
					chars[v] = chars[lv_in];
					chars[lv_in] = temp_ch;
					temp = no_chars[v];
					no_chars[v] = no_chars[lv_in];
					no_chars[lv_in] = temp;
				} //e
	}//for( v=0; v <= total_chars; v++ )

	printf("\n\nTotal character = %u" ,total );
	printf(", Total alphabet used = %d" ,total_chars+1 );

	fclose(fpr);
	getch();

	fpr = fopen( file_temp, "r" );
/*
	strcpy( file_temp, file_path );
	strcat( file_temp, file_name );
	strcat( file_temp, ".azp" );
	fpw = fopen( file_temp, "w" );
*/
	//start-starting tag
	//fputc( tmp_var, fpw );
	//total_bytes_stored++;
	//start-tree
	//start-	**************** TREE CODE **********************************
	rt = md_new_node();
	nw = md_new_node();	// /-->> total_chars+1+3, +3 for 3 special chars
	md_tree( total_chars+1+1, rt, 128 );//note +1 because total_chars starts
						// with 0 and +1 for index_special
	count_node_bits( rt);
	sort_nodes();
	//start-assign chars

	//end-assign chars
	//end-tree
	strcpy( file_temp, file_path );
	strcat( file_temp, file_name );
	strcat( file_temp, ".tzp" );
	tre_fpw = fopen( file_temp, "wb" );
	//fputc( tmp_var, tre_fpw );
	char zip_ver = 1;// ver. info. min. the bits
	fwrite( &zip_ver, sizeof(char), 1, tre_fpw);
	tre_total_bytes_stored++;
	//getch();
	char total_chars_used;
	total_chars_used = total_chars+1;// total bytes info
	//fputc( tmp_var, fpw );
	//total_bytes_stored++;
	//start-tree
	//fputc( tmp_var, tre_fpw );
	fwrite( &total_chars_used, sizeof(char), 1, tre_fpw);
	tre_total_bytes_stored++;
	for( v=0; v <= total_chars; v++ ) // first code is for index_special
	{
		ptr_node[v]->data = chars[v];//-1 !
		//fputc( chars[v], tre_fpw );// -1 due to index_special
		fwrite( &ptr_node[v]->data, sizeof(char), 1, tre_fpw);
		tre_total_bytes_stored++;
	}
	ptr_node[v]->data = EOF; // note this point
	show_sorted_nodes();
	getch();

	//end-tree
	//getch();
	//end-starting tag
	//my_byte=0;
	//my_bit=8;
	printf("\n\nVer figure stored code = %d" , zip_ver );
	printf(", Total bytes figure stored code = %d" , total_chars_used);
	printf("\n\nFile data in bits form\n" );

	//tre_sz = fread( &ch, sizeof(char), 1, tre_fpr);
	while( (ch = fgetc(fpr)) != EOF	)
	//while( tre_sz != 0	)
	{// read new char from file
		for( v=0; v <= total_chars; v++ )//start with 1 because of index_special
		{
			if( ch == chars[v] )//-1 because of index_special
			{
				//start-tree
				//int spe_char_prob = 0, old_v=v;
				for( int tre_lv=0; tre_lv < ptr_node[v]->no_bits; tre_lv++ )
				{
					if( tre_bit == 0 )
					{	//note write to disk and get new byte
						printf("-");
						show_byte( tre_byte );
						//getch();
						//getch();
						//fputc( tre_byte, tre_fpw );
						fwrite( &tre_byte, sizeof(char), 1, tre_fpw);
						tre_total_bytes_stored++;
						tre_byte = 0;
						tre_bit = 8;
					} // if( tre_bit == 0 )
					if( ptr_node[v]->bits[tre_lv] == '0' )
						tre_bit--;
					else if( ptr_node[v]->bits[tre_lv] == '1' )
					{
						char tre_var = 1;
						tre_var = tre_var << tre_bit-1; //starting_bit;
						tre_bit--;
						tre_byte = tre_byte | tre_var;
					}
				} // for( int tre_lv=0; tre_lv < ptr_node[v]->no_bits; tre_lv++ )
				//end-tree
			}// if( ch == chars[v] )
		}// for( v=0; v <= total_chars; v++ )
		//add_end_char_tag();
		//tre_sz = fread( &ch, sizeof(char), 1, tre_fpr);
	}// while( (ch = fgetc(fpr)) != EOF	) // read new char from file
	//start-tree
	if( tre_bit < 8 )
	{
		printf("-");
		show_byte( tre_byte );
		fwrite( &tre_byte, sizeof(char), 1, tre_fpw);
		tre_total_bytes_stored++;
	}
	//end-tree
	printf("\n\nTotal character = %u" ,total );
	printf(", Total alphabet used = %d" ,total_chars+1 );

	printf( "\n ==> Tree Total bytes stored = %u", tre_total_bytes_stored );

	fclose(fpr);
	//fclose(fpw);
	fclose(tre_fpw);
	getch();
	getch();
	return 0;
}// main()

void show_byte( char char_byte)
{
	int bit = 1;
	bit = bit << 7;
	//printf("\nChar Byte = %c = ", char_byte);
	for( int lv_bit=0; lv_bit < 8; lv_bit++ )
	{
	if( char_byte & bit )
		printf("1");
	else
		printf("0");
	bit = bit >> 1;
	}
	//getch();
}// show_byte()

//start-tree
void md_tree( int t_ch, node *top, int level )
{
	if( t_ch == 1 )
	{
		//no_ch_noded++;
		nw->data = 65;//v_ch++;
		nw->is_data = 1;

		ptr_node[ptr_counter++] = nw;
		strcpy(ptr_node[ptr_counter-1]->bits, "1");
		ptr_node[ptr_counter-1]->no_bits = 1;

		nw->top = top;
		//nw->ref = 1;//ref_id++;//
		top->right = nw;
		nw = md_new_node();
		return;
	}
	if( t_ch == 2 )
	{
		//no_ch_noded++;
		nw->data = 65;//v_ch++;
		nw->is_data = 1;

		ptr_node[ptr_counter++] = nw;
		strcpy(ptr_node[ptr_counter-1]->bits, "1");
		ptr_node[ptr_counter-1]->no_bits = 1;

		nw->top = top;
		//nw->ref = 1;//ref_id++;//
		top->right = nw;
		nw = md_new_node();

		//no_ch_noded++;
		nw->data = 65;//v_ch++;
		nw->is_data = 1;

		ptr_node[ptr_counter++] = nw;
		strcpy(ptr_node[ptr_counter-1]->bits, "1");
		ptr_node[ptr_counter-1]->no_bits = 1;

		top->left = nw;
		//nw->ref = 1;//ref_id++;//
		nw->top = top;
		nw = md_new_node();
		return;
	}
	int r1, r2, left, right;//float r1, r2, r3;
	r1 = t_ch / 2;
	r2 = r1 / 2;
	if( r2 < 1 )
		r2 = 1;
	left = r1 + r2;
	right = t_ch - left;

	if( left <= level )
	{
		if( right == 1 )
		{
			md_tree( right, top, level/2 );

			nw->data = '<';//r3;
			//nw->ref = 1;//ref_id++;//r3;
			top->left = nw;
			nw->top = top;
			cu = nw;
			nw = md_new_node();
			md_tree( left, cu, level/2 );
		}
		else if( right > 1 )
		{
			nw->data = '>';//t_ch - r3;
			nw->top = top;
			//nw->ref = 1;//ref_id++;//t_ch - r3;
			top->right = nw;
			cu = nw;
			nw = md_new_node();
			md_tree( right, cu, level/2);

			nw->data = '<';//r3;
			//nw->ref = 1;//ref_id++;//r3;
			top->left = nw;
			nw->top = top;
			cu = nw;
			nw = md_new_node();
			md_tree( left, cu, level/2 );
		}
		else // right < 1
		{ //it is here only for checking is there any possibility on this path
			printf("\n\n==> CHECK THIS LINE OF CODE BY Ctrl+Break <===");
			getch(); getch();

		}
	}
	else if( (left + right ) <= level*2 ) // left > level
	{
		nw->data = '<';//r3;
		//nw->ref = ref_id++;//r3;
		top->right = nw;
		nw->top = top;
		cu = nw;
		nw = md_new_node();
		if( (t_ch % 2) != 0 )
			md_tree( t_ch/2+1, cu, level/2 );
		else
			md_tree( t_ch/2, cu, level/2 );
		nw->data = '>';//r3;
		//nw->ref = ref_id++;//r3;
		top->left = nw;
		nw->top = top;
		cu = nw;
		nw = md_new_node();
		md_tree( t_ch/2, cu, level/2 );
	}
}//md_tree()

node* md_new_node()
{
	node *tm = new node;
	tm->data = '-';
	tm->left = NULL;
	tm->right = NULL;
	tm->top = NULL;
	//tm->ref = 0;//ref_id++;//0;
	tm->is_data = 0;

	return tm;
}//md_new_node()

void sort_nodes()
{
	for( int ev=0; ev <= ptr_counter; ev++ )
	{
		for( int iv=ev+1; iv <= ptr_counter; iv++ )
		{
			if( ptr_node[iv]->no_bits < ptr_node[ev]->no_bits )
			{
				node *temp = ptr_node[ev];
				ptr_node[ev] = ptr_node[iv];
				ptr_node[iv] = temp;
			}
		}
	}
}//sort_nodes()
void show_sorted_nodes()
{
	printf("\n\n\nSorted node\
					\nNo\tData\tCode\tBits\t# of Bits");
	for( int v=0; v < ptr_counter; v++ )
	{
/*
		if( ptr_node[v]->data == 13 || ptr_node[v]->data ==	26 ||
										ptr_node[v]->data ==	255 || ptr_node[v]->data == 10 )
			printf("\n%d \t%d \t%d \t%d \t%s \t%d", v+1, ptr_node[v]->ref,
							ptr_node[v]->data, ptr_node[v]->data, ptr_node[v]->bits,
							ptr_node[v]->no_bits
						);
		else
*/
			printf("\n%d \t%c \t%d \t%s \t%d", v+1,
							ptr_node[v]->data, ptr_node[v]->data, ptr_node[v]->bits,
							ptr_node[v]->no_bits
						);
		//if( v % 20 == 0 )
			//getch();
	}
}

void count_node_bits( node *temp )
{
	if( temp == NULL )
		return;

	if( temp->is_data == 1 )
	{
		int no_bits=0;
		char bits[9]="";
		node *temp2 = temp;
		while( temp2->top != NULL )
		{
			no_bits++;
			if( temp2 == temp2->top->left )
				strcat(bits, "0");
				//printf("0");
			else if( temp2 == temp2->top->right )
				strcat(bits, "1");
				//printf("1");
			temp2 = temp2->top;
		}
		strrev(bits);
		strcpy( temp->bits, bits );
		//printf("\t%s", bits);
		temp->no_bits = no_bits;
		return;
	}

	count_node_bits( temp->right );

	count_node_bits( temp->left );
}//count_node_bits()
//end-tree