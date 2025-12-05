#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>


#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

struct Range {
    long long start;
    long long end;
};

bool isInRange(long long value, const Range& range) {
    return value >= range.start && value <= range.end;
}

// Merge overlapping or adjacent ranges in-place
void mergeRanges(std::vector<Range>& ranges) {
    if (ranges.empty()) return;

    // Sort ranges by start value
    std::sort(ranges.begin(), ranges.end(), [](const Range& a, const Range& b) {
        return a.start < b.start;
    });

    std::vector<Range> merged;
    merged.push_back(ranges[0]);

    for (size_t i = 1; i < ranges.size(); ++i) {
        Range& last = merged.back();
        // If current range overlaps or is adjacent to the last, merge them
        if (ranges[i].start <= last.end + 1) {
            last.end = std::max(last.end, ranges[i].end);
        } else {
            merged.push_back(ranges[i]);
        }
    }
    ranges = std::move(merged);
}

void readEntries(const std::string& filename, std::vector<Range>& ranges, std::vector<long long>& numbers) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open input\n";
        return;
    }

    std::string line;
    bool readingRanges = true;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line.empty()) {
            readingRanges = false;
            continue;
        }

        if (readingRanges) {
            size_t dash = line.find('-');
            if (dash == std::string::npos) {
                std::cerr << "Invalid range line (missing '-'): \"" << line << "\"\n";
                continue;
            }

            // Extract the start and end of the range as strings
            std::string a_str = line.substr(0, dash);
            std::string b_str = line.substr(dash + 1);

            // Convert the extracted strings to long long integers
            long long a = std::stoll(a_str);
            long long b = std::stoll(b_str);

            if (a_str.empty() || b_str.empty()) {
                std::cerr << "Invalid range line (empty endpoints): \"" << line << "\"\n";
                continue;
            }

            ranges.push_back({a, b});
        } else {
            try {
                // Convert the line to a long long integer
                long long v = std::stoll(line);
                numbers.push_back(v);
            } catch (const std::exception &e) {
                std::cerr << "Invalid number line: \"" << line << "\"\n";
                continue;
            }
        }
    }
}

int main() {
    std::vector<Range> ranges;
    std::vector<long long> numbers;

    //readEntries("inputEASY", ranges, numbers);
    readEntries("input", ranges, numbers);

    // Merge overlapping or adjacent ranges
    mergeRanges(ranges);

    // Debug output
    std::cout << "Ranges:\n";
    for (const auto &r : ranges) {
        std::cout << r.start << "-" << r.end << "\n";
    }

    std::cout << "\nNumbers:\n";

    int fresh_count = 0;
    for (int i = 0; i < numbers.size(); i++) {
        long long n = numbers[i];
        std::cout << n ;
        for (size_t j = 0; j < ranges.size(); j++)
        { 
            if(isInRange(n, ranges[j])) {
                std::cout << "  In range" ;
                fresh_count++;            
            }
        }
        std::cout << "\n";
        
       
    }
    std::cout << "\nFresh count: " << fresh_count << "\n";


    
    return 0;
}
