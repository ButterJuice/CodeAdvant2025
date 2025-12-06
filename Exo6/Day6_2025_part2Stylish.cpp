#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <thread>
#include <chrono>


#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define WHITE   "\033[37m"
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

// Helper to print ranges as an animation
void animateRanges(const std::vector<Range>& ranges, const std::string& message, int delay_ms = 500) {
    std::cout << YELLOW << message << RESET << "\n";
    for (size_t i = 0; i < ranges.size(); ++i) {
        std::cout << "\r\033[K"; // Clear line
        for (size_t j = 0; j < ranges.size(); ++j) {
            if (j == i)
                std::cout << "\033[1;44;34m[" << ranges[j].start << "-" << ranges[j].end << "]\033[0m ";
            else
                std::cout << BLUE << "[" << ranges[j].start << "-" << ranges[j].end << "] " << RESET;
        }
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    std::cout << "\n";
}

// Helper to animate the merging process
void animateMerge(std::vector<Range> ranges, int delay_ms = 700) {
    std::cout << YELLOW << "Merging ranges step by step..." << RESET << "\n";
    if (ranges.empty()) return;

    // Sort ranges by start value
    std::sort(ranges.begin(), ranges.end(), [](const Range& a, const Range& b) {
        return a.start < b.start;
    });

    std::vector<Range> merged;
    merged.push_back(ranges[0]);
    std::cout << "\r\033[K" << BLUE << "[" << ranges[0].start << "-" << ranges[0].end << "] " << RESET;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

    for (size_t i = 1; i < ranges.size(); ++i) {
        Range& last = merged.back();
        std::cout << "\r\033[K";
        // Print merged so far
        for (const auto& m : merged) {
            std::cout << GREEN << "[" << m.start << "-" << m.end << "] " << RESET;
        }
        // Print current candidate in blue highlight
        std::cout << "\033[1;44;34m[" << ranges[i].start << "-" << ranges[i].end << "]\033[0m ";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

        if (ranges[i].start <= last.end + 1) {
            last.end = std::max(last.end, ranges[i].end);
        } else {
            merged.push_back(ranges[i]);
        }
    }
    // Final merged result
    std::cout << "\r\033[K";
    for (const auto& m : merged) {
        std::cout << GREEN << "[" << m.start << "-" << m.end << "] " << RESET;
    }
    std::cout << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
}

int main(int argc, char* argv[]) {
    // If not spawned, spawn a new terminal and exit
    if (argc == 1) {
        std::string cmd = "start cmd /k \"" + std::string(argv[0]) + " --spawn\"";
        system(cmd.c_str());
        return 0;
    }

    std::vector<Range> ranges;
    std::vector<long long> numbers;

    readEntries("inputEASY", ranges, numbers);
    //readEntries("input", ranges, numbers);

    // Save original ranges for animation
    std::vector<Range> original_ranges = ranges;

    // Animation: show original ranges
    animateRanges(original_ranges, "Original ranges before merging:", 400);

    // Animation: merge step by step
    animateMerge(original_ranges, 700);

    // Merge overlapping or adjacent ranges (actual merge for rest of program)
    mergeRanges(ranges);

    // Find the global min and max from the merged ranges
    long long min_val = ranges.empty() ? 0 : ranges.front().start;
    long long max_val = ranges.empty() ? 0 : ranges.front().end;
    for (const auto& r : ranges) {
        if (r.start < min_val) min_val = r.start;
        if (r.end > max_val) max_val = r.end;
    }

    // Cursor animation settings
    const int numbers_per_line = 20;
    const int cursor_delay_ms = 200; // Change this value for speed (milliseconds)

    // Print the ranges at the top
    std::cout << YELLOW << "Ranges: " << RESET;
    for (const auto& r : ranges) {
        std::cout << BLUE << "[" << r.start << "-" << r.end << "] " << RESET;
    }
    std::cout << "\n";

    std::cout << "\n" << YELLOW << "Cursor animation from " << min_val << " to " << max_val << ":" << RESET << "\n";

    // Prepare all numbers in a vector for easy access
    std::vector<long long> all_numbers;
    for (long long v = min_val; v <= max_val; ++v) {
        all_numbers.push_back(v);
    }

    // Track color state for each number: 0 = white, 1 = green, 2 = red
    std::vector<int> color_state(all_numbers.size(), 0);

    int fresh_count = 0; // Counter for numbers in range

    for (size_t cursor = 0; cursor < all_numbers.size(); ++cursor) {
        std::cout << "\r\033[K"; // Return to start of line and clear it
        for (size_t i = 0; i < all_numbers.size() && i < numbers_per_line; ++i) {
            long long v = all_numbers[i];
            if (i == cursor) {
                // Check if the cursor is on a value in range
                bool in_range = false;
                for (const auto& r : ranges) {
                    if (isInRange(v, r)) {
                        in_range = true;
                        break;
                    }
                }
                // Set color state for this number
                color_state[i] = in_range ? 1 : 2;
                // Cursor position: highlight with blue background and bold, green/red for in/out of range
                if (in_range)
                    std::cout << "\033[1;44;32m[" << v << "]\033[0m ";
                else
                    std::cout << "\033[1;44;31m[" << v << "]\033[0m ";
            } else {
                // Print according to color state
                if (color_state[i] == 1)
                    std::cout << GREEN << v << RESET << " ";
                else if (color_state[i] == 2)
                    std::cout << RED << v << RESET << " ";
                else
                    std::cout << WHITE << v << RESET << " ";
            }
        }

        // Update fresh_count
        fresh_count = 0;
        for (int state : color_state) {
            if (state == 1) fresh_count++;
        }

        // Move to next line and print the counter
        std::cout << "\n\033[K" << YELLOW << "Numbers in range so far: " << GREEN << fresh_count << RESET;
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(cursor_delay_ms));

        // Move cursor up one line to overwrite on next iteration
        std::cout << "\033[F";
    }
    // Final counter print
    std::cout << "\n\n" << YELLOW << "Done! Numbers in range: " << GREEN << fresh_count << RESET << "\n";

    system("pause"); // Keeps the window open
    return 0;
}
