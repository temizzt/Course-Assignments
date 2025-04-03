/*Taha Temiz
150210055
26/11/2024
*/

#include "methods.h"

std::vector<Item> countingSort(std::vector<Item> &items, const std::string &attribute, bool ascending)
{
    std::vector<Item> finalarray(items.size());
    if (attribute == "age")//Checking the attribute
    {
        int n = getMax(items, "age");//To do counting sort,we need the max age,our counts vector will be in length of maxelement.
        std::vector<int> counts(n + 1, 0);//From zero to the max element so n+1
        for (const Item &i : items)//Counting the numbers of each element in the whole vector.
        {
            counts[i.age] = counts[i.age] + 1;//Incrementing the count of the current "value" in the input vector.
        };
        for (int i = 1; i < n + 1; i++)//Adding all the elements with previous elements to get the minimum proper indexes for values.For example, if there is 2 zeros and 2 ones than index of an element with value 2 can be minimum 4 because of the summation.
        {
            counts[i] = counts[i] + counts[i - 1];
        };
        if (ascending)//Creating the counts vector is the same in ascending and descending order. The difference is in indexing part.
        {
            for (int i = items.size() - 1; i >= 0; i--)
            {
                finalarray[counts[items[i].age] - 1] = items[i];//As a mentioned before, inserting the current element of input vector to the proper minimum index.We are starting from leftside of finalarray so it is ascending order.
                counts[items[i].age]--;//Decrementing the count of the value of the element 1 because we added one of the elements.
            };
        }
        else
        {
            for (int i = items.size() - 1; i >= 0; i--)
            {
                finalarray[items.size() - 1 - (counts[items[i].age] - 1)] = items[i];//This time we are starting from rightside of finalarray..So index is items.size() - 1 - (counts[items[i].age] - 1). So it is descending order.
                counts[items[i].age]--;////Decrementing the count of the value of the element 1 because we added one of the elements.
            };
        }
        return finalarray;//Returning the final array
    }
    else if (attribute == "type")//They are all same as the age one.
    {
        int n = getMax(items, "type");
        std::vector<int> counts(n + 1, 0);
        for (const Item &i : items)
        {
            counts[i.type] = counts[i.type] + 1;
        };
        for (int i = 1; i < n + 1; i++)
        {
            counts[i] = counts[i] + counts[i - 1];
        };
        if (ascending)
        {
            for (int i = items.size() - 1; i >= 0; i--)
            {
                finalarray[counts[items[i].type] - 1] = items[i];
                counts[items[i].type]--;
            };
        }
        else
        {
            for (int i = items.size() - 1; i >= 0; i--)
            {
                finalarray[items.size() - 1 - (counts[items[i].type] - 1)] = items[i];
                counts[items[i].type]--;
            };
        }
        return finalarray;
    }
    else if (attribute == "rarity")
    {
        int n = getMax(items, "rarity");
        std::vector<int> counts(n + 1, 0);
        for (const Item &i : items)
        {
            counts[i.rarityScore] = counts[i.rarityScore] + 1;
        };
        for (int i = 1; i < n + 1; i++)
        {
            counts[i] = counts[i] + counts[i - 1];
        };
        if (ascending)
        {
            for (int i = items.size() - 1; i >= 0; i--)
            {
                finalarray[counts[items[i].rarityScore] - 1] = items[i];
                counts[items[i].rarityScore]--;
            };
        }
        else
        {
            for (int i = items.size() - 1; i >= 0; i--)
            {
                finalarray[items.size() - 1 - (counts[items[i].rarityScore] - 1)] = items[i];
                counts[items[i].rarityScore]--;
            };
        };
        return finalarray;
    }
    else if (attribute == "origin")
    {
        int n = getMax(items, "origin");
        std::vector<int> counts(n + 1, 0);
        for (const Item &i : items)
        {
            counts[i.origin] = counts[i.origin] + 1;
        };
        for (int i = 1; i < n + 1; i++)
        {
            counts[i] = counts[i] + counts[i - 1];
        };
        if (ascending)
        {
            for (int i = items.size() - 1; i >= 0; i--)
            {
                finalarray[counts[items[i].origin] - 1] = items[i];
                counts[items[i].origin]--;
            };
        }
        else
        {
            for (int i = items.size() - 1; i >= 0; i--)
            {
                finalarray[items.size() - 1 - (counts[items[i].origin] - 1)] = items[i];
                counts[items[i].origin]--;
            };
        }
        return finalarray;
    };
    return finalarray;
}

// Function to heapify a subtree rooted with node i in the array of items
void heapify(std::vector<Item> &items, int n, int i, bool descending)
{
    int root = i;//Assuming the given vector is a tree.And the root of it is the first element.
    int left = 2 * i + 1;//Left child of the root.
    int right = 2 * i + 2;//Right child of the root.
    bool needSwap = false;//To check if a swap is needed.

    if (left < n && (descending ? items[root].rarityScore > items[left].rarityScore : items[root].rarityScore < items[left].rarityScore))//Checking if there is a leftchild or we are out of the vector.If leftchild is avaible then according to the descending value, check if the leftchild is less than the root or bigger than the root. If descending is true then the root should be less than the child so check if the root is greater than the leftchild.In ascending order, check the opposite.
    {
        root = left;//If the order of root and leftchild does not match the descending value then our new root is left.
        needSwap = true;//We need swap.
    };
    if (right < n && (descending ? items[root].rarityScore > items[right].rarityScore : items[root].rarityScore < items[right].rarityScore))//Same checking for right child.
    {
        root = right;//If the order of root and rightchild does not match the descending value then our new root is right.
        needSwap = true;//We need swap.
    };
    if (needSwap)
    {
        std::swap(items[i], items[root]);//Swaping new root with the previous root.
        heapify(items, n, root, descending);//Heapifying again with our new root with the same descending value.
    };
}

// Function to perform heap sort on rarityScore scores of items
std::vector<Item> heapSortByRarity(std::vector<Item> &items, bool descending)
{
    for (int i = items.size() / 2 - 1; i >= 0; i--)//We are starting from the root with the smallest value.The items with greater indexes than size/2-1 are leafs of the tree.And heapifying until the start of the vector.
    {
        heapify(items, items.size(), i, descending);
    };
    for (int i = items.size() - 1; i > 0; i--)//We are starting from the end because first element is the biggest or the highest so we need to insert it to the end.
    {
        std::swap(items[0], items[i]);//Inserting the first element to the end.
        heapify(items, i, 0, descending);//Then heapifying the part excluded the sorted part.
    };
    return items;
}
