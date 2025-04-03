/*
    Name:Taha Temiz
    Number:150210055
    Date:03.11.2024
*/

#include "tweet.h"

int binarySearch(const std::vector<Tweet>& tweets, long long key, const std::string& sortBy)
{
    int low = 0;
    int high = tweets.size() - 1;
    if(sortBy == "tweetID"){
        while(low <= high){
        int mid = low + (high - low) / 2;

        if(key==tweets[mid].tweetID){
            return mid;
        }else if(key > tweets[mid].tweetID){
            low = mid + 1;
        }else if(key < tweets[mid].tweetID){
            high = mid - 1;
        };
        };
        return -1;


    }else if(sortBy == "favoriteCount"){
        while(low <= high){
        int mid = low + (high - low) / 2;

        if(key == tweets[mid].favoriteCount){
            return mid;
        }else if(key > tweets[mid].favoriteCount){
            low = mid + 1;
        }else if(key < tweets[mid].favoriteCount){
            high = mid - 1;
        };
        };
        return -1;


    }else if(sortBy == "retweetCount"){
        while(low <= high){
        int mid = low + (high - low) / 2;

        if(key == tweets[mid].retweetCount){
            return mid;
        }else if(key > tweets[mid].retweetCount){
            low = mid + 1;
        }else if(key < tweets[mid].retweetCount){
            high = mid - 1;
        };
        };
        return -1;


    }else{
        return -1;
    }

}

int countAboveThreshold(const std::vector<Tweet>& tweets, const std::string& metric, int threshold) 
{
    int n = 0;
    if(metric == "favoriteCount"){
        for(const Tweet &i : tweets){
            if(i.favoriteCount> threshold){
                n++;
            };
        };
        return n;
    }else if(metric == "retweetCount"){
        for(const Tweet &i: tweets){
            if(i.retweetCount > threshold){
                n++;
            };
        };
        return n;
    }else{
        return n;
    };

}
