/*Taha Temiz
150210055
26/11/2024
*/

#include "methods.h"

std::vector<Item> readItemsFromFile(const std::string& filename) 
{
    std::vector<Item> Items;
    std::ifstream file;
    std::string line;
    file.open(filename);
    std::getline(file, line, '\n');
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            std::stringstream currentline(line);
            std::string strage, strtype, strorigin,strrarity;
            Item x;
            std::getline(currentline, strage, ',');
            std::getline(currentline, strtype, ',');
            std::getline(currentline, strorigin, ',');
            std::getline(currentline, strrarity, '\n');
            x.age = std::stoi(strage);
            x.type = std::stoi(strtype);
            x.origin = std::stoi(strorigin);
            x.rarityScore = std::stoi(strrarity);
            Items.push_back(x);
        };
        file.close();
    };
    return Items;

}

void writeItemsToFile(const std::string& filename, const std::vector<Item>& items) 
{
        std::ofstream file(filename);

        file<<"age,type,origin,rarity\n";

        for(const Item &i:items){
            file<<i.age << ",";
            file<<i.type << ",";
            file<<i.origin << ",";
            file<<i.rarityScore << "\n";
        };

        file.close();

}
