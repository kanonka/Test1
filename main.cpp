#include <string>
#include <functional>
#include <vector>
#include <map>
#include <algorithm>

// Now this is hosted on GitHub - testing first time

struct PhoneCall 
{
    PhoneCall() : call_number(0), call_seconds(0) {};
    int call_number; // more than enough to keep "numeric" representation of the "phone" #
    int call_seconds;
};

void tokenize(std::string& str, std::vector<std::string> &tokens, const char* delimiter) {
    size_t start = str.find_first_not_of(delimiter);
    size_t end = start;
    while (start != std::string::npos) {
        end = str.find(delimiter, start);
        tokens.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(delimiter, end);
    }
}

PhoneCall parseCall(std::string& callLine) {
    PhoneCall result;
    // 1. We need to parse the line:
    std::vector<std::string> parts;
    // get lines - each line is a duration + number:
    tokenize(callLine, parts, ",");
    // now, parts[0] is duration, parts[1] is phone number.
    // let's convert them.
    // There are multiple ways to convert, let's try this one:
    int h, m, s = 0;
    if (sscanf(parts[0].c_str(), "%d:%d:%d", &h, &m, &s) >= 2)
        result.call_seconds = h * 3600 + m * 60 + s;
    
    // now, once we got total secs, let's calculate phone number "number":
    // There is no exact description what is phone "numerical value", so
    // I would assume that this is the number without dashes, i.e. "111-222-3333" = 111222333
    // Now, let's do the same parsing as before. We could have reused h,m,s for speed,
    // but, for the support clarity, i'd rather use new vars:
    int n1, n2, n3 = 0;

    if (sscanf(parts[1].c_str(), "%d-%d-%d", &n1, &n2, &n3) >= 2)
        result.call_number = n1*1000000 + n2*1000 + n3;
    return result;
}

struct Point2D {
    int x;
    int y;
};

bool comparePoints(const Point2D &p1, const Point2D &p2) {
    return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
}

int solution(std::vector<std::vector<int>> &A) {
    int count = 0;
    size_t rowCount = A.size();
    size_t colCount = rowCount > 0 ? A[0].size() : 0;
    if (!colCount || !rowCount)
        return 0;

    std::vector<std::vector<int>> B; // initializes to zeroes
    B.resize(rowCount, std::vector<int>(colCount));

    for (size_t i = 0; i < rowCount; ++i) {
        std::vector<int>& row = A[i];

        for (size_t j = 0; j < colCount; ++j) {
            int color = row[j];
            int& country = B[i][j];
            // logic is as folowing:
            // if left OR top OR right has same color, mark this one with that color, otherwise, start a new one:
            if (j > 0) { // left color
                if (row[j - 1] == color && country == 0) {
                    country = B[i][j - 1];
                }
            }
            if (i > 0) { // top color
                if (A[i - 1][j] == color && country == 0) {
                    country = B[i - 1][j];
                }
            }
            bool rightSet = false;
            if (j < colCount - 1) { // right color
                int& rightCountry = B[i][j + 1];
                if (A[i][j + 1] == color && rightCountry > 0) {
                    country = rightCountry;
                    rightSet = true;
                }
            }

            if (country == 0) {
                // no left or top has same color - start a new one:
                country = ++count;
            }
            // now, let's set right AND bottom to this coutry, if they have same color:
            if (!rightSet && j < colCount - 1) { // right
                if (A[i][j + 1] == color) {
                    B[i][j + 1] = country;
                }
            }
            if (i < rowCount - 1) { // bottom
                if (A[i + 1][j] == color) {
                    B[i + 1][j] = country;
                }
            }
        }
    }
    return count;
}

int main(int argc, char* argv[])
{
    std::vector<std::vector<int>> v = { {5, 4, 4}, 
                                        {4, 3, 4}, 
                                        {3, 2, 4}, 
                                        {2, 2, 2}, 
                                        {3 ,3, 4},
                                        {1, 4, 4},
                                        {4, 1, 1}
                                      };
    //std::vector<Point2D> v = { {2,5}, {6,3}, {4,5}, {7,8}, {7,0} };
    int res = solution(v);
    return 0;
}