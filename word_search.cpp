#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<iomanip>

//numbers used for directions
#define horizontal 0 
#define vertical 1
#define diagonal2 2 //diagonal from the bottom
#define diagonal1 3 //diagonal from the top

using namespace std;

class puzzle
{
private:

	string title; 
	string* words; //for input words
	int number_of_words, row, column, curr_word; //  row, column= dimension of the arr.  curr_word= current word to be placed 
	char* arr; // array where words are placed. Treating 1D array as 2D.
	struct answer //to store the point and direction of a word 
	{
		int point;
		int way; //way stores the direction
	} *ans ;

public:

	void input();
	void decide_size(); // decide size based on the input words
	void initalization(); 
	void make(); // to place words in the array
	bool selection(int); // select valid points for a word in a given direction
	bool horizontal_check(int);// check if a word can be placed in horizontal direction at a given point
	bool vertical_check(int);
	bool diagonal2_check(int);
	bool diagonal1_check(int);
	bool word_fill(int, int); // place the word in the array from a given point in a given direction
	void reset(); // reset the array
	void display();
	int random_num(int); // return a random number

};


void puzzle::input()
{
	cout << "Enter title\n";
	getline(cin, title);
	cout << "\nHow many words you want to enter?\n";
	cin >> number_of_words;
	if (number_of_words < 1)
	{
		cout << "\nNumber of words should be greater than or equal to one\n";
		exit(0);
	}

	words = new string[number_of_words];
	ans = new answer[number_of_words];

	cout << "\nEnter words\n";
	for (int r = 0; r < number_of_words; r++)
		cin >> words[r];

	for (int r = 0; r < number_of_words; r++)
		if (words[r].length() < 3)
		{
			cout << "\nMinimun length of a word should be 3";
			exit(0);
		}

	for (int r = 0; r < number_of_words; r++) //to convert to uppercase letters
		for (int j = 0; j < words[r].length(); j++)
			if (words[r][j] >= 97 && words[r][j] <= 122)
				words[r][j] = words[r][j] - 32;

}


void puzzle::decide_size()
{
	// first we arrange words in descending order
	int  sum = words[0].length();
	for (int r = 1, j; r < number_of_words; r++)
   	{  // insertion sort to arrange words in descending order and calculate the total letters in all words
		for (j = r - 1, sum = sum + words[r].length(); j >= 0; j--)
			if (words[r].length() <= words[j].length())
				break;

		if (j != r - 1)
		{
			j++;
			string m, temp;

			for (m = words[r]; j <= r; j++)
			{
				temp = m;
				m = words[j];
				words[j] = temp;
			}
		}
	}

	row = words[0].length(); //let number of rows equal length of longest word

	if ((row * row) <= sum)  //sum= total letters in all words
	{
		column = row;

		while (row * column < sum)
		{
			if (row == column)
				row++;
			else
				column++;
		}
	}

	else
	{
		column = sum / row;
		if (sum % row)
			column++;

		column = column * 2;  
		if (column > row) // we try to make size of column equal to length of longest word so words can be
			column = row; // placed in any direction and not only in vertical direction
			

	}

}



void puzzle::initalization()
{
	arr = new char[row * column];
	for (int r = 0; r < row * column; r++)
		arr[r] = '\0';

	for (int r = 0; r < number_of_words; r++)
	{
		ans[r].point = -1;
		ans[r].way = -1;
	}
}




void puzzle::make()
{
	
	
	bool a;
	int w1 = -1, w2 = -1; //variables to indicate if longest and next longer words are placed 
	                      // next longer word is the word of length less than the longest word
	while (1)
	{
		if (column >= words[0].length())  
		{  // (if size of column is >= length of longest word)
		   // first choose any random direction for the longest word and place it in the array
		   // Now next longer word(if exists) should not have same direction as longest word 
		   // By doing this we prevent the possibility that next words will be placed in only one direction.


			int m;
			for (m = 1; m < number_of_words; m++) // find next longer word
				if (words[m].length() < words[0].length())
					break;

			if (m < number_of_words) //if next longer word found
			{
				int way;
				vector<int> temp1, temp2; // temp1 for directions of longest word and temp2 for directions of next longer word
				w1 = 0; w2 = m; // words placed 
				temp1.push_back(horizontal); temp1.push_back(vertical); temp1.push_back(diagonal2); temp1.push_back(diagonal1);

				while (temp1.size())
				{

					curr_word = 0; //current word= longest word
					way = random_num(temp1.size()); // select a random direction
					selection(temp1[way]); // find the valid points and place the word

					for (int r = 0; r < 4; r++) //ways other than the one selected for longest word 
						if (r != temp1[way])
							temp2.push_back(r);

					temp1.erase(temp1.begin() + way);

					curr_word = m; //current word= next longer word 
					while (temp2.size())
					{
						way = random_num(temp2.size());
						a = selection(temp2[way]); 
						if (a == 1) // if word placed 
							break;
						temp2.erase(temp2.begin() + way); // else select another direction

					}

					if (a == 1)
						break;

					// if next longer word could not be placed remove the longest word from the array 
					for (int r = 0; r < row * column; r++)
						arr[r] = '\0';

					ans[0].point = -1;
					ans[0].way = -1;

				}

			}

		}

		// now we place every word in the array
		for (curr_word = 0; curr_word < number_of_words; curr_word++)
		{

			if (curr_word == w1 || curr_word == w2) //if words are already placed
				continue;


			if (words[curr_word].length() > column) //if column size < length of word then only valid direction would be vertical
			{ 
				a = selection(vertical);
				if (a == 0)
					break;
			}

			else
			{
				vector<int> ways;
				int w;
				ways.push_back(horizontal); ways.push_back(vertical); ways.push_back(diagonal2); ways.push_back(diagonal1);

				while (ways.size())
				{
					w = random_num(ways.size());// select random direction
					a = selection(ways[w]);
					if (a == 1) // if word is placed
						break;
					ways.erase(ways.begin() + w);
				}

				if (a == 0) // if word cannot be placed
					break;


			}



		}



		if (curr_word == number_of_words)// if all words are placed
			break;

		reset();// else reset

	}

	for (int r = 0; r < row * column; r++) // fill left spaces with random letters
	{
		if (arr[r] == '\0')
			arr[r] = (char)65 + random_num(26);
	}



}




int puzzle::random_num(int num) 
{
	return rand() % num;
}


bool puzzle::selection(int way)
{
	bool a;
	int l, k, length;
	vector<int>valid_points;

	length = words[curr_word].length();

	switch (way)
	{
		
	case horizontal: // check all the points form where the words can be placed horizontally
		for (l = 0; l < row * column; l += column)
		{
			for (k = 0; k <= column - length; k++)
			{
				a = horizontal_check(l + k); // if word can be placed then it is a valid point
				if (a == 1) 
					valid_points.push_back(l + k);
			}
		}

		break;
		// similarly we check for other directions
	case vertical:
		for (l = 0; l < column * ((row - length) + 1); l++)
		{
			a = vertical_check(l);
			if (a == 1)
				valid_points.push_back(l);
		}

		break;

	case diagonal2:
		for (l = column * (row - 1); l >= column * (length - 1); l -= column)
		{
			for (k = 0; k <= column - length; k++)
			{
				a = diagonal2_check(l + k);
				if (a == 1)
					valid_points.push_back(l + k);
			}
		}

		break;

	case diagonal1:
		for (l = 0; l < column * ((row - length) + 1); l += column)
		{
			for (k = 0; k <= column - length; k++)
			{
				a = diagonal1_check(l + k);
				if (a == 1)
					valid_points.push_back(l + k);
			}
		}


	}


	if (valid_points.size())
	{
		int d = random_num(valid_points.size()); // select one of the valid points randomly
		int point = valid_points[d];	
		ans[curr_word].point = point; 
		ans[curr_word].way = way;
		return word_fill(point, way);

	}

	else // if no valid point for a word in given direction is found
		return 0;

}


bool puzzle::horizontal_check(int point)
{
	int length = words[curr_word].length();
	for (int j = 0; j < length; j++)
		if (!((arr[point + j] == words[curr_word][j]) || (arr[point + j] == '\0'))) // ! (if space is equal to the letter or space is empty)
			return 0;

	return 1;
}


bool puzzle::vertical_check(int point)
{
	int length = words[curr_word].length();
	for (int j = 0; j < length; j++)
		if (!((arr[point + (column * j)] == words[curr_word][j]) || (arr[point + (column * j)] == '\0')))
			return 0;

	return 1;
}



bool puzzle::diagonal2_check(int point)
{
	int length = words[curr_word].length();
	for (int j = 0; j < length; j++)
		if (!((arr[point - (column * j) + j] == words[curr_word][j]) || (arr[point - (column * j) + j] == '\0')))
			return 0;

	return 1;
}



bool puzzle::diagonal1_check(int point)
{
	int length = words[curr_word].length();
	for (int j = 0; j < length; j++)
	{
		if (!((arr[point + (column * j) + j] == words[curr_word][j]) || (arr[point + (column * j) + j] == '\0')))
			return 0;
	}

	return 1;

}



bool puzzle::word_fill(int point, int way)
{
	int r;
	int length = words[curr_word].length();

	switch (way)
	{
	case horizontal:
		for (r = 0; r < length; r++)
			arr[point + r] = words[curr_word][r];
		
		return 1;

	case vertical:
		for (r = 0; r < length; r++)
			arr[point + (column * r)] = words[curr_word][r];
		
		return 1;

	case diagonal2:

		for (int j = 0; j < length; j++)
			arr[point - (column * j) + j] = words[curr_word][j];
		
		return 1;

	case diagonal1:

		for (int j = 0; j < length; j++)
			arr[point + (column * j) + j] = words[curr_word][j];
		
		return 1;
	}

	return 1;
}



void puzzle::display()
{    
	cout << "\n\n\n\n" << title << endl << endl;

	for (int r = 0; r < row * column; r++)
	{
	  if (r % column == 0)
	  cout << "\n\n ";
	  cout << arr[r] << "   ";
	}
		
      cout << "\n\n\n\nWords to search:";
	  int width = words[0].length() + 2;

		for (int r = 0; r < number_of_words; r++)
		{
			if (r % 3 == 0)
				cout << "\n\n";
			cout << left << setw(width) << words[r];

		}
           cout << "\n\n\n\na for answer";
		   cout << "\n\nq to quit\n\n";

		   char option;
		   
		   while (1)
		   {
			   cin >> option;
			   if (option == 'a' || option == 'q')
				   break;
			   cout << "\a";
		   }

		   if (option == 'a')
		   {
			   cout << "\n\n\nAnswer format : word (row, column, way)";
			   cout << "\n\nrow, column indicate the starting position of the word";
			   cout << "\n\nway gives the direction as:";
			   cout << "\n0= horizontal   1= vertical   2= diagonal from the bottom   3= diagonal from the top\n";

			   width = words[0].length()+1;

			   int r, c;

			   for (int e = 0; e < number_of_words; e++)
			   {
				   if (e % 3 == 0)
					   cout << "\n\n";

				   r = (ans[e].point + 1) / column; // from the point we find its row and column number
				   c = (ans[e].point + 1) % column;
				   if (c == 0)
					   c = column;
				   else
					   r++;

				   cout << right << setw(width) << words[e] << " " << "(" << r << ", " << c << ", " << ans[e].way << ")   ";
				   
			   }

			   cout << "\n\n\nq to quit\n";
			   while (1)
			   {
				   cin >> option;
				   if (option == 'q')
					   break;
				   cout << "\a";
			   }
		   
		   }
			

	delete[] arr;
	delete[] ans;
}


void puzzle::reset()
{
	delete[]arr;

	if (row == column)
		row++;
	else
		column++;

	initalization();
}


int main()
{
	srand(time(0));
	puzzle p;
	p.input();
	p.decide_size();
	p.initalization();
	p.make();
	p.display();
	return 0;
}
