/*
    Name:Taha Temiz
    Number:150210055
    Date:03.11.2024
*/

#include "tweet.h"

std::vector<Tweet> readTweetsFromFile(const std::string &filename)
{
    std::vector<Tweet> tweets;
    std::ifstream file;
    std::string line;
    file.open(filename);
    std::getline(file, line, '\n');
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            std::stringstream currentline(line);
            std::string strtweetID, strfav, strrt;
            Tweet x;
            std::getline(currentline, strtweetID, ',');
            std::getline(currentline, strrt, ',');
            std::getline(currentline, strfav, '\n');
            x.tweetID = std::stoi(strtweetID);
            x.retweetCount = std::stoi(strrt);
            x.favoriteCount = std::stoi(strfav);
            tweets.push_back(x);
        };
        file.close();
    };
    return tweets;
}

void writeTweetsToFile(const std::string &filename, const std::vector<Tweet> &tweets)
{
        std::ofstream file(filename);

        file<<"tweetID,retweet_count,favorite_count\n";

        for(const Tweet &i:tweets){
            file<<i.tweetID << ",";
            file<<i.retweetCount << ",";
            file<<i.favoriteCount << "\n";
        };

        file.close();



}