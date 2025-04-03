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

/////////////////// RB-Tree ///////////////////
class RB_tree
{
private:
    Node *root;

public:
    publisher *best_seller[3];
    stack<string> tree_deep_stack;

    Node *get_root();

    Node *RB_insert(Node *root, Node *ptr);
    void insertValue(vector<string>);
    void insertValueForPartF(vector<string>); // Added for the part f.
    void RB_left_rotate(Node *);
    void RB_right_rotate(Node *);
    void RB_insert_fixup(Node *);
    void preorder();
    void find_best_seller();

    Node *RB_search(Node *root, string name); //To search nodes

    RB_tree();
    ~RB_tree();
};

void print_best_sellers(int year, publisher *temp_publisher[3])
{
    cout.precision(5);
    cout << "End of the " + to_string(year) + " Year" << endl;
    cout << "Best seller in North America: " + temp_publisher[0]->name + " - " << temp_publisher[0]->na_sales << " million" << endl;
    cout << "Best seller in Europe: " + temp_publisher[1]->name + " - " << temp_publisher[1]->eu_sales << " million" << endl;
    cout << "Best seller rest of the World: " + temp_publisher[2]->name + " - " << temp_publisher[2]->others_sales << " million" << endl;
}

RB_tree generate_RBT_tree_from_csv(string file_name)
{
    RB_tree temp_RBtree;

    bool check90 = false, check00 = false, check10 = false, check20 = false; // To keep track of which decade is printed
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
                temp_RBtree.find_best_seller(); // If a decade is ended then we need to find the best sellers.
                print_best_sellers(1990, temp_RBtree.best_seller);
                check90 = true;
            };
            if (stoi(year) > 2000 && !check00)
            {
                temp_RBtree.find_best_seller();
                print_best_sellers(2000, temp_RBtree.best_seller);
                check00 = true;
            };
            if (stoi(year) > 2010 && !check10)
            {
                temp_RBtree.find_best_seller();
                print_best_sellers(2010, temp_RBtree.best_seller);
                check10 = true;
            };

            temp_RBtree.insertValue(x);
            if (stoi(year) >= 2020 && !check20) // This one is executed after inserting.Because the year of the last data is 2020. So we need to insert the last data right before finding and printing best sellers.
            {
                temp_RBtree.find_best_seller();
                print_best_sellers(2020, temp_RBtree.best_seller);
                check20 = true;
            };
        };
        file.close();
    };

    return temp_RBtree;
}
vector<vector<string>> order_data(string file_name) // To sort the data
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
    // 50 random publisher.I used the same lists for both of the trees.
    vector<string> publisherlist = {"Imagic", "Atari", "Activision", "Tigervision", "Coleco", "Sega", "Answer Software", "Quelle", "CPG Products",
                                    "Wizard Video Games", "Mattel Interactive", "Parker Bros.", "ITT Family Games", "Avalon Interactive", "Bomb", "Nintendo", "Namco Bandai Games", "Capcom", "Konami Digital Entertainment",
                                    "SquareSoft", "Hudson Soft", "Maxis", "HAL Laboratory", "Enix Corporation", "Square", "Sega", "Banpresto", "BPS", "Human Entertainment",
                                    "Tecmo Koei", "Culture Brain", "Kemco", "Arena Entertainment", "Takara", "ChunSoft", "SNK", "Square Enix", "Ubisoft", "Electronic Arts",
                                    "Idea Factory", "Red Flagship", "Epic Games", "Sony Computer Entertainment", "Paradox Interactive", "Microsoft Game Studios", "Deep Silver", "Take-Two Interactive", "Bethesda Softworks"};
    // 50 random game
    vector<string> gamelist = {"Dark Souls III", "Jikkyou Powerful Pro Baseball 2016", "Avalon Code", "Cabela's Adventure Camp", "Without Warning", "Harvest Moon: More Friends of Mineral Town", "Yu-Gi-Oh! The Duelists of the Roses", "Skylanders: Trap Team", "Ringling Bros. and Barnum & Bailey: Circus Friends", "Dynasty Warriors: Gundam 2", "Mata Hari: Betrayal is only a Kiss Away", "Minds and Mystery: 5 Game Pack", "Homefront", "Transformer: Rise of the Dark Spark", "Company of Heroes", "Capcom vs. SNK 2: Mark of the Millennium 2001", "Dynasty Warriors 6 Empires", "Top Spin 4", "PhotoKano", "Golden Axe: Beast Rider", "Ultimate Ghosts 'n Goblins (JP sales)", "Nobunaga no Yabou: Soutensoku with Power-Up Kit", "Worldwide Soccer Manager 2008", "Tekken Tag Tournament", "Starsky & Hutch", "Edna & Harvey: Harvey's New Eyes", "Bass Pro Shops: The Hunt", "TV Anime Idolm@ster: Cinderella Girls G4U! Pack Vol.7", "Antz Extreme Racing", "Go! Go! Hypergrind", "Tony Hawk's Underground", "Mike Tyson Heavyweight Boxing", "SoniPro", "TNA iMPACT: Cross the Line", "Famicom Mini: Nazo no Murasame-Jou", "Gauntlet: Dark Legacy", "Wipeout 2", "True Love Story 3", "Hatsune Miku: Project Mirai 2", "Stranglehold", "StarCraft II: Wings of Liberty", "GoGo's Crazy Bones", "Ys VIII: Lacrimosa of Dana", "Astrology DS", "Black", "Digimon World: Next Order", "Defendin' De Penguin", "Street Supremacy", "Conflict: Desert Storm II - Back to Bagdhad", "Alpha Protocol"};
    auto start = std::chrono::high_resolution_clock::now();
    RB_tree RBtree = generate_RBT_tree_from_csv(fname);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Inserting all the csv into RBT tree: " << duration.count() << " microseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (const auto &i : publisherlist)
    {
        x = RBtree.RB_search(RBtree.get_root(), i);
    };
    stop = std::chrono::high_resolution_clock::now();
    auto nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "Total search of 50 publisher RBT: " << nanoduration.count() << " nanoseconds" << std::endl;
    std::cout << "Average search time RBT : " << nanoduration.count() / 50.0 << " nanoseconds" << std::endl;
    RBtree.preorder();

    vector<vector<string>> ordered_data = order_data(fname);
    start = std::chrono::high_resolution_clock::now();
    RB_tree tree_with_ordered_data;
    for (const auto &x : ordered_data)
    {
        tree_with_ordered_data.insertValueForPartF(x);
    };
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Constructing RBT tree(by game name) with ordered data: " << duration.count() << " microseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (const auto &i : gamelist)
    {
        x = tree_with_ordered_data.RB_search(tree_with_ordered_data.get_root(), i);
    };
    stop = std::chrono::high_resolution_clock::now();
    nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "Total search of 50 game RBT(tree constructed with ordered data): " << nanoduration.count() << " nanoseconds" << std::endl;
    std::cout << "Average search time RBT(tree constructed with ordered data): " << nanoduration.count() / 50.0 << " nanoseconds" << std::endl;
}

/////////////////// Node ///////////////////

Node::Node(publisher key)
{
    this->key = key;
    this->color = 1; // "RED";
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
}

/////////////////// RB-Tree ///////////////////
Node *RB_tree::get_root()
{

    // Fill this function.

    return this->root;
}

Node *RB_tree::RB_insert(Node *root, Node *ptr)
{
    // The RBT fixup is in the insertvalue function.
    if (root == NULL) // If the tree is empty then the inserted node is the new root.
    {
        this->root = ptr;
        return ptr;
    };

    if (ptr->key.name < root->key.name) // If the key is less than the root then we need to insert it to the leftside.
    {
        root->left = RB_insert(root->left, ptr);
        root->left->parent = root;
    }
    else if (ptr->key.name > root->key.name) // If the key is greater than the root then we need to insert it to the rightside. Rest of them is the same as in leftinsert.
    {

        root->right = RB_insert(root->right, ptr);
        root->right->parent = root;
    }
    else
    {
        root->key.eu_sales += ptr->key.eu_sales;
        root->key.na_sales += ptr->key.na_sales;
        root->key.others_sales += ptr->key.others_sales;
    }
    return root;
}
void RB_tree::insertValue(vector<string> n)
{
    publisher newp;
    newp.name = n[3];
    newp.na_sales = stof(n[4]);
    newp.eu_sales = stof(n[5]);
    newp.others_sales = stof(n[6]);

    Node *newnode = new Node(newp);
    newnode->color = 1; // newnode is actually initialized with color red.But I added this to be ensure.
    this->root = RB_insert(this->root, newnode);
    RB_insert_fixup(newnode);
}
void RB_tree::insertValueForPartF(vector<string> n)
{
    publisher newp;                 // In this part, actually we do not need other information than the game name. But I wanted to use the same tree structure.
    newp.name = n[0];               // We are basically constructing the tree according to the game name.
    newp.na_sales = stof(n[4]);     // Unused
    newp.eu_sales = stof(n[5]);     // Unused
    newp.others_sales = stof(n[6]); // Unused

    Node *newnode = new Node(newp); // Creating new node with the new key.
    newnode->color = 1;
    this->root = RB_insert(this->root, newnode);
    RB_insert_fixup(newnode);
}
void RB_tree::RB_left_rotate(Node *ptr)
{
    Node *a = ptr->right;
    ptr->right = a->left;
    if (a->left != NULL)
    {
        a->left->parent = ptr;
    };
    a->parent = ptr->parent;
    if (ptr->parent == NULL)
    {
        root = a;
    }
    else if (ptr == ptr->parent->left)
    {
        ptr->parent->left = a;
    }
    else
    {
        ptr->parent->right = a;
    }
    a->left = ptr;
    ptr->parent = a;
};

void RB_tree::RB_right_rotate(Node *ptr)
{
    Node *a = ptr->left;
    ptr->left = a->right;
    if (a->right != NULL)
    {
        a->right->parent = ptr;
    };
    a->parent = ptr->parent;
    if (ptr->parent == NULL)
    {
        root = a;
    }
    else if (ptr == ptr->parent->right)
    {
        ptr->parent->right = a;
    }
    else
    {
        ptr->parent->left = a;
    }
    a->right = ptr;
    ptr->parent = a;
}

void RB_tree::RB_insert_fixup(Node *ptr)
{
    while (ptr != root && ptr->parent != NULL && ptr->parent->color == 1)
    {
        if (ptr->parent == ptr->parent->parent->left) // Checking if the parent of the ptr is the left child.
        {
            Node *uncle = ptr->parent->parent->right;  // Then uncle is in the rightside.
            if (uncle != nullptr && uncle->color == 1) // Case 1
            {
                ptr->parent->color = 0;         // Coloring parent black
                uncle->color = 0;               // Coloring uncle black
                ptr->parent->parent->color = 1; // Coloring grandparent red
                ptr = ptr->parent->parent;      // Switching "grandson" with "grandparent"
            }
            else
            {
                if (ptr == ptr->parent->right) // Case 2
                {
                    ptr = ptr->parent;
                    RB_left_rotate(ptr);
                }
                ptr->parent->color = 0;               // Case 3 coloring parent black
                ptr->parent->parent->color = 1;       // Coloring grandparent red
                RB_right_rotate(ptr->parent->parent); // Right rotating grandparent
            }
        }
        else
        {
            Node *uncle = ptr->parent->parent->left;
            if (uncle != nullptr && uncle->color == 1)
            {
                ptr->parent->color = 0;
                uncle->color = 0;
                ptr->parent->parent->color = 1;
                ptr = ptr->parent->parent;
            }
            else
            {
                if (ptr == ptr->parent->left)
                {
                    ptr = ptr->parent;
                    RB_right_rotate(ptr);
                }
                ptr->parent->color = 0;
                ptr->parent->parent->color = 1;
                RB_left_rotate(ptr->parent->parent);
            }
        }
    }
    root->color = 0;
}

void RB_tree::preorder()
{
    stack<pair<Node *, int>> s; // To keep node and depth info.
    s.push({root, 0});
    while (!s.empty())
    {
        Node *current = s.top().first;
        int depth = s.top().second;
        s.pop();
        string color = (current->color == 1) ? "(RED)" : "(BLACK)";
        cout << string(depth, '-') << color << " " << current->key.name << endl;
        if (current->right != NULL) // We are preorder travelling so the "rightest" nodes should be in the end of the stack.
        {
            s.push({current->right, depth + 1});
        }
        if (current->left != NULL) // Then we can push the "leftest" nodes.
        {
            s.push({current->left, depth + 1});
        }
    }
}

void RB_tree::find_best_seller()
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

Node *RB_tree::RB_search(Node *root, string name)
{
    if (root == NULL || root->key.name == name) // If the root is null or is equal to the searched name then returning the result.
    {
        return root;
    };
    if (name < root->key.name) // If the selected name is "less than" root then search the left subtree.
    {
        return RB_search(root->left, name);
    }
    else
    { // Else search the right subtree.
        return RB_search(root->right, name);
    };
}

RB_tree::RB_tree()
{
    this->root = NULL;
    this->best_seller[0] = NULL;
    this->best_seller[1] = NULL;
    this->best_seller[2] = NULL;
}

RB_tree::~RB_tree()
{
}
