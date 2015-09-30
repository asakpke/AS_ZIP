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
	int ref;
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
int is_byte_full();
void add_end_char_tag();
int is_bits_repeats( char );

//start-tree
char tre_byte=0;
int tre_bit=8;
//end-tree
char my_byte=0;
int my_bit;
unsigned long total_bytes_stored=0;
unsigned long tre_total_bytes_stored=0;
FILE *fpw, *fpr;
//start-tree
FILE *tre_fpw, *tre_fpr;
int index_special_13=0;
int index_special_26=0;
int index_special_255=0;
int index_special=0;
//end-tree

int main (int argc, char *argl[])
{
	int v;
	char tmp_var, file_temp[70];
	char file_path[50]="c:\\tc\\bin\\", file_name[9]="file", file_ext[5]=".tzp";
// *
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

// * /

	strcpy( file_temp, file_path );
	strcat( file_temp, file_name );
	strcat( file_temp, file_ext );
	tre_fpr = fopen( file_temp, "rb" );
	printf("\n\n\nReading data using tree");
	printf("\n********************************************************");
	size_t tre_sz;
	//tmp_var = fgetc(tre_fpr);
	tre_sz = fread( &tmp_var, sizeof(char), 1, tre_fpr);
	printf("\n\nVer figure stored code %d" , tmp_var );
	//getch();
	char t_chs;
	//tmp_var = fgetc(tre_fpr);
	tre_sz = fread( &tmp_var, sizeof(char), 1, tre_fpr);
	t_chs = tmp_var;
	printf(", Total bytes figure stored code = %d" , tmp_var );
	getch();
	rt = md_new_node();
	nw = md_new_node();	// /-->> total_chars+1+3, +3 for 3 special chars
	ptr_counter=0;
	md_tree( t_chs+1, rt, 128 );//+1 for index_special
	count_node_bits( rt);
	sort_nodes();
	printf("\n\nStored chars = ");
	for( v=0; v < t_chs; v++ ) //note v=1;
	{
		//tmp_var = fgetc(tre_fpr);
		tre_sz = fread( &tmp_var, sizeof(char), 1, tre_fpr);
		ptr_node[v]->data = tmp_var;
		printf("%c", ptr_node[v]->data);
	}
	getch();
	ptr_node[v]->data = EOF;
	show_sorted_nodes();
	getch();
//
	char tre_var;
	node *temp_node = rt;
	printf("\n\nStored data = \n");
	int tre_bit_var;
	tre_sz = fread( &tre_byte, sizeof(char), 1, tre_fpr);
	//while( (tre_byte = fgetc(tre_fpr)) != EOF	) // read new char from file
	unsigned long total_byte=0;
	unsigned long total_chars=0;
	FILE *fpw;
	strcpy( file_temp, file_path );
	strcat( file_temp, file_name );
	strcat( file_temp, ".Uzp" );
	fpw = fopen( file_temp, "w" );
	if( fpw == NULL)
	{
		fprintf(stderr, "Cannot open input file.\n");
		return 1;
	}
	while( tre_sz != 0 ) // read new char from file
	{	//now read all file and decompress the file
		total_byte++;
		tre_bit_var = 1;
		tre_bit_var <<= 7;
		for( v =8; v > 0; v-- )
		{
			if( temp_node->is_data == 1 )
			{
				total_chars++;
				printf("%c", temp_node->data);
				fputc( temp_node->data, fpw );
				temp_node = rt;
			}
			if( tre_byte & tre_bit_var )
				temp_node = temp_node->right;
			else
				temp_node = temp_node->left;
			tre_bit_var >>= 1;
		}
		if( temp_node->is_data == 1 )
		{
			total_chars++;
			printf("%c", temp_node->data);
			fputc( temp_node->data, fpw );
			temp_node = rt;
		}
		tre_sz = fread( &tre_byte, sizeof(char), 1, tre_fpr);
	} // while( (tre_byte = fgetc(tre_fpr)) != EOF	)
	fclose(fpw);
	fclose(tre_fpr);
	printf("\n\nTotal bytes stored = %d", total_byte);
	printf(" + Different chars = %d", t_chs );
	printf(" + ver and total char byte = 2" );
	printf(" ==> %d", total_byte+t_chs+2);
	printf("\nTotal chars ziped= %d", total_chars);
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
		nw->ref = 1;//ref_id++;//
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
		nw->ref = 1;//ref_id++;//
		top->right = nw;
		nw = md_new_node();

		//no_ch_noded++;
		nw->data = 65;//v_ch++;
		nw->is_data = 1;

		ptr_node[ptr_counter++] = nw;
		strcpy(ptr_node[ptr_counter-1]->bits, "1");
		ptr_node[ptr_counter-1]->no_bits = 1;

		top->left = nw;
		nw->ref = 1;//ref_id++;//
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
			nw->ref = 1;//ref_id++;//r3;
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
			nw->ref = 1;//ref_id++;//t_ch - r3;
			top->right = nw;
			cu = nw;
			nw = md_new_node();
			md_tree( right, cu, level/2);

			nw->data = '<';//r3;
			nw->ref = 1;//ref_id++;//r3;
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
	tm->ref = 0;//ref_id++;//0;
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
					\nNo\tRef\tData\tCode\tBits\t# of Bits");
	for( int v=0; v < ptr_counter; v++ )
	{
		//printf("\n%d \t%d \t%c \t%d \t%s \t%d", v+1, ptr_node[v]->ref, ptr_node[v]->data
		//				, ptr_node[v]->data, ptr_node[v]->bits, ptr_node[v]->no_bits
		//			);
/*
		if( ptr_node[v]->data == 13 || ptr_node[v]->data ==	26 ||
									ptr_node[v]->data ==	255 || ptr_node[v]->data ==	10 )
			printf("\n%d \t%d \t%d \t%d \t%s \t%d", v+1, ptr_node[v]->ref,
							ptr_node[v]->data, ptr_node[v]->data, ptr_node[v]->bits,
							ptr_node[v]->no_bits
						);
		else
*/
			printf("\n%d \t%d \t%c \t%d \t%s \t%d", v+1, ptr_node[v]->ref,
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