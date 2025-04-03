/*
    Name:Taha Temiz
    Number:150210055
    Date:03.11.2024


*/


#include "tweet.h"

void bubbleSort(std::vector<Tweet> &tweets, const std::string &sortBy, bool ascending)
{
    if (sortBy == "tweetID")
    {
        for (int i = 0; i < tweets.size() - 1; i++) //n times sorting the pairs from the begining until the end.
        {
            for (int j = 0; j < tweets.size() - i - 1; j++)
            {
                if (ascending ? tweets[j].tweetID > tweets[j + 1].tweetID : tweets[j].tweetID < tweets[j + 1].tweetID)//Sorting the pairs according to ascending value
                {
                    Tweet x; //I did not want to use swap function so every time a swap is needed, a tweet named x is created to swap pairs.
                    x = tweets[j];
                    tweets[j] = tweets[j + 1];
                    tweets[j + 1] = x;
                };
            };
        };
    }
    else if (sortBy == "retweetCount")
    {
        for (int i = 0; i < tweets.size() - 1; i++)
        {
            for (int j = 0; j < tweets.size() - i - 1; j++)
            {
                if (ascending ? tweets[j].retweetCount > tweets[j + 1].retweetCount : tweets[j].retweetCount < tweets[j + 1].retweetCount)
                {
                    Tweet x;
                    x = tweets[j];
                    tweets[j] = tweets[j + 1];
                    tweets[j + 1] = x;
                };
            };
        };
    }
    else if (sortBy == "favoriteCount")
    {
        for (int i = 0; i < tweets.size() - 1; i++)
        {
            for (int j = 0; j < tweets.size() - i - 1; j++)
            {
                if (ascending ? tweets[j].favoriteCount > tweets[j + 1].favoriteCount : tweets[j].favoriteCount < tweets[j + 1].favoriteCount)
                {
                    Tweet x;
                    x = tweets[j];
                    tweets[j] = tweets[j + 1];
                    tweets[j + 1] = x;
                };
            };
        };
    };
}

void insertionSort(std::vector<Tweet> &tweets, const std::string &sortBy, bool ascending)
{
    if (sortBy == "tweetID")
    {
        for (int i = 1; i < tweets.size(); i++) //Starting from the second element of the vector
        {
            Tweet currentTweet = tweets[i];
            int j = i - 1;

            while (j >= 0 && (ascending ? tweets[j].tweetID > currentTweet.tweetID : tweets[j].tweetID < currentTweet.tweetID))//Checking ascending
            {
                tweets[j + 1] = tweets[j];
                j = j - 1; //To swap elements
            };
            tweets[j + 1] = currentTweet;
        };
    }
    else if (sortBy == "retweetCount")
    {
        for (int i = 1; i < tweets.size(); i++)
        {
            Tweet currentTweet = tweets[i];
            int j = i - 1;

            while (j >= 0 && (ascending ? tweets[j].retweetCount > currentTweet.retweetCount : tweets[j].retweetCount < currentTweet.retweetCount))
            {
                tweets[j + 1] = tweets[j];
                j = j - 1;
            };
            tweets[j + 1] = currentTweet;
        }
    }
    else if (sortBy == "favoriteCount")
    {
        for (int i = 1; i < tweets.size(); i++)
        {
            Tweet currentTweet = tweets[i];
            int j = i - 1;

            while (j >= 0 && (ascending ? tweets[j].favoriteCount > currentTweet.favoriteCount : tweets[j].favoriteCount < currentTweet.favoriteCount))
            {
                tweets[j + 1] = tweets[j];
                j = j - 1;
            };
            tweets[j + 1] = currentTweet;
        };
    };
}

void merge(std::vector<Tweet> &tweets, int left, int mid, int right, const std::string &sortBy, bool ascending)
{
    std::vector<Tweet> leftpart(mid - left + 1), rightpart(right - mid); 
    for (int i = 0; i < mid - left + 1; i++) // Dividing the given vector into two sub-vectors.
    {
        leftpart[i] = tweets[left + i];
    };
    for (int i = 0; i < right - mid; i++)
    {
        rightpart[i] = tweets[mid + 1 + i];
    };

    int index = left; //To track the index of last sorted element.
    int i = 0, j = 0;

    if (sortBy == "tweetID")
    {
        while (i < leftpart.size() && j < rightpart.size())//Until there is no element left in one of the sub-vectors for adding to tweets vector. 
        {
            if (ascending ? leftpart[i].tweetID < rightpart[j].tweetID : leftpart[i].tweetID > rightpart[j].tweetID)//Adding elements from subvectors to the tweets vector in a sorted way according to ascending.
            {
                tweets[index] = leftpart[i];
                i++;
            }
            else
            {
                tweets[index] = rightpart[j];
                j++;
            };
            index++;
        };
        while (i < leftpart.size())//Adding remaining elements of sub-vectors.
        {
            tweets[index] = leftpart[i];
            i++;
            index++;
        }
        while (j < rightpart.size())
        {
            tweets[index] = rightpart[j];
            j++;
            index++;
        };
    }
    else if (sortBy == "retweetCount")
    {
        while (i < leftpart.size() && j < rightpart.size())
        {
            if (ascending ? leftpart[i].retweetCount < rightpart[j].retweetCount : leftpart[i].retweetCount > rightpart[j].retweetCount)
            {
                tweets[index] = leftpart[i];
                i++;
            }
            else
            {
                tweets[index] = rightpart[j];
                j++;
            };
            index++;
        };
        while (i < leftpart.size())
        {
            tweets[index] = leftpart[i];
            i++;
            index++;
        }
        while (j < rightpart.size())
        {
            tweets[index] = rightpart[j];
            j++;
            index++;
        };
    }
    else if (sortBy == "favoriteCount")
    {
        while (i < leftpart.size() && j < rightpart.size())
        {
            if (ascending ? leftpart[i].favoriteCount < rightpart[j].favoriteCount : leftpart[i].favoriteCount > rightpart[j].favoriteCount)
            {
                tweets[index] = leftpart[i];
                i++;
            }
            else
            {
                tweets[index] = rightpart[j];
                j++;
            };
            index++;
        };
        while (i < leftpart.size())
        {
            tweets[index] = leftpart[i];
            i++;
            index++;
        }
        while (j < rightpart.size())
        {
            tweets[index] = rightpart[j];
            j++;
            index++;
        };
    };
}

void mergeSort(std::vector<Tweet> &tweets, int left, int right, const std::string &sortBy, bool ascending)
{
    if (left < right)//repeating until divided one by one
    {
        int mid = left + (right - left) / 2;
        mergeSort(tweets, left, mid, sortBy, ascending);//recursively calling itself to divide repeatedly
        mergeSort(tweets, mid + 1, right, sortBy, ascending);
        merge(tweets, left, mid, right, sortBy, ascending);//merging the pieces in a sorted way
    }
    else
    {
        return;
    }
}
