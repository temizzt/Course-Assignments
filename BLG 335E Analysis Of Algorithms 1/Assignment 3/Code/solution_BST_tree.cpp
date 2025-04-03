/*
	Taha Temiz
	150210055
	20.12.2024

*/

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <iomanip>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;

/////////////////// Player ///////////////////
class publisher
{
public:
	string name;
	float na_sales;
	float eu_sales;
	float others_sales;
};

/////////////////// Node ///////////////////
class Node
{
public:
	publisher key;
	int color; // "Red"=1 or "Black"=0
	Node *parent, *left, *right;

	Node(publisher);
	~Node();
	int get_color();
	void set_color(int);
};

/////////////////// BST-Tree ///////////////////
class BST_tree
{
private:
	Node *root;

public:
	publisher *best_seller[3];
	stack<string> tree_deep_stack;

	Node *get_root();

	Node *BST_insert(Node *root, Node *ptr);
	Node *BST_delete(Node *root, Node *ptr);

	Node *BST_search(Node *root, string name); // To search publishers by name.

	void insertValue(vector<string>);
	void insertValueForPartF(vector<string>); //Added 
	void find_best_seller();

	BST_tree();
	~BST_tree();
};

void print_best_sellers(int year, publisher *temp_publisher[3])
{
	cout.precision(5);
	cout << "End of the " + to_string(year) + " Year" << endl;
	cout << "Best seller in North America: " + temp_publisher[0]->name + " - " << temp_publisher[0]->na_sales << " million" << endl;
	cout << "Best seller in Europe: " + temp_publisher[1]->name + " - " << temp_publisher[1]->eu_sales << " million" << endl;
	cout << "Best seller rest of the World: " + temp_publisher[2]->name + " - " << temp_publisher[2]->others_sales << " million" << endl;
}

BST_tree generate_BST_tree_from_csv(string file_name)
{

	bool check90 = false, check00 = false, check10 = false, check20 = false; // To keep track of which decade is printed
	BST_tree temp_BSTtree;
	string name, platform, year, publisher, na, eu, other;
	ifstream file;
	string line;
	file.open(file_name);
	getline(file, line, '\n');
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			vector<string> x;
			stringstream currentline(line);

			getline(currentline, name, ',');
			getline(currentline, platform, ',');
			getline(currentline, year, ',');
			getline(currentline, publisher, ',');
			getline(currentline, na, ',');
			getline(currentline, eu, ',');
			getline(currentline, other, '\n');
			x.push_back(name);
			x.push_back(platform);
			x.push_back(year);
			x.push_back(publisher);
			x.push_back(na);
			x.push_back(eu);
			x.push_back(other);
			if (stoi(year) > 1990 && !check90) // Checking if the year is greater than 1990.
			{
				temp_BSTtree.find_best_seller(); // If a decade is ended then we need to find the best sellers.
				print_best_sellers(1990, temp_BSTtree.best_seller);
				check90 = true;
			};
			if (stoi(year) > 2000 && !check00)
			{
				temp_BSTtree.find_best_seller();
				print_best_sellers(2000, temp_BSTtree.best_seller);
				check00 = true;
			};
			if (stoi(year) > 2010 && !check10)
			{
				temp_BSTtree.find_best_seller();
				print_best_sellers(2010, temp_BSTtree.best_seller);
				check10 = true;
			};

			temp_BSTtree.insertValue(x);
			if (stoi(year) >= 2020 && !check20) // This one is executed after inserting.Because the year of the last data is 2020. So we need to insert the last data right before finding and printing best sellers.
			{
				temp_BSTtree.find_best_seller();
				print_best_sellers(2020, temp_BSTtree.best_seller);
				check20 = true;
			};
		};
		file.close();
	};

	return temp_BSTtree;
}
vector<vector<string>> order_data(string file_name) // To sort the data according to the game name
{
	vector<vector<string>> finalarray;
	string name, platform, year, publisherr, na, eu, other;
	ifstream file;
	string line;
	file.open(file_name);
	getline(file, line, '\n');
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			vector<string> x;
			stringstream currentline(line);

			getline(currentline, name, ',');
			getline(currentline, platform, ',');
			getline(currentline, year, ',');
			getline(currentline, publisherr, ',');
			getline(currentline, na, ',');
			getline(currentline, eu, ',');
			getline(currentline, other, '\n');
			x.push_back(name);
			x.push_back(platform);
			x.push_back(year);
			x.push_back(publisherr);
			x.push_back(na);
			x.push_back(eu);
			x.push_back(other);
			finalarray.push_back(x);
		};
		file.close();
	}

	sort(finalarray.begin(), finalarray.end(), [](const vector<std::string> &a, const vector<std::string> &b)
		 { return a[0] < b[0]; });

	return finalarray;
}

////////////////////////////////////////////
//----------------- MAIN -----------------//
////////////////////////////////////////////
int main(int argc, char *argv[])
{

	string fname = argv[1];
	Node *x;
	// 50 random publisher. I used the same lists for both trees.
	vector<string> publisherlist = {"Imagic", "Atari", "Activision", "Tigervision", "Coleco", "Sega", "Answer Software", "Quelle", "CPG Products",
									"Wizard Video Games", "Mattel Interactive", "Parker Bros.", "ITT Family Games", "Avalon Interactive", "Bomb", "Nintendo", "Namco Bandai Games", "Capcom", "Konami Digital Entertainment",
									"SquareSoft", "Hudson Soft", "Maxis", "HAL Laboratory", "Enix Corporation", "Square", "Sega", "Banpresto", "BPS", "Human Entertainment",
									"Tecmo Koei", "Culture Brain", "Kemco", "Arena Entertainment", "Takara", "ChunSoft", "SNK", "Square Enix", "Ubisoft", "Electronic Arts",
									"Idea Factory", "Red Flagship", "Epic Games", "Sony Computer Entertainment", "Paradox Interactive", "Microsoft Game Studios", "Deep Silver", "Take-Two Interactive", "Bethesda Softworks"};
	// 50 random game.
	vector<string> gamelist = {"Dark Souls III", "Jikkyou Powerful Pro Baseball 2016", "Avalon Code", "Cabela's Adventure Camp", "Without Warning", "Harvest Moon: More Friends of Mineral Town", "Yu-Gi-Oh! The Duelists of the Roses", "Skylanders: Trap Team", "Ringling Bros. and Barnum & Bailey: Circus Friends", "Dynasty Warriors: Gundam 2", "Mata Hari: Betrayal is only a Kiss Away", "Minds and Mystery: 5 Game Pack", "Homefront", "Transformer: Rise of the Dark Spark", "Company of Heroes", "Capcom vs. SNK 2: Mark of the Millennium 2001", "Dynasty Warriors 6 Empires", "Top Spin 4", "PhotoKano", "Golden Axe: Beast Rider", "Ultimate Ghosts 'n Goblins (JP sales)", "Nobunaga no Yabou: Soutensoku with Power-Up Kit", "Worldwide Soccer Manager 2008", "Tekken Tag Tournament", "Starsky & Hutch", "Edna & Harvey: Harvey's New Eyes", "Bass Pro Shops: The Hunt", "TV Anime Idolm@ster: Cinderella Girls G4U! Pack Vol.7", "Antz Extreme Racing", "Go! Go! Hypergrind", "Tony Hawk's Underground", "Mike Tyson Heavyweight Boxing", "SoniPro", "TNA iMPACT: Cross the Line", "Famicom Mini: Nazo no Murasame-Jou", "Gauntlet: Dark Legacy", "Wipeout 2", "True Love Story 3", "Hatsune Miku: Project Mirai 2", "Stranglehold", "StarCraft II: Wings of Liberty", "GoGo's Crazy Bones", "Ys VIII: Lacrimosa of Dana", "Astrology DS", "Black", "Digimon World: Next Order", "Defendin' De Penguin", "Street Supremacy", "Conflict: Desert Storm II - Back to Bagdhad", "Alpha Protocol"};
	auto start = std::chrono::high_resolution_clock::now();
	BST_tree BSTtree = generate_BST_tree_from_csv(fname);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Inserting all the csv into BST tree: " << duration.count() << " microseconds" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	for (const auto &i : publisherlist)
	{
		x = BSTtree.BST_search(BSTtree.get_root(), i);
	};
	stop = std::chrono::high_resolution_clock::now();
	auto nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
	std::cout << "Total search of 50 publisher BST: " << nanoduration.count() << " nanoseconds" << std::endl;
	std::cout << "Average search time BST: " << nanoduration.count() / 50.0 << " nanoseconds" << std::endl;

	vector<vector<string>> ordered_data = order_data(fname);
	start = std::chrono::high_resolution_clock::now();
	BST_tree tree_with_ordered_data;
	for (const auto &x : ordered_data)
	{
		tree_with_ordered_data.insertValueForPartF(x);
	};
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Constructing bst tree(by game name) with ordered data: " << duration.count() << " microseconds" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	for (const auto &i : gamelist)
	{
		x = tree_with_ordered_data.BST_search(tree_with_ordered_data.get_root(), i);
	};
	stop = std::chrono::high_resolution_clock::now();
	nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
	std::cout << "Total search of 50 game BST(tree constructed with ordered data): " << nanoduration.count() << " nanoseconds" << std::endl;
	std::cout << "Average search time BST(tree constructed with ordered data): " << nanoduration.count() / 50.0 << " nanoseconds" << std::endl;

	return EXIT_SUCCESS;
}

/////////////////// Node ///////////////////

Node::Node(publisher key)
{
	this->key = key;
	this->parent = NULL;
	this->left = NULL;
	this->right = NULL;
}

Node *BST_tree::get_root()
{

	return this->root;
}

Node *BST_tree::BST_insert(Node *root, Node *ptr)
{

	if (root == NULL) // If the tree is empty then the inserted node is the new root.
	{
		this->root = ptr;
		return ptr;
	};

	if (ptr->key.name < root->key.name) // If the key is less than the root then we need to insert it to the leftside.
	{
		root->left = BST_insert(root->left, ptr);
		root->left->parent = root;
	}
	else if (ptr->key.name > root->key.name) // If the key is greater than the root then we need to insert it to the rightside. Rest of them is the same as in leftinsert.
	{

		root->right = BST_insert(root->right, ptr);
		root->right->parent = root;
	}
	else
	{ // If the key already exists in the tree then update its sale data.
		root->key.eu_sales += ptr->key.eu_sales;
		root->key.na_sales += ptr->key.na_sales;
		root->key.others_sales += ptr->key.others_sales;
	}

	return root;
}

void BST_tree::insertValue(vector<string> n)
{
	publisher newp; // Creating new publisher according to the input.
	newp.name = n[3];
	newp.na_sales = stof(n[4]);
	newp.eu_sales = stof(n[5]);
	newp.others_sales = stof(n[6]);

	Node *newnode = new Node(newp);				  // Creating new node with the new key.
	this->root = BST_insert(this->root, newnode); // Inserting to the tree.
}

void BST_tree::insertValueForPartF(vector<string> n)
{
	publisher newp;					// In this part, actually we do not need other information than the game name. But I wanted to use the same tree structure.
	newp.name = n[0];				// We are basically constructing the tree according to the game name.
	newp.na_sales = stof(n[4]);		// Unused
	newp.eu_sales = stof(n[5]);		// Unused
	newp.others_sales = stof(n[6]); // Unused

	Node *newnode = new Node(newp); // Creating new node with the new key.
	this->root = BST_insert(this->root, newnode);
}

Node *BST_tree::BST_search(Node *root, string name)
{
	if (root == NULL || root->key.name == name) // If the root is null or is equal to the searched name then returning the result.
	{
		return root;
	};
	if (name < root->key.name) // If the selected name is "less than" root then search the left subtree.
	{
		return BST_search(root->left, name);
	}
	else
	{ // Else search the right subtree.
		return BST_search(root->right, name);
	};
}

void BST_tree::find_best_seller()
{
	stack<Node *> stack; // To keep track of visited nodes
	Node *current = this->root;

	while (!stack.empty() || current != NULL)
	{
		while (current != NULL) // Traversing until the "leftest" side of the tree
		{
			stack.push(current);
			current = current->left;
		}

		current = stack.top();
		stack.pop();

		if (best_seller[0] == NULL) // Comparing the current element's sale data with the best sellers. If any of the best sellers is not found yet, then it is the current key.
		{
			best_seller[0] = &current->key;
		}
		else
		{
			if (current->key.na_sales > best_seller[0]->na_sales)
			{
				best_seller[0] = &current->key;
			}
		}
		if (best_seller[1] == NULL)
		{
			best_seller[1] = &current->key;
		}
		else
		{
			if (current->key.eu_sales > best_seller[1]->eu_sales)
			{
				best_seller[1] = &current->key;
			}
		}
		if (best_seller[2] == NULL)
		{
			best_seller[2] = &current->key;
		}
		else
		{
			if (current->key.others_sales > best_seller[2]->others_sales)
			{
				best_seller[2] = &current->key;
			}
		}

		current = current->right; // Then we can go to the right. It is like inorder traversal.
	}
}

BST_tree::BST_tree()
{
	this->root = NULL;
	this->best_seller[0] = NULL;
	this->best_seller[1] = NULL;
	this->best_seller[2] = NULL;
}

BST_tree::~BST_tree()
{
}
