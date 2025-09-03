#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

// Функция для поиска коллизий хеш-функции в тексте
// Хешер передается как параметр шаблона. Текст читается из потока по словам.
// Коллизией считается ситуация, когда новое слово имеет тот же хеш, что и предыдущее слово,
// но при этом отличается от всех слов с этим хешом.
template <typename Hash>
int FindCollisions(const Hash& hasher, istream& text) {
    string word;
    int collisions_count = 0;
    
    // Структура для хранения данных:
    // ключ - хеш слова, значение - множество уникальных слов с этим хешом
    unordered_map<size_t, unordered_set<string>> hash_to_words;

    while (text >> word) {
        size_t hash_value = hasher(word);
        
        // Если хеш уже встречался
        if (hash_to_words.count(hash_value)) {
            // Если слово с таким хешом уже есть в множестве - это не коллизия
            if (hash_to_words[hash_value].count(word)) {
                continue;
            }
            else {
                // Новое слово с существующим хешом - коллизия!
                ++collisions_count;
                hash_to_words[hash_value].insert(word);
            }
        }
        else {
            // Новый хеш - добавляем слово в множество
            hash_to_words[hash_value].insert(word);
        }
    }
    
    return collisions_count;
}

// Простой (и плохой) хешер для тестирования
// Недостаток: производит только сумму кодов символов,
// что приводит к частым коллизиям (например, "ab" и "ba" дают одинаковый хеш)
struct HasherDummy {
    size_t operator()(const string& str) const {
        size_t res = 0;
        for (char c : str) {
            res += static_cast<size_t>(c);
        }
        return res;
    }
};

int main() {
    // Используем стандартную хеш-функцию для строк
    hash<string> str_hasher;
    
    int collisions = FindCollisions(str_hasher, cin);
    cout << "Found collisions: "s << collisions << endl;
}
