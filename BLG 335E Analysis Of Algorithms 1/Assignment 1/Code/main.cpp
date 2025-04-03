/*
    Name:Taha Temiz
    Number:150210055
    Date:03.11.2024
*/

#include "tweet.h"
#include <chrono>

int main()
{

    std::vector<Tweet> tweets = readTweetsFromFile("data/permutations/tweets.csv");

    auto start = std::chrono::high_resolution_clock::now();
    bubbleSort(tweets, "retweetCount", true);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "bubble sort tweets csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/permutations/tweetsNS.csv");
    start = std::chrono::high_resolution_clock::now();
    bubbleSort(tweets, "retweetCount", true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "bubble sort tweetsNS csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/permutations/tweetsSA.csv");
    start = std::chrono::high_resolution_clock::now();
    bubbleSort(tweets, "retweetCount", true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "bubble sort tweetsSA csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/permutations/tweetsSD.csv");
    start = std::chrono::high_resolution_clock::now();
    bubbleSort(tweets, "retweetCount", true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "bubble sort tweetsSD csv: " << duration.count() << " microseconds" << std::endl;

//*********************************************************************************************************** */
    tweets = readTweetsFromFile("data/permutations/tweets.csv");
    start = std::chrono::high_resolution_clock::now();
    insertionSort(tweets,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "insertion sort tweets csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/permutations/tweetsNS.csv");

    start = std::chrono::high_resolution_clock::now();
    insertionSort(tweets,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "insertion sort tweetsNS csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/permutations/tweetsSA.csv");
    start = std::chrono::high_resolution_clock::now();
    insertionSort(tweets,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "insertion sort tweetsSA csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/permutations/tweetsSD.csv");
    start = std::chrono::high_resolution_clock::now();
    insertionSort(tweets,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "insertion sort tweetsSD csv: " << duration.count() << " microseconds" << std::endl;
//************************************************************************************************************ */
    tweets = readTweetsFromFile("data/permutations/tweets.csv");
    start = std::chrono::high_resolution_clock::now();
    mergeSort(tweets,0,tweets.size() - 1,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "mergesort tweets csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/permutations/tweetsNS.csv");

    start = std::chrono::high_resolution_clock::now();
    mergeSort(tweets,0,tweets.size()-1,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "mergesort sort tweetsNS csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/permutations/tweetsSA.csv");
    start = std::chrono::high_resolution_clock::now();
    mergeSort(tweets,0,tweets.size()-1,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "mergesort sort tweetsSA csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/permutations/tweetsSD.csv");
    start = std::chrono::high_resolution_clock::now();
    mergeSort(tweets,0,tweets.size()-1,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "mergesort sort tweetsSD csv: " << duration.count() << " microseconds" << std::endl;
    //*********************** */
    //*////////////////////////////////////////////////////////////////////////////////////////////////

    tweets = readTweetsFromFile("data/sizes/tweets5K.csv");
    start = std::chrono::high_resolution_clock::now();
    bubbleSort(tweets, "retweetCount", true);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "bubble sort tweets5K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets10K.csv");
    start = std::chrono::high_resolution_clock::now();
    bubbleSort(tweets, "retweetCount", true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "bubble sort tweets10K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets20K.csv");
    start = std::chrono::high_resolution_clock::now();
    bubbleSort(tweets, "retweetCount", true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "bubble sort tweets20K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets30K.csv");
    start = std::chrono::high_resolution_clock::now();
    bubbleSort(tweets, "retweetCount", true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);  
    std::cout << "bubble sort tweets30K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets50K.csv");
    start = std::chrono::high_resolution_clock::now();
    bubbleSort(tweets, "retweetCount", true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "bubble sort tweets50K csv: " << duration.count() << " microseconds" << std::endl;

//*********************************************************************************************************** */
    tweets = readTweetsFromFile("data/sizes/tweets5K.csv");
    start = std::chrono::high_resolution_clock::now();
    insertionSort(tweets,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "insertion sort tweets5K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets10K.csv");
    start = std::chrono::high_resolution_clock::now();
    insertionSort(tweets,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "insertion sort tweets10K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets20K.csv");
    start = std::chrono::high_resolution_clock::now();
    insertionSort(tweets,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "insertion sort tweets20K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets30K.csv");
    start = std::chrono::high_resolution_clock::now();
    insertionSort(tweets,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "insertion sort tweets30K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets50K.csv");
    start = std::chrono::high_resolution_clock::now();
    insertionSort(tweets,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "insertion sort tweets50K csv: " << duration.count() << " microseconds" << std::endl;
//************************************************************************************************************ */
    tweets = readTweetsFromFile("data/sizes/tweets5K.csv");
    start = std::chrono::high_resolution_clock::now();
    mergeSort(tweets,0,tweets.size()-1,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "mergesort tweets5K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets10K.csv");

    start = std::chrono::high_resolution_clock::now();
    mergeSort(tweets,0,tweets.size()-1,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "mergesort sort tweets10K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets20K.csv");
    start = std::chrono::high_resolution_clock::now();
    mergeSort(tweets,0,tweets.size()-1,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "mergesort sort tweets20K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets30K.csv");
    start = std::chrono::high_resolution_clock::now();
    mergeSort(tweets,0,tweets.size()-1,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "mergesort sort tweets30K csv: " << duration.count() << " microseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets50K.csv");
    start = std::chrono::high_resolution_clock::now();
    mergeSort(tweets,0,tweets.size()-1,"retweetCount",true);
    stop = std::chrono::high_resolution_clock::now();
    writeTweetsToFile("deneme.csv", tweets);
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "mergesort sort tweets50K csv: " << duration.count() << " microseconds" << std::endl;
    //******/*******/****-** */ */ */

    tweets = readTweetsFromFile("data/sizes/tweets5K.csv");
    mergeSort(tweets,0,tweets.size()-1,"tweetID",true);
    start = std::chrono::high_resolution_clock::now();
    int index = binarySearch(tweets,1773335,"tweetID");
    stop = std::chrono::high_resolution_clock::now();
    std::cout<<index<<std::endl;
    auto nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "binarysearch tweets5K csv: " << nanoduration.count() << " nanoseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets10K.csv");
    mergeSort(tweets,0,tweets.size()-1,"tweetID",true);
    start = std::chrono::high_resolution_clock::now();
    index = binarySearch(tweets,1773335,"tweetID");
    stop = std::chrono::high_resolution_clock::now();
    nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "binarysearch tweets10K csv: " << nanoduration.count() << " nanoseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets20K.csv");
    mergeSort(tweets,0,tweets.size()-1,"tweetID",true);
    start = std::chrono::high_resolution_clock::now();
    index = binarySearch(tweets,1773335,"tweetID");
    stop = std::chrono::high_resolution_clock::now();
    nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "binarysearch tweets20K csv: " << nanoduration.count() << " nanoseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets30K.csv");
    mergeSort(tweets,0,tweets.size()-1,"tweetID",true);
    start = std::chrono::high_resolution_clock::now();
    index = binarySearch(tweets,1773335,"tweetID");
    stop = std::chrono::high_resolution_clock::now();
    nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "binarysearch tweets30K csv: " << nanoduration.count() << " nanoseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets50K.csv");
    mergeSort(tweets,0,tweets.size()-1,"tweetID",true);
    start = std::chrono::high_resolution_clock::now();
    index = binarySearch(tweets,1773335,"tweetID");
    stop = std::chrono::high_resolution_clock::now();
    nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "binarysearch tweets50K csv: " << nanoduration.count() << " nanoseconds" << std::endl;


    tweets = readTweetsFromFile("data/sizes/tweets5K.csv");
    start = std::chrono::high_resolution_clock::now();
    int count = countAboveThreshold(tweets,"favoriteCount",250);
    stop = std::chrono::high_resolution_clock::now();
    nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "count tweets5K csv: " << nanoduration.count() << " nanoseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets10K.csv");
    start = std::chrono::high_resolution_clock::now();
    count = countAboveThreshold(tweets,"favoriteCount",250);
    stop = std::chrono::high_resolution_clock::now();
    nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "count tweets10K csv: " << nanoduration.count() << " nanoseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets20K.csv");
    start = std::chrono::high_resolution_clock::now();
    count = countAboveThreshold(tweets,"favoriteCount",250);
    stop = std::chrono::high_resolution_clock::now();
    nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "count tweets20K csv: " << nanoduration.count() << "nanoseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets30K.csv");
    start = std::chrono::high_resolution_clock::now();
    count = countAboveThreshold(tweets,"favoriteCount",250);
    stop = std::chrono::high_resolution_clock::now();
    nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "count tweets30K csv: " << nanoduration.count() << " nanoseconds" << std::endl;

    tweets = readTweetsFromFile("data/sizes/tweets50K.csv");
    start = std::chrono::high_resolution_clock::now();
    count = countAboveThreshold(tweets,"favoriteCount",250);
    stop = std::chrono::high_resolution_clock::now();
    nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "count tweets50K csv: " << nanoduration.count() << " nanoseconds" << std::endl;
    std::cout<<count<<std::endl;




}
