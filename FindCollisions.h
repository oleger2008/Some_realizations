#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

using namespace std;

template <typename Hash>
int FindCollisions(Hash& hasher, istream& text) {
	int result = 0;
	string word;
	unordered_map<size_t, unordered_set<string>> buckets;
	while (text >> word) {
		size_t current_hash = hasher(word);
		if (buckets.count(current_hash)) {
			if (buckets.at(current_hash).size() >= 2) {
				++result;
			}
			buckets.at(current_hash).emplace(word);
		} else {
			buckets[current_hash].emplace(word);
		}
	}
	return result;
}