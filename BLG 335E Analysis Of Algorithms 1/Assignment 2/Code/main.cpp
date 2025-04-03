/*Taha Temiz
150210055
26/11/2024
*/

#include "methods.h"
#include <chrono>

int main()
{

  auto start = std::chrono::high_resolution_clock::now();
  std::vector<Item> items = readItemsFromFile("data/test/items_s.csv");
  std::vector<Item> sorted = countingSort(items, "age", true);
  writeItemsToFile("data/items_s_sorted.csv", sorted);
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "counting sort items s : " << duration.count() << " microseconds" << std::endl;

  start = std::chrono::high_resolution_clock::now();
  calculateRarityScores(sorted, 50);
  writeItemsToFile("data/items_s_rarity.csv", sorted);
  stop = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "items s rarity calc csv: " << duration.count() << " microseconds" << std::endl;
  start = std::chrono::high_resolution_clock::now();
  sorted = heapSortByRarity(sorted, true);
  writeItemsToFile("data/items_s_rarity_sorted.csv", sorted);
  stop = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "items s rarity heap: " << duration.count() << " microseconds" << std::endl;


  
  start = std::chrono::high_resolution_clock::now();
  items = readItemsFromFile("data/test/items_m.csv");
  sorted = countingSort(items, "age", true);
  writeItemsToFile("data/items_m_sorted.csv", sorted);
  stop = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "counting sort items m : " << duration.count() << " microseconds" << std::endl;

  start = std::chrono::high_resolution_clock::now();
  calculateRarityScores(sorted, 50);
  writeItemsToFile("data/items_m_rarity.csv", sorted);
  stop = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "items m rarity calc csv: " << duration.count() << " microseconds" << std::endl;
  start = std::chrono::high_resolution_clock::now();
  sorted = heapSortByRarity(sorted, true);
  writeItemsToFile("data/items_m_rarity_sorted.csv", sorted);
  stop = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "items m rarity heap: " << duration.count() << " microseconds" << std::endl;

  start = std::chrono::high_resolution_clock::now();
  items = readItemsFromFile("data/test/items_l.csv");
  sorted = countingSort(items, "age", true);
  writeItemsToFile("data/items_l_sorted.csv", sorted);
  stop = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "counting sort items l : " << duration.count() << " microseconds" << std::endl;

  start = std::chrono::high_resolution_clock::now();
  calculateRarityScores(sorted, 50);
  writeItemsToFile("data/items_l_rarity.csv", sorted);
  stop = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "items l rarity calc csv: " << duration.count() << " microseconds" << std::endl;
  start = std::chrono::high_resolution_clock::now();
  sorted = heapSortByRarity(sorted, true);
  writeItemsToFile("data/items_l_rarity_sorted.csv", sorted);
  stop = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "items l rarity heap: " << duration.count() << " microseconds" << std::endl;

  return 0;
}