#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <regex>

using namespace std;
//Structure for News Article
struct Article {
    string title;
    string content;
    string category;
    int year;
    int month;
};

class StringMap {
    private:
        struct Entry {
            string key;
            int value;
            Entry* next;
            Entry(const string& k, int v) : key(k), value(v), next(nullptr) {}
        };
    
        static const int TABLE_SIZE = 10007;
        Entry* table[TABLE_SIZE];
    
        int hash(const string& key) const {
            int hash = 0;
            for (char c : key) {
                hash = (hash * 31 + c) % TABLE_SIZE;
            }
            return hash;
        }
    
    public:
        StringMap() {
            for (int i = 0; i < TABLE_SIZE; i++) {
                table[i] = nullptr;
            }
        }
    
        ~StringMap() {
            for (int i = 0; i < TABLE_SIZE; i++) {
                Entry* current = table[i];
                while (current != nullptr) {
                    Entry* next = current->next;
                    delete current;
                    current = next;
                }
            }
        }
    
        void insert(const string& key, int value) {
            int index = hash(key);
            Entry* current = table[index];
            while (current != nullptr) {
                if (current->key == key) {
                    current->value = value;
                    return;
                }
                current = current->next;
            }
            Entry* newEntry = new Entry(key, value);
            newEntry->next = table[index];
            table[index] = newEntry;
        }
    
        int& operator[](const string& key) {
            int index = hash(key);
            Entry* current = table[index];
            while (current != nullptr) {
                if (current->key == key) {
                    return current->value;
                }
                current = current->next;
            }
            Entry* newEntry = new Entry(key, 0);
            newEntry->next = table[index];
            table[index] = newEntry;
            return newEntry->value;
        }
    
       
        pair<string, int>* getEntries(int &size) const {
            size = 0;
    
        
            for (int i = 0; i < TABLE_SIZE; i++) {
                Entry* current = table[i];
                while (current != nullptr) {
                    size++;
                    current = current->next;
                }
            }
    
            if (size == 0) return nullptr;
    
         
            pair<string, int>* entries = new pair<string, int>[size];
    
            int index = 0;
            for (int i = 0; i < TABLE_SIZE; i++) {
                Entry* current = table[i];
                while (current != nullptr) {
                    entries[index++] = {current->key, current->value};
                    current = current->next;
                }
            }
    
            return entries;
        }
    };
    
// **Custom Dynamic Array Implementation**
class ArticleArray {
private:
    Article* data;
    int size;
    int capacity;

    void resize(int newCapacity) {
        Article* newData = new Article[newCapacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    ArticleArray() : data(new Article[100]), size(0), capacity(100) {}

    ~ArticleArray() { delete[] data; }

    void push_back(const Article& article) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        data[size++] = article;
    }

    Article& operator[](int index) { return data[index]; }

    int getSize() const { return size; }
    void analyzeWordFrequency() {
        StringMap wordCount;
        int processedArticles = 0;
    
        cout << "\nAnalyzing word frequency in government-related fake news..." << endl;
    
        for (int i = 0; i < size; i++) {
            string subjectLower = data[i].category;
            transform(subjectLower.begin(), subjectLower.end(), subjectLower.begin(), ::tolower);
    
            if (subjectLower.find("government") != string::npos) {
                stringstream ss(data[i].content);
                string word;
                while (ss >> word) {
                    transform(word.begin(), word.end(), word.begin(), ::tolower);
                    word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
                    if (word.length() > 3) {
                        wordCount[word]++;
                    }
                }
                processedArticles++;
            }
        }
        int wordCountSize = 0;
        pair<string, int>* wordFreq = wordCount.getEntries(wordCountSize);
    
        if (!wordFreq || wordCountSize == 0) {
            cout << "No significant word frequency data found.\n";
            return;
        }
        for (int i = 0; i < wordCountSize - 1; i++) {
            for (int j = i + 1; j < wordCountSize; j++) {
                if (wordFreq[i].second < wordFreq[j].second) {
                    swap(wordFreq[i], wordFreq[j]);
                }
            }
        }
        cout << "\nMost common words in fake government news:\n";
        for (int i = 0; i < min(10, wordCountSize); i++) {
            cout << wordFreq[i].first << ": " << wordFreq[i].second << " occurrences" << endl;
        }
        delete[] wordFreq;
    }
    
};


string cleanField(string field) {
    while (!field.empty() && (field[0] == ' ' || field[0] == '"' || field[0] == '\t')) {
        field = field.substr(1);
    }
    while (!field.empty() && (field.back() == ' ' || field.back() == '"' || field.back() == '\t')) {
        field.pop_back();
    }
    return field;
}
void extractYearMonth(const string &date, int &year, int &month) {
    regex date_regex(R"((\d{1,2})-([A-Za-z]+)-(\d{2}))"); 
    smatch match;

    if (regex_search(date, match, date_regex)) {
        int year_last_two_digits = stoi(match[3]);
        year = (year_last_two_digits >= 0 && year_last_two_digits <= 25) 
               ? (2000 + year_last_two_digits) 
               : (1900 + year_last_two_digits); 
                string monthStr = match[2];
                string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        month = 0;
        for (int i = 0; i < 12; i++) {
            if (monthStr == months[i]) {
                month = i + 1;
                break;
            }
        }
    } else {
        year = 0;
        month = 0;
    }
}
void loadArticles(const string &filename, ArticleArray& articles) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string title, content, category, dateStr;
        getline(ss, title, ','); getline(ss, content, ','); getline(ss, category, ','); getline(ss, dateStr, ',');
        Article article; article.title = cleanField(title); article.content = cleanField(content); article.category = cleanField(category); extractYearMonth(cleanField(dateStr), article.year, article.month);
        articles.push_back(article);
    }
    file.close();
}
int partition(Article* arr, int low, int high) {
    int pivot = arr[high].year;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].year <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(Article* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
void displayArticles(ArticleArray& articles) {
    if (articles.getSize() == 0) {
        cout << "No articles available to display.\n";
        return;
    }
    cout << "\nDisplaying Articles (Sorted by Year):\n";
    for (int i = 0; i < articles.getSize(); i++) {
        cout << articles[i].year << " - " << articles[i].title << " (" << articles[i].category << ")\n";
    }
} void sortArticlesByYear(ArticleArray& articles) {
    quickSort(&articles[0], 0, articles.getSize() - 1);  
    displayArticles(articles);
}
void searchByYear(ArticleArray& articles) {
    int searchYear;
    cout << "Enter year to search: ";
    cin >> searchYear;

    bool found = false;
    cout << "\nArticles from year " << searchYear << ":\n";
    for (int i = 0; i < articles.getSize(); i++) {
        if (articles[i].year == searchYear) {
            cout << articles[i].title << " (" << articles[i].category << ")\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No articles found from year " << searchYear << ".\n";
    }
}

void bubbleSort(ArticleArray& articles) {
    int n = articles.getSize();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (articles[j].year > articles[j + 1].year) {
                swap(articles[j], articles[j + 1]);
            }
        }
    }
}
void merge(ArticleArray& articles, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Article* leftArr = new Article[n1];
    Article* rightArr = new Article[n2];

    for (int i = 0; i < n1; i++) leftArr[i] = articles[left + i];
    for (int i = 0; i < n2; i++) rightArr[i] = articles[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i].year <= rightArr[j].year) {
            articles[k++] = leftArr[i++];
        } else {
            articles[k++] = rightArr[j++];
        }
    }

    while (i < n1) articles[k++] = leftArr[i++];
    while (j < n2) articles[k++] = rightArr[j++];

    delete[] leftArr;
    delete[] rightArr;
}

void mergeSort(ArticleArray& articles, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(articles, left, mid);
        mergeSort(articles, mid + 1, right);
        merge(articles, left, mid, right);
    }
}
void compareSortingPerformance(ArticleArray& articles) {
    if (articles.getSize() == 0) {
        cout << "No articles to sort.\n";
        return;
    }
    ArticleArray temp1, temp2;
    for (int i = 0; i < articles.getSize(); i++) {
        temp1.push_back(articles[i]);
        temp2.push_back(articles[i]);
    }

    cout << "Starting Bubble Sort...\n";  
    auto start1 = chrono::high_resolution_clock::now();
    bubbleSort(temp1);  
    auto stop1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::milliseconds>(stop1 - start1);
    cout << "Bubble Sort completed!\n"; 

    cout << "Starting Merge Sort...\n";  
    auto start2 = chrono::high_resolution_clock::now();
    mergeSort(temp2, 0, temp2.getSize() - 1);
    auto stop2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(stop2 - start2);
    cout << "Merge Sort completed!\n";  
    cout << "\nSorting Performance Comparison:\n";
    cout << "Bubble Sort took " << duration1.count() << " ms\n";
    cout << "Merge Sort took " << duration2.count() << " ms\n";
}





void analyzeFakeNewsByMonth(ArticleArray& articles) {
    int monthCount[12] = {0}; 
    int totalFakeNews = 0;
    int totalPoliticalFakeNews = 0;

    cout << "\nFake News Analysis for 2016:\n";

    for (int i = 0; i < articles.getSize(); i++) {
        if (articles[i].year == 2016 && articles[i].month >= 1 && articles[i].month <= 12) {
            monthCount[articles[i].month - 1]++;
            totalFakeNews++;
            string categoryLower = articles[i].category;
            for (char &c : categoryLower) c = tolower(c);
            if (categoryLower.find("politics") != string::npos || categoryLower.find("government") != string::npos) { 
                totalPoliticalFakeNews++;
            }
        }
    }

  
    string months[] = {"January", "February", "March", "April", "May", "June",
                       "July", "August", "September", "October", "November", "December"};

    for (int i = 0; i < 12; i++) {
        int percentage = (totalFakeNews > 0) ? (monthCount[i] * 100 / totalFakeNews) : 0;
        cout << months[i] << " | " << string(percentage, '*') << " " << percentage << "%\n";
    }
    double overallPercentage = (totalFakeNews > 0) ? (totalPoliticalFakeNews * 100.0 / totalFakeNews) : 0;
    cout << "\nPercentage of fake political news in 2016: " << fixed << setprecision(2) << overallPercentage << "%\n";
}

void menu() {
    cout << "\nMenu Options:" << endl;
    cout << "1. Sort and display all articles by publication year" << endl;
    cout << "2. Display total number of articles" << endl;
    cout << "3. Search articles (by year)" << endl;
    cout << "4. Analyze fake political news by month in 2016" << endl;
    cout << "5. Compare sorting performance" << endl;
    cout << "6. Analyze most common words in fake government-related news" << endl;
    cout << "7. Exit" << endl;
}int main() {
    try {
        ArticleArray trueNews;
        ArticleArray fakeNews;
        cout << "Starting to read CSV file...\n";
        loadArticles("data/cleaned_true.csv", trueNews);
        loadArticles("data/cleaned_fake.csv", fakeNews);
        int choice;
        do {
            menu(); 
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    cout << "\nSorting articles by year...\n";
                    sortArticlesByYear(fakeNews);
                    cout << "Sorting complete.\n";
                    break;
                case 2:
                    cout << "Total True News Articles: " << trueNews.getSize() << endl;
                    cout << "Total Fake News Articles: " << fakeNews.getSize() << endl;
                    break;
                case 3:
                    searchByYear(fakeNews);
                    break;
                case 4:
                    analyzeFakeNewsByMonth(fakeNews);
                    break;
                case 5:
                    compareSortingPerformance(fakeNews);
                    break;
                case 6:
                    fakeNews.analyzeWordFrequency();
                    break;
                case 7:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice! Please enter a valid option.\n";
            }

            cout << "\n"; 

        } while (choice != 7);

    } catch (const exception& e) {
        cerr << "Program terminated with error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
