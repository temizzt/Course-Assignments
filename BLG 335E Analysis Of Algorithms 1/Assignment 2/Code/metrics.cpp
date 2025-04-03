/*Taha Temiz
150210055
26/11/2024
*/


#include "methods.h"

int getMax(std::vector<Item> &items, const std::string &attribute)
{
    int max = 0;   
    if (attribute == "age") // Checking attribute
    {
        for (const Item &i : items) // Comparing all the items in the vector
        {
            if (i.age > max)   //If the current one's age is bigger than max, then this is our max age. 
            {
                max = i.age;
            };
        };
        return max;
    }
    else if (attribute == "type")   //Other ones works exactly same as "age" one. I am not sure if this is necessary but there is an attribute input so I implemented getmax for all attributes.
    {
        for (const Item &i : items)
        {
            if (i.type > max)
            {
                max = i.type;
            };
        };
        return max;
    }
    else if (attribute == "origin")
    {
        for (const Item &i : items)
        {
            if (i.origin > max)
            {
                max = i.origin;
            };
        };
        return max;
    }
    else if (attribute == "rarity")
    {
        for (const Item &i : items)
        {
            if (i.rarityScore > max)
            {
                max = i.rarityScore;
            };
        };
        return max;
    };
    return max; //If attribute is undefined then returns zero
}

// min = age - ageWindow
// max = age + ageWindow
// rarityScore = (1 - probability) * (1 + item.age/ageMax)
void calculateRarityScores(std::vector<Item> &items, int ageWindow)
{
    int maxage = getMax(items,"age"); //Getting the maxage
    for (Item &i : items) //Computing the rarity score for all items in items vector.
    {
        double p = 0.0;     //Initializing values.
        int countSimilar = 0;
        int countTotal = 0;
        double rarity = 0.0;
        for (const Item &j : items) //To count the total and similar ones we should check every items in vector.
        {
            if (j.age >= i.age - ageWindow && j.age <= i.age + ageWindow)   //If the current item is in the agewindow of the main item that we are calculating the rarity score of itself,
            {
                countTotal++;       //Then counttotal should be incremented
                if (j.type == i.type && j.origin == i.origin) //And if the current element is also with the same type and origin with the main item that we are calculating the rarity of itself, then countsimilar should be incremented.
                {
                    countSimilar++;
                }
            };
        };
        if (countTotal > 0) //After counting, we should check counttotal.This part is in the formula in the assignment file. So I added this if else condition but this if else part is actually unnecessary. Because in counting part, we are counting the the item itself that we are calculating the rarity score of it. So even in a situation with zero similar items; counttotal and countsimilar are minimum 1.I think there is no way for counttotal<=0 but as a mentioned before,the formula has this part.
        {
            p = static_cast<double>(countSimilar) /countTotal; //I added static_cast to calculate with double values.Without it, the results were mistaken because of integer calculation.
        }
        else
        {
            p = 0.0;
        };
        rarity = (1 - p) * (1 + static_cast<double>(i.age) / maxage); //static_cast for the same reason.
        i.rarityScore = rarity;//After the calculations updating the rarity score.
    };
}
